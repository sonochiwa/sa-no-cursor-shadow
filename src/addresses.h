#pragma once

#include <array>
#include <cstdint>

// GTA San Andreas 1.0 US draws each front-end cursor twice inside
// CMenuManager::Draw: a grey, offset copy first, then the cursor itself. Each
// site below is the relative CALL to CSprite2d::Draw for the grey copy; the
// two stack arguments are normally removed by the __thiscall callee.
struct PatchSite {
    uintptr_t address;
    std::array<uint8_t, 5> expected;
};

// Map crosshair.
constexpr PatchSite kMapCursorShadowCall = {0x57C0BC, {0xE8, 0x8F, 0xC2, 0x1A, 0x00}};
// Standard arrow cursor.
constexpr PatchSite kArrowCursorShadowCall = {0x57C1B2, {0xE8, 0x99, 0xC1, 0x1A, 0x00}};

constexpr std::array<PatchSite, 2> kShadowDrawCalls = {
    kMapCursorShadowCall,
    kArrowCursorShadowCall,
};

// add esp, 8; nop; nop. Discards the arguments the skipped callee would have
// popped.
constexpr std::array<uint8_t, 5> kDiscardArguments = {0x83, 0xC4, 0x08, 0x90, 0x90};
