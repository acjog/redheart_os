ASM		=	nasm	
ASMFLAG		=	-f elf -g
CC		=	gcc
CCFLAG		=	-fno-builtin -g -c -m32 -march=i686 -fno-stack-protector  -nostdinc -I ../include
LD		=	ld
#这个程序的入口地址为6M+24k 
LDFLAG		=	-melf_i386  -Ttext 0x706000 -Tdata 0x708000   -nostdlib -L ../lib  ../lib/sys/libcrt0.a 
MM		=        mm
OBJET		=	 ./mm.o ./table.o ./put.o   ./fork.o \
			 ./exec.o  ./utility.o  ./exit.o  ./vm_area.o
LIB		=	../lib/sys/libsys.a  ../lib/user/libc.a
EXTRA  = ../lib/common/page_alloc.o
LIBFLAG		=	-lsys
clean		:
		rm  *~ $(OBJET) $(MM)  mm.map

all		: $(MM)
		
tag		:
		rm tags 2> erro.txt ; ctags -R *

map     :$(MM)
	    objdump -t -S -mi386:intel $< > mm.map

$(MM)		: $(OBJET)
		$(LD)  $(LDFLAG) $(OBJET) $(EXTRA)  $(LIB) -o $@


mm.o		: ./mm.c 
		$(CC)		$(CCFLAG)	$<  -o $@

put.o		: ./put.c 
		$(CC)		$(CCFLAG)	$<  -o $@

table.o		: ./table.c 
		$(CC)		$(CCFLAG)	$<  -o $@

fork.o	        : ./fork.c 
		$(CC)		$(CCFLAG)	$<  -o $@

exec.o	        : ./exec.c 
		$(CC)		$(CCFLAG)	$<  -o $@

utility.o	: ./utility.c 
		$(CC)		$(CCFLAG)	$<  -o $@

exit.o	        : ./exit.c 
		$(CC)		$(CCFLAG)	$<  -o $@

vm_area.o		:  ./vm_area.c
		$(CC)    $(CCFLAG)  $< -o $@ 

../lib/common/page_alloc.o 		: ../lib/common/page_alloc.c $(INCLUDE) 
			$(CC)  $(CCFLAG)   $< -o  $@

