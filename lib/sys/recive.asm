
global    receve

;;常量
A_RECEVE   equ     	2
A_SEND     equ		1  
A_SENDREC  equ      3

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
