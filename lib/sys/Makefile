# program ,flags, etc
ASM		    =	nasm
ASMFLAG		=	-f elf -I  ./  -g
CC		    =	gcc
CCFLAG		=  	-fno-builtin -nostdinc  -fno-stack-protector -I ../../include -g  -c
LD		    =	ld
LDFLAG		=
OBJECTS		=  	send.o crt0.o exit.o recive.o sendrec.o  \
			sys_fork.o   ./sys_copy.o  ./sys_exit.o  \
			sys_exec.o  ./sys_newmem.o ./sys_freemem.o

AR		    =	ar
ARFLAG		=	-rc
LIBNAME		=	libsys.a  
LIBCRT0     =   libcrt0.a

all	:  $(OBJECTS)

clean	:  
	rm  *~   $(OBJECTS)  $(LIBNAME)  2> erro.txt 
	
sendrec.o   :  sendrec.asm
	$(ASM)  $(ASMFLAG)  $<  -o $@ 
	$(AR)	$(ARFLAG)   $(LIBNAME)  $@

	
send.o   :  send.asm
	$(ASM)  $(ASMFLAG)  $<  -o $@ 
	$(AR)	$(ARFLAG)   $(LIBNAME)  $@
	
recive.o   :  recive.asm
	$(ASM)  $(ASMFLAG)  $<  -o $@ 
	$(AR)	$(ARFLAG)   $(LIBNAME)  $@


crt0.o  :  crt0.asm
	$(ASM)  $(ASMFLAG)  $<  -o $@ 
	$(AR)   $(ARFLAG)   $(LIBCRT0)   $@

exit.o:
	cp ../user/exit.o  ./
	$(AR)   $(ARFLAG)   $(LIBCRT0)   $@

sys_fork.o   : sys_fork.c
	$(CC)   $(CCFLAG)   $<  -o $@
	$(AR)	$(ARFLAG)   $(LIBNAME)  $@

sys_copy.o   : sys_copy.c
	$(CC)   $(CCFLAG)   $<  -o $@
	$(AR)	$(ARFLAG)   $(LIBNAME)  $@

sys_exit.o   : sys_exit.c
	$(CC)   $(CCFLAG)   $<  -o $@
	$(AR)	$(ARFLAG)   $(LIBNAME)  $@

sys_exec.o   : sys_exec.c
	$(CC)   $(CCFLAG)   $<  -o $@
	$(AR)	$(ARFLAG)   $(LIBNAME)  $@

sys_newmem.o : sys_newmem.c
	$(CC)   $(CCFLAG)   $<  -o $@
	$(AR)	$(ARFLAG)   $(LIBNAME)  $@

./sys_freemem.o:./sys_freemem.c
	$(CC)   $(CCFLAG)   $<  -o $@
	$(AR)	$(ARFLAG)   $(LIBNAME)  $@

