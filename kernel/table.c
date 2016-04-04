#ifndef _TABLE_H_
#define _TABLE_H_
#endif

#include <type.h>
#include <glo_fun.h>

#include <protect.h>
#include <proc.h>
//将所有定义的全局变量存储于此
#include <const.h>
#include <glo_var.h>
#include <tty.h>
#include <ipc.h>
#include <page.h>
#include <memory.h>

struct bootimage boot[] = {
    /* proc nr , initial pc , queue, time ticks,  flag ,  proc name */
    {	  0 ,  floppy_task, TASK_Q,   64,      READY,   "floppy_task" },  // 30
    {     1 ,  tty_task,    TASK_Q,   32,      READY,   "tty_task" },  // 38
    {     2 ,  mem_task,    TASK_Q,   64,      READY,   "mem_task"  },   // 40
    {     3 ,  clock_task,  TASK_Q,   64,      READY,   "clock_task" },  // 48
    {     4 ,  sys_task,    TASK_Q,   64,      READY,   "sys_task"  },  // 50
    {	  5 ,  idle,	    IDLE_Q,   64,	   READY,   "harware"  },  // 58
    {     6 ,  (void*)MM_ENTRY,SERVER_Q ,64 ,     READY ,  "mm_server" },//60
    {     7 ,  (void*)FS_ENTRY,SERVER_Q, 64,      READY,   "fs_server" },  //68
    {     8 ,  (void*)INIT_ENTRY,USER_Q ,  64 ,     UNREADY , "init"   }   ,//70
};
// 将运行init是在系统组件已经初始化完成后
//利用预编译器检查boot数组大小是否定义有误  :) 
extern char   dump[ sizeof(boot)/sizeof(struct bootimage) == BOOT_NR ? 1 : -1];

void  ( *exception_fun[] )()= {
    divide_error,
    single_step_exception,
    nmi,
    breakpoint_exception,
    overflow,
    bounds_check,
    inval_opcode,
    copr_not_available,
    double_fault,
    copr_seg_overrun,
    inval_tss,
    segment_not_present,
    stack_exception,
    general_protection,
    page_fault,
    copr_error
};

