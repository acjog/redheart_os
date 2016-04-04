#ifndef _GLO_FUN_H_
#define _GLO_FUN_H_

#include <type.h>
#include <ipc.h>
#include <memory.h>
#include <proc.h>
#include <const.h>

/*全局函数*/
/*/lib/mpxlib.asm*/
extern  void    io_delay() ;
extern  void    set_frame();
extern  void	setlock();
extern  void    restore() ;
extern  void	unlock();
extern  u8_t	port_in(u16_t port);   
extern  void	port_out(u16_t port, u8_t value);
extern  void	DispStr(u8_t*  str);         
extern  void	Memset(void* addr,u32_t len);
extern	void	MemCpy(void* sour, void* dest, u32_t len);
extern  void    vid_copy ( void * ,u32_t ,  u32_t  , u32_t ) ;
extern  void	restart();
extern  void	load_gdt();
extern  void 	tty_int();
extern  void	disable_int(u32_t irq);
extern  void	enable_int(u32_t	irq);
extern  void    init_8253A();
extern  void	nohandler();
extern  void	floppyhandler();
extern  void	clock_int();
extern  void    idle();
extern  void    sendrec(u32_t dest , message *ptr);
extern  void    send( u32_t dest, message *ptr);
extern  void    receve( u32_t src , message *ptr);
extern  void    divide_error();
extern  void    exception(u32_t vec_nr , u32_t erro_nr);
extern  void    single_step_exception();
extern  void    nmi();
extern  void    breakpoint_exception();
extern void	overflow(); 
extern void	bounds_check(); 
extern void	inval_opcode();
extern void	copr_not_available();
extern void	double_fault();
extern void	copr_seg_overrun();
extern void	inval_tss();
extern void	segment_not_present();
extern void	stack_exception();
extern void	general_protection();
extern void	page_fault();
extern void	copr_error();

/*kernel/klib.c*/
extern  void    write_verify( i32_t , void *);
extern  void    un_wp_page( i32_t   , void *);
extern  void	set_dec(u32_t  dec_nr,u32_t* addr,u8_t dpl,u16_t attr,u32_t limit);
extern  void	set_vec(u32_t  vec_nr, u32_t* addr, u8_t count, u8_t dpl);
extern  void	Init_tss();
extern  void    switch_page();
extern  void    pick_proc() ;
extern  void    interrupt( i32_t , message*) ;
extern  void	schedule();
extern  void	keyboard_handler(); 
extern  void	DispInt(i32_t num);
extern  void	itoa(i32_t num,  char* str);
extern  void	sys_call(i32_t call,i32_t src_dest,message* m_ptr);
extern  void	s_call();
extern  i32_t 	mini_send(i32_t caller, i32_t dest, message *m_ptr);
extern  i32_t	mini_rec( i32_t caller, i32_t src, message * m_ptr);
extern  void    ready(PROCESS * rp );
extern  void    unready(PROCESS *rp );
extern  void    nothing();
extern  void    panic( u8_t *);
extern  u8_t*   umap( i32_t procnr ,u8_t *lineraddr  );
extern  void    assign_caller(message *m_ptr , i32_t caller );
extern  void    copy_data( i32_t  caller ,u8_t *vir_src,  i32_t dest , u8_t *vir_dest , i32_t count ) ;
//宏
#define proc_addr(caller)    &proc[caller + TASK_NR ]

#define   cp_mess(source , dest )   MemCpy( source , dest , sizeof ( message ))  
#define   printf  printk 

#define   MIN(a,b)    ((a>b)?b:a)
#define   MAX(a,b)    ((a>b)?a:b)


//tty.c
extern  void	tty_task();
extern  i32_t   printk( u8_t * , ... ) ;
// clock.c
extern  void    clock_task();

// floppy.c
extern  void    floppy_task();

extern void     mem_task();

// sys.c
extern void     sys_task();


#endif
