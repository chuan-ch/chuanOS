#include "hal/memory.h"
#include "hal/screen.h"

uint32_t MemoryManager::total_memory = 0;
uint32_t MemoryManager::used_memory = 0;
uint8_t* MemoryManager::bitmap = nullptr;
uint32_t MemoryManager::bitmap_size = 0;

extern "C" {
    extern uint32_t _memory_map_count;
    extern uint8_t _memory_map[];
}

void MemoryManager::initialize() {
    total_memory = detect_memory();
    bitmap_size = (total_memory / 4096) / 8 + 1;
    bitmap = (uint8_t*)0x100000;
    
    for(uint32_t i = 0; i < bitmap_size; i++)
        bitmap[i] = 0;
    
    // 标记可用内存区域
    MemoryRegion* regions = (MemoryRegion*)_memory_map;
    for(uint32_t i = 0; i < _memory_map_count; i++) {
        if(regions[i].type == MEMORY_USABLE) {
            mark_region(regions[i].base_low, regions[i].length_low, true);
        }
    }
    
    Screen::writef("Memory: %d MB initialized\n", total_memory / 1024 / 1024);
}

void* MemoryManager::allocate(size_t size) {
    uint32_t pages = (size + 4095) / 4096;
    
    for(uint32_t i = 0; i < bitmap_size * 8; i++) {
        if(!(bitmap[i/8] & (1 << (i%8)))) {
            bool found = true;
            for(uint32_t j = 1; j < pages; j++) {
                if(i+j >= bitmap_size*8 || (bitmap[(i+j)/8] & (1 << ((i+j)%8)))) {
                    found = false;
                    break;
                }
            }
            
            if(found) {
                for(uint32_t j = 0; j < pages; j++) {
                    bitmap[(i+j)/8] |= (1 << ((i+j)%8));
                }
                used_memory += pages * 4096;
                return (void*)(i * 4096 + 0x100000);
            }
        }
    }
    
    return nullptr;
}

void MemoryManager::free(void* ptr) {
    uint32_t page = ((uint32_t)ptr - 0x100000) / 4096;
    if(page < bitmap_size * 8) {
        bitmap[page/8] &= ~(1 << (page%8));
        used_memory -= 4096;
    }
}