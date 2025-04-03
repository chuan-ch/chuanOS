#include "hal/keyboard.h"
#include "hal/portio.h"
#include "hal/screen.h"
#include "hal/idt.h"

bool Keyboard::shift_pressed = false;
bool Keyboard::caps_lock = false;

const uint8_t Keyboard::scan_codes[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-',
    '4', '5', '6', '+', '1', '2', '3', '0', '.'
};

const uint8_t Keyboard::shift_scan_codes[] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-',
    '4', '5', '6', '+', '1', '2', '3', '0', '.'
};

void Keyboard::initialize() {
    IDT::set_interrupt_handler(0x21, (uint32_t)interrupt_handler);
    PortIO::outb(0x21, PortIO::inb(0x21) & 0xFD);
    Screen::write("Keyboard initialized\n");
}

char Keyboard::get_char() {
    while(true) {
        uint8_t scan_code = PortIO::inb(0x60);
        if(!(scan_code & 0x80)) {
            char c = scan_code_to_ascii(scan_code);
            if(c) return c;
        }
    }
}

uint8_t Keyboard::scan_code_to_ascii(uint8_t scan_code) {
    if(scan_code >= sizeof(scan_codes)) return 0;
    bool shift = shift_pressed ^ caps_lock;
    return shift ? shift_scan_codes[scan_code] : scan_codes[scan_code];
}