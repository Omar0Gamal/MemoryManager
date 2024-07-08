#include "IAllocator.h"
#include <cassert>

namespace Memory
{
    IAllocator::IAllocator(const std::size_t sizeBytes, void* const start) noexcept
        :
        m_size(sizeBytes),
        m_usedBytes(0),
        m_numAllocations(0),
        m_start(start)
    {
        assert(sizeBytes > 0);
    }
    IAllocator::IAllocator(IAllocator&& other) noexcept
        : m_size(other.m_size),
        m_usedBytes(other.m_usedBytes),
        m_numAllocations(other.m_numAllocations),
        m_start(other.m_start)
    {
        other.m_start = nullptr;
        other.m_size = 0;
        other.m_numAllocations = 0;
        other.m_usedBytes = 0;
    }
    IAllocator& IAllocator::operator=(IAllocator&& rhs) noexcept
    {
        m_size = rhs.m_size;
        m_usedBytes = rhs.m_usedBytes;
        m_numAllocations = rhs.m_numAllocations;
        m_start = rhs.m_start;

        rhs.m_start = nullptr;
        rhs.m_size = 0;
        rhs.m_numAllocations = 0;
        rhs.m_usedBytes = 0;

        return *this;
    }
    IAllocator::~IAllocator() noexcept
    {
        assert(m_numAllocations == 0 && m_usedBytes == 0);
    }
    const std::size_t& IAllocator::GetSize() const noexcept
    {
        return m_size;
    }
    const std::size_t& IAllocator::GetUsed() const noexcept
    {
        return m_usedBytes;
    }
    const std::size_t& IAllocator::GetNumAllocation() const noexcept
    {
        return m_numAllocations;
    }
    const void* IAllocator::GetStart() const noexcept
    {
        return m_start;
    }
}