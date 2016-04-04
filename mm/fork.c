#include<type.h>
#include<const.h>
#include<callnr.h>
#include<erro.h>
#include<ipc.h>
#include<com.h>
#include<sys.h>
#include<stdio.h>
#include<page.h>

#include "h/const.h"
#include "h/glo.h"
#include "h/mproc.h"
#include "h/param.h"
#include "h/utility.h"
#include "h/mm.h"

pid   next_pid  = INIT_PROC_NR  +  1 ;

i32_t  do_fork()
{
    i32_t  i , dir , frame      ;
    struct  mproc  *rmp , *rmc  ; 
    u8_t  *sptr , *dptr         ;
    pid       parent , child  ;
    struct vm_area_struct  *p , *tmp   ;
    // 检测
    if ( procs_in_use ==  NR_PROCS  )
    {
        return ERROR  ;
    }
    for ( rmc=&mproc[3]; rmc < &mproc[NR_PROCS] ; rmc++ ) 
    {
        if ( ! rmc->mp_flag & IN_USE  )
            break ;
    }
    rmc->mp_flag     |= IN_USE   ;
    child     =  rmc - &mproc[0] ;
#ifdef  _MM_DEBUG
    printf("child:%d\n",child);
#endif
    parent    =  who             ;
    rmp       =  &mproc[parent]  ;
    procs_in_use++     ;
    sptr      =  (u8_t*)rmp      ;
    dptr      =  (u8_t*)rmc      ;
    i         =  sizeof( struct mproc );
    // 复制进程表项
    for ( ; i > 0 ; i-- ) 
    {
        *dptr++  = *sptr++   ;
    }
    // 处理地址空间 
    rmc->mm = NULL ;
    for ( p = rmp->mm ; p != NULL ; p = p->vm_next  )
    {
           tmp = get_vm_struct() ;
           tmp->flag           =  p->flag          ;        
           tmp->start_address  =  p->start_address ;
           tmp->end_address    =  p->end_address   ;
           tmp->vm_next    = NULL ;
           if ( rmc->mm == NULL )
           {
                   rmc->mm = tmp ;
           }
           else
           {
                  tmp->vm_next  = rmc->mm ;
                  rmc->mm = tmp ;
           }
    }
    sys_fork(parent,child,(void*)0,(void*)0   );
    // 通知 fs
    tell_fs(FORK,parent,child);
    result   = child ;
    // 唤醒孩进程
    #ifdef _MM_DEBUG
    printf(" coming here child :%d ",child ) ;
    #endif
    reply(child,0);
    return child ;
}

