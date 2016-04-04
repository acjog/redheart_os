    org 0x7c00
     
    STACKTOP   equ 0x7c00

    START_SEC equ 0x0
    BUFF_SEG   equ 0x7c1
    BUFF_OFFSET equ 0x0
    STRUCT_SIZE equ 0x10
    READ_SECTORS equ 129
    LONG_BUFFOFFSET equ 0

    DISKID equ 0x80
    RETSET_OP equ 0
    READ_OP  equ 0x2
    READ_EXT_OP equ 0x42

start:
    mov dl, DISKID
    mov ah, RETSET_OP
    int 0x13

    mov ax, cs
    mov es, ax

    cli
    mov ss, ax
    mov sp, STACKTOP
    sti

    push dword LONG_BUFFOFFSET
    push dword START_SEC
    push word BUFF_SEG
    push word BUFF_OFFSET
    push word READ_SECTORS
    push word STRUCT_SIZE

    mov ah, READ_EXT_OP
    mov dl, DISKID
    mov si, sp
    int 0x13

    nop
    nop
    nop
    times 510 - ($-$$) db 0
        db 0x55,0xaa
