#include <windows.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>

namespace {

// GTA San Andreas 1.0 US has separate map-crosshair and standard-cursor
// branches. Each relative CALL draws the grey, offset cursor rectangle. The
// two stack arguments are normally removed by the __thiscall callee.
struct PatchSite {
    uintptr_t address;
    std::array<uint8_t, 5> expected;
};

constexpr std::array<PatchSite, 2> kShadowDrawCalls = {{
    {0x57C0BC, {0xE8, 0x8F, 0xC2, 0x1A, 0x00}}, // map crosshair
    {0x57C1B2, {0xE8, 0x99, 0xC1, 0x1A, 0x00}}, // standard cursor
}};
constexpr std::array<uint8_t, 5> kDiscardArguments = {
    0x83, 0xC4, 0x08, 0x90, 0x90, // add esp, 8; nop; nop
};

bool IsReadable(uintptr_t address, size_t size) {
    MEMORY_BASIC_INFORMATION info = {};
    if (VirtualQuery(reinterpret_cast<const void*>(address), &info,
                     sizeof(info)) != sizeof(info)) {
        return false;
    }

    constexpr DWORD readable = PAGE_READONLY | PAGE_READWRITE |
                               PAGE_WRITECOPY | PAGE_EXECUTE_READ |
                               PAGE_EXECUTE_READWRITE |
                               PAGE_EXECUTE_WRITECOPY;
    if (info.State != MEM_COMMIT || (info.Protect & readable) == 0) {
        return false;
    }

    const auto end = reinterpret_cast<uintptr_t>(info.BaseAddress) +
                     info.RegionSize;
    return address <= end && size <= end - address;
}

bool WriteMemory(uintptr_t address, const void* data, size_t size) {
    auto* destination = reinterpret_cast<void*>(address);
    DWORD oldProtect = 0;
    if (!VirtualProtect(destination, size, PAGE_EXECUTE_READWRITE,
                        &oldProtect)) {
        return false;
    }

    std::memcpy(destination, data, size);
    FlushInstructionCache(GetCurrentProcess(), destination, size);

    DWORD ignored = 0;
    VirtualProtect(destination, size, oldProtect, &ignored);
    return true;
}

DWORD WINAPI ApplyPatch(LPVOID) {
    for (const auto& site : kShadowDrawCalls) {
        if (!IsReadable(site.address, site.expected.size())) {
            return 0;
        }
        const auto* code = reinterpret_cast<const uint8_t*>(site.address);
        if (!std::equal(site.expected.begin(), site.expected.end(), code)) {
            return 0;
        }
    }

    for (const auto& site : kShadowDrawCalls) {
        if (!WriteMemory(site.address, kDiscardArguments.data(),
                         kDiscardArguments.size())) {
            return 0;
        }
    }
    return 0;
}

} // namespace

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(instance);
        const HANDLE thread = CreateThread(nullptr, 0, ApplyPatch, nullptr, 0,
                                           nullptr);
        if (thread) {
            CloseHandle(thread);
        }
    }
    return TRUE;
}
