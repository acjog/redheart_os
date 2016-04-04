ASM		=	nasm	
ASMFLAG		=	-f elf -g
CC		=	gcc
CCFLAG		=	-fno-builtin -g -c -m32  -march=i686 -fno-stack-protector -nostdinc -I ../include
LD		=	ld
#这个程序的入口地址为6M+24k 
LDFLAG		=	 -melf_i386 -Ttext 0x906000 -Tdata 0x907000 -nostdlib -L ../lib/sys  ../lib/sys/libcrt0.a

INIT		=        init
OBJET		=	 ./init.o ./put.o
LIB		=	../lib/sys/libsys.a  ../lib/user/libc.a
LIBFLAG		=	-lsys
clean		:
		rm  *~ $(OBJET) $(INIT)

all		: $(INIT)
		
tag		:
		rm tags 2> erro.txt ; ctags -R *
		
map		:
	objdump -t -S -mi386:intel  init > init.map


$(INIT)		: $(OBJET)
		$(LD)  $(LDFLAG) $(OBJET) $(LIB) -o $@

init.o		: ./init.c 
		$(CC)		$(CCFLAG)	$<  -o $@

put.o		: ./put.c 
		$(CC)		$(CCFLAG)	$<  -o $@
