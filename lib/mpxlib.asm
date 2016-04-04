%include "sconst.inc"
;;中断-- 宏
;;导出的函数符号
global	setlock
global  restore
global	unlock
global  port_in
global  port_out
global  DispStr
global  set_frame 
global  vid_copy

global  sendrec
global  send
global  receve

global  idle
global  MemCpy
global	Memset
global	restart
global	load_gdt
global	disable_int
global	enable_int
global  init_8253A
global	init_8259A
global  io_delay
;;中断处理 
global	tty_int
global nohandler
global clock_int
global floppyhandler	
global s_call
global int_mess
extern exception
global	divide_error
global	single_step_exception
global	nmi
global	breakpoint_exception
global	overflow
global	bounds_check
global	inval_opcode
global	copr_not_available
global	double_fault
global	copr_seg_overrun
global	inval_tss
global	segment_not_present
global	stack_exception
global	general_protection
global	page_fault
global	copr_error



;;导进的函数符号	
extern DispInt
extern lock_notify 
extern  schedule
extern	keyboard_handler  
extern DispInt
extern sys_call
extern interrupt 
;;全局变量	
extern TopOfStack
extern scr_loc
extern cur_proc
extern cur_ptr
extern tss
extern gdt_ptr
extern  k_reenter
extern clock

[SECTION .text]

set_frame:
         mov	ebp, esp
	 ret 
;;********************************************
;;u8  port_int(u16 port)实际上这里压栈的是一个32位的值
;;********************************************	
port_in:
	push    edx
	mov	dx,	[esp+8]
	xor	eax,	eax
	in	al,	dx
	nop
	nop
	nop
	nop
	pop	edx
	ret


;;*********************************************
;;port_out(u16 port, u8 value)
;;*********************************************
port_out:
	push	edx
	mov	al,	[esp+0ch]	;value
        mov	dx,	[esp+8]
        out     dx,	al
	nop
	nop
	nop
	nop	
	nop
	nop
	nop
	pop	edx
	ret

;; *************************************************
;; DispStr(char*  str)
;; *************************************************
DispStr:
	push	ebp
	mov	ebp,	esp
	pushad
	mov	edi,	dword[scr_loc]
	mov	esi,	dword[ebp+8]
	and     edi,    0xfff
.Dis_l:
	cmp	byte[esi],0
	jz	.ret
	cmp	byte[esi],10
	jz	.Dis_n	
	mov	al,	byte[esi]
	mov	ah,	0x6
	mov	[gs:edi],ax
	inc	edi
	inc	edi
	cmp	edi,	8000
	ja	.Dis_a
	inc	esi
	jmp	.Dis_l
.Dis_n:				;实现一个回车的功能
	inc	esi
	add	edi,	160
	mov	edx,	edi
	shr	edx,	16
	mov	eax,	edi
	and	eax,	0FFFFh
	mov	bx,	160
	div	bx
	mul	bx
	mov	bx,	ax
	mov	ax,	dx
	shl	eax,	16
	mov	ax,	bx
	mov	edi,	eax
	jmp	.Dis_l
.Dis_a:
	mov	edi,	0
	jmp     .Dis_l
.ret:
	mov	dword[scr_loc],	edi
	popad
	mov	esp,	ebp
	pop	ebp
	ret
	
;;*****************************************************************
;;	等效void MemCpy(void* source,void* dest,int len)
;;*****************************************************************
MemCpy:
	push	ebp
	mov	ebp,	esp
	pushad
	mov	ecx,	[ebp+010h]
	mov	esi,	[ebp+08h]
	mov	edi,	[ebp+0Ch]
.Me_l0:
	cmp	ecx,	0
	jz	.Me_ret
	mov	al,	byte[ds:esi]
	mov	byte[ds:edi],al
	dec	ecx
	inc	esi
	inc	edi
	jmp	.Me_l0
.Me_ret:
	popad
	mov	esp,	ebp
	pop	ebp
	ret
	

;;**********************************************************************
;;  等效vid_copy ( void * source ,void  base ,  void * dest , u32_t len ) 
;;**********************************************************************
vid_copy:
	push	ebp
	mov	ebp, 	esp
	pushad
	mov	esi,	[ebp + 8]
	mov	ecx,	[ebp + 0x14]
	mov	edi, 	[ebp + 0x10]
	mov	ebx,	[ebp + 0xc]
.vid_copy_l:
	cmp	ecx,	0
	jz	.vid_copy_ret
	mov	ax,	word[esi]
	mov	word[gs:( edi + ebx )], ax
	inc     esi
	inc     esi
	inc	ebx
	inc	ebx
	dec	ecx
	jmp	.vid_copy_l
.vid_copy_ret:
	popad
	mov	esp,	ebp
	pop	ebp
	ret
	


;;**********************************************************
;;   等效 void Memset(void* addr,int len)
;;**********************************************************
Memset:
	push	ebp
	mov	ebp,	esp
	pushad
	mov	ecx, [ebp+0Ch]
	mov	edi, [ebp+08h]
.Memset_l0:
	cmp	ecx,	0
	jz	.Mem_ret
	mov	byte[ds:edi],0
	inc	edi
	dec	ecx
	jmp	.Memset_l0
.Mem_ret:
	popad
	mov	esp,	ebp
	pop	ebp
	ret
	


;;**********************************************************
;;关中断
;;**********************************************************
setlock:
    pushf
    pop dword[eflags]
	cli
	ret

restore:
       push  dword[eflags] 
       popf
       ret

;;**********************************************************
;;开中断
;;**********************************************************
unlock:
	sti
	ret
	
;;**********************************************************
;;加载gdt
;;**********************************************************
load_gdt:
	cli 
	lgdt [gdt_ptr]
	sti
	ret

;;**********************************************************
;;重新运行-版本0
;;**********************************************************	
restart:
	cmp   dword [cur_proc],	IDLE
	jz	  idle
	mov	esp, [cur_ptr]         ;
	lldt	[esp+LDT_SEL]          ;
	lea	eax,	[esp+P_STACK_TOP]
	mov	dword	[tss+TSS_SP0],	eax
	mov	eax,	[esp + A_CR3 ]
	mov	cr3,	eax
	inc	dword[k_reenter]
	pop	gs
	pop	fs
	pop	es
	pop	ds
	popad
	add	esp,4
	iretd
idle:
    sti
l1:
;wait  ;  实际这里永远目前还不能调用 
       jmp  l1


;;*************************************************************
;;发生重入时--restart1版本
;;*************************************************************
restart1:			
		inc	dword[k_reenter]
		pop	gs
		pop	fs
		pop	es
		pop	ds
		popad
		add	esp,4
		iretd


;;**************************************************************
;;保存处理器的状态信息 切换到内核栈 同时判断是否发生中断重入 
;;**************************************************************
save:
		cld
		pushad
		push	ds
		push	es
		push	fs
		push	gs
		
		mov	dx,	ss	
		mov	ds,	dx
		mov	es,	dx
        mov edx,  0x100000          ;切换目录
        mov cr3,  edx
		mov	eax,	esp				;准备返回
		dec	dword [k_reenter]
		cmp	dword[k_reenter],0
		jl	.set_restart1

		mov	esp,	TopOfStack
		push	restart
		jmp	dword[eax+A_RET_ADDR]
.set_restart1:
		push	restart1
		jmp	dword[eax+A_RET_ADDR]			
	
;;*********************************************************
;;时钟中断处理
;;*********************************************************
clock_int:
		call	save
		in	al,     INT_M_01
		or	al,	1
		out	INT_M_01,	al   ;屏蔽同类型中断
		
        sti
		mov	dword[clock_mess+4],A_CLOCK_TICK
		push    clock_mess 
		push	A_CLOCK_TASK
		call	interrupt 
		add	esp,	8
	    cli
		inc   byte[gs:3990]
        
        in	al,	INT_M_01
		and	al,	0xFE
		out	INT_M_01,	al  ;开中断

		ret
	
floppyhandler:		
       	call	save
		in	al,     INT_M_01
		or	al,	0x40
		out	INT_M_01,	al   ;屏蔽同类型中断

		mov	dword[floppy_mess+4],A_CLOCK_TICK
		push    floppy_mess
		push	A_FLOPPY_TASK 
		call	interrupt 
		add	esp,	8

	    mov    byte[gs:3992] , 'N'	

		in	al,	INT_M_01
		and	al,	0xBF
		out	INT_M_01,	al  ;开中断
		ret

;;
;;  no handler	
nohandler:
		call	save
		ret
;******************************************************
;;tty_int任务
;;******************************************************
tty_int:
		call	save
		
		in	al,	INT_M_01
		or	al,	2
		out	INT_M_01,	al		;屏蔽键盘中断
		
		mov	al,	0x20
		out     0x20 ,  al

		inc	byte[gs:4]	

	    sti
        call	keyboard_handler
        cli
				
		in	al,	INT_M_01
		and	al,	0xFD
		out	INT_M_01,	al		;reenable 8259芯片
		ret
			
;;*******************************************************
;;void disable_int(int  irq)			;;中断号0～15
;;*******************************************************		
disable_int:
		push	ebp
		mov	ebp,	esp
		mov	ecx,	[ebp+8]
		pushf
		cli
		mov	ah,	1
		rol	ah,	cl			;1 << (irq%8)
		cmp	cl,	8
		jae	.disable_s			; irq >= 8
		in	al,	INT_M_01	
		or	al,	ah
		out	INT_M_01,	al	
		jmp	.disable_r
.disable_s:
		in	al,	INT_S_01	
		or	al,	ah
		out	INT_S_01,	al		
.disable_r:
		popf
		mov	esp,	ebp
		pop	ebp
		ret


;;********************************************************
;;void	enable_int(int irq)
;;********************************************************
enable_int:
	    push	ebp
	    mov	ebp,	esp
	    mov	ecx,	[ebp+8]
	    pushf
	    cli
	    mov	ah,	1
	    rol	ah,	cl
	    not	ah
	    cmp	cl,	8
	    jae	.enable_s
	    in		al,	INT_M_01
	    and	al,	ah
	    out	INT_M_01,	al
	    jmp	.enable_r
.enable_s:
	     in	al,	INT_S_01
	     and	al,	ah
	     out	INT_S_01,	al
.enable_r:
	     popf
	     mov	esp,	ebp
	     pop	ebp
	     ret
	      

;;********************************************************
;;sys_call中断
;;********************************************************
s_call:
		call	save
		mov	ebp,  [cur_ptr]
		push	dword [ebp + A_EBX]		;ebx m_ptr
		push	dword [ebp + A_EDX]		;edx src_dest
		push	dword [ebp + A_ECX]		;eax call
		call	sys_call
		add	esp,	0ch
		ret
;;注意这里会有可能会改变cur_ptr 所以mini_notify是需要的
;;
;; receve (u32_t src , message *ptr ) ;
receve:
		push	ebp
		mov	ebp,	esp
		mov	ebx,  dword[ebp + 0ch] ; m_ptr
		mov	edx,  dword[ebp + 08h] ; src 
		mov	ecx,  A_RECEVE
		int	80h
		mov	esp,	ebp
		pop	ebp
	        ret

;;
;;  send (u32_t dest , message *ptr );
send:
		push	ebp
		mov	ebp,	esp
		mov	ebx,  dword[ebp + 0ch] ; m_ptr
		mov	edx,  dword[ebp + 08h] ; dest 
		mov	ecx,  A_SEND
		int	80h
		mov	esp,	ebp
		pop	ebp
		ret	

;;这里要改
sendrec:
		push	ebp
		mov	ebp,	esp
		mov	ebx,	dword[ebp + 0ch ] ; m_ptr
		mov	edx,	dword[ebp + 08h ] ; dest
		mov	ecx,	A_SENDREC
		int	80h
		mov	esp,	ebp
		pop	ebp
		ret
;;
;;  sendrec ( u32_t  dest , message *ptr)
;sendrec:
;		push	ebp
;		mov	ebp,	esp
;		mov	ebx,	dword[ebp + 0ch ] ; m_ptr
;		mov	edx,	dword[ebp + 08h ] ; dest
;		mov	ecx,	A_SEND
;		int	80h
		
;		mov	ebx,    dword[ebp + 0ch ]
;		mov	edx,	dword[ebp + 08h ]
;		mov	ecx,	A_RECEVE
;		int	80h
;
;		mov	esp,	ebp
;		pop	ebp
;		ret
;;
;;
init_8253A:
	mov	al,	036h
	out	043h,	al
	mov	ax,	19886  ;  1193182/60
	out	040h,	al
	mov	al,	ah
	out	040h,	al ;200HZ			5ms
	ret
;;
init_8259A:
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

	mov	al,	0ffh
	out	021h,	al

	mov	al,	0ffh          ;屏蔽某些信号
	out	0A1h,	al
	call	io_delay
	ret
io_delay:
	nop
	nop
	nop
	nop
	nop
	ret

; 中断和异常 -- 异常
divide_error:
	call	save
	push	0xFFFFFFFF	; no err code
	push	0		; vector_no	= 0
	call	exception
	add	esp,	8
	ret
single_step_exception:
	call	save
	push	0xFFFFFFFF	; no err code
	push	1		; vector_no	= 1
	call	exception
	add	esp,	8
	ret
nmi:
	call	save
	push	0xFFFFFFFF	; no err code
	push	2		; vector_no	= 2
	call	exception
	add	esp,	8
	ret

breakpoint_exception:
	call	save
	push	0xFFFFFFFF	; no err code
	push	3		; vector_no	= 3
	call	exception
	add	esp,	8
	ret

overflow:
	call	save
	push	0xFFFFFFFF	; no err code
	push	4		; vector_no	= 4
	call	exception
	add	esp,	8
	ret

bounds_check:
	call	save
	push	0xFFFFFFFF	; no err code
	push	5		; vector_no	= 5
	call	exception
	add	esp,	8
	ret

inval_opcode:
	call	save
	push	0xFFFFFFFF	; no err code
	push	6		; vector_no	= 6
	call	exception
	add	esp,	8
	ret

copr_not_available:
	call	save
	push	0xFFFFFFFF	; no err code
	push	7		; vector_no	= 7
	call	exception
	add	esp,	8
	ret 

double_fault:
	call	save
	push	8		; vector_no	= 8
	call	exception
	add	esp,	8
	ret

copr_seg_overrun:
	call	save
	push	0xFFFFFFFF	; no err code
	push	9		; vector_no	= 9
	call	exception
	add	esp,	8
	ret
inval_tss:
	pop	ecx
	call	save
	push	ecx
	push	10		; vector_no	= A
	call	exception
	add	esp,	8
	ret

segment_not_present:
	pop	ecx
	call	save
	push	ecx
	push	11		; vector_no	= B
	call	exception
	add	esp,	8
	ret

stack_exception:
	pop	ecx
	call	save
	push	ecx
	push	12		; vector_no	= C
	call	exception
	add	esp,	8
	ret

general_protection:
	pop	ecx
	call	save
	push	ecx
	push	13		; vector_no	= D
	call	exception
	add	esp,	8
	ret

page_fault:
	pop	ecx
	call	save
    mov     eax,    cr2
    push    eax
    call    DispInt
    add     esp,4
	push	ecx
	push	14		; vector_no	= E
	call	exception
	add	esp,	8
	ret

copr_error:
	call	save
	push	0xFFFFFFFF	; no err code
	push	16		; vector_no	= 10h
	call	exception	
	add	esp,	8
	ret
	


[SECTION .data]
eflags  :       dd      0
ret_save:	dd	0
M0	:	db	"\nrestart . . .",0
M1	:   db  "test sys_call ...",0
clock_mess: dd 0xFFFFFFFF  ;进程号 HARDWARE
times   7   dd   0          ;  32 byte for message use  
floppy_mess: dd 0xFFFFFFFF  ;进程号 HARDWARE
times   7   dd   0          ;  32 byte for message use  
page_mess : dd 0xFFFFFFFF  ;进程号 HARDWARE
times   7   dd   0          ;  32 byte for message use  
