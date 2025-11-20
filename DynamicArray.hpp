
#include <list>
#include <iterator>
#include <functional>
#include <unordered_map>
#include <initializer_list>
#include <stdexcept>

/**
* @brief
* An implementation of Dynamic type array which contains the properties of Array, 
* but the size of the array is initially is 0, and it increases over time.
* It differs from `std::vector<T>` because vector needs re-allocation and copying of elements 
* as the size of the array increases. But @class`DArr(DynamicArray)` uses a combination of 
* `std::list<T>` and `std::unordered_map<T>`, that's why it provides 
* the list type addition & deletion of elements from the back, 
* along with the array's random access time O(1) property for it's elements.
* 
* @note 
* The main drawback is, though it uses the `std::list<T>` but the insertion and deletion 
* of elements from front or middle are prevented for synchoronization purposes.
*/
template <typename __t>
class DArr {
    
    size_t _index;
    std::list<__t> _data;
    std::unordered_map<size_t, typename std::list<__t>::iterator> _address;
    
    public :
    DArr(): _index(0){ _address.max_load_factor(0.25); }
    ~DArr(){ this->clear(); }
    
    // add the elements to the end of the array in O(1) time addition
    void add(__t __val);
    // add all the elements at once @note adds elements in the same order as given
    void addAll(std::initializer_list<__t> __vals);
    
    // remove the last element from the array in O(1) time deletion, but doesn't return it
    // to get the last item `obj[obj.size()-1]` can be used before calling pop_back()
    void pop_back(); 
    
    // returns the reference of __index element's data in O(1) time access
    __t& operator[](size_t __index);
    const __t& operator[](size_t __index) const;
    
    // return the iterator of `std::list<T>` type pointing to the first element of the array
    inline typename std::list<__t>::iterator begin(){ return _data.begin(); }
    inline typename std::list<__t>::const_iterator begin() const { return _data.begin(); }
    
    // return the iterator of `std::list<T>` type pointing to the last of the array
    inline typename std::list<__t>::iterator end(){ return _data.end(); }
    inline typename std::list<__t>::const_iterator end() const { return _data.end(); }
    
    // delete all elements at once 
    // @note if the elements themselves are pointers, the pointed-to memory is not touched in any way. 
    // Managing the pointer is the user's responsibility. 
    inline void clear(){ _address.clear(); _data.clear(); _index = 0; }
    
    // checks that the array is empty or not
    inline bool empty() const { return _index == 0; }
    
    // returns the size of the array
    inline size_t size() const { return _index; }
    
    // sort the array in acending order
    inline void sort(){ _data.sort(); }
    
    // shirnk the array to the specified size @note removes elements from the back
    inline void shrink(size_t __new_size){
        while (_index > __new_size){ this->pop_back(); }
    }
    
    // custom sort functions for UDT's
    inline void sort(std::function< bool(const __t &, const __t &)> __comparator){ 
        _data.sort(__comparator); 
    }
};

template <typename __t>
void DArr<__t>::add(__t __val){
    _data.push_back(__val);
    // std::prev() gives the recently inserted elem iterator
    _address[_index] = std::prev(_data.end());
    _index++;
}

template <typename __t>
void DArr<__t>::addAll(std::initializer_list<__t> __vals){
    for (__t __val : __vals){
        _data.push_back(__val);
        _address[_index++] = std::prev(_data.end());
    }
}

template <typename __t>
void DArr<__t>::pop_back(){
    if (_data.empty()  or  _address.empty())  return;
    _address.erase(_index - 1);
    _data.pop_back();
    _index--;
}

template <typename __t>
__t& DArr<__t>::operator[](size_t __index){
    try { 
        return  *(_address.at(__index)); 
    }
    catch (const std::out_of_range &e){ throw; /*re-throw*/ }
}

template <typename __t>
const __t& DArr<__t>::operator[](size_t __index) const {
    try { 
        return  *(_address.at(__index)); 
    }
    catch (const std::out_of_range &e){ throw; /*re-throw*/ }
}
