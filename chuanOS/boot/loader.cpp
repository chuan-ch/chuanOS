extern "C" {
    #include <stdint.h>
    void init_gdt();
    void init_idt();
    void kernel_main();
    
    // 从引导加载器传递的内存信息
    extern uint32_t _memory_map_count;
    extern uint8_t _memory_map[20*24];
}

struct MemoryRegion {
    uint32_t base_low;
    uint32_t base_high;
    uint32_t length_low;
    uint32_t length_high;
    uint32_t type;
    uint32_t acpi_extended;
};

extern "C" void __main() {
    init_gdt();
    init_idt();
    
    // 设置简单的屏幕输出
    volatile char* video = (volatile char*)0xB8000;
    video[0] = 'O';
    video[1] = 0x0F;
    video[2] = 'K';
    video[3] = 0x0F;
    
    kernel_main();
    
    while(1) __asm__("hlt");
}