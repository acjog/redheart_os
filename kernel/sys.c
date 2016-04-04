#include <const.h>
#include <ipc.h>
#include <glo_fun.h>
#include <com.h>
#include <glo_var.h>
#include <callnr.h>
#include <tty.h>
#include <elf.h>
#include <page.h>

//宏定义
#ifdef CMD_SIZE
#undef CMD_SIZE
#endif
#define CMD_SIZE          1
#define SYS_BUF_SIZE      256
#define MAX_ARGV          10
#define EXEC_BUF_SIZE     256 
#define HEAD_NUM          10

struct cmd{
   u8_t *name    ;
   i32_t (*fn)() ;
};

static i32_t caller  ;

//   函数声明 
i32_t  do_fork(message*)          ;
i32_t  do_exec( message *)        ;
i32_t  do_xit( message *)         ;
i32_t  do_sig( message *)         ;
i32_t  do_copy( message *)        ;
i32_t  do_newmap( message *);
i32_t  do_free_page( message *)   ;
i32_t  do_put_page( message *)    ;
i32_t  do_newsp( message *)       ;
i32_t  do_sysbuildin( message *)  ;
i32_t  do_newmem( message * );
i32_t  do_freemem( message * );
void  free_page_mem(u32_t pgd);
//在本文件使用的函数
i32_t  copy_page_tables(u32_t from , u32_t to , i32_t num  );
void   process_page_share( PROCESS  *mp  );
void   parseline( u8_t *cmdline , u8_t **argv );
i32_t  sys_clear() ;

i32_t  put_page( i32_t , u8_t * ,u8_t * , i32_t  );

//文件内部使用缓冲
struct cmd inner_cmd[CMD_SIZE] ={ { "clear",sys_clear } };
u8_t   cmd_buf[EXEC_BUF_SIZE] ;
static message  system_mess;

void sys_task()
{/*{{{*/
    i32_t opcode  , r ;
    printf(" sys_task ");
    while ( TRUE )
    {
        receve( ANY,&system_mess);
        caller = system_mess.m_source ;
        opcode = system_mess.m_type ;
        switch ( system_mess.m_type )
        {
            case	SYS_FORKED:
                {
                    r = do_fork(&system_mess );
                    break;
                }
            case    SYS_NEWMAP:
                {
                    r = do_newmap(&system_mess);
                    break ;
                }
            case	SYS_EXEC:
                {
                    r = do_exec( &system_mess );
                    break ;
                }
            case	SYS_XIT:
                {
                    r = do_xit( &system_mess );
                    break ;
                }
            case	SYS_SIG:
                {
                    r = do_sig( &system_mess );
                    break ;
                }
            case	SYS_COPY:
                {
                    r = do_copy( &system_mess ) ;
                    break ;
                }
            case    SYS_FREE_PAGE:
                {
                    r = do_free_page( &system_mess );
                    break ;
                }
            case    SYS_PUT_PAGE:
                {
                    r = do_put_page( &system_mess ) ;
                    break ;
                }
            case    SYS_REVIVE_INIT:
                {
                    proc[INIT_PROC_NR+TASK_NR].p_flag &= ~UNREADY  ;
                    ready(proc_addr(INIT_PROC_NR));
                    break ;
                }
            case    SYS_NEWSP:
                {
                    r = do_newsp( &system_mess );
                    break ;
                }
            case    BUILD_IN_CMD:
                {
                    r = do_sysbuildin( &system_mess ) ;
                    break ;
                }
            case   SYS_NEWMEM:
                {
                    r  = do_newmem( &system_mess ) ;
                    break ;
                }
            case   SYS_FREEMEM:
                {
                    r  = do_freemem(&system_mess);
                    break ;
                }
            default:
                {
                    break ;
                }
        }
        system_mess.m_type = r ;
        send(caller , &system_mess );
    }
    return ;
}/*}}}*/

i32_t  do_exec( message *m_ptr )
{/*{{{*/
    
    PROCESS *rp  ;
    u8_t    *buf ;
    u32_t   dir,m,n,tmp , addr ;
    i32_t   k  , count , i  ;
    k     = m_ptr->SYS_PROC1 ;
    buf   = m_ptr->SYS_ADDR1 ;
    // 该栈缓存存在mm里面
    if ( k < 0 || k > PROCE_NR )
    {
        return FALSE  ;
    }
    rp = proc_addr(k);
    // 分配栈  count为环境所占用的空间
    count = ENV_PAGES                ;
    addr   =  get_pages(count)        ;
    dir   =  rp->p_cr3        ;
    n     =  ( HIGH_MEM - count*0x1000 ) >> 22 ;
    m     =  ( (HIGH_MEM - count*0x1000 ) >> 12) & 0x3ff  ;
    tmp   =  ( (*(u32_t *)(dir + n*4 ) & 0xfffff000) + m*4);
    for ( i = 0 ; i < count ; i++ )
    {
       *(u32_t*) tmp  = addr | PG_P | PG_USU | PG_RWW ;
       tmp            += 4                            ;
       addr           += 0x1000                       ;
    }
    count = m_ptr->SYS_COUNT ;
    copy_data( m_ptr->m_source , buf , k , HIGH_MEM - count , count  );
    rp->regs.esp     = (u32_t*)( HIGH_MEM - count  );
    rp->regs.ebp     = (u32_t*)( HIGH_MEM - count  );
    rp->regs.eip     = (void  (*)())ENTRY     ; // 应用程序入口
    rp->p_flag      &=  ~RECEVING ;
    rp->p_nextalarm  =  0         ;
    if ( (rp->p_flag & UNREADY) ==  READY )
    {
        ready(rp);
    }
#ifdef  _KERNEL_DEBUG
    printf("SYS_EXEC");
#endif
    return 0 ;
}/*}}}*/

i32_t  do_xit( message *m_ptr  )
{/*{{{*/
    PROCESS *rp , *rc , *np , *xp  ;
    i32_t parent , child ;
    u8_t  found ;
    u32_t page ;
    parent  = m_ptr->SYS_PROC2 ;
    if ( parent < 0 || parent > PROCE_NR  )
    {
        return  -1;
    }
    rc = proc_addr(parent);
    unready(rc);
    rc->p_flag  &= ~P_SLOT_FREE  ;
    //释放页表内存
    free_page_mem(rc->p_cr3);
    found  =  0 ;
    // 检验一下 若该进程是否在发送消息队列
    for ( rp = &proc[0] ; rp < &proc[PROCE_NR] && !found  ; rp++ )
    {
        if ( rp->p_callerq == NULL )
        {
            continue ;
        }
        if ( rp->p_callerq == rc )
        {
            // 删掉它
            rp->p_callerq = rc->p_q_link ;
            found = 1 ;
            break ;
        }
        else
        {
            np = rp->p_callerq ;
            while ( ( xp=np->p_q_link)!= NULL )
            {
                if ( xp == rc )
                {
                    np->p_q_link = xp->p_q_link ;
                    found = 1 ;
                    break ;  
                }
                else
                {
                    np = xp ;
                }
            }
        }
    }
#ifdef _KERNEL_DEBUG
    printf( "sys_exit");
#endif
    return 0;
}

i32_t  do_sig( message *m_ptr  )
{
    return 0;
}
// 不同进程之间拷贝
i32_t  do_copy( message *m_ptr )
{/*{{{*/
    u8_t  *src , *dest       ;
    i32_t k1 , k2 ; 
    i32_t bytes ;
    k1     =  m_ptr->SYS_PROC1  ;
    k2     =  m_ptr->SYS_PROC2  ;
    src    =  m_ptr->SYS_ADDR1  ;
    dest   =  m_ptr->SYS_ADDR2  ;
    bytes  =  m_ptr->SYS_COUNT  ;
    if ( k1 < 0 || k1 > PROCE_NR || k2 < 0 || k2 > PROCE_NR )
    {
        return -1 ;
    }
    if ( 0 == bytes )
    {
        return 0 ;
    }
    copy_data(k1,src,k2,dest,bytes);
    return 0;
}/*}}}*/

//不需要对fork后的进程 ready操作 因为本身阻塞
i32_t  do_fork( message *m_ptr )
{/*{{{*/
    
    PROCESS *rpc  , *rpp     ;       //孩子进程表项指针
    u8_t  *sptr , *dptr ;
    u32_t  page_dir  , tmp , i , base ;
    i32_t k1 , k2 , pid , bytes   ;
    k1   =  m_ptr->SYS_PROC1    ;
    k2   =  m_ptr->SYS_PROC2    ;
    pid  =  m_ptr->SYS_PID      ;
    if ( k1 < 0 || k1 > PROCE_NR || k2 < 0 || k2 > PROCE_NR  )
    {
        // 所有的错误号 现在还没有整理
        // 都是只要是负数 就表示出错 但没有携带
        // 错误信息
        return FALSE ;     
    }
    //  拷贝进程表结构到孩子进程
    rpp   =  proc_addr(k1) ;
    rpc   =  proc_addr(k2) ;
    rpc->p_flag   |=  P_SLOT_FREE    ;
    sptr  = (u8_t*)rpp     ;
    dptr  = (u8_t*)rpc     ;
    bytes = sizeof(PROCESS);
    while (bytes--)
    {
        *dptr++ = *sptr++ ;
    }
    rpc->p_callerq = NULL ;
    //页目录
    page_dir        =  get_pages(16);
    Memset((void*)page_dir , 16<<PAGE_SHIT);
    base            =  get_page();
    Memset((void*)base , 1<<PAGE_SHIT);
    rpc->p_cr3      =  base   ;
    // 将父进程的页面共享
    process_page_share( rpp );
#ifdef  _KERNEL_DEBUG
    printf( "pc_cr3:%d ", rpc->p_cr3  );
#endif
    // 设置页目录   复制页表
    for ( i = 0 ; i < 16 ; i ++  ) 
    {
         *(u32_t*)base = page_dir  |PG_USS | PG_P | PG_RWW ;  
         page_dir   += 0x1000  ;
         base          +=  4      ;
    }
    //拷贝内核的页表
    sptr      =  (void*)((*(u32_t*)rpp->p_cr3)&0xfffff000)  ;
    dptr      =  (void*)((*(u32_t*)rpc->p_cr3)&0xfffff000)  ;
    MemCpy( sptr , dptr ,PAGE_SIZE);
    //拷贝其他页表
    copy_page_tables(rpp->p_cr3+4,rpc->p_cr3+4,15 );
    rpc->p_pid          =  pid          ;
    rpc->ldt_sel        =  sel_ldt      ;
    sel_ldt             += 8            ;
    rpc->regs.eax       =  0            ;  // 置eax等于0
    rpc->p_usertime     =  0            ;
    rpc->p_systime      =  0            ;
    set_dec( rpc->ldt_sel>>3,(u32_t*)rpc->ldts,USER_DPL,ATTR_LDT,2*sizeof(DESCRIPTOR)-1);
#ifdef _KERNEL_DEBUG
    printf( " set_dec:%d ", rpc->ldt_sel );
#endif
    return 0;
}/*}}}*/

i32_t do_newmap( message *m_ptr )
{/*{{{*/
    PROCESS  *rp ;
    i32_t    m ,  n ; 
    i32_t    procnr , addr , page_dir, ind_dir  ;
    u32_t     vir_user  , phy_user ;
    procnr    = m_ptr->SYS_PROC1 ;
    rp        = proc_addr( procnr );/*{{{*//*}}}*/
    vir_user  = (u32_t)(m_ptr->SYS_ADDR1) ;
    phy_user  = (u32_t)(m_ptr->SYS_ADDR2) ;
    page_dir  = rp->p_cr3        ;
    m         = vir_user >> 22 & 0x3ff ;
    n         = vir_user >> 12 & 0x3ff ;
    ind_dir   = *(u32_t*)((page_dir&0xfffff000)+m*4)   ;
    //  在内存中不存在 
    if ( ind_dir & PG_P )
    {
        return -1 ;
    }
    *(u32_t*)((ind_dir&0xfffff000)+n*4)  = phy_user | PG_P | PG_RWW  ;
    return  0 ;
}/*}}}*/

i32_t do_free_page( message *m_ptr )
{/*{{{*/
    PROCESS  *rp ;
    i32_t    m ,  n ; 
    i32_t    procnr , addr , page_dir, ind_dir  ;
    u32_t     vir_user  , phy_user ;
    procnr    = m_ptr->SYS_PROC1 ;
    rp        = proc_addr( procnr );
    vir_user  = (u32_t)(m_ptr->SYS_ADDR1) ;
    page_dir  = rp->p_cr3        ;
    m         = vir_user >> 22 & 0x3ff ;
    n         = vir_user >> 12 & 0x3ff ;
    ind_dir   = *(u32_t*)((page_dir&0xfffff000)+m*4)   ;
    //  在内存中不存在 
    if ( ind_dir & PG_P )
    {
        printf(" not present ");
        return -1 ;
    }
    *(u32_t*)((ind_dir&0xfffff000)+n*4)  =  0   ;
#ifdef _KERNEL_DEBUG
    printf(" free page  now ... ");
#endif
    return 0 ;
}/*}}}*/

i32_t do_put_page( message *m_ptr ) 
{/*{{{*/
    PROCESS *rp ;
    i32_t     procnr  ;
    u32_t     pgd , vir_user  , phy_user , count  ;
    procnr    = m_ptr->SYS_PROC1   ;
    rp        = proc_addr(procnr)   ;
    pgd       = rp->p_cr3            ;
    vir_user  = (u32_t)(m_ptr->SYS_ADDR1) ;
    phy_user  = (u32_t)(m_ptr->SYS_ADDR2) ;
    count     = (u32_t)(m_ptr->SYS_COUNT) ;
    set_page(pgd,vir_user,phy_user,count);
    return  OK  ;
}/*}}}*/

// 将地址的高端映射为栈
i32_t  do_newsp( message *m_ptr )
{/*{{{*/
   i32_t  pid , count , i  ;
   u32_t  addr , dir , m , n  , tmp ;
   PROCESS  *rp       ;
   pid   =  m_ptr->SYS_PROC1      ;
   count =  m_ptr->SYS_COUNT      ;
   addr  =  m_ptr->SYS_ADDR1      ;
   rp    =  proc_addr( pid )      ;
   dir   =  rp->p_cr3             ;
   n     =  ( HIGH_MEM - count*0x1000 ) >> 22 ;
   m     =  ( (HIGH_MEM - count*0x1000 ) >> 12) & 0x3ff  ;
   tmp   =  ( (*(u32_t *)(dir + n*4 ) & 0xfffff000) + m*4);
   for ( i = 0 ; i < count ; i++ )
   {
       *(u32_t*) tmp  = addr | PG_P | PG_USS | PG_RWW ;
       tmp            += 4                            ;
       addr           += 0x1000                       ;
   }
   rp->regs.esp      = HIGH_MEM   ;
#ifdef _KERNEL_DEBUG
   printf( "init esp:%d ", rp->regs.esp );
#endif
   return  0 ;
}/*}}}*/

//注意这个拷贝目录表的函数
//目录对应的映射已经做好的 这里仅仅是共享页框
i32_t  copy_page_tables(u32_t from , u32_t to , i32_t num  ) 
{/*{{{*/
    i32_t  i , j  ;
    u32_t  spage , dpage  , tmp ;
    num  *= 4  ;
    for ( i = 0 ; i < num ; i += 4  )
    {
        spage  =  *(u32_t*)( from + i )  ;
        if (! ( 1 & spage )  )
        {
            continue  ;
        }
        dpage  =  *(u32_t*)( to  + i )  ;
        if ( ! ( 1 & dpage ) )
        {
            panic("copy_page_tables:ddir is not present !");
        }
        spage &=   0xfffff000  ;
        dpage &=   0xfffff000  ;
        // 这里页面要置零 当然我知道这里可能还有问题 
        // 但是要到那个时候才来解决
        // 现在， 我已经做好了重构的决心啦
        Memset((void *)dpage , 4096 );
        for ( j = 0 ; j < 4096  ; j += 4  )
        {
            tmp  =  *(u32_t*)( spage + j )  ;
            if ( ! ( 1 & tmp ) )
            {
                continue  ;
            }
            tmp  &= ~2 ;
            *(u32_t*)( spage + j )  = tmp ;
            *(u32_t*)( dpage + j )  = tmp ;
        }
    }
    return  0 ; 
}/*}}}*/
// 这里补充的vim的折叠功能 设置了foldmethod以后一般会自动创建和保存折叠的视图 可以
// 使用zm zi 一下关闭或打开所有的视图
void  process_page_share( PROCESS  *mp  )
{/*{{{*/
    u32_t  base, page_frame, index ;
    i32_t   i ,j  ;
    // 这里是fork后调用 并不复制这张表
    // 进程结构体记录的仅仅该进程分配的物理页面
    for ( i = 1 ; i < 16 ; ++i  ) 
    {
        base = mp->p_cr3 +i*4 ; 
        if ( *(u32_t*)base & PG_P  )
        {
         for ( j = 0 ;  j < PMD_PAGES ; ++j ) 
         {
             page_frame = ((*(u32_t*)base&0xfffff000) + j*4);
             if ( *(u32_t*)page_frame & PG_P ) 
             {
                   *(u32_t*)page_frame    &= ~PG_RWW         ;
                   index = (*(u32_t*)page_frame)>>PAGE_SHIT ; 
                   mm_map[index]++ ;
             }
         }
        }
    }
   
}/*}}}*/


i32_t  do_newmem( message *m_ptr )
{
  PROCESS  *rp                 ;
  u8_t  *buf                   ;
  Elf32_Phdr phead[ HEAD_NUM ] ; 
  i32_t i , j , n , k          ;
  i32_t  user ,len,size , bnum ;
  u32_t base ;
  user  = m_ptr->SYS_NEWMEM_PROC  ;
  rp      = proc_addr(user ) ;
  buf  = m_ptr->SYS_NEWMEM_BUF  ;
  len  = m_ptr->SYS_NEWMEM_LEN  ;
  copy_data( caller , buf , HARDWARE , phead , len );
  n    = len/sizeof(Elf32_Phdr) ;
  //分配新内存
  for ( i = 0 ; i < n ; i ++ ) 
  {
      bnum  = (phead[i].p_memsz+0xFFF)&0xFFFFF000   ;
      bnum >>= PAGE_SHIT ;
      base   = get_pages( bnum ) ;
      set_page( rp->p_cr3 , phead[i].p_vaddr , base , bnum  );
  }
#ifdef _KERNEL_DEBUG
  printf("SYS_NEWMEM");
#endif
  return OK ;
}

i32_t  do_freemem(message *m_ptr)
{
  PROCESS  *rp                 ;
  u8_t  *buf                   ;
  Elf32_Phdr phead[ HEAD_NUM ] ; 
  i32_t i , j , n , k          ;
  i32_t  user ,len,size , num  ;
  user  = m_ptr->SYS_NEWMEM_PROC  ;
  rp      = proc_addr(user )      ;
  buf  = m_ptr->SYS_NEWMEM_BUF    ;
  len  = m_ptr->SYS_NEWMEM_LEN    ;
  copy_data( caller , buf , HARDWARE , phead , len );
  n    = len/sizeof(Elf32_Phdr) ;
  //释放内存
  for ( i = 0 ; i < n ; i ++ ) 
  {
      num  = (phead[i].p_memsz+0xFFF)&0xFFFFF000   ;
      unset_page( rp->p_cr3 , phead[i].p_vaddr , num , 1  );
  }
#ifdef _KERNEL_DEBUG
  printf("SYS_FREEMEM");
#endif
  return OK ;

}


i32_t do_sysbuildin( message *m_ptr )
{
    i32_t  len , caller ,r , i ;
    u8_t  *p  , *argv[MAX_ARGV];
    p      = m_ptr->SYS_ADDR1  ;
    len    = m_ptr->SYS_LEN    ;
    caller = m_ptr->m_source   ;
    copy_data(caller , p ,  HARDWARE , cmd_buf ,len  );
#ifdef _KERNEL_DEBUG
    printf( "caller:%d cmd_buf:%s ", caller,cmd_buf  );
#endif
    parseline( cmd_buf , argv );
    for ( i = 0 ; i < CMD_SIZE ; i++  )
    {
        if ( !strcmp( inner_cmd[i].name , argv[0] ) ) 
        {
           r =  (*inner_cmd[i].fn)() ;
        }
    }
    return r  ;
}

void parseline( u8_t *cmdline , u8_t **argv )
{
  u8_t *p0, *p1 ;
  i32_t argc = 0 , len  ;
  p0  = cmdline ;
  len = strlen( cmdline ) ; 
  if( p0[len-1] == '\n')
  {
         p0[len-1] = 0 ;
  }
  if ( p0 == NULL )
  {
    printf(" cmd wrong ");
    return ;
  }
  while( *p0 != 0 )
  {
     while( *p0 == ' ' ) 
     {
         p0++ ;
     }
     if ( *p0 == 0 )
     {
        break ;
     }
     argv[argc++] = p0 ;
     while( *p0!= ' '  && *p0 != 0 ) 
     {
         p0++;
     }
     if( *p0 == ' ' )
     {
         *p0 = 0 ;
         p0++;
     }
  }
  //补充null
  argv[argc]=0;
}

//这里内部命令直接调用echo显示即可
i32_t sys_clear()
{
   i32_t  i ; 
   struct tty_line  *tp = &tty_struct[current_tty] ;
   tp->tty_column = 0 ;
   tp->tty_row    = 0 ;
   tp->tty_vid    = 0 ;
   for ( i = 0 ; i < 80*25 ; i ++   ) 
   {
     echo( tp ,' ');
   }
   tp->tty_column = 0 ;
   tp->tty_row    = 0 ;
   tp->tty_vid    = 0 ;
   return 0 ;
}

void free_page_mem( u32_t pgd )
{
  u32_t page ;
  page = *(u32_t*)pgd ;
  free_pages(phys_to_pfn(page),16);
  free_page(phys_to_pfn(pgd));
}
