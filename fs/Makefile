ASM		=	nasm	
ASMFLAG		=	-f elf -g
CC		=	gcc
CCFLAG		=	-fno-builtin -g -c  -m32 -march=i686 -fno-stack-protector -nostdinc -I ../include
LD		=	ld
#这个程序的入口地址为6M+24k 
LDFLAG		=	-melf_i386 -Ttext 0x806000 -Tdata 0x80B000  -nostdlib  ../lib/sys/libcrt0.a 
FS		=        fs
OBJET		=	 ./fs.o ./put.o ./table.o ./super.o ./cache.o \
   ./device.o  ./misc.o ./read.o ./write.o  ./inode.o ./utility.o  \
   ./mount.o  ./pipe.o  ./filedes.o  ./open.o  ./path.o ./buildin_cmd.o \
   ./exec.o ./fork.o

LIB		=	../lib/sys/libsys.a ../lib/user/libc.a
LIBFLAG		=	-lsys
clean		:
		rm  *~ $(OBJET) $(FS)  fs.map

all		: $(FS)
		
tag		:
		rm tags 2> erro.txt ; ctags -R *

map     :$(FS)
	    objdump  -t -S -mi386:intel  $< > fs.map 

$(FS)		: $(OBJET)
		$(LD)  $(LDFLAG) $(OBJET) $(LIB) -o $@


fs.o		: ./fs.c 
		$(CC)		$(CCFLAG)	$<  -o $@

put.o		: ./put.c 
		$(CC)		$(CCFLAG)	$<  -o $@
		
table.o		: ./table.c 
		$(CC)		$(CCFLAG)	$<  -o $@

super.o		: ./super.c 
		$(CC)		$(CCFLAG)	$<  -o $@

cache.o		: ./cache.c 
		$(CC)		$(CCFLAG)	$<  -o $@

device.o	: ./device.c 
		$(CC)		$(CCFLAG)	$<  -o $@

misc.o	: ./misc.c 
		$(CC)		$(CCFLAG)	$<  -o $@

read.o	: ./read.c 
		$(CC)		$(CCFLAG)	$<  -o $@

write.o	: ./write.c 
		$(CC)		$(CCFLAG)	$<  -o $@

inode.o	: ./inode.c 
		$(CC)		$(CCFLAG)	$<  -o $@

utility.o	: ./utility.c 
		$(CC)		$(CCFLAG)	$<  -o $@

mount.o	: ./mount.c 
		$(CC)		$(CCFLAG)	$<  -o $@

pipe.o	: ./pipe.c 
		$(CC)		$(CCFLAG)	$<  -o $@

filedes.o: ./filedes.c 
		$(CC)		$(CCFLAG)	$<  -o $@

open.o  : ./open.c 
		$(CC)		$(CCFLAG)	$<  -o $@
	
path.o: ./path.c 
		$(CC)		$(CCFLAG)	$<  -o $@

buildin_cmd.o: ./buildin_cmd.c
		$(CC)		$(CCFLAG)	$<  -o $@

exec.o: ./exec.c 
		$(CC)		$(CCFLAG)	$<  -o $@

fork.o: ./fork.c 
		$(CC)		$(CCFLAG)	$<  -o $@

