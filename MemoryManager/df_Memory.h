#pragma once

#include "IAllocator.h"
#include "FreeListAllocator.h"
#include "LinearAllocator.h"

#include "MemoryManager.h"
#include "STLAdaptor.h"

#include <vector>
#include <unordered_map>
#include <string>

template<typename T, typename Alloc>
using vector = std::vector<T, Memory::STLAdaptor<T, Alloc>>;