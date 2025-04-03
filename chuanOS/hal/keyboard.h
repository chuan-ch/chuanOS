#pragma once

#include <stdint.h>

class Keyboard {
public:
    static void initialize();
    static char get_char();
    
private:
    static void interrupt_handler();
    static uint8_t scan_code_to_ascii(uint8_t scan_code);
    
    static bool shift_pressed;
    static bool caps_lock;
    
    static const uint8_t scan_codes[];
    static const uint8_t shift_scan_codes[];
};