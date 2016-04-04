#ifndef __K_MEM_H__
#define __K_MEM_H__

#define IN_USE    0x1
#define M_CR3     0x2
#define M_PAGE    0x4
#define M_STACK   0x8
#define M_MEM     0x10
#define M_COW     0x20

// 定义起始地址 主内存最多64M
#define HIGH_MEM    0x4000000
#define LOW_MEM     4*1024*1024
#define TOTAL_MEM   64*1024*1024


#endif 
