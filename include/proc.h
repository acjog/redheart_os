#ifndef _PROC_H_
#define _PROC_H_
#ifndef  OFFSET 
#include <type.h>
#include <ipc.h>
#include <protect.h>
#endif
#ifndef _TABLE_H_          //将全局变量都集中在table.c文件
#define  EXTERN  extern 
#else
#define  EXTERN  
#endif

typedef	struct{
    u32_t	gs;
    u32_t	fs;
    u32_t	es;
    u32_t	ds;
    u32_t	edi;
    u32_t	esi;
    u32_t	*ebp;
    u32_t	kernel_esp;
    u32_t	ebx;
    u32_t	edx;
    u32_t	ecx;
    u32_t	eax;
    u32_t	retaddr;
    void	(*eip)();  // function poiter
    u32_t	cs;
    u32_t	eflags;
    u32_t	*esp;
    u32_t	ss;    //中断进入压入这四个值 :)
}STACK_FRAME;  //72 byte

#define GS	    0
#define FS	    GS+4
#define ES	    FS+4
#define DS	    ES+4
#define EDI	    DS+4
#define ESI	    EDI+4
#define EBP	    ESI+4
#define KERNEL_ESP  EBP+4
#define EBX         KERNEL_ESP+4
#define EDX         EBX+4
#define ECX         EDX+4
#define EAX         ECX+4
#define RET_ADDR    EAX+4
#define EIP         RET_ADDR+4
#define CS	    EIP+4
#define EFLAGS      CS+4
#define ESP         EFLAGS+4
#define SS	    ESP+4
#define STACKFRAME_SIZE  SS+4
#define LDT_SEL     SS + 4
#define LDTS        LDT_SEL + 4
#define PARENT      LDTS + 4
#define PID         PARENT + 24
#define CR3         PID  + 4
#define USER_TIME   CR3  + 4
#define SYS_TIME    USER_TIME + 4
#define TICKS_QUANTUM  SYS_TIME + 4
#define TICKS_LEFT     TICKS_QUANTUM + 4 
#define M_PTR          TICKS_LEFT + 4 
#define P_FLAG         M_PTR  + 4
#define CALLER_Q       P_FLAG + 4
#define Q_LINK         CALLER_Q + 4 
#define NEXTREADY      Q_LINK  + 4
#define  GETFROM       NEXTREADY + 4
#define  SENDTO        GETFROM   + 4

#define RET_REG        EAX

typedef	struct pro{
    STACK_FRAME	 regs            ;            
    u32_t	     ldt_sel         ;         // +72 
    DESCRIPTOR	 ldts[LDT_SIZE]  ;  //+ 74 byte
    i32_t        p_parent        ;
    u32_t		 p_pid           ;             // +98
    //  这里的cr3指的是物理内存 而不是虚拟的线性空间
    //  而且这里用的u32_t 因为要位与操作
    u32_t        p_cr3           ;     //  每个进程有个也目录

    clock_t		 p_usertime      ;     // 102
    clock_t		 p_systime       ;     // 106

    clock_t	     p_ticks_quantum ;     // 110
    clock_t		 p_ticks_left    ;     // 114

    message 	*m_ptr           ;	   //消息指针 // 118

    u32_t		 p_flag          ;     //标记 RECEVING SENDING etc 122

    struct pro 	*p_callerq       ;	   //发送进程队列头
    struct pro 	*p_q_link        ;	   //发送进程的指针

    struct pro 	*p_nextready     ;	   //形成调度队列
    i32_t		 p_getfrom       ;
    u32_t        p_nextalarm     ;     //闹钟
    i32_t		 p_sendto        ;

    char	     p_name[12]      ;
    u32_t		 p_sigmap        ;     //信号位图	
}PROCESS;  		
//暂时定32个信号

#define READY         0
#define UNREADY       3
#define RECEVING      1
#define SENDING       2
#define P_SLOT_FREE   4
#define NO_MAP        8

/*进程表数据*/
#define	 PROCE_NR  64
EXTERN   PROCESS  proc[PROCE_NR];   	// A4 164 PROCESS

EXTERN   i32_t    cur_proc ;
EXTERN	 PROCESS *cur_ptr  ;		//当前进程的指针
EXTERN   PROCESS *bill_ptr ;        //收费进程
EXTERN   PROCESS *pre_ptr  ;
EXTERN   i32_t	  pre_proc ;
EXTERN   u32_t    busy_map ;  //忙任务记录
#define  READY_Q_NR  4
EXTERN PROCESS *rdy_head[READY_Q_NR];	//就绪队列  task server user idle
EXTERN PROCESS *rdy_tail[READY_Q_NR];

//栈和环境变量占用空间
#define  ENV_PAGES     2 
#define  INIT_STACK_BASE   (HIGH_MEM-(ENV_PAGES-1)*0x1000)


#define  RECEVE_BLOCK  0
#define  SEND_BLOCK    1
#define  TASK_Q        0
#define  SERVER_Q      1
#define  USER_Q        2
#define  IDLE_Q        3
//  这里为什么只有就绪队列，而没有阻塞队列? 其实阻塞也是有队列的。
//  但是注意进程发送消息时的，对象必须明确。反之，接收消息时，则不然，可以接收
//  来自多个进程的消息。所以用p_getfrom  和标志位 就形成一个接收阻塞的队列。
//  发送消息时，则将自己插入目的进程的p_callerq里。因为发送消息时，已经形成了
//  消息。同时，利用这两个指针可以判断死锁的存在。
#endif
