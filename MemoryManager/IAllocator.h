#pragma once
#include <cstddef>
#include <cstdint>

namespace Memory
{
	class IAllocator
	{
	public:
		IAllocator(const std::size_t sizeBytes, void* const start) noexcept;
		IAllocator(const IAllocator&) = delete;
		IAllocator& operator=(IAllocator&) = delete;
		IAllocator(IAllocator&&) noexcept;
		IAllocator& operator=(IAllocator&&) noexcept;
		virtual ~IAllocator() noexcept;
	public:
		virtual void* Allocate(const std::size_t& size, const std::uintptr_t& alignment = sizeof(std::intptr_t)) = 0;
		virtual void Free(void* const ptr) = 0;
	public:
		const std::size_t& GetSize() const noexcept;
		const std::size_t& GetUsed() const noexcept;
		const std::size_t& GetNumAllocation() const noexcept;
		const void* GetStart() const noexcept;
	protected:
		std::size_t m_size;
		std::size_t m_usedBytes;
		std::size_t m_numAllocations;
		void* m_start;
	};
}

