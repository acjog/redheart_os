#ifndef _GLO_VAR_H_
#define _GLO_VAR_H_

#ifndef _TABLE_H_
#define  EXTERN  extern
#else
#define EXTERN 
#endif
#include <ipc.h>
#include <memory.h>

EXTERN u32_t    scr_loc                 ;
EXTERN u32_t    lost_ticks              ;
EXTERN i32_t	k_reenter               ;
EXTERN u16_t    sel_ldt                 ;
EXTERN message  kernel_mess             ;
EXTERN u8_t     task_stack[BOOT_NR*KERNEL_STACK_SIZE];


#define  BUF_BYTES    32
extern  struct bootimage {
  u16_t proc_nr;
  void  (*initial_pc)()  ;
  u16_t queue            ;  //调度队列
  u16_t ticks            ;  //time ticks
  u32_t flag             ;  // flag
  char  name[30]         ;  //进程名字
}boot[];

//keyboard.c
extern u32_t  keymap[]              ;
extern u32_t  l_shift               ;
extern message int_mess             ;
EXTERN  message *task_mess[TASK_NR] ;

extern struct {
    u32_t  base ;
    u32_t  num  ;
}seg_base[4]    ;

#endif

