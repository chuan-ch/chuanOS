[org 0x7C00]
[bits 16]

section .text
global _start

_start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    
    mov [boot_drive], dl
    
    mov si, boot_msg
    call print_string
    
    ; 加载内核到内存0x10000处
    mov bx, 0x1000
    mov es, bx
    xor bx, bx
    
    mov ah, 0x02
    mov al, 16      ; 读取16个扇区(8KB)
    mov ch, 0       ; 柱面0
    mov cl, 2       ; 从扇区2开始(扇区1是MBR)
    mov dh, 0       ; 磁头0
    mov dl, [boot_drive]
    int 0x13
    jc disk_error
    
    cmp al, 16
    jne disk_error
    
    ; 获取内存信息
    mov ax, 0xE820
    mov edx, 0x534D4150 ; 'SMAP'
    mov ecx, 24         ; 缓冲区大小
    mov di, _memory_map ; 缓冲区地址
    xor ebx, ebx

memory_map_loop:
    int 0x15
    jc memory_map_done
    add di, 24
    inc word [_memory_map_count]
    test ebx, ebx
    jnz memory_map_loop

memory_map_done:
    jmp 0x1000:0x0000

print_string:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp print_string
.done:
    ret

disk_error:
    mov si, error_msg
    call print_string
    jmp $

; 数据区放在代码之后以减少填充计算误差
boot_msg db "Booting MyOS...", 0x0D, 0x0A, 0
error_msg db "Disk Error!", 0x0D, 0x0A, 0
boot_drive db 0
_memory_map_count dw 0
_memory_map times 20*24 db 0

; 确保引导扇区正好512字节
times 510-($-$$) db 0  ; 填充剩余空间
dw 0xAA55              ; 引导扇区结束标志
