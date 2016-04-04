;导出全局符号
global    sendrec

;常量
A_RECEVE   equ     	2
A_SEND     equ		1  
A_SENDREC  equ      3

;;
;;  sendrec ( u32_t  dest , message *ptr)
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

