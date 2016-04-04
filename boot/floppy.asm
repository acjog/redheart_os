org	 200h	
	jmp   LABEL_START	
%include  "./include/pm.inc"
%include "./include/floppy.inc"
[SECTION .data16]
ALIGN  32 
[BITS  16]
message1    db  "load kernel ..."
len       equ  $ - message1
_reply_buffer:
reply_buffer   equ  $ + 10000h	
  times   8   db  0
reply_num	equ	$ + 10000h
_reply_num:   db 0
time	equ	$  + 10000h
_time:	dd	0
pos     equ     $  + 10000h
_pos:	dw	960	                  ;  global  var  all use 
pos1	equ	$	+	10000h    ;  Disp_int use pos   status       1 
_pos1:	dw	160 
pos2	equ	$	+	10000h
_pos2:	dw	320  	                  ;  result  array  print pos  2 
pos3	equ	$	+	10000h
_pos3:	dw	320                    ;   else use 
do_floppy	equ	$	+	10000h   ;每次软盘中断后执行的函数 因为每次中断原因不一样 所以要调用相应的函数
_do_floppy:	dd	0
Stack:
	times	512    db    0
TopOfStack	equ	 10000h+$
LABEL_DESC_GDT:		Descriptor	0,	0,	0
LABEL_DESC_FLAT_CODE32:	Descriptor	0,	0FFFFh,	DA_C|DA_32|DA_LIMIT_4G	
LABEL_DESC_FLAT_DATA32:	Descriptor	0,	0FFFFh,	DA_DRW|DA_32|DA_LIMIT_4G   ; DA_SS32表示默认使用32位操作数
LABEL_DESC_VIDEO:	Descriptor	0B8000h,0FFFFh,	DA_DRW|DA_DPL3		
SelectorFlatCode32		equ	LABEL_DESC_FLAT_CODE32 - LABEL_DESC_GDT	;08h
SelectorFlatData32		equ	LABEL_DESC_FLAT_DATA32 - LABEL_DESC_GDT	;10h
SelectorVideo			equ	LABEL_DESC_VIDEO  - LABEL_DESC_GDT+SA_RPL3	;18h+03h
GdtLen				equ	$	-	LABEL_DESC_GDT
GdtPtr:
	dw	GdtLen	-	1	;GDT界限
	dd	0			; GDT基址
LABEL_IDT:
%rep   128	
	Gate	SelectorFlatCode32,	0,	0,	DA_386IGate
%endrep
IdtLen		equ	$	-	LABEL_IDT
IdtPtr:
	dw	IdtLen-1
	dd	0
[SECTION .code16]
[BITS 16]
LABEL_START:	
	mov	ax,	0b800h
	mov	gs,	ax
	mov	ax,	cs
	mov	ds,	ax
	mov	ss,	ax
	mov     es,     ax
	mov	esp,	 200h
	
	mov	bx,	0	
.Clear_src:
	mov	byte[gs:bx],  ' '
	add	bx,	2
	cmp	bx,	4000	
	jnz	.Clear_src
	
	mov	ah,	0h
	mov	al,	03h
	int	10h	     ;  80*25 text mode
	
	xor	ax,	ax
	xor	bx,	bx
	mov	ah,	0bh
	mov	bl,	0   ;  set screen color
	int	10h

	mov	byte[gs:(3998)], 'B'
	
        mov     bp,     message1
	mov	bx,	 0003h
	mov	dx,	 1500h
	mov	cx,	len
	mov	ax,	 1301h
	int     10h
	
	call    KillMontor
	

	xor	eax,	eax
	mov	ax,	cs
	shl	eax,	4
	add	eax,	LABEL_DESC_GDT
	mov	dword[GdtPtr+2],eax

	xor	eax,	eax
	mov	ax,	cs
	shl	eax,	4
	add	eax,	LABEL_IDT
	mov	dword [IdtPtr+2],eax

	xor	eax,	eax
	mov	eax,	ClockHandler     ; clock handle init 
	mov	esi,	20h
	mov	word[LABEL_IDT+8*esi],ax
	shr	eax,	16
	mov	word[LABEL_IDT+8*esi+6],ax  	
	
	xor	eax,	eax
	mov	eax,	FloppyHandle
	mov	esi,	26h
	mov	word[LABEL_IDT+8*esi],ax
	shr	eax,	16
	mov	word[LABEL_IDT+8*esi+6],ax

	cli
	lgdt    [GdtPtr]
	lidt    [IdtPtr]
        ;  move A20 on
	in	al,	 92h
	or	al,	00000010b	
	out	 92h,	al
	; prepare to protect
	mov	eax,	cr0
	or	eax,	1
	mov	cr0,	eax
	
	jmp	dword  SelectorFlatCode32:(LABEL_PM_START+ 10000h)
	jmp	$

KillMontor:
	push	dx
	mov	dx,	 3f2h
	mov	al,	0
	out	dx,	al
	call	io_delay
	pop	dx
	ret

[SECTION .code32]
ALIGN  32
[BITS  32]
LABEL_PM_START:
	mov	ax,	SelectorFlatData32
	mov	es,	ax
	mov	ss,	ax
	mov	ds,	ax
	mov	ax,	SelectorVideo
	mov	gs,	ax
	cli
	mov	byte[gs:(480+0)],	'P'	
	mov	byte[gs:(480+118)],	'1'
	mov	esp,	TopOfStack
	call    Initial8259A
	call    Initial8253

	mov	ax,	0x1234
	mov	esi,	BUFFER+10
	mov	word[ds:esi],	ax
	call	Disp_int
	sti
	mov	dx,	FD_DOR   ; 复位软驱
	mov	al,	0
	out	dx,	al        ; 置位软驱 start motor
	call    io_delay
	mov	al,	0ch
	out	dx,	al
	call	io_delay
	mov	dword[do_floppy],	_Floppy_sence	
	
.testchange:
	cmp	byte[ds:reply_buffer],   FD_CHANG
	jnz      .testchange	
	mov	eax,	FD_SPECIFY	;设定驱动器参数		
	push	eax		
	call	out_put
	add	esp,	4
	call	io_delay1

	mov	eax,	0cfh	;  步进速率 磁头卸载速率
	push	eax	
	call	out_put
	add	esp,	4
	call	io_delay1
	mov	eax,	6    ;  磁头加载时间
	push	eax
	call	out_put
	add	esp,	4
	call	io_delay1


	mov	dx,	FD_DCR
	mov	al,	0	
	out	dx,	al
	call	io_delay	;数据传送速率
	mov	byte[gs:(480+2)],	'M'
	


	mov	al,	1ch
	mov	dx,	FD_DOR
	out	dx,	al
	call	io_delay
	mov	dword[ds:time],	 0
.1timer:
	cmp	dword [ds:time],  120 
	jbe	.1timer           ;  0.5s 延迟	
	mov	eax,	FD_RECALIBRATE   ;校验磁头 移动到0柱面
	push	eax
	call	out_put
	call	io_delay1
	
	xor	eax,	eax
	push	eax
	call	out_put
	call	io_delay1
	mov	dword[time],  0
.3timer:
	cmp	dword[time],  5
	jbe      .3timer

	mov	dword[do_floppy],   _Get_result	
	mov	byte[gs:(480+120)],   'T'
	cli
	mov	al,	4|2     ;设置DMA 屏蔽通道2
	out	0ah,	al
	call	io_delay

	mov	al,	00h
	out	0ch,	al
	call	io_delay
	
	mov	eax,	BUFFER   ;设置dma地址
	out	4h,	al
	call	io_delay
	shr	eax,	8
	out	4h,	al
	call	io_delay
	shr	eax,	8
	mov	bl,	al	

	mov	al,	00h
	out	0ch,	al
	call	io_delay	

	mov	al,	0ffh  ;设置计数器  1kb
	out	5h,	al
	call	io_delay
	mov	al,	03h
	out	5h,	al
	call	io_delay

	mov	al,	DMA_READ   ;设置DMA命令
	out	0bh,	al
	call	io_delay
	
	mov	al,	bl
	out	81h,	al
	call	io_delay
		
	sti
	mov	al,	0|2 ;恢复dma2的请求信号
	out	0ah,	al
	call	io_delay
	
	mov	dx,	FD_DOR	
	mov	al,	1ch
	out	dx,	al
	call	io_delay
	mov	dword[time],  0
.2timer:
	cmp	dword[time],  100
	jbe     .2timer

	mov	eax,	FD_READ      ;  读扇区命令字
	push	eax
	call	out_put
	add	esp,	4
	call	io_delay
	
	mov	eax,	00h    ; 磁头+ 驱动器号
	push	eax
	call	out_put
	add	esp,	4
	call	io_delay
	
	mov	eax,	00h  ; 磁道号
	push	eax
	call	out_put
	add	esp,	4
	call	io_delay
	
	mov	eax,	00h  ;  磁头号
	push	eax
	call	out_put
	add	esp,	4
	call	io_delay
	
	mov	eax,	01h ; 扇区号
	push	eax
	call	out_put
	add	esp,	4
	call	io_delay
	mov	eax,	2      ; 扇区大小 一  0x200
	push	eax
	call	out_put
	add	esp,	4
	call	io_delay
	mov	eax,	12h     ; 每磁道的扇区数
	push	eax
	call	out_put
	add	esp,	4
	call	io_delay
	mov	eax,	1bh     ; 扇区间隔长度
	push	eax
	call	out_put
	add	esp,	4
	call	io_delay
	mov	eax,	0ffh   ; 扇区大小二   0xff
	push	eax
	call	out_put
	add	esp,	4
	call	io_delay
	sti
	mov	dword[time],   0
.4timer:
	cmp	dword[time],  0x5
	jb       .4timer

	mov	esi,	BUFFER+10
	mov	ax,	[ds:esi]
	call	Disp_int

	call	Print_result	
	jmp	$
GetData:
	
;;
_Floppy_sence	equ	10000h  + $
Floppy_sence:
	push	eax
	mov	eax,	FD_SENSEI
	push	eax
	call	out_put
	add	esp,	4
	call	io_delay
	call	Get_result
	call	Print_result
	pop	eax	
	ret
;;    Get_result 读取结果数组 当命令没有结果时， 执行FD——SENSEI命令查看中断状态
;;	reply_num为返回值
_Get_result	equ	$   + 10000h
Get_result:
	push	ebp
	mov	ebp,	esp
	push	ecx
	push	esi
	push	edx
	push	ebx
	push	eax
	mov	bx,	word[ds:pos]
	mov	byte[gs:bx], 'G'
	add	word[ds:pos],	2
	mov	ecx,	10000       ;循环一万次
	mov	ebx,	8
	mov	esi,	reply_buffer
	push	esi
	push	ebx         ;  --->  push 8 
	call	Memset
	add	esp,	8
	mov	byte[reply_num],	0   ; reply_num is global, so you need to clear it :)
	xor	ebx,	ebx         ;  set ebx = 0 
.result_l:
	dec	ecx
	cmp	ecx,	0
	jz	.result_e
	mov	dx,	FD_STATUS
	in	al,	dx
;	call	Disp_int
	and	al,	STATUS_READY | STATUS_DIR | STATUS_BUSY	
	cmp	al,	STATUS_READY | STATUS_DIR | STATUS_BUSY	
	jz	.result_read
	cmp	al,	STATUS_READY
	jz	.result_e
	jmp	.result_l
.result_read:
	mov	dx,	FD_DATA
	in	al,	dx
	call	io_delay
;	call	Disp_int
	mov	byte[ds:(esi+ebx)],al
	inc	ebx
	cmp	ebx,	8      ;  最多只能读7b结果数据
	jz	.result_e
	jmp	.result_l	
.result_e:
	mov	byte[reply_num],	bl   ;  return value	
	pop	eax
	pop	ebx
	pop	edx
	pop	esi
	pop	ecx
	mov	esp,	ebp
	pop	ebp
	ret

;;             reply_num  equ the numbers of result
Print_result:
	push	ebp
	mov	ebp,	esp
	push	eax
	push	ebx
	push	esi
	push	ecx
	push	edi
	push	word[ds:pos]
	mov	bx,	word[pos2]
	mov	byte[gs:bx],   'R'
	mov	byte[gs:(bx+2)], ':'
	add	bx,	4
	mov	word[ds:pos],	bx	
	xor	cx,	cx
	xor	ebx,	ebx
	mov	cl,	byte[reply_num]
	mov	esi,	reply_buffer
.Print_l:
	cmp	cl,	0	
	jz	.Print_e
	mov	al,	byte[esi+ebx]
	xor	ah,	ah	
	call	Disp_int
.Print_0:
	dec	cl
	inc	ebx
	jmp	.Print_l
.Print_e:
	add     word[ds:pos2],    160
	pop	word[ds:pos]
	pop	edi
	pop	ecx	
	pop	esi
	pop	ebx
	pop	eax
	mov	esp,	ebp
	pop	ebp
	ret

;  Disp a int  which in  ax register 
Disp_int:      
	push	esi
	push	ebx
	push	ecx                        ;  ax -> display int 
	mov	si,   word[ds:pos]
	mov	byte[gs:si],  '0'
	add	si,	2
	mov	byte[gs:si],  'x'
	add	si,	2	
	cmp	ax,	0	  ;  if ax = zero 
	jz      .zero
	mov	cx,	16 
.loop:
	sub	cx,	4
	mov	bx,	ax
	shr	bx,	cl
	and	bx,	 000fh
	cmp	bl,	9
	ja	.az
	add	bl,	'0'
	jmp	.show
.az:
	sub	bl,	 0ah
	add	bl,	'a'
.show:
	mov   byte[gs:si], bl
	add	si,	2
	cmp	cx,	0
	jnz	.loop
	jmp	.exit
.zero:
	mov	byte[gs:si],	'0'
.exit:
	add	si,	6
	mov	word[ds:pos],	si
	pop	ecx
	pop	ebx
	pop	esi
	ret
;
;   output a byte to FD_DATA
out_put:
	push	ebp
	mov	ebp,	esp
	push	eax
	push	ecx
	push	edx
	mov	dx,	FD_STATUS
	mov	ecx,	10000
.l_out:
	dec	cx
	cmp	ecx,	0
	jz	.e_out
	in	al,	dx
	and	al,	0c0h     ; data  port  ready  cpu->fdc  
	cmp	al,	080h
	jnz	.l_out
	mov	dx,	FD_DATA
	mov	al,	[ebp+8]
	out	dx,	al
	inc     byte[gs:(480+118)]
.e_out:
	pop	edx
	pop	ecx
	pop	eax
	mov	esp,	ebp	
	pop	ebp
	ret
	
;***************************
;等效C语言函数void MemCopy(void*	source,	void*	destination,	int	len)
;入栈的顺序遵守c语言调用约定 即入栈顺序从右到左
;***************************
MemCopy:
	push	ebp
	mov	ebp,	esp			;[bp+4]=len [bp+8]=destination [bp+12]=source
	push	esi
	push	edi
	push	eax
	push	ebx
	push	ecx
	push	edx
	mov	ecx,	[ebp+010h]
	mov	esi,	[ebp+0Ch]
	mov	edi,	[ebp+08h]
.l2:
	cmp	ecx,	0
	jz	.r0
	mov	bl,	[es:esi]
	mov	[ds:edi],	bl
	inc	esi
	inc	edi
	dec	ecx
	jmp	.l2
.r0:
	pop	edx
	pop	ecx
	pop	ebx
	pop	eax
	pop	edi
	pop	esi
	mov	esp,	ebp
	pop	ebp
	ret		
_ClockHandler:
ClockHandler	equ	$	+	010000h
		push	eax
		mov	al,	20h
		out	020h,	al
		inc	dword[ds:time]
		cmp	dword[ds:time], 10000
		jbe	.clock_exit
	        mov	dword[ds:time],0
.clock_exit:
		inc	byte[gs:(4000-160)]
		mov	ax,	word[ds:pos]
		cmp	ax,	0	
		jnz	.Clock_ex
		call	Disp_int
.Clock_ex:
		pop	eax
		iretd
_FloppyHandle:								;  软盘中断函数
FloppyHandle	equ	$	+	010000h
		push	edx
		push	eax
		push	ebx
		mov	al,	20h
		out	020h,	al              ; 对中断服务寄存器清零
		mov	bx,	word[ds:pos]
		mov	byte[gs:bx],   'F'
		cmp	dword[ds:do_floppy],	0
		jz	.Floppy_e
		call	dword[ds:do_floppy]        ;写函数时要记得这是个空函数 所有这里没有对栈操作
.Floppy_e:	
		pop	ebx
		pop	eax
		pop	edx
		iretd
Initial8259A:
		mov	al,	011h
		out	020h,	al

		out	0Ah,	al
		call	io_delay

		mov	al,	020h
		out	021h,	al
		call	io_delay

		mov	al,	028h
		out	0A1h,	al

		mov	al,	4
		out	021h,	al
		call	io_delay

		mov	al,	2
		out	0A1h,	al
		call	io_delay ;8086 8088 system

		mov	al,	1
		out	021h,	al
		call	io_delay

		mov	al,	0beh         ; mask  main 8259
		out	021h,	al

		mov	al,	0ffh          ;mask  slave  8259
		out	0A1h,	al
		call	io_delay
		ret
	

Initial8253:
		mov	al,	036h
		out	043h,	al
		call	io_delay
		mov	ax,	61930      ;  11.93M HZ
		out	040h,	al
		call	io_delay
		mov	al,	ah
		out	040h,	al ;200HZ			5ms
		call	io_delay
		ret
io_delay:
		nop
		nop
		nop
		nop
		nop
		ret
io_delay1:
		push	ecx
		mov	cx,	100
.delay_loop:
		nop
		dec	cx
		cmp	cx,	0
		jnz	.delay_loop
		pop	ecx
		ret
Memset:
	;等效 void Memset(void* addr,int len)
	push	ebp
	mov	ebp,	esp
	push	ecx
	push	edi
	mov	ecx, [ebp+08h]
	mov	edi, [ebp+0Ch]
.Memset_l0:
	cmp	ecx,	0
	jz	.Mem_ret
	mov	byte[ds:edi],0
	inc	edi
	dec	ecx
	jmp	.Memset_l0
.Mem_ret:
	pop	edi
	pop	ecx
	mov	esp,	ebp
	pop	ebp
	ret
