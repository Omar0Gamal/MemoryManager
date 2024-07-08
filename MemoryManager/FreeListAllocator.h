#pragma once
#include "IAllocator.h"

namespace Memory
{
	struct FreeBlock
	{
		std::size_t size;
		FreeBlock* next;
	};

	struct AllocationHeader
	{
		std::size_t size;
		std::uintptr_t adjustment;
	};

	class FreeListAllocator : public IAllocator
	{
	public:
		FreeListAllocator(const std::size_t sizeBytes, void* const start) noexcept;
		FreeListAllocator(const FreeListAllocator&) = delete;
		FreeListAllocator& operator=(const FreeListAllocator&) = delete;
		FreeListAllocator(FreeListAllocator&&) noexcept;
		FreeListAllocator& operator=(FreeListAllocator&&) noexcept;
		virtual ~FreeListAllocator() noexcept;
	public:
		virtual void* Allocate(const std::size_t& size, const std::uintptr_t& alignment = sizeof(std::intptr_t)) override;
		virtual void Free(void* const ptr) noexcept override final;
	protected:
		FreeBlock* m_freeBlocks;
	private:
		template<typename T>
		inline std::size_t align_forward_adjustment_with_header(const FreeBlock* ptr, const std::uintptr_t& alignment) noexcept
		{
			const auto iptr = reinterpret_cast<std::uintptr_t>(ptr);
			const auto adjusted_ptr = iptr + sizeof(T);
			const auto aligned = static_cast<std::intptr_t>(iptr - 1u + alignment) & -static_cast<std::intptr_t>(alignment);
			return aligned - iptr;
		}

		inline void* ptr_add(const void* const p, const std::uintptr_t& amount) noexcept
		{
			return reinterpret_cast<void*> (reinterpret_cast<std::uintptr_t>(p) + amount);
		}

		inline void* ptr_sub(const void* const p, const std::uintptr_t& amount) noexcept
		{
			return reinterpret_cast<void*> (reinterpret_cast<std::uintptr_t>(p) - amount);
		}
	};
}
