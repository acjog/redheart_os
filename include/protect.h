#ifndef _PROTECT_H_
#define _PROTECT_H_

#ifdef  OFFSET
#include "type.h"
#else
#include <type.h>
#endif


#define LDT_SIZE 3

#ifndef _TABLE_H_          //将全局变量都集中在table.c文件
#define  EXTERN  extern 
#else
#define  EXTERN  
#endif

typedef	struct{
	u16_t	limit_low;	/*limit*/
	u16_t	base_low;
	u8_t	base_mid;
	u8_t    attr1;	/*P DPL S TYPE*/
	u8_t	limit_high_attr2;	/*G(1) D(1) 0(1) AVL(1) limithigh(4)*/ 
	u8_t	base_high;	/*base*/
}DESCRIPTOR;

typedef struct{
  u16_t   offset_low;
  u16_t   selector;
  u8_t    dcount;
  u8_t    attr;
  u16_t   offset_high;  
}GATE;

typedef struct {
	u32_t	backlink;
	u32_t	esp0;
	u32_t	ss0;
	u32_t	esp1;
	u32_t	ss1;
	u32_t	esp2;
	u32_t	ss2;
	u32_t	cr3;         //   这里的cr3目的
	u32_t	eip;
	u32_t	eflag;
	u32_t	eax;
	u32_t	ebx;
	u32_t	edx;
	u32_t	ecx;
	u32_t	esp;
	u32_t	ebp;
	u32_t	esi;
	u32_t	edi;
	u32_t	es;
	u32_t	cs;
	u32_t	ss;
	u32_t	ds;
	u32_t	fs;
	u32_t	gs;
	u32_t	ldt;
	u16_t	trap;
	u16_t	io_base;
	u16_t  io_map;
}TSS;

#define  T_BACKLINK   0
#define  T_ESP0        T_BACKLINK + 4
#define  T_SS0         T_ESP0 +  4
#define  T_ESP1        T_SS0  +  4
#define  T_SS1         T_ESP1 +  4
#define  T_ESP2        T_SS1  +  4
#define  T_SS2         T_ESP2 +  4
#define  T_CR3         T_SS2  +  4
#define  T_EIP         T_CR3  +  4
#define  T_EFLAG       T_EIP  +  4
#define  T_EAX         T_EFLAG+  4
#define  T_EBX         T_EAX  +  4
#define  T_EDX         T_EBX  +  4
#define  T_ECX         T_EDX  +  4
#define  T_ESP         T_ECX  +  4
#define  T_EBP         T_ESP  +  4
#define  T_ES          T_EBP  +  4
#define  T_CS          T_ES   +  4
#define  T_SS          T_CS   +  4
#define  T_DS          T_SS   +  4
#define  T_FS          T_DS   +  4
#define  T_GS          T_FS   +  4
#define  T_LDT         T_GS   +  4
#define  T_TRAP        T_LDT  +  4
#define  T_IO_BASE     T_TRAP +  4
#define  T_IO_MAP      T_IO_BASE +  4

EXTERN  DESCRIPTOR  gdt[64];
EXTERN  GATE	    idt[256];

EXTERN  TSS	    tss;

EXTERN u8_t	gdt_ptr[6];
EXTERN u8_t	idt_ptr[6];

#endif
