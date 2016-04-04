;;minix3 filesystem  superblock
;;only use a little, put  it here to make it easy understand 
;;and i use a sector as zone ,so the number will different with
;;minix, it is just convenience to me
s_ninodes	dd	0x300
s_nzones	dd	0
s_imap_blocks	dw	1
s_zmap_blocks	dw	1
s_firstdatazone	dd	0x10
s_log_zone_size	dw	0
s_pad		dw	0
s_max_size	dd	0
s_zones		dd	0
s_magic		dw	0

;;the flowing are not in disk,it is added by me, i use it 
;;to find the kernel file
s_inode_array	dd	0x20

;;some constant  used 
BLOCK_SIZ	equ	0x1000
SEC_SIZ		equ	0x200

WORDSIZE        equ    2
DWORDSIZE       equ    4

LOADER_SEG     equ	0x1000
KERNEL_SEG     equ      0x3000

LOADER_OFF     equ      0x200
KERNEL_OFF     equ      0

LINERADDLOADER  equ  0x10*LOADER_SEG
LINERADDKERNEL    equ  0x10*KERNEL_SEG

PHYOFFSETKERNEL	  equ  LINERADDKERNEL + KERNEL_OFF
PHYOFFSETLOADER   equ  LINERADDLOADER + LOADER_OFF
;; inode offset
D_MODE    equ	   0
D_NLINKS  equ	   D_MODE    + WORDSIZE
D_UID     equ      D_NLINKS  + WORDSIZE
D_GID     equ      D_UID     + WORDSIZE
D_SIZE	  equ      D_GID   + WORDSIZE
D_ATIME   equ      D_SIZE  + DWORDSIZE
D_MTIME   equ      D_ATIME + DWORDSIZE
D_CTIME   equ      D_MTIME + DWORDSIZE
D_ZONE    equ      D_CTIME + DWORDSIZE
INODESIZE	equ	D_ZONE + 10*DWORDSIZE  ;(D_MODE )
