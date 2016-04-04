LDT_SIZE		equ	3
GDT_SIZE		equ  	64

SelectorFlatCode32	equ	008h
SelectorFlatData32	equ	010h
SelectorTss		equ	020h
SelectorVideo		equ	01bh


;;    mpxlib.asm use
LDT_SEL			equ	72
P_STACK_TOP		equ	72
TSS_SP0			equ	4	
INT_M_00		equ	20h
INT_M_01		equ	21h		
INT_S_00		equ	0A0h
INT_S_01		equ	0A1h

IDLE			equ	  -66      ;;是否处于空闲状态
;;  自动计算stackfram偏移值 注意这里的符号虽然是在汇编中使用
;;  但是由于是elf格式 所以还要注意不要与c语言文件宏冲突 :)
A_GS	    		equ	0 
A_FS	    		equ	A_GS + 4
A_ES	    		equ	A_FS + 4
A_DS	    		equ	A_ES + 4
A_EDI	    		equ	A_DS + 4
A_ESI	    		equ	A_EDI + 4
A_EBP	    		equ	A_ESI + 4
A_KERNEL_ESP  		equ	A_EBP + 4
A_EBX         		equ	A_KERNEL_ESP + 4
A_EDX         		equ	A_EBX + 4
A_ECX         		equ	A_EDX + 4
A_EAX         		equ	A_ECX + 4
A_RET_ADDR    		equ 	A_EAX + 4
A_EIP         		equ 	A_RET_ADDR + 4
A_CS	    		equ 	A_EIP + 4
A_EFLAGS      		equ 	A_CS  + 4
A_ESP         		equ 	A_EFLAGS + 4
A_SS	    		equ 	A_ESP + 4

A_LDT_SEL		equ	A_SS + 4
A_LDTS			equ	A_LDT_SEL  + 4
A_PARENT        equ A_LDTS  + 4
A_PID			equ	A_PARENT	+  24
A_CR3			equ	A_PID  + 4 
A_SENDREC		equ	3
A_RECEVE		equ	2
A_SEND			equ	1

A_CLOCK_TICK		equ	1

;;  task number 
A_CLOCK_TASK		equ	-3
A_MEM_TASK		equ	-4
A_TTY_TASK		equ	-5
A_FLOPPY_TASK		equ	-6
