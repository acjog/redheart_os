#include <type.h>
#include <const.h>
#include <glo_fun.h>
#include <glo_var.h>
#include <proc.h>
#include <ipc.h>
#include <page.h>

extern void (*exception_fun[]);

void main(void)
{
  printf("kernel.c start...\n");
  PROCESS*    rp  =  NULL;
  u8_t       *sp  =  &task_stack[BOOT_NR*KERNEL_STACK_SIZE];
  i32_t task_num  =  0;
  k_reenter       =  0;
  u32_t        i  =  0;
  sel_ldt  =  SEL_LDT_FIRST;
  init_8259A();
  page_init();
  printf("for start...         \n");
  for( rp =  proc ; rp < &proc[BOOT_NR] ;rp++ )  //初始化进程表
   {
      task_num  =  rp - proc - TASK_NR ;
      rp->ldt_sel=sel_ldt;
      MemCpy(&gdt[1], rp->ldts, 2*sizeof(DESCRIPTOR));
      set_dec(rp->ldt_sel>>3,(u32_t*)rp->ldts,TASK_DPL,ATTR_LDT,2*sizeof(DESCRIPTOR)-1);
      rp->ldts[0].attr1+=(TASK_DPL << 5); //+ DA_C;
      rp->ldts[1].attr1+=(TASK_DPL << 5); //+ DA_DRW;
      rp->regs.cs=(8*0)|SA_TIL|RPL_TASK;			//7
      rp->regs.ds=(8*1)|SA_TIL|RPL_TASK;			//f
      rp->regs.es=(8*1)|SA_TIL|RPL_TASK;			//f
      rp->regs.ss=(8*1)|SA_TIL|RPL_TASK;
      rp->regs.fs=(8*1)|SA_TIL|RPL_TASK;			//f
      rp->regs.gs=(8*3)+3;					//0x18
      rp->regs.eflags=0x2242;
      rp->regs.eip=boot[task_num + TASK_NR].initial_pc;		//这里要进行强制转化
      rp->p_callerq  = NULL ;
      // 因为init的栈独立 没有放在内核空间里 故这里不设栈
      // 8.10
      if (  INIT_PROC_NR != task_num )
      {
         rp->regs.esp=(u32_t*)sp;
         sp-= KERNEL_STACK_SIZE ;
      }
      else
      {
         rp->regs.esp=(u32_t*)sp;
      }
      rp->p_nextalarm =  MAX_P_LONG  ;
      sel_ldt         +=8;
      rp->p_ticks_quantum  = boot[task_num + TASK_NR ].ticks;
      rp->p_ticks_left = rp->p_ticks_quantum ;
      rp->p_pid      = boot[task_num + TASK_NR ].proc_nr;	
      rp->p_flag     = boot[task_num + TASK_NR ].flag | P_SLOT_FREE ;
      MemCpy( boot[task_num +TASK_NR ].name, rp->p_name, sizeof( boot[task_num+TASK_NR ].name)  );     
      // 插入就绪队列
      i = boot[task_num+TASK_NR].queue;
      if ( !(rp->p_flag&UNREADY)    )
      {
           if ( rdy_head[i] == NULL )
	    {
                 rdy_head[i] = rdy_tail[i] = rp ;
	    }
	    else
	    {
           rdy_tail[i]->p_nextready = rp ;
	       rdy_tail[i] = rp ;
	       rp->p_nextready = NULL ;
	    }
      }
    }
 for ( i = 0 ; i < 16 ; i++ )
 {
	set_vec(i, (u32_t*)exception_fun[i], 0, 0);
 }
 for ( rp = &proc[0] ; rp < &proc[PROCE_NR] ; rp++ )
 {
    rp->p_callerq  = NULL ;
 }
 //  处理异常 
 for ( i = 16 ; i < 256 ; i++ ) 
 {
    set_vec(i ,(u32_t*)nohandler , 0 , 0);
 }
 set_vec(0x80,(u32_t*)s_call,0,TASK_DPL);
 printf ( " for end ...    \n"   ) ;
 printf ( " cr3 is %d \n" , CR3  ) ;
 pick_proc();
 restart();
}

