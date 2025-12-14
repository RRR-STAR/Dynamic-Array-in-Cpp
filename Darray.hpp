#ifndef DARRAY_HPP
#define DARRAY_HPP

#include <list>
#include <functional>
#include <initializer_list>
#include <stdexcept>
#include <utility>

/**
 * @brief
 * An implementation of Dynamic type array, 
 * it's working a kind of like vectors but not exactly like vectors.
 * The size of the array is initially is given by the user(default is 25), and it increases over time.
 * It differs from `std::vector<T>` because vector needs re-allocation and copying of actual object elements
 * while this implementation only copies the addresses(pointers) for re-allocation, reducing the overhead for complex objects.
 * This @class`Darray(DynamicArray)` uses a combination of `std::list<T>` and a array of iterators `std::list<T>::iterator`, 
 * that's why it provides the list type addition & deletion of elements,
 * along with the array's random access time O(1) property for it's elements.
*/
template <typename T>
class Darray final {
    
    size_t index, maxSize;
    std::list<T> data;
    typename std::list<T>::iterator *addresses;
    
    // resize the addresses array when capacity is full
    void resizeAddressTable(const size_t newSize){
        auto newAddresses = new typename std::list<T>::iterator[newSize];
        size_t bound = (newSize < index) ? newSize : index;
        for (size_t i = 0; i < bound; ++i) {
            newAddresses[i] = addresses[i];
        }
        delete[] addresses;
        addresses = newAddresses;// newAddresses automatically goes out-of-scope
        maxSize = newSize;
    }
    
    // rebuild addresses array after sorting to maintain correct index mappings
    void rebuildAllAddresses(){
        size_t i = 0;
        for (auto it = data.begin(); it != data.end(); ++it, ++i){
            addresses[i] = it;
        }
    }
    
    public :
    
    // default constructor
    explicit Darray(const size_t defaultCapacity = 25): index(0), maxSize(defaultCapacity){
        addresses = new typename std::list<T>::iterator[defaultCapacity];
    }
    // Copy constructor - deep copy
    Darray(const Darray &other): index(other.index), maxSize(other.maxSize), data(other.data){
        addresses = new typename std::list<T>::iterator[maxSize];
        rebuildAllAddresses();
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
    // destructor
    ~Darray() noexcept { delete[] addresses;  addresses = nullptr; }
    
    // Copy assignment operator
    Darray& operator=(const Darray &other);
    // Move assignment operator
    Darray& operator=(Darray &&other) noexcept;
    
    // add the elements to the end of the array in O(1) time addition
    void add(const T &val);
    void add(T &&val);
    // add the element at specified index
    void addAt(const size_t index, const T &val);
    void addAt(const size_t index, T &&val);
    // add all the elements at once @note adds elements in the same order as given
    void addAll(const std::initializer_list<T> &vals);
    
    // returns the reference of index element's data in O(1) time access
    T& operator[](const size_t index);
    const T& operator[](const size_t index) const;
    
    // return the iterator of `std::list<T>` type pointing to the first element of the array
    inline typename std::list<T>::iterator begin() noexcept { return data.begin(); }
    inline typename std::list<T>::const_iterator begin() const noexcept { return data.begin(); }
    inline typename std::list<T>::const_iterator cbegin() const noexcept { return data.cbegin(); }
    // return the iterator of `std::list<T>` type pointing to the last of the array
    inline typename std::list<T>::iterator end() noexcept { return data.end(); }
    inline typename std::list<T>::const_iterator end() const noexcept { return data.end(); }
    inline typename std::list<T>::const_iterator cend() const noexcept { return data.cend(); }
    
    // remove the specified element/element(s) from the array
    // @note if flag is true then removes all the occurrences of the element
    void remove(const T &val, const bool removeAllOccurrences = false);
    // remove the specified index element from the array
    void removeAt(const size_t index);
    
    // delete all elements at once 
    // @note if the elements themselves are pointers, the pointed-to memory is not touched in any way. 
    // Managing the pointer is the user's responsibility. 
    void clear() noexcept { data.clear(); index = 0; }
    
    // checks that the array is empty or not
    inline bool empty() const noexcept { return index == 0; }
    
    // returns the size of the array
    inline size_t size() const noexcept { return index; }
    
    // shrink the array to the specified size @note removes elements from the back
    void shrinkToSize(const size_t newSize);
    
    // sort the array in ascending order and rebuild index mappings
    void sort(){ data.sort();  rebuildAllAddresses(); }
    
    // custom sort functions for UDT's
    void sort(std::function<bool(const T &, const T &)> comparatorFunction){ 
        data.sort(comparatorFunction); 
        rebuildAllAddresses();
    }
};


template <typename T>
Darray<T>& Darray<T>::operator=(const Darray &other){
    
    if (this != &other){
        data = other.data;
        index = other.index;
        maxSize = other.maxSize;
        delete[] addresses;
        addresses = new typename std::list<T>::iterator[maxSize];
        rebuildAllAddresses();
    }
    return *this;
}


template <typename T>
Darray<T>& Darray<T>::operator=(Darray &&other) noexcept {
    
    if (this != &other){
        data = std::move(other.data);
        maxSize = other.maxSize;
        index = other.index;
        delete[] addresses;         // list iterators are consistent after move
        addresses = other.addresses; // so we can simply move the address table
        other.data.clear();
        other.addresses = nullptr;
        other.maxSize = 0;
        other.index = 0;
    }
    return *this;
}


template <typename T>
void Darray<T>::add(const T &val){
    
    if (index >= maxSize){ resizeAddressTable(maxSize * 2); }
    data.push_back(val);
    // std::prev() gives the recently inserted elem iterator
    addresses[index] = std::prev(data.end());
    ++index;
}


template <typename T>
void Darray<T>::add(T &&val){
    
    if (index >= maxSize){ resizeAddressTable(maxSize * 2); }
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
    
    auto it = data.begin();
    std::advance(it, index);
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
    if (this->index + 1 > maxSize){ resizeAddressTable(maxSize + 25); }
    auto it = data.begin();
    std::advance(it, index);
    auto newIt = data.insert(it, std::move(val));
    
    for (size_t i = this->index; i > index; --i){
        addresses[i] = addresses[i - 1];
    }
    addresses[index] = newIt;
    ++this->index;
}


template <typename T>
void Darray<T>::addAll(const std::initializer_list<T> &vals){
    
    if (index + vals.size() > maxSize){ resizeAddressTable(index + vals.size()); }
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
            --index;  --i; // so the subsequent redundant element is not skipped
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