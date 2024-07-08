#pragma once
#include "LinearAllocator.h"
#include "FreeListAllocator.h"
#include <memory>

#define defaultSize 100 * 1024 * 1024

namespace Memory
{
	template<typename T>
	class MemoryManager
	{
	public:
		MemoryManager() = delete;
		MemoryManager(const MemoryManager&) = delete;
		MemoryManager& operator=(const MemoryManager&) = delete;
		MemoryManager(MemoryManager&&) = delete;
		MemoryManager& operator=(MemoryManager&&) = delete;
		MemoryManager(const std::size_t sizeBytes = defaultSize) noexcept;
		~MemoryManager() noexcept;
	public:
		std::shared_ptr<T> getAllocator() const noexcept;
	private:
		std::shared_ptr<T> m_allocator;
	};
	template<typename T>
	inline MemoryManager<T>::MemoryManager(const std::size_t sizeBytes) noexcept
	{
		void* const start = malloc(sizeBytes);
		m_allocator = std::make_shared<T>(sizeBytes, start);
	}
	template<typename T>
	inline MemoryManager<T>::~MemoryManager() noexcept
	{
		m_allocator.reset();
	}
	template<typename T>
	inline std::shared_ptr<T> MemoryManager<T>::getAllocator() const noexcept
	{
		return m_allocator;
	}
}

