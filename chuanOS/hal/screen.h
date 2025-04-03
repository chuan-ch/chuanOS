#pragma once

class Screen {
public:
    static void initialize();
    static void write(const char* str);
    static void writef(const char* fmt, ...);
    static void put_char(char c);
    
private:
    static volatile char* video_memory;
    static int cursor_x;
    static int cursor_y;
    
    static void scroll();
    static void move_cursor();
};