extern "C" {
    #include <stdint.h>
    void kernel_main() {
        // 初始化各子系统
        Screen::initialize();
        MemoryManager::initialize();
        Keyboard::initialize();
        
        Screen::write("MyOS Kernel Ready\n");
        Screen::write("Press keys to test (ESC to quit)...\n> ");
        
        // 简单shell循环
        while(true) {
            char c = Keyboard::get_char();
            if(c == '\n') {
                Screen::write("\n> ");
            } else if(c == 0x1B) { // ESC
                Screen::write("\nShutting down...\n");
                break;
            } else {
                Screen::put_char(c);
            }
        }
        
        while(1) __asm__("hlt");
    }
}