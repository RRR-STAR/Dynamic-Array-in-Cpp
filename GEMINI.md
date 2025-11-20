# GEMINI.md

This file provides a comprehensive overview of the "DArr" project, designed to give context to an AI assistant.

## Project Overview

This project implements a custom `DArr` class in C++. The goal is to create a dynamic, array-like data structure that avoids the memory wastage and reallocation overhead associated with `std::vector` while providing the O(1) element access time that `std::list` lacks.

The key idea is to combine the strengths of two standard library containers:
- `std::list<T>`: Used for efficient, O(1) insertion and deletion at the end, avoiding the need to shift elements.
- `std::unordered_map<size_t, typename std::list<T>::iterator>`: Used to map an integer index to an iterator pointing to the corresponding element in the list, enabling O(1) random access.

The implementation is contained within the `DArr.hpp` header file and is demonstrated in `main.cpp`.

## Building and Running

This is a simple C++ project that can be compiled with a standard C++ compiler like g++.

### 1. Building the Project

To compile the project, navigate to the root directory and run the following command:

```bash
g++ main.cpp -o main
```

This will create an executable file named `main`.

### 2. Running the Executable

To run the compiled program, execute the following command:

```bash
./main
```

The program will demonstrate the functionality of the `DArr` class, including adding elements, sorting, and removing elements, and print the output to the console.

## API

The `DArr` class has the following methods:
- `add(__t __val)`: Adds a single element to the end of the array.
- `addAll(std::initializer_list<__t> __vals)`: Adds multiple elements from an initializer list.
- `pop_back()`: Removes the last element.
- `shrink(size_t __new_size)`: Shrinks the array to a new size.
- `operator[]`: Provides O(1) access to elements.
- `sort()`: Sorts the array.
- `clear()`: Clears the array.
- `empty()`: Checks if the array is empty.
- `size()`: Returns the size of the array.
- `begin()` and `end()`: Provide iterators to the underlying list.

## Development Conventions

The code follows several conventions:

*   **Header-only Implementation:** The `DArr` class is fully implemented in the `DArr.hpp` header file, which is a common practice for C++ template classes.
*   **STL Usage:** The project makes extensive use of the C++ Standard Template Library (STL), particularly `std::list`, `std::unordered_map`, `std::function`, and `std::initializer_list`.
*   **Custom Sorting:** The class provides its own `sort()` method, which wraps the `std::list::sort()` member function. It supports both default ascending sort and custom comparison functions passed as lambdas.
*   **Error Handling:** Out-of-bounds access in the `operator[]` is handled by re-throwing a `std::out_of_range` exception.
*   **Namespace:** The `DArr` class is defined in the global namespace.
*   **Comments:** The code is well-documented with comments explaining the purpose of the class, its methods, and its limitations.