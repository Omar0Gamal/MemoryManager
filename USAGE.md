# df_Memory Usage Guide

Welcome to the df_Memory repository! This simple memory manager and allocator, written in C++, is designed to streamline your future projects. Let's get started on how to use it. 🚀

## Getting Started

First, include the header file in your application:

```cpp
#include "df_Memory.h"
```

Create a `MemoryManager` object at the start of your application and define the size of the memory you wish to manage:

```cpp
Memory::MemoryManager<Memory::LinearAllocator> manager(100 * 1024 * 1024);
```

## Allocation

To allocate memory, simply get the allocator from your manager and use it:

```cpp
auto allocator = manager.getAllocator();
int* array = (int*)allocator->Allocate(10 * sizeof(int));
```

## Initialization

You can now initialize your memory as needed:

```cpp
for (int i = 0; i < 10; i++) {
    array[i] = i;
}
```

## Deallocation

Once done, don't forget to free the allocated memory:

```cpp
allocator->Free(array);
```

## Using with STL Containers

df_Memory can also be used with STL containers like vectors:

```cpp
vector<int, Memory::LinearAllocator> vec(*allocator);
```

## Choosing Allocators

You have the option to choose between `LinearAllocator` and `FreeListAllocator`. Specify your choice when creating the `MemoryManager` object:

```cpp
Memory::MemoryManager<Memory::FreeListAllocator> manager(100 * 1024 * 1024);
```

Happy coding! 😄

---

For more detailed information, please refer to the examples provided in the repository. If you encounter any issues or have suggestions, feel free to open an issue or a pull request.

Omar Gamal
