#ifndef __M_PROC_H__
#define __M_PROC_H__

#include "mm.h"

EXTERN  struct mproc{
      // 与组用户权限有关
      u32_t  mp_pid        ;  //进程号
      u32_t  mp_parent     ;
      uid    mp_effuid     ;  //有效用户id 
      uid    mp_realuid    ;  //实际用户id
      gid    mp_effgid     ;
      gid    mp_realgid    ;
      // 与虚拟地址有关
      struct  vm_area_struct *mm ;
      // 与信号量位图有关
      u16_t mp_flags       ;
      u32_t (*mp_func)()   ;
      u32_t   mp_flag      ;  

}mproc[NR_PROCS];

#define  IN_USE   0x1
#define  EXITING  0x2
#define  ALARM_ON 0x4


#endif
