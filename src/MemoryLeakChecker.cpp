#include "MemoryLeakChecker.hpp"

#undef new
std::vector<MemoryLeakChecker::AllocationInformation> MemoryLeakChecker::allocations;

void MarkAsDeallocated(void* ptr)
{
    bool foundNonDeleted = false;
    MemoryLeakChecker::AllocationInformation* allocationFound = nullptr;

    for(auto& allocation : MemoryLeakChecker::allocations)
    {
        if(allocation == ptr)
        {
            if(!allocation.free)
            {
                allocation.free = true;
                foundNonDeleted = true;
                break;
            }
            else
            {
                allocationFound = &allocation;
            }
        }
    }
    if(!foundNonDeleted && allocationFound)
    {
        std::cerr << "Error on attempt to delete " << ptr;
        
        std::cerr << " allocated on line " << allocationFound->line << " in file " << allocationFound->file;
        
        std::cerr << std::endl;
    }
}

void* operator new(size_t sizeToAllocate)
{
    //Called from outside of our control (standard library headers)
    void* temp = std::malloc(sizeToAllocate);
    MemoryLeakChecker::CheckIfExistingAndRemark(temp);
    return temp;
}
void operator delete(void* ptr, size_t) noexcept
{
    //Called from outside of our control (standard library headers)
    MarkAsDeallocated(ptr);
    std::free(ptr);
}

void* operator new[](size_t sizeToAllocate)
{
    //Called from outside of our control (standard library headers)
    void* temp = std::malloc(sizeToAllocate);
    MemoryLeakChecker::CheckIfExistingAndRemark(temp);
    return temp;
}
void operator delete[](void* ptr, size_t) noexcept
{
    //Called from outside of our control (standard library headers)
    MarkAsDeallocated(ptr);
    std::free(ptr);
}

void* operator new(std::size_t sizeToAllocate, char const* fileName, unsigned int fileLine){  
    return MemoryLeakChecker::MemoryLeakCheckingNew(sizeToAllocate, fileName, fileLine);
}

void* operator new[](std::size_t sizeToAllocate, char const* fileName, unsigned int fileLine){  
    return MemoryLeakChecker::MemoryLeakCheckingNew(sizeToAllocate, fileName, fileLine);
}

void* operator new  ( std::size_t sizeToAllocate, const std::nothrow_t&, char const* fileName, unsigned int fileLine) noexcept
{
    try
    {
        return MemoryLeakChecker::MemoryLeakCheckingNew(sizeToAllocate, fileName, fileLine);
    }
    catch(std::exception*)
    {
        return nullptr;
    }
}
void* operator new[]( std::size_t sizeToAllocate, const std::nothrow_t&, char const* fileName, unsigned int fileLine) noexcept
{
    try
    {
        return MemoryLeakChecker::MemoryLeakCheckingNew(sizeToAllocate, fileName, fileLine);
    }
    catch(std::exception*)
    {
        return nullptr;
    }
}

void operator delete(void* ptr) noexcept{

}

void operator delete(void * ptr, char const * fileName, unsigned int fileLine) noexcept
{    
	MarkAsDeallocated(ptr);
    std::free(ptr);
}

void operator delete[](void* ptr) noexcept{
    MarkAsDeallocated(ptr);
    std::free(ptr);
}

#define new new(__FILE__, __LINE__)
