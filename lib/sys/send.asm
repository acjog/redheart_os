global    send
;常量
A_RECEVE   equ     	2
A_SEND     equ		1  

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



