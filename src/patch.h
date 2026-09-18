#pragma once

#include <cstddef>
#include <cstdint>

bool IsReadable(uintptr_t address, size_t size);
bool WriteMemory(uintptr_t address, const void* data, size_t size);
