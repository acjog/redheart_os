#ifndef __KERNEL_CONST_H__
#define __KERNEL_CONST_H__
//调试宏
//#define  _KERNEL_DEBUG  1
//#define  _MM_DEBUG      1
//#define  _FS_DEBUG      1
//#define _INIT_DEBUG       1
// switch_page函数需要用到
#define  DIR_BASE   0x100000
// 用户程序入口地址
#define         ENTRY           0x606000
#define         MM_ENTRY        (ENTRY+0x100000)
#define         FS_ENTRY        (ENTRY+0x200000)
#define         INIT_ENTRY      (ENTRY+0x300000)

#define     MAX_P_LONG      200000000
#define 	LDT_SIZE	3
#define		GDT_SIZE	64

#define 	SelectorFlatCode32			8h
#define		SelectorFlatData32		 	10h
#define 	SelectorVideo				18h+3h
#define		SelectorTss			 	20h

// 注意这些东西要随着table.c boot数组一起变化
#define         TASK_NR         6
#define         BOOT_NR         9
#define         LOW_TASK        -6
#define         LOW_SERVER      0
#define         LOW_USER        2

//注意这些东西 改变时，一定要注意引用的地方同时更改
#define		TRUE            1
#define		FALSE		0

//function = 1 2   send reciv botch
#define SEND    1
#define RECEIVE 2
#define SENDREC 3
#define OK      0

#define SYS_CALL     	3
#ifndef NULL
#define NULL            0
#endif

// 定义内核起始地址以及末尾地址
#define  KERNEL_DIR_BASE    0x100000
#define  KERNEL_ADDR_START  0
#define  KERNEL_ADDR_END    0x400000


// interrupt  consant
#define  ENABLE   0x20
#define  INT_M_00 0x20
#define  INT_S_00 0xA0

#define  SECT_SIZ   512  

#define  MM_PROC_NR   0
#define  FS_PROC_NR   1
#define  INIT_PROC_NR 2
#define  SU_UID       0

#define SEL_LDT_FIRST 5*8  //全局描述符从第五个开始 28 30 38 40 
#define ATTR_LDT      0x82  

#define DA_C	      0x98
#define DA_DRW        0x92
#define TASK_DPL      2 
#define USER_DPL      3
#define SA_TIL        4 
#define RPL_TASK      2

#define KERNEL_STACK_SIZE  1024 

#define  M              10
#define  WORD_B_SIZE    512

#define INIT_MEM_NUM    10
// 调试使用
#define  _DEBUG

#define   TO_USER     1
#define   FROM_USER   2

#define R_BIT               1
#define W_BIT               2
#define X_BIT               4

#endif

