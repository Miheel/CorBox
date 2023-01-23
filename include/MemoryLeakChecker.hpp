#ifndef MEMORY_LEAK_CHECKER_HPP
#define MEMORY_LEAK_CHECKER_HPP

/**
 * @file MemoryLeakChecker.hpp
 * @author Andreas Jonasson
 * @brief Used in the course DV1581 to give a platform-independent way to find memoryleaks.
 * @version 0.1
 * @date 2018-11-01
 * 
 * This file provides a _GLOBAL_ redefine of the new, new[], delete, and delete[] operators.
 * Each time one of these functions are called, 
 * the calling location will be stored together with the associated pointer.
 * At the end of your program, call "DumpInformation()" to show all allocated memory that are yet to be freed.
 * Note! If your memory is in a static variable, it will not be freed before after the DumpInformation()
 * has run. To work around this, write your main like:
 * 
 * int main(int argc, char **argv)
 * {
 *     {
 *         //Normal code inside of the inner block
 *     }
 *     MemoryLeakChecker::DumpInformation();
 * }
 * 
 * This will ensure that the local variables (living inside the inner block) have their memory properly checked.
 * 
 * This header _MUST_ be placed _LAST_ in your include list, and it is _STRONGLY_ recommended that _ALL_ of your
 * header files involved in a project using this header file has the compiler directive "#undef new" directly
 * after the header include guard!
 * 
 * This header _MUST_ be placed in _ALL_ headers that are allocating memory to be efficient. Make sure to 
 * _ALWAYS_ include it in the source file including your entry point function as the _LAST_ header!
 * 
 */

#include <cstdlib>
#include <algorithm>
#include <string>
#include <vector>
#include <new>
#include <csignal>
#include <iostream>
#include <iomanip>
#include <cstddef>

namespace MemoryLeakChecker
{
typedef void *PointerVariable;

struct AllocationInformation
{
    PointerVariable memoryAddress;
    std::string file;
    unsigned int line;
    std::size_t allocationIndex;
    bool free;

    bool operator==(PointerVariable address)
    {
        return memoryAddress == address;
    };
};

extern std::vector<AllocationInformation> allocations;
static std::size_t numberOfAllocations = 0;
static bool callingPushBack = false;

inline bool CheckIfExistingAndRemark(PointerVariable ptr)
{
    if(callingPushBack)
        return false;
    for (auto &allocation : allocations)
    {
        if (allocation == ptr)
        {
            allocation.free = false;
            allocation.file = "Unknown";
            allocation.line = 0;
            allocation.allocationIndex = numberOfAllocations++;
            return true;
        }
    }
    return false;
}

inline bool CheckIfExistingAndRemark(PointerVariable ptr, char const *filename, unsigned int fileline)
{ 
    if(callingPushBack)
        return false;
    for (auto &allocation : allocations)
    {
        if (allocation == ptr)
        {
            allocation.free = false;
            allocation.file = filename;
            allocation.line = fileline;
            allocation.allocationIndex = numberOfAllocations++;
            return true;
        }
    }
    return false;
}

inline void *MemoryLeakCheckingNew(std::size_t sizeToAllocate, char const *filename, unsigned int fileline)
{
    PointerVariable temp = std::malloc(sizeToAllocate);
    if(callingPushBack)
        return temp;
    if (!CheckIfExistingAndRemark(temp, filename, fileline))
    {
        AllocationInformation newAllocation;
        newAllocation.memoryAddress = temp;
        newAllocation.file = filename;
        newAllocation.line = fileline;
        newAllocation.allocationIndex = numberOfAllocations++;
        newAllocation.free = false;
        
        //Don't want to mark internal memory
        callingPushBack = true;
        allocations.push_back(newAllocation);
        callingPushBack = false;
    }

    return temp;
}

inline void DumpInformation(bool leaksOnly = false)
{
    std::cout << std::endl;
    std::cout << "!Start of memory dump!" << std::endl;
    std::cout << "Allocations: " << allocations.size() << std::endl;
    for (const auto &allocation : allocations)
    {
        if (!leaksOnly || !allocation.free)
        {
            std::cout << "Addr: " << std::hex << allocation.memoryAddress << " allocated on line " << std::dec << allocation.line << " in file " << allocation.file;
            if (!allocation.free)
                std::cout << " Status: Leak" << std::endl;
            else
                std::cout << " Status: Free" << std::endl;
        }
    }
    std::cout << "!End of memory dump!" << std::endl;
}

} // namespace MemoryLeakChecker
void *operator new(size_t sizeToAllocate);
void operator delete(void *ptr, size_t allocationSize) noexcept;
void *operator new[](size_t sizeToAllocate);
void operator delete[](void *ptr, size_t allocationSize) noexcept;
void *operator new(std::size_t sizeToAllocate, char const *fileName, unsigned int fileLine);
void *operator new[](std::size_t sizeToAllocate, char const *fileName, unsigned int fileLine);
void *operator new(std::size_t sizeToAllocate, const std::nothrow_t &tag, char const *fileName, unsigned int fileLine) noexcept;
void *operator new[](std::size_t sizeToAllocate, const std::nothrow_t &tag, char const *fileName, unsigned int fileLine) noexcept;
void operator delete(void *ptr) noexcept;
/**/
void operator delete(void *ptr, char const *fileName, unsigned int fileLine) noexcept;

/**/
void operator delete[](void *ptr) noexcept;

#endif
#ifndef new
#define new new (__FILE__, __LINE__)
#endif