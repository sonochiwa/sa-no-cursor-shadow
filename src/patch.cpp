#include "patch.h"

#include <windows.h>

#include <cstring>

bool IsReadable(uintptr_t address, size_t size) {
    MEMORY_BASIC_INFORMATION info = {};
    if (VirtualQuery(reinterpret_cast<const void*>(address), &info, sizeof(info)) != sizeof(info)) {
        return false;
    }

    constexpr DWORD readable = PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY |
                               PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE |
                               PAGE_EXECUTE_WRITECOPY;
    if (info.State != MEM_COMMIT || (info.Protect & readable) == 0) {
        return false;
    }

    const auto end = reinterpret_cast<uintptr_t>(info.BaseAddress) + info.RegionSize;
    return address <= end && size <= end - address;
}

bool WriteMemory(uintptr_t address, const void* data, size_t size) {
    auto* destination = reinterpret_cast<void*>(address);
    DWORD oldProtect = 0;
    if (!VirtualProtect(destination, size, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        return false;
    }

    std::memcpy(destination, data, size);
    FlushInstructionCache(GetCurrentProcess(), destination, size);

    DWORD ignored = 0;
    VirtualProtect(destination, size, oldProtect, &ignored);
    return true;
}
