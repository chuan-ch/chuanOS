#pragma once

#include <stdint.h>
#include <stddef.h>

enum MemoryType {
    MEMORY_USABLE = 1,
    MEMORY_RESERVED,
    MEMORY_ACPI_RECLAIMABLE,
    MEMORY_NVS,
    MEMORY_BADRAM
};

class MemoryManager {
public:
    static void initialize();
    static void* allocate(size_t size);
    static void free(void* ptr);
    
private:
    static uint32_t detect_memory();
    static void init_bitmap();
    static void mark_region(uint32_t base, uint32_t length, bool available);
    
    static uint32_t total_memory;
    static uint32_t used_memory;
    static uint8_t* bitmap;
    static uint32_t bitmap_size;
};