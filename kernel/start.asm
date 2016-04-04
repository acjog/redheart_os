;/*函数*/
extern main
extern DispStr
extern   switch_page

;/*全局变量*/
extern	 gdt_ptr
extern	 gdt
extern  idt_ptr
extern  idt
extern	 scr_loc
extern  Memset
extern  MemCpy
extern  Init_tss
SelectorTss	equ	0x20
;导出的符号
global  TopOfStack
global	nointerhandler
global  seg_base
[SECTION .text]
ALIGN  32
[BITS 32]
global	_start
_start:
	jmp  near begin           ; 3 个字节
	db   0,0,0
seg_base  :
kernel_seg:     dd   0
		dd   0 
mm_seg    :     dd   0
		dd   0
fs_seg    :     dd   0
	        dd   0 
init      :     dd   0
                dd   0  
	times   30  dd  0        ;前面62个字节 加上jmp跳转指令2个字节
begin:
	mov	dword[scr_loc],0
	mov	esp, TopOfStack
	
	mov	dword[scr_loc],160
	
	mov	eax, Sta
	push	eax
	call	DispStr
	add	esp,4
	
	sgdt	[gdt_ptr]
	mov	eax,	5*8           ;len --> 8*5
	push	eax
	mov	eax,	[gdt_ptr+2]	;source
	push	eax
	mov	eax,	gdt		;dest  < --  source
	push	eax
	call	MemCopy 
	add	esp,	12
	
	mov	eax,	gdt
	mov	[gdt_ptr+2],	eax
	mov	ax,	64*8-1
	mov	[gdt_ptr],	ax
	
	cli
	
	lgdt [gdt_ptr]
	
	sidt	[idt_ptr]
	mov	eax,	8*128
	push	eax
	mov	eax,	[idt_ptr+2]
	push	eax
	mov	eax,	idt
	push	eax
	call	MemCopy
	add	esp,	12
	
	mov	eax,	idt
	mov	[idt_ptr+2],	eax
	mov	ax,	8*256-1
	mov	[idt_ptr],	ax
	lidt	[idt_ptr]
	mov	dword[scr_loc],320
	mov	eax, World
	push	eax
	call	DispStr
	add	esp,4
	call	Init_tss		;初始化tss

	call	switch_page             ;  开启分页

	mov	eax,	0x100000        ;  1M的地址
	mov	cr3,	eax

	mov	eax,	cr0
	or	eax,	0x80000000      ;  开启分页标志
	mov	cr0,    eax
	
	mov	ax,	SelectorTss
	ltr	ax
	push	0
	popf	       ; set flag state 
	cli
	jmp	 main
	
;***************************
;等效C语言函数void MemCopy(void*	destination,void*	source,	int	len)
;入栈的顺序遵守c语言调用约定 即入栈顺序从左到右
;***************************
MemCopy:
	push	ebp
	mov	ebp,	esp	 ;[bp+4]=len [bp+8]=destination [bp+12]=source
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
	mov	bl,	 [es:esi]
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
		

times	   2048 	db	0
TopOfStack:
Erro:	   db "something  to happen!!!",0
Sta:       db "start.asm begin ...",0
World:     db "welcome to Binux world ^_^ *",10,0
