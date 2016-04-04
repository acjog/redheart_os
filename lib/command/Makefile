# program ,flags, etc
ASM		=	nasm
ASMFLAG		=	-f elf -I  ./  -g
CC		=	gcc
CCFLAG		=  	-fno-builtin -nostdinc 	-I ../../include/  -g  -c
LD		=	ld
LDFLAG		=	 -Ttext 0x606000 -Tdata 0x607000  -nostdlib  ../sys/libcrt0.a 
LIB         =  ../user/libc.a ../sys/libsys.a
OBJECTS		=  	./echo

all	:  $(OBJECTS)

clean	:  $(OBJECTS)
	rm  *~  $(OBJECTS)   2> erro.txt

map:  echo
	    objdump  -t -S -mi386:intel  $< > echo.map 

	
echo  :  ./echo.c  ./put.c
		$(CC) $(CCFLAG) echo.c -o echo.o
		$(CC) $(CCFLAG) put.c -o put.o
		$(LD) $(LDFLAG) echo.o put.o $(LIB)  -o $@
#		strip  $@
		../../filemanage ../../a.img  1
