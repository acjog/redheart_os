#include <type.h>
#include <const.h>
#include <sys.h>
#include <stdio.h>
#include <callnr.h>
#include <erro.h>
#include <ipc.h>
#include <com.h>
#include <proc.h>

#include "h/const.h"
#include "h/glo.h"
#include "h/mproc.h"
#include "h/elf.h"
#include "h/mm.h"

//声明系统调用数组
extern  i32_t  (*call_vector[])() ;
int  test ;
// 函数声明
void  mm_init() ;
void  get_work();

i32_t main()
{
    mm_init();
    test  = 1 ;
    printf( " mm " );
    while ( TRUE  ) 
    {
        get_work()          ;
        error = OK          ;
        dont_reply = FALSE  ;
        if ( mm_call < 0 || mm_call > NCALLS )
        {
            error  =  E_BAD_CALL  ;
        }
        else
        {
            // printf( " mm_call:0x%d \n ", mm_call );
            error  = (*call_vector[mm_call])();
        }
        if ( dont_reply )
        {
            continue  ;
        }
        if ( mm_call == EXEC && error == OK  )
        {
            continue   ;
        }
        mm_out.m_type  = result    ;      
        // printf( " hello world,who :%d ",who );
        send( who , &mm_out);
    }
    return 0;
}

void mm_init() 
{
    i32_t  i,blocksiz = 4096 , n ; 
    u32_t  m  ;
    struct vm_area_struct  *mm ;
    // 全局变量初始化
    procs_in_use   =  INIT_PROC_NR + 1 ;
    // mm_struct结构初始化
    for ( i = 0 ; i < MM_STRUCTS -1  ; i ++ )
    {
       mm_struct[i].vm_next  = &mm_struct[i+1] ;
    }
    mm_struct[i+1].vm_next  =  NULL            ;
    vm_front                 =  &mm_struct[0]   ;
    //初始化mm table
    mproc[MM_PROC_NR].mp_flag    |= IN_USE ;
    mproc[FS_PROC_NR].mp_flag    |= IN_USE ;
    mproc[INIT_PROC_NR].mp_flag  |= IN_USE ; 
    //初始化init的虚拟地址空间
    mproc[MM_PROC_NR].mm  = NULL ;
    mproc[FS_PROC_NR].mm  = NULL ;
    mm  = get_vm_struct();
    mm->flag           = MM_PRO   ;
    mm->start_address  = 0x906000 ;
    mm->end_address    = 0x908000 ;
    mproc[INIT_PROC_NR].mm   = mm ;
    mm->vm_next  = NULL ;
    mm  = get_vm_struct();
    mm->start_address = (HIGH_MEM-ENV_PAGES*0x1000) ;
    mm->end_address = HIGH_MEM ;
    mm->vm_next  = mproc[INIT_PROC_NR].mm;
    mproc[INIT_PROC_NR].mm  = mm ;
}

void get_work()
{
    receve(ANY, &mm_in);
    who    = mm_in.m_source  ;
    if ( who < HARDWARE || who > NR_PROCS )
    {
        panic( " error happen " );
    }
    mm_call = mm_in.m_type   ; 
    mp      = &mproc[who]    ;
}
