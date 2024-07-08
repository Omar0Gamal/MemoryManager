#include <iostream>

#include "df_Memory.h"


int main()
{
	Memory::MemoryManager<Memory::LinearAllocator> manager(100 * 1024 * 1024);
	auto allocator = manager.getAllocator();
	return 0;
}