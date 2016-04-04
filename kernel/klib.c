#include <type.h>
#include <const.h>
#include <com.h>
#include <protect.h>
#include <memory.h>
#include <proc.h>
#include <callnr.h>
#include <glo_var.h>
#include <glo_fun.h>
#include <page.h>

#define  KERNEL_CS_SELECTOR  0x8
#define  KERNEL_DS_SELECTOR  0x10
#define  KERNEL_TSS_SELECTOR 0x20

#define  KERNEL_DPL	0
#define  USER_DPL	3

#define  IGATE          0x8E
#define  DA_386TSS	0x89



//dpl ->  DPL  取值范围 0~3
void set_vec(u32_t  vec_nr, u32_t* addr, u8_t count, u8_t dpl )		
{
    //系统自定义
    idt[vec_nr].attr=IGATE;
    idt[vec_nr].selector=KERNEL_CS_SELECTOR;
    //用户设定
    idt[vec_nr].offset_low=(u32_t)addr&0xFFFF;
    idt[vec_nr].offset_high=((u32_t)addr>>16)&0xFFFF;
    idt[vec_nr].dcount=count;    //一般默认0
    idt[vec_nr].attr+=dpl<<5;
    return ;
}


//dpl ->DPL  取值范围 0~3
//attr 0x08 执行段  0x0c  一致代码段
//limit  只用到20位
void set_dec(u32_t  dec_nr,u32_t* addr,u8_t dpl,u16_t attr,u32_t limit)
{
    //系统自定义
    gdt[dec_nr].limit_high_attr2=(attr>>8)&0xFF;//G(1) D(1) 0 AVL(0)  limit(1111)
    //p(1)   DPL(000) S(0)  TYPE(0)
    //用户设定
    gdt[dec_nr].base_low=(u32_t)addr&0xFFFF;
    gdt[dec_nr].base_mid=((u32_t)addr>>16)&0xFF;
    gdt[dec_nr].base_high=((u32_t)addr>>24)&0xFF;
    gdt[dec_nr].attr1=attr&0xFF;
    gdt[dec_nr].attr1+=dpl<<5;
    gdt[dec_nr].limit_low=limit&0xFF;
    gdt[dec_nr].limit_high_attr2+=(limit>>16)&0x0F;
    return ;
}

//extern void Memset(u8_t* addr,u32_t len);
void Init_tss()
{
    DispStr("Init_tss start...");
    Memset (&tss,sizeof(TSS));
    tss.ss0 = KERNEL_DS_SELECTOR;
    tss.cr3 = KERNEL_DIR_BASE   ;
    set_dec (KERNEL_TSS_SELECTOR>>3, (u32_t* )&tss, USER_DPL, DA_386TSS, sizeof(tss)-1);	//USER_DPL
    tss.io_base = sizeof(tss)-2;
    tss.io_map = 0xfe;
    DispStr("\nInit_tss end...\n");
    return ;
}

//  1M + 4*17*4k
//  这里每个进程都有内核页表 
void switch_page () 
{
    u32_t  dir_base=DIR_BASE,tmp,old_base,new_base; // 从1m的地址开始
    u32_t  i , j , mm_base , m , n  ;
    PROCESS  *rp ;
    //  内核的页面 64M
    mm_base  = 0 ;
    for ( i = 0 ; i < 16 ; i ++ ) 
    {
        tmp = DIR_BASE + PAGE_SIZE + i*PAGE_SIZE ;
        *(u32_t *) ( dir_base + i*4 )  =  tmp | PG_P | PG_USS | PG_RWW  ;  // 注意这里还要一些flag
        for ( j = 0 ; j < 1024  ; j ++ ) 
        {
            *(u32_t*)( tmp + j*4 )  = mm_base | PG_P | PG_USS | PG_RWW ; 
            mm_base  += 0x1000 ;
        }
    }
    // 内核cr3
    for ( i = 0 ; i < TASK_NR  ; i ++ ) 
    {
        proc[i].p_cr3  = DIR_BASE ;
    }
    // mm 
    old_base  = dir_base + 0x1000 ;
    new_base  = old_base + 0x1000 * 16  ;
    dir_base  = new_base ;
    new_base  = new_base + 0x1000 ;
    proc[TASK_NR+0].p_cr3 = dir_base ;
    MemCpy((void*)old_base ,(void*)new_base , 0x1000 * 1 ) ;
    for ( i = 0 ; i < 16 ; i ++  ) 
    {
        *(u32_t*)(dir_base + i*4) = ( new_base + i*0x1000 ) | PG_P | PG_USS | PG_RWW ;
    }
    n = MM_ENTRY  >> 22 ;
    m = MM_ENTRY  >> 12 & 0x3ff ;
    tmp = ( ( *(u32_t *)(dir_base + n*4 ) & 0xfffff000 ) + m*4 )  ;
    for ( i = 0 ; i < seg_base[MM_PROC_NR+1].num ; i ++ ) 
    {
        *(u32_t *) tmp = ( seg_base[MM_PROC_NR+1].base + i*0x1000 ) | PG_P | PG_USU | PG_RWW  ;
        tmp +=  4 ;
    }
    // fs 
    old_base  = dir_base + 0x1000;
    new_base  = old_base + 0x1000 * 16  ;
    dir_base  = new_base ;
    new_base  = new_base + 0x1000 ;
    proc[TASK_NR+1].p_cr3 = dir_base ;
    MemCpy((void*)old_base ,(void*)new_base , 0x1000 * 1 ) ;
    tmp = dir_base ;
    for ( i = 0 ; i < 16 ;  ++i  ) 
    {
        *(u32_t*) tmp  = ( new_base + i*0x1000 ) |  PG_P | PG_USU | PG_RWW ;
        tmp += 4 ;
    }
    n = FS_ENTRY  >> 22 ;
    m = FS_ENTRY  >> 12 & 0x3ff ;
    tmp = ( (*(u32_t *)(dir_base + n*4 ) & 0xfffff000) + m*4); 
    for ( i = 0 ; i < seg_base[FS_PROC_NR+1].num ; i ++ ) 
    {
        *(u32_t *) tmp = ( seg_base[FS_PROC_NR+1].base + i*0x1000) | PG_P | PG_USU | PG_RWW  ;
        tmp +=  4 ;
    }
    // init 
    old_base  = dir_base + 0x1000;
    new_base  = old_base + 0x1000 * 16  ;
    dir_base  = new_base ;
    new_base  = new_base + 0x1000 ;
    proc[TASK_NR+2].p_cr3 = dir_base ;
    MemCpy((void*)old_base ,(void*)new_base , 0x1000 * 1 ) ;
    tmp = dir_base  ; 
    for ( i = 0 ; i < 16 ;  ++i  ) 
    {
        *(u32_t*)tmp = new_base + i*0x1000 |   PG_P | PG_USU | PG_RWW;
        tmp += 4 ;
    }
    printf ( " switch page ...."  ) ;
}


void interrupt( i32_t  task , message *m_ptr )
{
    port_out(INT_M_00 , ENABLE);
    u32_t this_bit, old_map ,i ,ret ;
    this_bit = 1 << ( -task) ;
    if ( mini_send(HARDWARE,task,m_ptr) != OK )
    {
        // busy_map 每次对其置位
        old_map = busy_map ;
        if ( CLOCK_TASK == task )
        {
            lost_ticks++ ;
        }
        else
        {
            busy_map |= this_bit ;
            task_mess[-task] = m_ptr ;
        }
    }
    else
    {
        busy_map &= ~this_bit ;
        old_map = busy_map ;
    }
    if (  0 != old_map )
    {
        //printf( " old_map %d " , old_map  );
        for ( i = 2 ; i < TASK_NR ; i++)
        { 
            if ( (old_map>>i) & 1 ) 
            { 
                ret = mini_send (HARDWARE , -i , task_mess[i]) ;
                if ( OK == ret )
                {
                    busy_map  &=  ~(1<< i ) ;
                    //printf( "busy:%d", busy_map);
                }
            }
        }
    }
    if ( NULL != rdy_head[TASK_Q] && ( cur_proc>=0 || HARDWARE ==cur_proc ) )
    {         
        pick_proc();
    }
}
//职责仅仅选取在调度队列中选取进程
void pick_proc()
{
    i8_t queue ;
    if ( rdy_head[TASK_Q] != NULL )
    {
        queue  = TASK_Q;
    }
    else if ( rdy_head[SERVER_Q] != NULL ) 
    {
        queue = SERVER_Q;
    }
    else if ( rdy_head[USER_Q] != NULL ) 
    {
        queue = USER_Q ;
    }
    else 
    {
        queue  = IDLE_Q ;
    }
    if (rdy_head[queue] != NULL  ) 
    { 
        if ( cur_proc >= LOW_USER  )
        {
           pre_ptr  =  bill_ptr    ;
        }
        cur_proc =  rdy_head[queue] - proc - TASK_NR  ;
        cur_ptr  =  proc_addr ( cur_proc ) ;
        if ( cur_proc >= LOW_USER  ) 
        {
            bill_ptr = cur_ptr ;
        }
    }
    else
    {
        cur_proc  = IDLE ;
        cur_ptr   = proc_addr (HARDWARE) ;
    }
}

//  选择一个调度的进程 将指针放在cur_ptr
void  schedule()
{
    setlock() ;
    if ( NULL == rdy_head[USER_Q] ) 
    {
        restore() ;
        return ;
    }
    rdy_tail[USER_Q]->p_nextready = rdy_head[USER_Q] ;
    rdy_tail[USER_Q] = rdy_head[USER_Q] ;
    rdy_head[USER_Q] = rdy_head[USER_Q]->p_nextready ;
    rdy_tail[USER_Q]->p_nextready = NULL ;
    pick_proc() ;
    restore() ;
    return ;
}

void DispInt(i32_t num)
{
    char str[12];
    Memset(str,12);
    str[0]='0';
    str[1]='x';
    itoa(num,&str[2]);
    DispStr(str);
    return ;
}

void itoa(i32_t num,  char* str)
{
    //00b800h  - > 'b800'
    i32_t	mask = 32, i = 0;
    char	value,index=0;
    for( i=0; i<=7; i++)
    {
        mask-=4;
        value= (num >> mask )&0xf;
        if ( 0 == value && 0 == index)
        {
            continue;
        }
        else if( value > 9)
        {
            str[index]=value-10+'A';
            index++;
        }
        else if( value <= 9)
        {
            str[index]=value+'0';
            index++;
        }
    }
    return ;
}


//  sys_call 是要返回的， 在中断最后调用 restart
void sys_call(i32_t call,i32_t src_dest, message  *m_ptr)
{
    //	message *kernel_m_ptr ;
    i32_t	n;
    PROCESS* rp = cur_ptr ;        //陷入内核态
    i32_t  function = call & SYS_CALL;
    i32_t  caller = rp - proc - TASK_NR ;
    // kernel_m_ptr = m_ptr ;
    //	if ( caller >= 0 ) 
     //	{
    //		kernel_m_ptr = (message*)umap( caller , (u8_t*)m_ptr ) ;
     //	}
    assign_caller(m_ptr , caller );
    // 注意这里开始我在疑惑，现在不会疑惑了，确保消息发送成功，这是用户库所做的事
    // 而内核仅仅做机械工作，转发消息，并不管消息是否真正到了，
    // 所以需要在用户端进行检测 一个健壮的程序所做的也包括这些事
    if( function & SEND )
    {
        rp->p_sendto = src_dest;
        n = mini_send(caller,src_dest,m_ptr);
        rp->regs.eax = n;
        if(OK != n)
         {
            printf(" not send sucess ");
            return ;
         }
    }
    if( function & RECEIVE )
    {
        rp->p_getfrom = src_dest;
        n = mini_rec(caller,src_dest,m_ptr);
        rp->regs.eax = n;
    }
    return ;	
}

i32_t mini_send(i32_t caller,i32_t dest, message *m_ptr)
{
    message  *smess_ptr , *dmess_ptr ; 
    PROCESS *rp = proc_addr(dest);
    PROCESS *caller_p = proc_addr ( caller )  ;
    PROCESS **xp  = NULL , *xxp  ;
    //目的进程正在等待消息 
    if ( RECEVING & rp->p_flag  )
    {
        if ( rp->p_getfrom == caller || rp->p_getfrom == ANY  )
        {
            caller_p->p_sendto  = 0 ;
            rp->p_getfrom       = 0 ;
            copy_data(caller,(u8_t*)m_ptr,dest,(u8_t*)(rp->m_ptr),sizeof(message));
            rp->m_ptr           = 0 ;
            rp->p_flag         &= ~RECEVING;
            if ( (rp->p_flag & UNREADY ) == READY  )
            {
                ready(rp);
            }
            return OK;
        }
    }
    else if( SENDING & rp->p_flag )//否则插入目的进程的发送 先进行死锁检测
    {
        xxp = proc_addr( rp->p_sendto );
        while ( SENDING & xxp->p_flag )
        {
            if ( xxp->p_sendto == caller )
            {
                panic( "dead lock ");
                //死锁
                return 2;
            }
            xxp = proc_addr( xxp->p_sendto );
        }
    }
    if ( HARDWARE == caller )
    {
        return 2;  //内核发送消息 不阻塞直接返回
    }
    // 不知不觉中想到了minix代码 :) 这里也是用二级指针使代码看起来明了
    xp  =&( rp->p_callerq );
    while ( *xp  != NULL )
    {
        xp  =&( (*xp)->p_q_link );
    }
    *xp = caller_p ;
    caller_p->p_q_link = NULL ;
    caller_p->m_ptr    = m_ptr ;
    if ( (caller_p->p_flag & UNREADY) == READY )
    {
        unready(caller_p);
    }
    caller_p->p_flag |= SENDING ;
    return OK;
}

//^_^ 以前我还因为这里是抄袭minix 原来这里二级指针用的这么妙8.15
i32_t mini_rec(i32_t caller, i32_t src, message* m_ptr)
{
    i32_t   dest ;
    message *smess_ptr , *dmess_ptr  ;
    PROCESS *rp ;
    PROCESS *caller_p = proc_addr( caller );
    PROCESS **xp  = &(caller_p->p_callerq);
    // 在自身的callerq队列寻找 
    while  ( *xp != NULL   )
    {
        if ( src == ANY || (*xp)->p_sendto == caller  )
        {
            dest  = (*xp) - proc  - TASK_NR ; 
            (*xp)->p_sendto  = 0 ;
            copy_data(dest,(u8_t*)((*xp)->m_ptr ),caller,(u8_t*)m_ptr,sizeof(message));
            (*xp)->p_flag &= ~SENDING;
            if ( ((*xp)->p_flag &UNREADY ) == READY )
            {
                (*xp)->m_ptr        = NULL ;
                ready(*xp);
            }
            if ( *xp == caller_p->p_callerq )
            {
                   caller_p->p_callerq  = (*xp)->p_q_link ;
            }
            else
            {
                  *xp = (*xp)->p_q_link ;        
            }
            return  OK ;
        }
        xp = &((*xp)->p_q_link );
    }
    // 若没有则阻塞
    caller_p->p_getfrom = src ;
    caller_p->m_ptr     = m_ptr ;
    if ( (caller_p->p_flag  & UNREADY)== READY  )
    {
        unready(caller_p);
    }
    caller_p->p_flag |= RECEVING ;	
    if ( caller == MM_PROC_NR || caller == FS_PROC_NR  )
    {
         //若为服务进程 这里还有部分工作要处理
         inform();
    }
    return 0;
}

//该函数只是将rp插入就绪队列
void ready( PROCESS *rp )
{
    i8_t  queue  ;
    i32_t r ;
    r = rp - proc - TASK_NR ;
    queue = ( r < 0 ? TASK_Q : r < LOW_USER ? SERVER_Q : USER_Q ) ;
    if ( NULL == rdy_head[queue] )
    {
        rdy_head[queue] = rp ;
    }
    else
    {
        rdy_tail[queue]->p_nextready = rp ;
    }
    rdy_tail[queue] = rp ;
    rdy_tail[queue]->p_nextready = NULL ; 
    return ;
}

//该函数的目的只是将rp移除
void unready( PROCESS *rp )
{
    PROCESS *xp ;
    i32_t r = rp - proc - TASK_NR ;
    i8_t  queue  ;
    queue = ( r < 0 ? TASK_Q : r < LOW_USER ? SERVER_Q : USER_Q ) ;
    if ( ( xp = rdy_head[queue] ) == NULL )
    {
        return ;
    }
    if (xp == rp  )
    {
        rdy_head[queue] = rdy_head[queue]->p_nextready ;
        pick_proc();
    }
    else
    {
        while ( rp != xp->p_nextready )
        {
            if ( ( xp = xp->p_nextready ) == NULL ) 
            {
                return ;
            }
        }
        xp->p_nextready = xp->p_nextready->p_nextready ;
        if ( rp  == rdy_tail[queue] )
          {
            rdy_tail[queue] = xp ;    
	  }
    }
    return ;
}

char   irq[][25] = {
    " divide_erro ",
    " single_step_exception ",
    " nmi ",
    " breakpoit_exception  ",
    " overflow ",
    " bounds_check ",
    " inval_opcode ",
    " copr_not_available ",
    " double_fault ",
    " copr_seg_overrun ",
    " inval_tss ",
    " segment_not_present ",
    " stack_exception ",
    " general_protection ",
    " page_fault ",
    " copr_erro "
};

void exception(u32_t vec_nr , u32_t erro_nr)
{
    DispStr(irq[vec_nr]);
   // DispInt(erro_nr);
    // printf( "cur_proc:%d ", cur_proc);
    DispInt( cur_proc );
    return ;
}

void nothing()
{}

u8_t *umap( i32_t procnr , u8_t *lineraddr )
{
    PROCESS  *rp  ;
    i32_t m , n  , _lineraddr ;
    u32_t dir_base , phy_addr , offset , ind_addr  ;
    _lineraddr  = (u32_t)lineraddr  ;
    if (  procnr > PROCE_NR - TASK_NR  ) 
    {
        return  (u8_t*)0 ; 
    }
    // 内核地址则不必查页目录表 
    if ( procnr < 0 ) 
    {
        return lineraddr ;
    }
    rp  = proc_addr( procnr );
    dir_base = rp->p_cr3	& 0xfffff000 ;
    m = _lineraddr >> 22 & 0x3ff ;
    n = _lineraddr >> 12 & 0x3ff ;  
    //  ind_addr  = (*(u32_t*)(dir_base + m*4))&0xfffff000 ;
    //  这里的赋值 不知少打了一对括号 就有问题  而我多用两个变量 就没有问题
    //  所以以后要绝对安全地写代码 多打括号和多用变量 思路清晰 
    phy_addr = (*(u32_t*) ( ( ( *(u32_t*) (dir_base + m*4) ) & 0xfffff000 )+n*4))&0xfffff000;
    offset =  _lineraddr & 0xfff  ;
    phy_addr =  phy_addr + offset ;
    return  (u8_t *)phy_addr ;
}

//  这个函数是后来补的 因为不想改动已有的接口
//  故在此增加新到接口 来对老接口进行封装   以后开发过程中
//  考虑问题要尽量仔细  否则推掉重来是一件很让人蛋疼的问题  
void  copy_data( i32_t  caller ,u8_t *vir_src,  i32_t dest , u8_t *vir_dest , i32_t size  )
{
    u8_t *dest_ptr , *src_ptr ;
    u32_t  src_phy , dest_phy ,  src_gap , dest_gap , len  ;
    // 出于保护指针的考虑 要在这里将指针拷贝到栈上即局部空间
    src_phy  = (i32_t)vir_src  ;
    dest_phy = (i32_t)vir_dest ;
    src_gap  = ((src_phy +  0x1000 -1  )&0xfffff000) - src_phy  ;
    dest_gap = ((dest_phy + 0x1000 -1  )&0xfffff000) - dest_phy ;
    if ( src_gap == 0 )
    {
        src_gap  = 0x1000 ;
    }
    if ( dest_gap == 0 ) 
    {
        dest_gap = 0x1000 ;
    }
    // 若到下一个虚拟页面的间距大于要复制的块大小 
    // 则直接拷贝
    if ( src_gap >= size && dest_gap >= size )
    {
        write_verify( dest , (void*)dest_phy );
        src_ptr   = umap( caller , (u8_t*)src_phy ) ;
        dest_ptr  = umap( dest   , (u8_t*)dest_phy) ;
        MemCpy(src_ptr,dest_ptr,size);
        return ;
    }
    else // 否则要用一个循环拷贝
    {
        // 第一次复制的长度等于最小的间距
        while ( size > 0 )
        {
            len = MIN(src_gap,dest_gap) ;
            len = MIN(len,size) ;
            write_verify( dest   , (void*)dest_phy);
            src_ptr  = umap( caller , (u8_t*)src_phy ) ;
            dest_ptr = umap( dest   , (u8_t*)dest_phy) ;
            MemCpy(src_ptr,dest_ptr,len);
            src_phy  += len ;
            dest_phy += len ;
            src_gap  = ((src_phy  + 0x1000 -1 )&0xfffff000) - src_phy  ;
            dest_gap = ((dest_phy + 0x1000 -1 )&0xfffff000) - dest_phy ;   
            if ( src_gap == 0 )
            {
                src_gap   = 0x1000 ;
            }
            if ( dest_gap == 0 )
            {
                dest_gap  = 0x1000 ;
            }
            size  -=  len ;
        }
    }
    return ;
}

// 这个函数也还没有测试的 
void  assign_caller( message *m_ptr , i32_t caller )
{
    message  *kernel_m_ptr ;
    i32_t  gap , vir_user ;
    u8_t  *p ;
    vir_user  = (i32_t)m_ptr ;
    gap = ((( vir_user + 0x1000 - 1 ) )&0xfffff000 ) - vir_user ;
    if ( gap == 0 )
    {
        gap = 0x1000 ;
    }
    kernel_m_ptr  = m_ptr ;
    if ( gap > sizeof(i32_t)  )
    {

        if ( caller >= 0 )
        {
            kernel_m_ptr = (message*)umap( caller , (u8_t*)vir_user ) ;
        }
        // 这里的source也考虑的太简单的 
        // 也要考虑到虚拟线性地址的分页
        // 居然为这个写了一个函数
        kernel_m_ptr->m_source = caller ;
        return ;
    }
    // 很不幸 一个整型数被分隔的 
    // 需要为这几个字节去调用MemCpy 而且是两次 
    p  = (u8_t*)&caller ;
    kernel_m_ptr = (message*)umap( caller , (u8_t*)vir_user ) ;
    MemCpy(p,(u8_t*)kernel_m_ptr,gap);
    vir_user   += gap ;
    p          += gap ;
    kernel_m_ptr = (message*)umap( caller , (u8_t*)vir_user ) ;
    write_verify(caller,vir_user);
    MemCpy(p,(u8_t*)kernel_m_ptr,sizeof(i32_t) - gap);
    return ;
}

void  panic( u8_t * str  )
{
    printf( str );
    while(1) ;
}

// 这两个函数8.8号添加
// 这里的写验证函数，若向写保护的页面写时，首先检测是否有共享，共享则重新申请一个页面，并解保护，否则直接解保护即可。
// 但不修改其他进程的虚拟页面。
void  write_verify( i32_t caller , void *addr )
{
  i32_t parent , i ;
  u32_t  vir_addr = (u32_t)addr   ,dir , m , n , tmp ;
  PROCESS *rp = proc_addr( caller );
  dir         = rp->p_cr3    ;
  parent      = rp->p_parent ;
  n           = vir_addr >> 22 ;
  m           = (vir_addr >> 12) & 0x3ff ;
  tmp         = *(u32_t*)((*(u32_t *)(dir + n*4 ) & 0xfffff000 ) + m*4)  ;
  if ( (3&tmp) == 1  )
  {
      if ( (tmp&0xfffff000)<LOW_MEM )// 共享内核数据，注意这里是因为共享init数据
      {
         panic( " counld not release kernel memory ");     
      }
      else
      {
        i  =  (tmp&0xfffff000)>>12;
        if ( 1 == mm_map[i] )
        {
               *(u32_t*)((*(u32_t *)(dir + n*4 ) & 0xfffff000) + m*4 ) |= PG_RWW ;
               return ;
        }
        un_wp_page(caller,addr);
        mm_map[i]--;
       }
  }
  return  ;
}

void  un_wp_page( i32_t caller , void *addr )
{
    u32_t   _addr , dir , tmp ,  m , n , phyaddr ,base;
    PROCESS  *rp  ;
    _addr  = (u32_t)addr ;
    rp     = proc_addr( caller );
    dir    = rp->p_cr3        ;
    base    = get_page();
    m       = ( _addr >> 22  ) & 0x3ff ;
    n       = ( _addr >> 12  ) & 0x3ff ;
    tmp     = *(u32_t*)( dir + 4*m  )  ;
    phyaddr = *(u32_t*)( (tmp&0xfffff000) + 4*n    ) ;   // 注意这里不加括号 则错误原因在于...
    MemCpy( (phyaddr&0xfffff000) , base , 4096) ;   //拷贝页面
    *(u32_t*)( (tmp&0xfffff000) + 4*n  )  = base|PG_USU | PG_P | PG_RWW ;
    return ;
}


