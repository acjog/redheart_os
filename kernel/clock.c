#include <type.h>
#include <glo_fun.h>
#include <glo_var.h>
#include <const.h>
#include <com.h>
#include <ipc.h>

#define HZ           60
#define SCHED_RATE   10
void    init_clock()                         ;
u32_t	do_clockclick()                      ;
static  void  do_get_time()                  ;
static  void  do_set_alarm( message *)       ;
static  void  accounting()                   ;

static  message     clock_mess               ;
static  realtime    real_time                ;
static  realtime    boot_time                ;
static  realtime    next_alarm               ;
static  realtime    sched_ticks              ;
static  i32_t    (*watch_dog[TASK_NR+1])()   ;

void clock_task()
{
	i32_t opcode , caller; 
	u32_t	r;
	real_time = 0 ;
	init_clock();
	printf(" clock_task  ");
	while ( TRUE )
	{
	 receve(ANY, &clock_mess);
	 opcode = clock_mess.m_type   ;
	 caller = clock_mess.m_source ;
	 switch (opcode)
	 {
		 case	CLOCK_TICK:
			 {
				r = do_clockclick();
				break;
			 }
		 case   SET_ALARM: 
			 {
				do_set_alarm( &clock_mess);
				break ;
			 }
		 case   GET_TIME :
			 {
				do_get_time();
				break;
			 }
		default:
			break;
	 }
	 clock_mess.m_type = TASK_REPLY ;
	 if ( opcode != CLOCK_TICK )
	 {
         send( caller  , &clock_mess );
	 }
	}
}

extern u8_t  mm_buf[] ;
u32_t	do_clockclick()
{
	PROCESS *rp ;
	i32_t  pro_nr ; 
	u32_t  t ; 
	t            =  lost_ticks     ;
	real_time    += t + 1          ;
  // printf( " real_time:%d ", real_time );
	lost_ticks   -= t              ;
	if ( next_alarm <=  real_time )
	{
        //printf( " alarm happen ");
		next_alarm = MAX_P_LONG  ;
		// 检测是否有进程的定时器到达  
		for ( rp = &proc[0] ; rp < &proc[PROCE_NR] ; rp ++ )
		{
			if ( rp->p_nextalarm <= real_time )
			{
			  pro_nr  =   rp - proc -TASK_NR ;
			  //  用户进程建立警报信号
			  if ( pro_nr >= 0 )
				  nothing();
			  else
			  {
			  // 调用task的看门狗函数
				  (*watch_dog[-pro_nr])();
			  }
			  rp->p_nextalarm  =  MAX_P_LONG  ; 
			}
			else
			{
			   // 确定下一个警报定时器
				if (  rp->p_nextalarm <=  next_alarm ) 
				{
					next_alarm = rp->p_nextalarm ;
				}
			}
		}
	}
	// 向进程收费
	accounting();
	if ( --sched_ticks == 0 )
	{
       sched_ticks =  SCHED_RATE ;
	   if ( bill_ptr == pre_ptr  )
       {
		   schedule() ;
           //printf( "  %d " , bill_ptr - &proc[0]  );
       }
	}
	return 0;
}

void do_set_alarm( message *m_ptr )
{
	PROCESS *rp ;
	i32_t proc_nr ;
	long  delta_time ;
	i32_t  (*fun)();
	proc_nr    = m_ptr->CLOCK_PROC_NR ; 
	fun        = m_ptr->FUNC_TO_CALL ;
	delta_time = m_ptr->DELTA_TICKS ;
	rp  = proc_addr ( proc_nr ) ;
	rp->p_nextalarm = real_time + delta_time ;
//    printf( "delta_time: %d ", delta_time  );
	if ( proc_nr < 0 )
	{
		watch_dog[-proc_nr] = fun ;
	}
	next_alarm = MAX_P_LONG ;
	for ( rp = proc ; rp < &proc[BOOT_NR] ; rp++ )
	{
               if ( rp->p_nextalarm < next_alarm )
	                   next_alarm = rp->p_nextalarm ;
	}
        // printf( "alarm :%d,  real_time:%d ", next_alarm, real_time );
	return ;
}
void do_get_time()
{
  clock_mess.m_type = REAL_TIME ;
  clock_mess.m1_i1  = boot_time + real_time/HZ;
}

void accounting()
{
	if (cur_proc  >= LOW_USER)
	{
           bill_ptr->p_usertime++;
	}
	else
	{
	   bill_ptr->p_systime++;
	}
}

void init_clock()
{
 init_8253A();
 next_alarm = MAX_P_LONG ;
 set_vec(0x20,(u32_t*)clock_int,0,0);		//设置时钟中断	
 enable_int(0);
 sched_ticks   = SCHED_RATE ;
 return ; 
}
