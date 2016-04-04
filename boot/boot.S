00000000  EB63              jmp short 0x65
00000002  90                nop
00000003  D0BC007C          sar byte [si+0x7c00],1
00000007  FB                sti
00000008  50                push ax
00000009  07                pop es
0000000A  50                push ax
0000000B  1F                pop ds
0000000C  FC                cld
0000000D  BE1B7C            mov si,0x7c1b
00000010  BF1B06            mov di,0x61b
00000013  50                push ax
00000014  57                push di
00000015  B9E501            mov cx,0x1e5
00000018  F3A4              rep movsb
0000001A  CB                retf
0000001B  BDBE07            mov bp,0x7be
0000001E  B104              mov cl,0x4
00000020  386E00            cmp [bp+0x0],ch
00000023  7C09              jl 0x2e
00000025  7513              jnz 0x3a
00000027  83C510            add bp,byte +0x10
0000002A  E2F4              loop 0x20
0000002C  CD18              int 0x18
0000002E  8BF5              mov si,bp
00000030  83C610            add si,byte +0x10
00000033  49                dec cx
00000034  7419              jz 0x4f
00000036  382C              cmp [si],ch
00000038  74F6              jz 0x30
0000003A  A0B507            mov al,[0x7b5]
0000003D  B407              mov ah,0x7
0000003F  8BF0              mov si,ax
00000041  AC                lodsb
00000042  3C00              cmp al,0x0
00000044  74FC              jz 0x42
00000046  BB0700            mov bx,0x7
00000049  B40E              mov ah,0xe
0000004B  CD10              int 0x10
0000004D  EBF2              jmp short 0x41
0000004F  884E10            mov [bp+0x10],cl
00000052  E84600            call word 0x9b
00000055  732A              jnc 0x81
00000057  FE4610            inc byte [bp+0x10]
0000005A  00800100          add [bx+si+0x1],al
0000005E  0000              add [bx+si],al
00000060  0000              add [bx+si],al
00000062  0000              add [bx+si],al
00000064  FF                db 0xff
00000065  FA                cli
00000066  90                nop
00000067  90                nop
00000068  F6C280            test dl,0x80
0000006B  7405              jz 0x72
0000006D  F6C270            test dl,0x70
00000070  7402              jz 0x74
00000072  B280              mov dl,0x80
00000074  EA797C0000        jmp word 0x0:0x7c79
00000079  31C0              xor ax,ax
0000007B  8ED8              mov ds,ax
0000007D  8ED0              mov ss,ax
0000007F  BC0020            mov sp,0x2000
00000082  FB                sti
00000083  A0647C            mov al,[0x7c64]
00000086  3CFF              cmp al,0xff
00000088  7402              jz 0x8c
0000008A  88C2              mov dl,al
0000008C  52                push dx
0000008D  BB1704            mov bx,0x417
00000090  F60703            test byte [bx],0x3
00000093  7406              jz 0x9b
00000095  BE887D            mov si,0x7d88
00000098  E81701            call word 0x1b2
0000009B  BE057C            mov si,0x7c05
0000009E  B441              mov ah,0x41
000000A0  BBAA55            mov bx,0x55aa
000000A3  CD13              int 0x13
000000A5  5A                pop dx
000000A6  52                push dx
000000A7  723D              jc 0xe6
000000A9  81FB55AA          cmp bx,0xaa55
000000AD  7537              jnz 0xe6
000000AF  83E101            and cx,byte +0x1
000000B2  7432              jz 0xe6
000000B4  31C0              xor ax,ax
000000B6  894404            mov [si+0x4],ax
000000B9  40                inc ax
000000BA  8844FF            mov [si-0x1],al
000000BD  894402            mov [si+0x2],ax
000000C0  C7041000          mov word [si],0x10
000000C4  668B1E5C7C        mov ebx,[0x7c5c]
000000C9  66895C08          mov [si+0x8],ebx
000000CD  668B1E607C        mov ebx,[0x7c60]
000000D2  66895C0C          mov [si+0xc],ebx
000000D6  C744060070        mov word [si+0x6],0x7000
000000DB  B442              mov ah,0x42
000000DD  CD13              int 0x13
000000DF  7205              jc 0xe6
000000E1  BB0070            mov bx,0x7000
000000E4  EB76              jmp short 0x15c
000000E6  B408              mov ah,0x8
000000E8  CD13              int 0x13
000000EA  730D              jnc 0xf9
000000EC  5A                pop dx
000000ED  84D2              test dl,dl
000000EF  0F83D000          jnc word 0x1c3
000000F3  BE937D            mov si,0x7d93
000000F6  E98200            jmp word 0x17b
000000F9  660FB6C6          movzx eax,dh
000000FD  8864FF            mov [si-0x1],ah
00000100  40                inc ax
00000101  66894404          mov [si+0x4],eax
00000105  0FB6D1            movzx dx,cl
00000108  C1E202            shl dx,byte 0x2
0000010B  88E8              mov al,ch
0000010D  88F4              mov ah,dh
0000010F  40                inc ax
00000110  894408            mov [si+0x8],ax
00000113  0FB6C2            movzx ax,dl
00000116  C0E802            shr al,byte 0x2
00000119  668904            mov [si],eax
0000011C  66A1607C          mov eax,[0x7c60]
00000120  6609C0            or eax,eax
00000123  754E              jnz 0x173
00000125  66A15C7C          mov eax,[0x7c5c]
00000129  6631D2            xor edx,edx
0000012C  66F734            div dword [si]
0000012F  88D1              mov cl,dl
00000131  31D2              xor dx,dx
00000133  66F77404          div dword [si+0x4]
00000137  3B4408            cmp ax,[si+0x8]
0000013A  7D37              jnl 0x173
0000013C  FEC1              inc cl
0000013E  88C5              mov ch,al
00000140  30C0              xor al,al
00000142  C1E802            shr ax,byte 0x2
00000145  08C1              or cl,al
00000147  88D0              mov al,dl
00000149  5A                pop dx
0000014A  88C6              mov dh,al
0000014C  BB0070            mov bx,0x7000
0000014F  8EC3              mov es,bx
00000151  31DB              xor bx,bx
00000153  B80102            mov ax,0x201
00000156  CD13              int 0x13
00000158  721E              jc 0x178
0000015A  8CC3              mov bx,es
0000015C  60                pushaw
0000015D  1E                push ds
0000015E  B90001            mov cx,0x100
00000161  8EDB              mov ds,bx
00000163  31F6              xor si,si
00000165  BF0080            mov di,0x8000
00000168  8EC6              mov es,si
0000016A  FC                cld
0000016B  F3A5              rep movsw
0000016D  1F                pop ds
0000016E  61                popaw
0000016F  FF265A7C          jmp word [0x7c5a]
00000173  BE8E7D            mov si,0x7d8e
00000176  EB03              jmp short 0x17b
00000178  BE9D7D            mov si,0x7d9d
0000017B  E83400            call word 0x1b2
0000017E  BEA27D            mov si,0x7da2
00000181  E82E00            call word 0x1b2
00000184  CD18              int 0x18
00000186  EBFE              jmp short 0x186
00000188  47                inc di
00000189  52                push dx
0000018A  55                push bp
0000018B  42                inc dx
0000018C  2000              and [bx+si],al
0000018E  47                inc di
0000018F  656F              gs outsw
00000191  6D                insw
00000192  004861            add [bx+si+0x61],cl
00000195  7264              jc 0x1fb
00000197  204469            and [si+0x69],al
0000019A  736B              jnc 0x207
0000019C  005265            add [bp+si+0x65],dl
0000019F  61                popaw
000001A0  640020            add [fs:bx+si],ah
000001A3  45                inc bp
000001A4  7272              jc 0x218
000001A6  6F                outsw
000001A7  720D              jc 0x1b6
000001A9  0A00              or al,[bx+si]
000001AB  BB0100            mov bx,0x1
000001AE  B40E              mov ah,0xe
000001B0  CD10              int 0x10
000001B2  AC                lodsb
000001B3  3C00              cmp al,0x0
000001B5  75F4              jnz 0x1ab
000001B7  C3                ret
000001B8  27                daa
000001B9  0B28              or bp,[bx+si]
000001BB  0B00              or ax,[bx+si]
000001BD  00802021          add [bx+si+0x2120],al
000001C1  0007              add [bx],al
000001C3  FE                db 0xfe
000001C4  FF                db 0xff
000001C5  FF00              inc word [bx+si]
000001C7  0800              or [bx+si],al
000001C9  0008              add [bx+si],cl
000001CB  0020              add [bx+si],ah
000001CD  0300              add ax,[bx+si]
000001CF  FE                db 0xfe
000001D0  FF                db 0xff
000001D1  FF83FEFF          inc word [bp+di-0x2]
000001D5  FF00              inc word [bx+si]
000001D7  1020              adc [bx+si],ah
000001D9  0300              add ax,[bx+si]
000001DB  A89A              test al,0x9a
000001DD  0000              add [bx+si],al
000001DF  FE                db 0xfe
000001E0  FF                db 0xff
000001E1  FF82FEFF          inc word [bp+si-0x2]
000001E5  FF00              inc word [bx+si]
000001E7  B8BA03            mov ax,0x3ba
000001EA  00D0              add al,dl
000001EC  B200              mov dl,0x0
000001EE  00FE              add dh,bh
000001F0  FF                db 0xff
000001F1  FF05              inc word [di]
000001F3  FE                db 0xfe
000001F4  FF                db 0xff
000001F5  FF                db 0xff
000001F6  FE8F6D04          dec byte [bx+0x46d]
000001FA  02B88B09          add bh,[bx+si+0x98b]
000001FE  55                push bp
000001FF  AA                stosb
