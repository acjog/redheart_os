    org	0200h
	jmp	LABEL_START
%include "include/minix3.inc"
%include "include/pm.inc"
[SECTION .data]
[BITS 32]
LABEL_DESC_GDT:		Descriptor	0,	0,	0						
LABEL_DESC_FLAT_CODE32:	Descriptor	0,	0FFFFh,	DA_C|DA_32|DA_LIMIT_4G		
LABEL_DESC_FLAT_DATA32:	Descriptor	0,	0FFFFh,	DA_DRW|DA_32|DA_LIMIT_4G		
LABEL_DESC_VIDEO:	Descriptor	0B8000h,0FFFFh,		DA_DRW|DA_DPL3				
LABEL_DESC_TSS:		Descriptor	0,	TssLen-1,	DA_386TSS|DA_DPL3			
SelectorFlatCode32		equ	LABEL_DESC_FLAT_CODE32 - LABEL_DESC_GDT			;08h
SelectorFlatData32		equ	LABEL_DESC_FLAT_DATA32 - LABEL_DESC_GDT			;10h
SelectorTss			equ	LABEL_DESC_TSS	-	LABEL_DESC_GDT			;20h
SelectorVideo			equ	LABEL_DESC_VIDEO	  - LABEL_DESC_GDT+SA_RPL3	;18h+03h

GdtLen				equ	$	-	LABEL_DESC_GDT
GdtPtr:
	dw	GdtLen	-	1	;GDT界限
	dd	0			; GDT基址

LABEL_IDT:
%rep	256
	Gate	SelectorFlatCode32, 0,      0, DA_386IGate
%endrep
IdtLen		equ	$	-	LABEL_IDT
IdtPtr:
	dw	IdtLen-1
	dd	0
Message:	
	db	"Loading    ",0
	db	"ready......",0
	db	"NO   KERNEL",0		;11字符+0

_Scr_loc:	dd	0
Scr_loc		equ	_Scr_loc  +  LINERADDLOADER
KernelFileName:		db	 "image",0		;查找的文件文件名
FILENAME_OFFSET		equ	4
Elf_PH_Offset			equ	0x34
KernelEntry			equ	0x30200
sectors_per_track	db       18 ; 在bootsec得到
sectors_per_cyl		dw       18*2
bootdev		db	 0  ;驱动器号  
LABEL_TSS:
	dd	0		;back
	dd	0		;0 level stack
	dd	0	
	dd	0
	dd	0		;1 level stack
	dd	0
	dd	0		;2 level stack
	dd	0		;cr3	
	dd	0		;eip
	dd	0		;eflags	
	dd	0		;eax
	dd	0		;ecx
	dd	0		;edx
	dd	0		;ebx
	dd	0    
	dd	0		;ebp
	dd	0		;esi
	dd	0		;edi
	dd	0		;es
	dd	0		;cs
	dd	0		;es
	dd	0		;ds
	dd	0		;fs
	dd	0		;gs
	dd	0		;ldt
	dw	0		;trap	flag
	dw	0		;io bitmap base address
	db	0ffh		;end flag
TssLen	equ	$	-	LABEL_TSS
Stack:
	times	512	db	0
TopOfStack	equ  LINERADDLOADER + $   ;32位保护模式使用的栈

zonenum         dd     0      ;控制外层循环
index           dd     0
number          dd     0

zoneptr	        dw     0
directptr       dw     0
indzoneptr	dw     0

inodenum	dd     0
pages           dd     0
kerneloff	dw     0x200   ;  加载内核时计算偏移
ROOTDIR_SEG	    equ	    KERNEL_SEG - 0x400  ;存放根目录的缓冲
INODE_SEG       equ     KERNEL_SEG - 0x300  ;inode数组缓冲
ZONE_SEG        equ	    KERNEL_SEG - 0x200  ;存放一级间接寻址的块号缓冲
INZONE_SEG      equ     KERNEL_SEG - 0x100  ;存放二级间接寻址的块号缓冲
ZONE_NR		equ	10
; 二十位地址
HIGH_ROOT	equ	(ROOTDIR_SEG*0x10 >> 16) & 0xf
LOW_ROOT	equ	ROOTDIR_SEG*0x10 & 0xffff

HIGH_INODE      equ     ( INODE_SEG * 0x10 >> 16) & 0xf 
LOW_INODE	equ	INODE_SEG * 0x10 & 0xffff

HIGH_ZONE	equ	( ZONE_SEG * 0x10 >> 16 ) & 0xf 
LOW_ZONE	equ	ZONE_SEG * 0x10 & 0xffff

HIGH_INZONE	equ	( INZONE_SEG * 0x10 >> 16 ) & 0xf 
LOW_INZONE	equ	INZONE_SEG * 0x10 & 0xffff 

HIGH_KERNEL	equ	( KERNEL_SEG * 0x10 >> 16 ) & 0xf 
LOW_KERNEL	equ	KERNEL_SEG * 0x10 & 0xffff 

[SECTION .code16]
[BITS 16]
LABEL_START:
	mov	ax,	cs
	mov	ds,	ax
	mov	ss,	ax
	mov	sp,	0200h
	mov	ax,	0b800h
	mov	gs,	ax

	mov	bx,	0	 ;clear the screen
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


	mov	dh,	0
	call	DispStr
	
	push	HIGH_INODE	
	push    LOW_INODE
	push	1
	push	0
	push	0x4
	call	ReadBlock
	add	esp,	10

        ; 读取根目录
 	push	HIGH_ROOT
	push    LOW_ROOT
	push	1
	push	0
	push	0x10   ;这些值我不想去写的通用 所以采用硬编码的方式
	call	ReadBlock
	add	esp,	10

		
	mov	ax,	ROOTDIR_SEG
	mov	es,	ax
	xor	si,	si
	mov	dword[ds:index], 0	
lookup:
	cmp	dword[ds:index], 64    ; 要求image文件在rootdir前一个页面以内
	ja      .NoFound
	mov	di,   KernelFileName
	add	si,   4                ;  跳过前面四个字节 存放属性值
.0d_compare:
	mov	al,	byte[ds:di]
	cmp	al,	0
	jz	.Found
	cmp	al,	byte[es:si]
	jnz	.0d_nextfile
	inc	si
	inc	di
	jmp	.0d_compare
.0d_nextfile:
	and	si,	0xffc0
	add	si,	0x40
	inc	dword[ds:index]       ; 查找文件计数器
	jmp     lookup

;;***************************************	
.NoFound:
	mov	dh,2		
	call	DispStr
	jmp	$
.Found:
	mov	dh,1
	call	DispStr
	and	si,	0xffc0
	mov	eax,	dword[es:si]
	mov	dword[ds:inodenum],eax   ;存放image的inode number 
	mov	ax,	INODE_SEG
	mov	es,	ax
	xor	si,	si
.offset:
	dec	dword[ds:inodenum]
	cmp	dword[ds:inodenum], 0
	jz      .inode
	add	si,	INODESIZE
	jmp	.offset                  ; 计算inode偏移
.inode:
	add     si,	D_SIZE           ; 将指针指向结构体文件大小
	mov	eax,	dword[es:si]     ; 取得文件的大小
	sub	si,	D_SIZE           ; 将指针退回
	add	eax,	0xfff
	shr	eax,	12               ; 多少个页面
	mov	dword[ds:pages],eax
	cmp	eax,	2048             ; 大于8M 直接死循环
	jg     load4G
	cmp	eax,	7  
	ja	load8M                   ; 大于28k
smallload:
	add	si,	D_ZONE           ; 将指针定位inode的存储块号
	mov	word[ds:zoneptr], si
	;  先将第一块读到指定位置 这里要使用20位偏移地址
	;  因为这里有偏移信息
        push    HIGH_KERNEL
	push	LOW_KERNEL
	push	1
	push	word[es:(si+2)]
	push	word[es:si]
	call	ReadBlock
	add	esp,	10
	mov	bx,	word[ds:kerneloff]  ; 0x400
	add	bx,	8                ;  跳过8字节的偏移字节
	add	bx,	8*4		    ; 组件的起始地址
	add	bx,	4                   ; 刚才读了一个块
	dec	dword[ds:pages]
	mov	ax,	KERNEL_SEG
	mov	ds,	ax
	add	si,	4
	;  循环读取n - 1 块
.small_loop:
	cmp	dword[ds:pages],  0
	jz      success
	mov	ax,	LOADER_SEG
	mov	ds,	ax
	mov	si,	word[ds:zoneptr]
	mov	ax,	KERNEL_SEG
	mov	es,	ax
	push	word[es:(bx+2)]
	push	word[es:bx]   ;这里要重新改写接口
	push	1
	push	word[ds:(si+2)]
	push	word[ds:si]
	call	ReadBlock
	add	sp,	10
	mov	ax,	LOADER_SEG
	mov	ds,	ax
	dec	dword[ds:pages]
	add	word[ds:zoneptr],DWORDSIZE
	mov	ax,	INODE_SEG          ;inode节点段
	mov	es,	ax
	add	bx,	4                  ;存储在image代码段的块的加载地址
	jmp	.small_loop
load8M:	
	add	si,	D_ZONE
	add	si,	7*DWORDSIZE    ; 定位到间接寻址号  
	push	HIGH_INZONE
	push	LOW_INZONE
	push	1
	push	word[es:( si + 2 )]
	push	word[es:si]
	call	ReadBlock 
	add	esp,	10
	mov	ax,	INZONE_SEG
	mov	es,	ax
	xor	si,	si            ;  es:si指向第一个块号
	push    HIGH_KERNEL
	push	LOW_KERNEL
	push	1
	push	word[es:(si+2)]
	push	word[es:si]
	call	ReadBlock
	add	esp,	10
	mov	ax,	LOADER_SEG
	mov	ds,	ax
	mov	bx,	word[ds:kerneloff]  ; 0x400 
	add	bx,     8
	add	bx,	4*8
	add	bx,	4
	add	si,     4
	dec	dword[ds:pages]
	mov	word[ds:zoneptr],  bx
.load8M:
	cmp	dword[ds:pages], 0 
	jz	success
	mov	ax,	INZONE_SEG
	mov	ds,	ax
	mov	ax,	KERNEL_SEG
	mov	es,	ax
	push	word[es:(bx+2)]
	push	word[es:bx]
	push	1
	push	word[ds:(si+2)]		
	push	word[ds:si]
	call	ReadBlock
	add	esp,	10
	mov	ax,	LOADER_SEG
	mov	ds,	ax
	dec	dword[ds:pages]
	add	bx,	4
	add	si,	4
	jmp	.load8M
load4G:
	jmp     $	             ;不存在这种情况 

success:
	call	KillMontor ;关闭马达
	mov	byte[gs:3998],'B'
	xor	eax,	eax
	mov	ax,	cs
	shl	eax,	4
	add	eax,	LABEL_DESC_GDT
	mov	dword[GdtPtr+2], eax
		
	xor	eax,	eax
	mov	ax,	cs
	shl	eax,	4
	add	eax,	LABEL_TSS
	mov	word	[LABEL_DESC_TSS+2],ax
	shr	eax,	16
	mov	byte	[LABEL_DESC_TSS+4],al
	mov	byte	[LABEL_DESC_TSS+7],ah	;初始化LABEL_TSS
		
	xor	eax,	eax	
	mov	ax,	cs
	shl	eax,	4
	add	eax,	LABEL_IDT
	mov	dword	[IdtPtr + 2],	eax
		
	xor	eax,	eax
	mov	eax,	NormalHandler0
	mov	word[LABEL_IDT],	ax
	shr	eax,	16
	mov	word[LABEL_IDT+6],ax

	cli
	lgdt	[GdtPtr]
	lidt	[IdtPtr]
	;打开地址线A20
	in	al, 92h
	or	al, 00000010b
	out	92h, al

	; 准备切换到保护模式
	mov	eax, cr0
	or	eax, 1
	mov	cr0, eax

	jmp	dword SelectorFlatCode32:(LINERADDLOADER+LABEL_PM_START)		;跳入保护模式
	jmp	$

;; ******************************
;; KillMontor
;; ******************************
KillMontor:
	push	dx
	mov	dx,	03F2h
	mov	al,	0
	out	dx,	al
	pop	dx
	ret
;***************************
;ReadBlock  压入参数   
;       +12    +10         +8          +6          +4      +2                                                                
;  offset  / segment  / count /  high zonenr / low zonenr/ip/ bp
;  void ReadBlock( long lineraddr , short  count , long zonenr ) 
;  考虑20位的线性地址  这里我的入栈顺序和c语言的不同
;  偏移量从4起
;***************************
; 这里有一个限制 每次最多读64k 因为es:si
ReadBlock:
	push	bp
	mov	bp,	sp                 ; 保存栈帧
	push	ax
	push	bx
	push	cx
	push	dx                         ;  保存寄存器
	xor	eax,	eax
	mov	eax,	dword[ss:(bp+10)]
	and	eax,	0xfffff
	shr	eax,	4
	mov	es,	ax                 ;  线性地址转化为段式地址
	mov	bx,	LOADER_SEG         ;  因为前面破坏了ds段
	mov	ds,	bx
	xor	ebx,	ebx
	mov	bx,	word[ss:(bp+6)]
	shl	ebx,	16
	mov	bx,	word[ss:(bp+4)]     ;  ebx  = zone number 
        shl     ebx,	3                   ;  ebx  = ebx * 8  zone转化为sector号
	mov	edx,	ebx                 ;  edx  等于从那个扇区开始读 
	push	ebx			    ;  ss:sp --> 将读取的扇区偏移保存在栈中 
	mov	al,	byte[ss:(bp+8)]
	shl	al,	3                   ;  要乘以8 每次读取为扇区
	mov	byte[ss:(bp + 8 )],  al
	mov	byte[ss:(bp + 9 )],  al     ;  将要读取的扇区保存在此
.read:
	cmp	word[ss:(bp+8)],  0         ;  是否为零 为零则读完
	jz	.exitread
	mov	bx,	es
	mov	dx,	bx
	add	dx,	0x240               ;  每次最多读一个磁道的扇区数
	and	dx,	0xf000
	and	bx,	0xf000
	cmp	dx,	bx
	jz      .continue0
	mov	bx,	es
	sub	dx,	bx
	shr	dx,	5
	cmp	dl,	byte[ss:(bp+8)]
	ja     .continue0
	mov	byte[ss:(bp + 8)], dl       ;  计算出距离64k边界扇区数
.continue0:
	xor	bx,	bx
	mov	dx,	word[ss:(bp-10)]    ;  上个步骤保存在栈中
	mov	ax,	word[ss:(bp-12)]    ;  这个为读取的扇区偏移
	div     word[ds:sectors_per_cyl]    ;  ax = cyliner  dx = sectors with cylinder  除以每个柱面的扇区数
	xchg    ax,    dx                   ;  dx = cylinder ax = sectors  除以每个磁道的扇区
	div     byte[ds:sectors_per_track]  ;  ah = sectors al = head
	mov	cl,	dh
	shl	cl,	6
	or	cl,	ah
	inc	cl                          ;  cl = 6 7 high 2 bit of cylinder 0-5 sectors
	mov	ch,	dl
	mov	dh,	al
	mov	dl,	byte[ds:bootdev]
	mov	al,	byte[ds:sectors_per_track]
	sub	al,	ah
	cmp	al,	byte[ss:(bp+8)]
	jbe     .GoOnRead
	mov	al,	byte[ss:(bp+8)]	
.GoOnRead:
	xor	ah,	ah
    int     13h
	mov	ah,	02					
	int	13h
	cmp	ah,	0
	jnz	.read
.rdeval:
	xor	ah,	ah
	sub	byte[ss:(bp+8)],  al        ;   bp + 8存放下次要读取的扇区数
	sub	byte[ss:(bp+9)],  al        ;   bp + 9为剩下扇区
	add	word[ss:(bp-12)], ax        ;   偏移多添ax个扇区
	shl	ax,	5                   ;   每次es的值改变
	mov	bx,	es
	add	bx,	ax
	mov	es,	bx
	cmp	byte[ss:(bp+8)], 0 
	jnz	.read
	mov	byte[ss:(bp+8)],  0x12
	mov	al,	byte[ss:(bp+9)]     ;这里低字节存储要读的扇区
	cmp	al,	0x12
	ja     .read
	mov	byte[ss:(bp+8)], al
	jmp	.read
.exitread:
	pop	ebx                         ; 恢复寄存器
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	mov	sp,	bp
	pop	bp
	ret		
				
		
									
;***************************			
;DispStr	显示字符串
;dh=字符串号  ds:si=Message gs:di=显示的地址	
;***************************
DispStr:
	push	si
	push	ax
	push	bx
	push	cx
	push	dx
	mov	si,	Message
	mov	al,	160
	mul	dh
	mov	di,	ax				;计算显示地址
	xor	ax,	ax
	mov	al,	12
	mul	dh
	add	si,	ax
.loop:
	cmp	byte[si],	0
	jz		.over
	mov	al,	[si]
	mov     [gs:di], al
	inc	si
	add	di,	2
	jmp	.loop	
.over:
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	pop	si
	ret

[SECTION .code32]
ALIGN 32
[BITS 32]
LABEL_PM_START:
	mov	ax,	SelectorFlatData32
	mov	es,	ax
	mov	ds,	ax
	mov	ss,	ax
	mov	fs,	ax
	mov	ax,	SelectorVideo
	mov	gs,	ax
	mov	byte[gs:(480+120)],'P'
	mov	byte[gs:(480+122)],'M'	
	mov	esp,	TopOfStack
	cli
	jmp	dword SelectorFlatCode32:KernelEntry
	jmp	$

;***************************
;等效C语言函数void MemCopy(void* destination ,void* source,int	len)
;入栈的顺序遵守c语言调用约定 即入栈顺序从左到右
;***************************
MemCopy:
	push	ebp
	mov	ebp,	esp		
	push	esi
	push	edi
	push	eax
	push	ebx
	push	ecx
	push	edx                    ; 保存寄存器
	mov	ecx,	[ebp+010h]     ; 拷贝字节数
	mov	esi,	[ebp+0Ch]      ; 源地址
	mov	edi,	[ebp+08h]      ; 目的地址
.l2:
	cmp	ecx,	0
	jz	.r0
	mov	bl,	[es:esi]
	mov	[ds:edi],bl
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
	
_NormalHandler0:	
NormalHandler0	equ	_NormalHandler0 + LINERADDLOADER
	push	eax
	push	ebx
	mov	bx,	SelectorFlatData32
	mov	ds,	bx
	mov	ebx,	dword [ds:Scr_loc]
	shl	ebx,	1
	mov	byte [gs:ebx],'N'
	shr	ebx,	1
	inc	ebx
	mov	dword	[ds:Scr_loc],	ebx
	mov	al,	20h
	out	020h,	al
	pop	ebx
	pop	eax
	iretd		
io_delay:
	nop
	nop
	nop
	ret
Memset:
	;等效 void Memset(void* addr,int len)
	push	ebp
	mov	ebp,	esp
	pushad
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
	popad
	mov	esp,	ebp
	pop	ebp
	ret
