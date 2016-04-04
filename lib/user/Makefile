# program ,flags, etc
ASM		=	nasm
ASMFLAG		=	-f elf -I  ./  -g
CC		=	gcc
CCFLAG		=  	-fno-builtin -nostdinc 	-fno-stack-protector -I ../../include/  -g  -c
LD		=	ld
LDFLAG		=
OBJECTS		=  	 ./printf.o  ./string.o  ./fork.o ./read.o  \
				 ./exec.o  ./open.o  ./close.o ./mknode.o \
				 ./dup.o  ./exit.o  ./seek.o

AR		=	ar
ARFLAG		=	-rc
LIBNAME		=	libc.a

all	:  $(OBJECTS)

clean	:  $(OBJECTS)
	rm  *~  $(OBJECTS)  $(LIBNAME) 2> erro.txt
	
printf.o : ./printf.c
	 $(CC)    $(CCFLAG)  $<   -o   $@
	 $(AR)  $(ARFLAG)   $(LIBNAME)  $@
	 
string.o : ./string.c
	 $(CC)    $(CCFLAG)  $<   -o   $@
	 $(AR)  $(ARFLAG)   $(LIBNAME)  $@

fork.o : ./fork.c
	 $(CC)    $(CCFLAG)  $<   -o   $@
	 $(AR)    $(ARFLAG)   $(LIBNAME)  $@

read.o : ./read.c
	 $(CC)    $(CCFLAG)  $<   -o   $@
	 $(AR)    $(ARFLAG)   $(LIBNAME)  $@

exec.o : ./exec.c
	 $(CC)    $(CCFLAG)  $<   -o   $@
	 $(AR)    $(ARFLAG)   $(LIBNAME)  $@

open.o : ./open.c
	 $(CC)    $(CCFLAG)  $<   -o   $@
	 $(AR)    $(ARFLAG)   $(LIBNAME)  $@

close.o : ./close.c
	 $(CC)    $(CCFLAG)  $<   -o   $@
	 $(AR)    $(ARFLAG)   $(LIBNAME)  $@
	 
mknode.o : ./mknode.c
	 $(CC)    $(CCFLAG)  $<   -o   $@
	 $(AR)    $(ARFLAG)   $(LIBNAME)  $@

dup.o : ./dup.c
	 $(CC)    $(CCFLAG)  $<   -o   $@
	 $(AR)    $(ARFLAG)   $(LIBNAME)  $@

exit.o : ./exit.c
	 $(CC)    $(CCFLAG)  $<   -o   $@
	 $(AR)    $(ARFLAG)   $(LIBNAME)  $@

seek.o : ./seek.c
	 $(CC)    $(CCFLAG)  $<   -o   $@
	 $(AR)    $(ARFLAG)   $(LIBNAME)  $@

