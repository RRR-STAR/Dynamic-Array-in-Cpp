
# Dynamic Array in C++

This project implements a custom `DArr` class in C++. The goal is to create a dynamic, array-like data structure that avoids the memory wastage and reallocation overhead associated with `std::vector` while providing the O(1) element access time that `std::list` lacks.

## Key Idea

The `DArr` class combines the strengths of two standard library containers:
- `std::list<T>`: Used for efficient, O(1) insertion and deletion at the end, avoiding the need to shift elements.
- `std::unordered_map<size_t, typename std::list<T>::iterator>`: Used to map an integer index to an iterator pointing to the corresponding element in the list, enabling O(1) random access.

The implementation is contained within the `DArr.hpp` header file and is demonstrated in `main.cpp`.

## Building and Running

This is a simple C++ project that can be compiled with a standard C++ compiler like g++.

## Usage

The `DArr` class provides the following public methods:

- `void add(T value)`: Adds an element to the end of the array.
- `void addAll(std::initializer_list<T> values)`: Adds multiple elements to the end of the array.
- `void pop_back()`: Removes the last element from the array.
- `void shrink(size_t new_size)`: Shrinks the array to a specified size.
- `T& operator[](size_t index)`: Accesses an element by its index. Throws `std::out_of_range` if the index is invalid.
- `void sort()`: Sorts the array in ascending order.
- `void sort(Comparator func)`: Sorts the array using a custom comparison function.
- `void clear()`: Removes all elements from the array.
- `bool empty() const`: Checks if the array is empty.
- `size_t size() const`: Returns the number of elements in the array.

### Example Usage

```cpp
#include <iostream>
#include "DArr.hpp"

int main() {
    DArr<int> arr;
    arr.addAll({10, 5, 20});
    
    std::cout << "Initial array: ";
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

## Development Conventions

*   **Header-only Implementation:** The `DArr` class is fully implemented in the `DArr.hpp` header file, a common practice for C++ template classes.
*   **STL Usage:** The project makes extensive use of the C++ Standard Template Library (STL), particularly `std::list`, `std::unordered_map`, and `std::function`.
*   **Custom Sorting:** The class provides its own `sort()` method, which wraps `std::list::sort()`. It supports both default ascending sort and custom comparison functions.
*   **Error Handling:** Out-of-bounds access in `operator[]` is handled by throwing a `std::out_of_range` exception.
*   **Namespace:** The `DArr` class is defined in the global namespace.
*   **Comments:** The code is well-documented with comments explaining the purpose of the class and its methods.