// The game draws every front-end cursor twice: a translucent grey copy shifted
// down and right, then the cursor itself. Both draws are CSprite2d::Draw calls
// inside CMenuManager::Draw. The plugin replaces the CALL of the grey copy at
// each of the two cursor sites with a stack cleanup, so the shadow is never
// drawn and the cursor draw that follows is untouched. Nothing else in the
// executable changes, and the patch is skipped unless both sites hold exactly
// the expected bytes.

#include "addresses.h"
#include "patch.h"

#include <windows.h>

#include <algorithm>

namespace {

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
        if (!WriteMemory(site.address, kDiscardArguments.data(), kDiscardArguments.size())) {
            return 0;
        }
    }
    return 0;
}

} // namespace

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(instance);
        const HANDLE thread = CreateThread(nullptr, 0, ApplyPatch, nullptr, 0, nullptr);
        if (thread) {
            CloseHandle(thread);
        }
    }
    return TRUE;
}
