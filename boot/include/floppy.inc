FD_DOR   	 equ  3f2h
FD_DATA		 equ  3f5h
FD_STATUS  	 equ  3f4h
FD_DIR   	 equ  3f7h
FD_DCR		 equ  3f7h


FD_CHANG	equ	0c0h

FD_SPECIFY      equ  	03h
FD_SENSEI	equ	08h
FD_WRITE	equ	0c5h

FD_SEEK		equ	0fh
FD_READ		equ	0e6h
FD_RECALIBRATE	equ	7h


DMA_READ	equ	46h
DMA_WRITE	equ	4ah

;buffer
BUFFER	equ	01000h  ;愧对这个buffer 居然自己将它擦写 又找为什么其值改变


;status
STATUS_BUSYMASK	equ	0fh
STATUS_BUSY	equ	10h
STATUS_DMA	equ	20h
STATUS_DIR	equ	40h
STATUS_READY	equ	80h


ST0_INTR	equ	0c0h
