#include "hal/screen.h"
#include "hal/portio.h"

volatile char* Screen::video_memory = (volatile char*)0xB8000;
int Screen::cursor_x = 0;
int Screen::cursor_y = 0;

void Screen::initialize() {
    // 清屏
    for(int i = 0; i < 80*25*2; i += 2) {
        video_memory[i] = ' ';
        video_memory[i+1] = 0x0F;
    }
    cursor_x = cursor_y = 0;
}

void Screen::put_char(char c) {
    if(c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if(c == '\r') {
        cursor_x = 0;
    } else if(c == '\t') {
        cursor_x = (cursor_x + 8) & ~7;
    } else {
        int pos = (cursor_y * 80 + cursor_x) * 2;
        video_memory[pos] = c;
        video_memory[pos+1] = 0x0F;
        cursor_x++;
    }
    
    if(cursor_x >= 80) {
        cursor_x = 0;
        cursor_y++;
    }
    
    if(cursor_y >= 25) {
        scroll();
        cursor_y = 24;
    }
    
    move_cursor();
}

void Screen::write(const char* str) {
    while(*str) {
        put_char(*str++);
    }
}

void Screen::scroll() {
    // 上移一行
    for(int y = 0; y < 24; y++) {
        for(int x = 0; x < 80; x++) {
            int src = (y + 1) * 80 + x;
            int dst = y * 80 + x;
            video_memory[dst * 2] = video_memory[src * 2];
            video_memory[dst * 2 + 1] = video_memory[src * 2 + 1];
        }
    }
    
    // 清空最后一行
    for(int x = 0; x < 80; x++) {
        video_memory[24 * 80 * 2 + x * 2] = ' ';
        video_memory[24 * 80 * 2 + x * 2 + 1] = 0x0F;
    }
}

void Screen::move_cursor() {
    uint16_t pos = cursor_y * 80 + cursor_x;
    PortIO::outb(0x3D4, 0x0F);
    PortIO::outb(0x3D5, (uint8_t)(pos & 0xFF));
    PortIO::outb(0x3D4, 0x0E);
    PortIO::outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}