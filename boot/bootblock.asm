        STACKSEG  equ    0x0
		STACKOFF  equ    0x7b00
		BOOTSEG   equ    0x1000
		BOOTOFF	  equ	 0x200	
                BUFFER    equ    0x0600
		
		LOGSEC    equ    8

		DEVICE    equ    0
		LOWSEC	  equ    2   
                SECPCYL   equ    6

                           
		org	0x7c00
boot:
		mov	ax,  cs
		mov	ds,  ax
		mov	es,	ax	

                mov     ax,  STACKSEG
		cli
		mov	ss,  ax
	        mov     sp,  STACKOFF
		sti 
                
                xor	ax,  ax
		push	ax
		push	ax

		push    dx
		mov	bp,	sp
		push    es
		push	si             ;  es:si  = partition table
		
		push    bp
		mov	bp,	message0
                mov	bx,	0x0003
		mov	dx,	0x0000
		mov	cx,	message1-message0
		mov	ax,	0x1301
		int	0x10	      ;  print loading ...  
		pop	bp

		mov	di,	sectors
		jmp     floppy
next:
		inc	di
                
floppy:         xor	ah,	ah
		int	0x13
		mov	cl,	[di]
		mov	cl,	9
		je	success
		mov	es,	[bp+LOWSEC]
		mov	bx,	BUFFER         ;  es:bx  buffer  0x0000:0x0600
		mov	ax,	0x0201     ;  read  sector  sectors = 1
		xor     ch,	ch         ;  track  0  
		xor	dh,	dh         ;  drive  dl , head  0
		int     0x13
		jc	next
success:
		mov	dh,	2
loadboot:
		; load boot  from boot device 
                mov     al,     [di]
		mul	dh
                mov	word[bp+SECPCYL],  ax
                mov	ax,     BOOTSEG
		mov	es,	ax
		mov	bx,	BOOTOFF
		mov	si,	address
load:
		mov	ax,	[si+1]
		mov	dl,	byte[si+3]
	        xor	dh,	dh
		add	ax,	[bp+LOWSEC+0]
		adc	dx,	[bp+LOWSEC+2]
                div     word[bp+SECPCYL]      ;  ax = cylinder dx=sector with cylinder	
		xchg	ax,	dx            ;  ax = sector with cyl dx = cyli
		mov	ch,	dl	 
		div     byte  [di]        ;  al = head  ah = sector
                xor  	dl,	dl
		shr	dx,	2
		or	dl,	ah        ;  dl[6..7] = high cylinder
		mov	cl,	dl	  ;  cl[6..7] = high cylinder
		inc     cl		  ;  cl[0..5] = sector  [1-origin]
		mov	dh,	al        ;   head
		mov	dl,	[bp+DEVICE]
		mov	al,	[di]
		sub	al,	ah
		cmp	al,	[si]
		jbe     read
		mov	al,	[si]
read:
		push    ax
		mov	ah,	0x02
		int	0x13
		pop	cx
rdeval:
		jc	error
		mov	al,	cl
		add	bh,	al
		add	bh,	al
		add	[si+1], ax
		adc     [si+3], ah
		sub	[si],	al
		jnz     load
		add	si,	4
		cmp	ah,	[si]
		jnz	load
done:
		push	bp
		xor	ax,	ax	
		mov	es,	ax	
		mov	bp,	message2
                mov	bx,	0x0003
		mov	dx,	0x0100
		mov	cx,	sectors-message2
		mov	ax,	0x1301
		int	0x10
		pop	bp	
		pop	si
		pop	es
		jmp	BOOTSEG:BOOTOFF
error:
		push    bp
		xor	ax,	ax	
		mov	es,	ax	
		mov	bp,	message1
                mov	bx,	0x0003
		mov	dx,	0x0200
		mov	cx,	message2-message1
		mov	ax,	0x1301
		int	0x10
		xor	ah,	ah
		int	13h	
		pop	bp
		jmp	load
		jmp     $
		

message0:        db    "loading ..."
message1:        db    "load fail  "
message2:        db    "success load",0
sectors:         db     18,15,9
address:	 db     0x10,0x88,0x0,0x0	    
times   510-($-$$)  db  0
dw	        0xaa55
