# program ,flags, etc
ASM		=	nasm
ASMFLAG		=	-f elf -I  ./include/  -g
CC		=	gcc
CCFLAG		=  	-fno-builtin -nostdinc -m32 -march=i686 -fno-stack-protector   -I include -g  -c
LD		=	ld
LDFLAG		=	-melf_i386 -Ttext 0x30200 -Tdata 0x38000 
BACKUP	        =       ./kernel/*~  ./lib/*~  ./*~ ./include/*~  
OBJET 		=	./boot/boot.bin ./boot/loader.bin
BINUX		=	./kernel.bin
DG_BINUX        =       ./dg_kernel.bin
KERNEL		=  	./kernel/start.o  ./kernel/kernel.o  \
     ./kernel/klib.o  ./kernel/keyboard.o ./kernel/table.o \
     ./kernel/tty.o   ./kernel/clock.o   ./kernel/floppy.o \
     ./kernel/sys.o  ./kernel/memory.o  ./kernel/page_map.o    \
	 ./kernel/string.o  ./kernel/ipc.o
EXTRA  = ./lib/common/page_alloc.o 

FS		=       ./fs/
MM		=	./mm/
INIT            =      ./init/
TOOLS		=      ./tools/
LIB		= 	./lib/mpxlib.o 
INCLUDE		=       ./include/
TARGET		=       $(KERNEL)  $(LIB)

#action here
clean 		     :
			cd  $(MM)  ; make clean ;cd ..  
			cd  $(FS)  ; make clean ; cd ..
		     cd  $(INIT) ; make clean ; cd .. 
	         rm   $(DG_BINUX)  $(BINUX) $(TARGET)  $(BACKUP)  ./image   kernel.map nm.map  2> erro.txt
		     
img		     :
		     sudo dd if=/dev/zero of=a.img bs=512 count=2880 
		     sudo chown zhuangbian a.img
		     sudo chgrp zhuangbian a.img
		     dd if=boot/bootblock.bin of=a.img bs=512 count=1 conv=notrunc
		     ./mkfs  a.img
tag		     :
	             rm  tags 2> erro.txt  ;  ctags  -R  * 
#拷贝 保存
everything           :  $(TARGET)  
		       ./filemanage a.img 1
              
all           :   $(KERNEL) $(EXTRA) $(LIB)  $(FS) $(MM) $(INIT)
		      $(LD)   $(LDFLAG)  $(TARGET) $(EXTRA) -o $(BINUX)	
		     cp      $(BINUX)  $(DG_BINUX)
		     strip   $(BINUX)
		     cd      $(FS)  ; make all ; cd ..
		     cd      $(MM)  ; make all ; cd ..
		     cd      $(INIT) ; make all ; cd ..
		     cd  $(TOOLS)  ;	make  build_image ; cp ./image .. ;  cd    ..
		       ./filemanage a.img 1
run		     : all
		       dbochs -rc bochsrc_cmdrc -qf dbochsrc 

debug		     : all
#	          cp  fs/fs          ./debug_d
#	          cp  mm/mm          ./debug_d
#	          cp  init/init       ./debug_d
			  cp  dg_kernel.bin  ./debug_d
#			  cp   lib/command/echo ./debug_d
		      gbochs -qf gbochsrc 2> bochs_gdb.txt  &
		      gdb 
debug0		      : all
		      dbochs -qf bochsrc1 -rc bochsrc_cmdrc
			
map                   :  ./kernel.map  ./nm.map 
		
		      
#map file
./kernel.map  : $(DG_BINUX)
	objdump -t -D -S -mi386:intel  $< > $@

./nm.map	: $(DG_BINUX)
			nm $< > $@
# kernel  object file 	
./kernel/start.o      : ./kernel/start.asm   ./include/sconst.inc
		      $(ASM) $(ASMFLAG)  $< -o  $@

./kernel/kernel.o     : ./kernel/kernel.c   $(INCLUDE) 
		      $(CC)  $(CCFLAG)   $< -o  $@  
            	   
./kernel/klib.o       : ./kernel/klib.c  $(INCLUDE)
	              $(CC)  $(CCFLAG)   $< -o  $@ 
	
./kernel/keyboard.o   : ./kernel/keyboard.c $(INCLUDE)
		      $(CC)  $(CCFLAG)   $< -o  $@ 
		   
./kernel/table.o      :  ./kernel/table.c   $(INCLUDE)
		      $(CC)  $(CCFLAG)   $< -o  $@
			
./kernel/tty.o        :  ./kernel/tty.c    $(INCLUDE)
	              $(CC)  $(CCFLAG)   $< -o  $@

./kernel/clock.o      :	./kernel/clock.c  $(INCLUDE)
		      $(CC)  $(CCFLAG)   $< -o  $@

./kernel/sys.o        :  ./kernel/sys.c   $(INCLUDE)
		      $(CC)  $(CCFLAG)   $< -o  $@

./kernel/floppy.o     : ./kernel/floppy.c $(INCLUDE)
		      $(CC)  $(CCFLAG)   $< -o  $@

./kernel/memory.o     : ./kernel/memory.c $(INCLUDE)
		      $(CC)  $(CCFLAG)   $< -o  $@


#  lib object file

./lib/mpxlib.o        : ./lib/mpxlib.asm   ./include/sconst.inc
	              $(ASM)  $(ASMFLAG) $< -o  $@ 

./kernel/string.o	  : ./kernel/string.c
			$(CC)  $(CCFLAG)   $< -o  $@

./kernel/ipc.o		  : ./kernel/ipc.c
			$(CC)  $(CCFLAG)   $< -o  $@

./kernel/page_map.o			: ./kernel/page_map.c  $(INCLUDE)
			$(CC)  $(CCFLAG)   $< -o  $@

./lib/common/page_alloc.o 		: ./lib/common/page_alloc.c $(INCLUDE) 
			$(CC)  $(CCFLAG)   $< -o  $@
