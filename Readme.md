
# Dynamic Array in C++

This project implements a custom `Darray` class in C++. The goal is to create a dynamic, array-like data structure that avoids the memory wastage and reallocation overhead associated with `std::vector` while providing the O(1) element access time that `std::list` lacks.

## Key Idea

The `Darray` class combines the strengths of two standard library containers:
- `std::list<T>`: Used for efficient, O(1) insertion and deletion at the end, avoiding the need to shift elements.
- `std::list<T>::iterator*`: An array of iterators used to map an integer index to an iterator pointing to the corresponding element in the list, enabling O(1) random access.

The implementation is contained within the `Darray.hpp` header file and is demonstrated in `main.cpp`.

## Usage

The `Darray` class provides the following public methods:

- `void add(const T &value)`: Adds an element to the end of the array in O(1) time. (more speed efficient but less memory efficient, reallocation => X * 2)
- `void add(T &&value)`: Adds an element to the end of the array in O(1) time. [overload for std::move() -> objs]
- `void addAt(const size_t index, const T &value)`: Inserts an element at the specified index. (more memory efficient but less speed efficient, reallocation => X + 25)
- `void addAt(const size_t index, T &&value)`: Inserts an element at the specified index. [overload for std::move() -> objs]
- `void addAll(std::initializer_list<T> values)`: Adds multiple elements to the end of the array.
- `void remove(const T& value)`: Removes the first occurrence of the specified element.
- `void removeAt(const size_t index)`: Removes the element at the specified index.
- `void shrinkToSize(const size_t new_size)`: Shrinks the array to a specified size (removes from back).
- `T& operator[](const size_t index)`: Accesses an element by its index. Throws `std::out_of_range` if the index is invalid.
- `void sort()`: Sorts the array in ascending order.
- `void sort(std::function<bool(const T&, const T&)> comparator)`: Sorts using a custom comparison function.
- `void clear()`: Removes all elements from the array.
- `bool empty() const noexcept`: Checks if the array is empty.
- `size_t size() const noexcept`: Returns the number of elements in the array.
- `begin()`, `end()`, `cbegin()`, `cend()`: Iterator access for range-based for loops.

### Example Usage

```cpp
#include <iostream>
#include "Darray.hpp"

int main() {
    Darray<int> arr = {10, 5, 20};
    
    std::cout << "Initial array: ";
    for (size_t i = 0; i < arr.size(); ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;    
    
    arr.addAt(0/*0th-index*/, 15);
    std::cout << "Add 15 at index 0 => arr[0]: ";
    for (size_t i = 0; i < arr.size(); ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
    
    arr.sort();
    
    std::cout << "Sorted array: ";
    for (int val : arr) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    return 0;
}
```