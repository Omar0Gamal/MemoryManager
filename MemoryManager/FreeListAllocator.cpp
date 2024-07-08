#include "FreeListAllocator.h"
#include <cassert>
#include <utility>
#include <new>

namespace Memory
{
	FreeListAllocator::FreeListAllocator(const std::size_t sizeBytes, void* const start) noexcept
		: IAllocator(sizeBytes, start), m_freeBlocks(static_cast<FreeBlock*>(start))
	{
		m_freeBlocks->size = sizeBytes;
		m_freeBlocks->next = nullptr;

	}
	FreeListAllocator::FreeListAllocator(FreeListAllocator&& other) noexcept
		: IAllocator(std::move(other)), m_freeBlocks(other.m_freeBlocks)
	{
		other.m_freeBlocks = nullptr;
	}
	FreeListAllocator& FreeListAllocator::operator=(FreeListAllocator&& rhs) noexcept
	{
		IAllocator::operator=(std::move(rhs));
		m_freeBlocks = rhs.m_freeBlocks;
		rhs.m_freeBlocks = nullptr;
		return *this;
	}
	FreeListAllocator::~FreeListAllocator() noexcept
	{
		m_freeBlocks = nullptr;
	}
	void* FreeListAllocator::Allocate(const std::size_t& size, const std::uintptr_t& alignment)
	{
        assert(size > 0 && alignment > 0);

        FreeBlock* prevFreeBlock = nullptr;
        FreeBlock* freeBlock = m_freeBlocks;

        FreeBlock* bestFitPrev = nullptr;
        FreeBlock* bestFit = nullptr;

        std::uintptr_t bestFitAdjustment = 0;
        std::size_t bestFitTotalSize = 0;

        while (freeBlock != nullptr)
        {
            std::uintptr_t adjustment = align_forward_adjustment_with_header<AllocationHeader>(freeBlock, alignment);

            std::size_t totalSize = size + adjustment;

            if (freeBlock->size > totalSize &&
                (bestFit == nullptr || freeBlock->size < bestFit->size))
            {
                bestFitPrev = prevFreeBlock;
                bestFit = freeBlock;
                bestFitAdjustment = adjustment;
                bestFitTotalSize = totalSize;

                if (freeBlock->size == totalSize)
                {
                    break;
                }
            }

            prevFreeBlock = freeBlock;
            freeBlock = freeBlock->next;
        }

        if (bestFit == nullptr)
        {
            throw std::bad_alloc();
        }

        if (bestFit->size - bestFitTotalSize <= sizeof(AllocationHeader))
        {
            bestFitTotalSize = bestFit->size;

            if (bestFitPrev != nullptr)
                bestFitPrev->next = bestFit->next;
            else
                m_freeBlocks = bestFit->next;
        }
        else
        {
            assert(bestFitTotalSize > sizeof(FreeBlock));

            FreeBlock* newBlock = reinterpret_cast<FreeBlock*>(ptr_add(bestFit, bestFitTotalSize));

            newBlock->size = bestFit->size - bestFitTotalSize;
            newBlock->next = bestFit->next;

            if (bestFitPrev != nullptr)
                bestFitPrev->next = newBlock;
            else
                m_freeBlocks = newBlock;
        }

        std::uintptr_t alignedAddr = reinterpret_cast<std::uintptr_t>(bestFit) + bestFitAdjustment;

        AllocationHeader* header = reinterpret_cast<AllocationHeader*>(alignedAddr - sizeof(AllocationHeader));
        header->size = bestFitTotalSize;
        header->adjustment = bestFitAdjustment;

        m_usedBytes += bestFitTotalSize;
        ++m_numAllocations;

        return reinterpret_cast<void*>(alignedAddr);
    }
	void FreeListAllocator::Free(void* const ptr) noexcept
	{
        assert(ptr != nullptr);

        AllocationHeader* header = reinterpret_cast<AllocationHeader*>(ptr_sub(ptr, sizeof(AllocationHeader)));

        std::uintptr_t blockStart = reinterpret_cast<std::uintptr_t>(ptr) - header->adjustment;
        std::size_t blockSize = header->size;
        std::uintptr_t blockEnd = blockStart + blockSize;

        FreeBlock* prevFreeBlock = nullptr;
        FreeBlock* freeBlock = m_freeBlocks;

        while (freeBlock != nullptr)
        {
            if (reinterpret_cast<std::uintptr_t>(freeBlock) >= blockEnd)
            {
                break;
            }

            prevFreeBlock = freeBlock;
            freeBlock = freeBlock->next;
        }

        if (prevFreeBlock == nullptr)
        {
            prevFreeBlock = reinterpret_cast<FreeBlock*>(blockStart);
            prevFreeBlock->size = blockSize;
            prevFreeBlock->next = m_freeBlocks;

            m_freeBlocks = prevFreeBlock;
        }
        else if (reinterpret_cast<std::uintptr_t>(prevFreeBlock)
            + prevFreeBlock->size == blockStart)
        {
            prevFreeBlock->size += blockSize;
        }
        else
        {
            FreeBlock* temp = reinterpret_cast<FreeBlock*>(blockStart);
            temp->size = blockSize;
            temp->next = prevFreeBlock->next;

            prevFreeBlock->next = temp;
            prevFreeBlock = temp;
        }

        if (reinterpret_cast<std::uintptr_t>(prevFreeBlock) + prevFreeBlock->size
            == reinterpret_cast<std::uintptr_t>(prevFreeBlock->next))
        {
            prevFreeBlock->size += prevFreeBlock->next->size;
            prevFreeBlock->next = prevFreeBlock->next->next;
        }

        --m_numAllocations;
        m_usedBytes -= blockSize;
	}
}