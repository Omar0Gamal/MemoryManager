#pragma once
#include "IAllocator.h"

namespace Memory
{
	class LinearAllocator : public IAllocator
	{
	public:
		LinearAllocator(const std::size_t sizeBytes, void* const start) noexcept;
		LinearAllocator(const LinearAllocator&) = delete;
		LinearAllocator& operator=(const LinearAllocator&) = delete;
		LinearAllocator(LinearAllocator&&) noexcept;
		LinearAllocator& operator=(LinearAllocator&&) noexcept;
		virtual ~LinearAllocator() noexcept;
	public:
		virtual void* Allocate(const std::size_t& size,const std::uintptr_t& alignment = sizeof(std::intptr_t)) override;
		virtual void Free(void* const ptr) noexcept override final;
		virtual void Rewind(void* const mark) noexcept;
		virtual void Clear() noexcept;
	public:
		void* GetCurrent() const noexcept;
	protected:
		void* m_current;
	private:
		inline std::size_t align_forward_adjustment(const void* const ptr, const std::size_t& alignment) noexcept
		{
			const auto iptr = reinterpret_cast<std::uintptr_t>(ptr);
			const auto aligned = (iptr - 1u + static_cast<std::uintptr_t>(alignment)) & -static_cast<std::intptr_t>(alignment);
			return aligned - iptr;
		}

		inline void* ptr_add(const void* const p, const std::uintptr_t& amount) noexcept
		{
			return reinterpret_cast<void*> (reinterpret_cast<std::uintptr_t>(p) + amount);
		}
	};
}
