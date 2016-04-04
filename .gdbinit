file  debug_d
#设置远程端口调试
target remote localhost:1234
#个人设定
set disassembly-flavor intel
set history expansion 
set history save 
set step-mode on 
#设定断点
#b   pick_proc
#b  floppy_task
#b   clock_task
#b   tty_task
#b   mem_task
#b   sys_task
#b    main
#b   sys_call  
#condition 3  call==1 && src_dest==100 
#b   restart
#b    process_page_share
b   kernel/tty.c:41
#condition 3 j==518
#b    do_newmem
#b main
#b    do_fork
#b    do_exec
#b    do_buildin
#b     MemCpy
#b     strcpy
#b     do_exit
c
#obtain the dir an page 
#finish
#dump memory kernel_dir.hex 0x100000 0x101000
#dump memory kernel_page.hex 0x101000 0x102000

#b  dma_setup
#b  transfer
#b  seek
#设置观察点
layout split

#打印进程的运行状态
#这里的pintf一定要加一个回车刷新缓冲
define  pflag
   printf "f:%d ,t:%d, m:%d ,c:%d ,s:%d ,H:%d ,M:%d,F:%d,I:%d,F:%d\n",proc[0].p_flag, \
   proc[1].p_flag, proc[2].p_flag , proc[3].p_flag,proc[4].p_flag,\
   proc[5].p_flag, proc[6].p_flag,proc[7].p_flag,proc[8].p_flag,\
   proc[9].p_flag
end


#commands  2
#  pflag
#end

# tmp define 
define pcode
     printf  "fl_opcode:%d\n" , floppy[0].fl_opcode
end

#每次打印栈帧值
set  $myframe=0x246000
define  pframe
   append binary memory memory.bin $myframe  $myframe+0x1000
   set  $myframe = $myframe + 0x1000 
end
#显示栈帧值
define pstack
    printf "cur_proc:%d\n" , cur_proc
    x   /6x  $sp  
end
#查看eflag
define peflag
   info register eflags
end

#观察寄存器
define  reg 
    info register $arg0
end
