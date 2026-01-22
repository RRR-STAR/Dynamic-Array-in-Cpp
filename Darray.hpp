#ifndef DARRAY_HPP
#define DARRAY_HPP

#include <list>
#include <iterator>
#include <functional>
#include <initializer_list>
#include <stdexcept>
#include <utility>

/**
 * @brief
 * An implementation of Dynamic type array.
 * It provides std::vector-like random access (O(1)) with std::list-like reference stability.
 * 
 * The size of the array is initially given by the user (default is 25), and it increases over time.
 * This class uses a combination of `std::list<T>` and an array of iterators `std::list<T>::iterator`.
 */
template <typename T>
class Darray final {
    
    using iterator = typename std::list<T>::iterator;
    using const_iterator = typename std::list<T>::const_iterator;
    
    size_t index, maxSize;
    std::list<T> data;
    iterator *addresses; // Array of iterators mapping index -> list node
    
    // Resize the addresses array when capacity is full
    void resizeAddressTable(const size_t newSize){
        auto newAddresses = new iterator[newSize];
        size_t bound = (newSize < index) ? newSize : index;
        for (size_t i = 0; i < bound; ++i) {
            newAddresses[i] = addresses[i];
        }
        delete[] addresses;
        addresses = newAddresses;// newAddresses automatically goes out-of-scope
        maxSize = newSize;
    }
    
    // Rebuild addresses array after sorting or copying to maintain correct index mappings
    void rebuildAllAddresses(){
        size_t i = 0;
        for (auto it = data.begin(); it != data.end(); ++it, ++i){
            addresses[i] = it;
        }
    }
    
    public :
    
    // Default constructor
    explicit Darray(const size_t defaultCapacity = 25): index(0), maxSize(defaultCapacity){
        addresses = new iterator[defaultCapacity];
    }
    // Copy constructor - deep copy
    Darray(const Darray &other): index(other.index), maxSize(other.maxSize), data(other.data){
        addresses = new iterator[maxSize];  rebuildAllAddresses();
    }
    // Move constructor
    Darray(Darray &&other) noexcept : index(other.index), maxSize(other.maxSize){
        data = std::move(other.data); 
        addresses = other.addresses;
        other.data.clear();
        other.addresses = nullptr;
        other.index = 0;
        other.maxSize = 0;
    }
    // Parameterized constructor with initializer list
    Darray(const std::initializer_list<T> &vals): Darray(vals.size()){
        this->addAll(vals);
    }
    // Destructor
    ~Darray() noexcept { delete[] addresses;  addresses = nullptr; }
    
    // Copy assignment operator (Strong Exception Guarantee)
    Darray& operator=(const Darray &other);
    // Move assignment operator
    Darray& operator=(Darray &&other) noexcept;
    
    // Add the elements to the end of the array in O(1) time
    void add(const T &val);
    void add(T &&val);
    // Add the element at specified index
    void addAt(const size_t index, const T &val);
    void addAt(const size_t index, T &&val);
    // Add all the elements at once
    void addAll(const std::initializer_list<T> &vals);
    
    // Returns the reference of index element's data in O(1) time access
    T& operator[](const size_t index);
    const T& operator[](const size_t index) const;
    
    // Iterators
    // there are 2 different types of iterators: iterator and const_iterator
    // and the 3rd type is for explicitly requesting a const_iterator
    inline iterator begin() noexcept { return data.begin(); }
    inline const_iterator begin() const noexcept { return data.begin(); }
    inline const_iterator cbegin() const noexcept { return data.cbegin(); }
    inline iterator end() noexcept { return data.end(); }
    inline const_iterator end() const noexcept { return data.end(); }
    inline const_iterator cend() const noexcept { return data.cend(); }
    
    // Remove the specified element/element(s) from the array
    void remove(const T &val, const bool removeAllOccurrences = false);
    // Remove the specified index element from the array
    void removeAt(const size_t index);
    
    // Delete all elements at once 
    void clear() noexcept { data.clear(); index = 0; }
    
    // Checks that the array is empty or not
    inline bool empty() const noexcept { return index == 0; }
    
    // Returns the size of the array
    inline size_t size() const noexcept { return index; }
    
    // Shrink the array to the specified size
    void shrinkToSize(const size_t newSize);
    
    // Sort the array in ascending order and rebuild index mappings
    void sort(){ data.sort();  rebuildAllAddresses(); }
    
    // Custom sort functions
    void sort(std::function<bool(const T &, const T &)> comparatorFunction){ 
        data.sort(comparatorFunction);  rebuildAllAddresses();
    }
};


template <typename T>
Darray<T>& Darray<T>::operator=(const Darray &other){
    
    if (this != &other){
        // Allocate new resources first
        auto newAddresses = new iterator[other.maxSize];
        try {
            std::list<T> newData = other.data; // Copy list
            delete[] addresses;
            addresses = newAddresses;
            data = std::move(newData);
            index = other.index;
            maxSize = other.maxSize;
            rebuildAllAddresses();
        } catch (...) {
            delete[] newAddresses;
            throw;
        }
    }
    return *this;
}


template <typename T>
Darray<T>& Darray<T>::operator=(Darray &&other) noexcept {
    
    if (this != &other){
        delete[] addresses;         
        data = std::move(other.data);
        addresses = other.addresses;
        maxSize = other.maxSize;
        index = other.index;
        other.data.clear();
        other.addresses = nullptr;
        other.maxSize = 0;
        other.index = 0;
    }
    return *this;
}


template <typename T>
void Darray<T>::add(const T &val){
    
    if (index >= maxSize) {
        size_t newSize = (maxSize == 0) ? 25 : maxSize * 2;
        resizeAddressTable(newSize);
    }
    data.push_back(val);
    // std::prev() gives the recently inserted elem iterator
    addresses[index] = std::prev(data.end());
    ++index;
}


template <typename T>
void Darray<T>::add(T &&val){
    
    if (index >= maxSize) {
        size_t newSize = (maxSize == 0) ? 25 : maxSize * 2;
        resizeAddressTable(newSize);
    }
    data.push_back(std::move(val));
    addresses[index++] = std::prev(data.end());
}


template <typename T>
void Darray<T>::addAt(const size_t index, const T &val){
    
    // allow insertion at index 0 when empty, or at any valid position
    if (index > this->index){
        throw std::out_of_range("Darray.addAt(): index out of bounds");
    }
    // if array is already full with elements, resize it
    if (this->index + 1 > maxSize){ resizeAddressTable(maxSize + 25); } 
    
    // Use address table for O(1) lookup
    auto it = (index == this->index) ? data.end() : addresses[index];
    auto newIt = data.insert(it, val);
    
    // shift addresses right
    for (size_t i = this->index; i > index; --i){
        addresses[i] = addresses[i - 1];
    }
    addresses[index] = newIt;
    ++this->index;
}


template <typename T>
void Darray<T>::addAt(const size_t index, T &&val){
    
    if (index > this->index){
        throw std::out_of_range("Darray.addAt(): index out of bounds");
    }
    if (this->index + 1 > maxSize)  resizeAddressTable(maxSize + 25);
    // Use address table for O(1) lookup
    auto it = (index == this->index) ? data.end() : addresses[index];
    auto newIt = data.insert(it, std::move(val));
    
    for (size_t i = this->index; i > index; --i){
        addresses[i] = addresses[i - 1];
    }
    addresses[index] = newIt;
    ++this->index;
}


template <typename T>
void Darray<T>::addAll(const std::initializer_list<T> &vals){
    
    if (index + vals.size() > maxSize)  resizeAddressTable(index + vals.size());
    for (const T &val : vals){
        data.push_back(val);
        addresses[index++] = std::prev(data.end());
    }
}


template <typename T>
T& Darray<T>::operator[](const size_t index){
    
    if (index >= this->index){ 
        throw std::out_of_range("Darray[]: index out of bounds");
    }
    return *(addresses[index]);
}


template <typename T>
const T& Darray<T>::operator[](const size_t index) const {
    
    if (index >= this->index){ 
        throw std::out_of_range("Darray[]: index out of bounds");
    }
    return *(addresses[index]);
}


template <typename T>
void Darray<T>::remove(const T &val, const bool removeAllOccurrences){
    
    if (data.empty() || index == 0)  return;
    for (size_t i = 0; i < index; ++i){
        
        if (*(addresses[i]) == val){
            auto addressOfElementToBeRemoved = addresses[i];
            data.erase(addressOfElementToBeRemoved);
            
            // shift addresses left
            for (size_t j = i; j < index - 1; ++j){
                addresses[j] = addresses[j + 1];
            }
            --index;  --i; // --i, so the subsequent redundant element is not skipped
            // remove only the first occurrence if the flag is false
            if (not removeAllOccurrences)  return; 
        }
    }
}


template <typename T>
void Darray<T>::removeAt(const size_t index){
    
    if (index >= this->index){
        throw std::out_of_range("Darray.removeAt(): index out of bounds");
    }
    auto addressOfElementToBeRemoved = addresses[index];
    data.erase(addressOfElementToBeRemoved);
    
    // shift addresses left
    for (size_t i = index; i < this->index - 1; ++i) {
        addresses[i] = addresses[i + 1];
    }
    --this->index;
}


template <typename T>
void Darray<T>::shrinkToSize(const size_t newSize){
    
    if (newSize >= index)  return;
    while (index > newSize)  data.erase(addresses[--index]);
    resizeAddressTable(newSize);
}


#endif // DARRAY_HPP