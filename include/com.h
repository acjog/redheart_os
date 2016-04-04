#ifndef  _COM_H_
#define  _COM_H_

#define		IDLE		   -66

#define     TASK_REPLY      99

#define     FLOPPY_TASK     -6
#define     DISK_READ        1
#define     DISK_WRITE       2


#define     TTY_TASK        -5
#define		TTY_READ	     1
#define		TTY_WRITE	     2
#define     CHAR_INT         3 
#define		TTY_CANCEL	     4
#define		TTY_IOCTL	     5


#define     MEM_TASK        -4
#define		MEM_READ	     1
#define		MEM_WRITE	     2
#define		MEM_IOCTL	     3
#define     MEM_SETUP       4

#define		CLOCK_TASK	    -3
#define     CLOCK_TICK       1
#define     SET_ALARM        2
#define     GET_TIME         3
#define     REAL_TIME        1

#define     SYS_TASK        -2
#define 	SYS_FORKED	     1
#define		SYS_NEWMAP	     2
#define		SYS_EXEC	     3
#define		SYS_XIT		     4
#define		SYS_GETSP	     5
#define		SYS_TIMES	     6
#define		SYS_SIG		     7
#define		SYS_COPY	     8
#define		SYS_FREE_PAGE    9
#define     SYS_PUT_PAGE     10
#define     SYS_REVIVE_INIT  11
#define     SYS_NEWSP        12
#define     SYS_BUILDIN      13
#define     SYS_NEWMEM       14
#define     SYS_FREEMEM      15
#define   	HARDWARE	    -1

#define 	ANY  		    100

/******************************************************/
// 返回出错代码
#define     OK               0
#define  	EINVAL          -1 
#define		ENXIO		    -2
#define		E_TTY_AGAIN     -3

/*****************************************************/
// 时钟任务参数
#define  DELTA_TICKS      m6_l1  
#define  FUNC_TO_CALL     m6_f1 
#define  NEW_TIME         m6_l1 
#define  CLOCK_PROC_NR    m6_i1 
#define  SECONDS_LEFT     m6_i1

// 终端任务参数
#define  CHAR_ADDRESS     m2_p1 
#define  TTY_LINE         m2_i1
#define  TTY_STATUS       m2_i3
#define  TTY_FLAGS        m2_l1 
#define  TTY_SPEK         m2_l2

#define  TTY_DEVICE       m2_i1
#define  TTY_PROC_NR      m2_i2
#define  TTY_COUNT        m2_i3
#define  TTY_POSITION     m2_l1
#define  TTY_ADDRESS      m2_p1

// 终端状态 
#define  WAITING          1
#define  NOT_WAITING      0 
#define  RUNNING 	      0

// 软盘任务
#define  FL_DEVICE        m2_i1
//#define  FL_OPCODE      m2_i2
#define  FL_PRONR         m2_i2
#define  FL_COUNT         m2_i3
#define  FL_SECTOR        m2_l1
#define  FL_ADDR          m2_p1

// Mem任务
#define MEM_DEVICE        m2_i1
#define MEM_PROCNR	      m2_i2
#define	MEM_COUNT	      m2_i3
#define	MEM_POS		      m2_l1
#define	MEM_ADDR	      m2_p1
#define MEM_BASE          m2_i1
#define MEM_NUM           m2_i2

// 系统任务
#define SYS_PROC1         m1_i1
#define SYS_EXIT_F        m1_i1
#define SYS_PROC2	      m1_i2
#define SYS_PID           m1_i3
#define SYS_ADDR1         m1_p1
#define SYS_ADDR2         m1_p2 
#define SYS_COUNT         m1_i3 
#define SYS_STACK_PTR     m1_p1 
#define SYS_CR3           m1_p1
#define SYS_PAGE_FRAME    m1_p2
#define SYS_LEN           m1_i1 
#define SYS_NEWMEM_BUF    m1_p1 
#define SYS_NEWMEM_LEN    m1_i1
#define SYS_NEWMEM_PROC   m1_i2

//与虚拟页面相关项
#define SYS_SRC_ADDR      m1_i1
#define SYS_DEST_ADDR     m1_i2

// 这里的地址有重复的没有关系 因为不会在同一时刻用到
#define  REP_STATUS       m2_i3
#define  REP_PROC_NR      m2_i2

#endif
