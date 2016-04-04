#include <const.h>
#include <com.h>
#include <ipc.h>
#include <glo_fun.h>
#include <glo_var.h>
#include <memory.h>
#include <page.h>
#include <proc.h>

// 函数声明
i32_t	do_mem(message *);
i32_t	do_setup(message *);
void    mem_init();

static message mem_mess;

//关于虚拟盘的支持 我直接将基址和大小用数组存放在这里
//若我以后有闲功夫 可以从软盘中读取这个信息
//但是我现在不想弄的这么复杂 
//这样做 即使以后改也是很方便的 
#define   NR_RAMS	1	
#define	  RAM		0
#define   SIZE		0x11000*3
#define	  BASE		(u8_t*)(0x246000 )
#define	  LIMIT		(u8_t*)(0x400000 )

u8_t  *ram_origin[NR_RAMS] = { BASE  } ;
u8_t  *ram_limit[NR_RAMS]  = { LIMIT } ;

void mem_task()
{
	i32_t  opcode , caller , r ;
    printf( "mem_task ");
    mem_init();
	while( TRUE )
	{
	  receve( ANY , &mem_mess);
	  opcode = mem_mess.m_type ;
	  caller = mem_mess.m_source ;
	  switch ( opcode  )
	  {
	      case	MEM_READ:
		      {
			      r  = do_mem(&mem_mess);
			      break;
		      }
	      case	MEM_WRITE:
		      {
			      r = do_mem(&mem_mess);
			      break;
		      }
	      case	MEM_IOCTL:
		      {
			    break;
		      }
          case  MEM_SETUP:
              {
                r = do_setup(&mem_mess);
                break ;
              }
          default:
		      {
			      r = EINVAL ;
		      }
	  }
	  mem_mess.m_type = TASK_REPLY ;
	  send( caller , &mem_mess );
	}
}

// 这里的考虑错误了  6.11 因为没有考虑拷贝时
// 跨越多个内存物理页 
i32_t	do_mem( message  *m_ptr )
{
   PROCESS *rp ;
   i32_t  device , count ,  len , procnr , pos  ;
   u8_t *mem_phy , *user_phy   ;
   u8_t *vir_addr ;

   device  = m_ptr->MEM_DEVICE ;
   count   = m_ptr->MEM_COUNT  ;
   procnr  = m_ptr->MEM_PROCNR ;
   count   = 0x1000;
   // 检测
   if ( device < 0 || device >= NR_RAMS )
   {
	return ENXIO ;
   }
   //  这里的地址一定要扇区对齐
   pos  = ((i32_t)(m_ptr->MEM_POS))<<9 ;
   mem_phy  = ram_origin[device] + pos ;
   if ( mem_phy > ram_limit[device] )
   {
	return 0;
   }
   // 若数据超过范围 则截断超过部分 
   if ( mem_phy + count > ram_limit[device] )
   {
	count  = ram_limit[device] - ram_origin[device] ;
   }
   vir_addr = m_ptr->MEM_ADDR ;
  // printf("mem_phy:%d,flag:%d,addr:%d",mem_phy,m_ptr->m_type,vir_addr);
   if ( m_ptr->m_type == DISK_READ )
   {  
//	printf("task:%d,addr:%d,proc:%d,vir_addr:%d,count:%d",MEM_TASK,mem_phy,procnr,vir_addr,count);
	// 这个是今天封装的接口  6.11 蛮好用的 
	copy_data(MEM_TASK,mem_phy,procnr,vir_addr,count);   
   }
   else
   {
//	printf("task:%d,addr:%d,proc:%d,vir_addr:%d,count:%d",MEM_TASK,mem_phy,procnr,vir_addr,count);
	copy_data(procnr,vir_addr,MEM_TASK,mem_phy,count);
   }
   return count ;
}

// 这里的setup可能不必要用 因为ram盘的基址和大小 一开始我都知道 
// 但是 8.8 mm需要init所使用内存的情况
i32_t   do_setup( message *m_ptr )
{
  mem_mess.MEM_BASE =  seg_base[INIT_PROC_NR +1].base ;
  mem_mess.MEM_NUM  =  seg_base[INIT_PROC_NR +1].num ;
  printf( " setup now ... " );
}

void mem_init()
{
    PROCESS *rp ;
    u32_t  m , n , base ,tmp , count , i , test ; 
    //将init程序从内核内存移除
    base = get_pages(seg_base[INIT_PROC_NR+1].num);
    MemCpy((void*)seg_base[INIT_PROC_NR+1].base,(void*)base,(seg_base[INIT_PROC_NR+1].num)<<12);
    seg_base[INIT_PROC_NR+1].base = base ;
    rp = proc_addr(INIT_PROC_NR);
    n = INIT_ENTRY >> 22 ;
    m = INIT_ENTRY >> 12 & 0x3ff ;
    tmp = ( (*(u32_t *)(rp->p_cr3 + n*4 ) & 0xfffff000) + m*4); 
    for ( i = 0 ; i < seg_base[INIT_PROC_NR+1].num ; i ++ ) 
    {
        *(u32_t *) tmp = ( seg_base[INIT_PROC_NR+1].base + i*0x1000) | PG_P | PG_USU | PG_RWW  ;
        tmp +=  4 ;
    }
    //设置init的栈
    base  = get_pages(ENV_PAGES);
   count = ENV_PAGES ;
    for ( i = 0 ; i < ENV_PAGES ; ++i )
    {
       n    = ( HIGH_MEM - ( count<<PAGE_SHIT ) ) >> 22;
       m    = ( ( HIGH_MEM - ( count<<PAGE_SHIT ) ) >>12 ) & 0x3ff ;
       tmp = ( (*(u32_t *)(rp->p_cr3 + n*4 ) & 0xfffff000) + m*4); 
       *(u32_t *) tmp = ( base + i*0x1000) | PG_P | PG_USU | PG_RWW  ;
       count-- ;
    }
    rp->regs.esp = (u32_t*)INIT_STACK_BASE ;
    rp->regs.ebp = (u32_t*)INIT_STACK_BASE ; 
}
