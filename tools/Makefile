ASM		=	nasm	
ASMFLAG		=	-f elf -g
CC		=	gcc
CCFLAG		=	 -g -c 
LD		=	ld
#这个程序的入口地址为32M+384k 
LDFLAG		=	-Ttext 0x2060000
BUILD		=        build
OBJET		=	./build.o
IMAGE           =      image
KERNEL		=	../kernel.bin
MM		=	../mm/mm
FS		=	../fs/fs
INIT		=	../init/init
clean		:
		-rm  *~  2> /dev/null
		-rm ./*.map 2> /dev/null
		-rm $(OBJET) $(BUILD)  $(IMAGE) 2> /dev/null

all		: $(BUILD)
		 ./build   $(KERNEL) $(MM) $(FS) $(INIT)  $(IMAGE)
build_image		:  $(BUILD)
	         ./build   $(KERNEL) $(MM) $(FS) $(INIT) $(IMAGE)
		 cp ./image ..
		
tag		:
		rm tags 2> erro.txt ; ctags -R *

map		:  $(BUILD)
		objdump  -t -S -mi386:intel  $< > build.map

$(BUILD)	: $(OBJET)
		$(CC)  -g  $(OBJET)  -o  $@

build.o		: ./build.c 
		$(CC)		$(CCFLAG)	$<  -o $@
