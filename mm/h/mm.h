#ifndef __MM_STRUCT_H__
#define __MM_STRUCT_H__

#include <type.h>
#include <k_mem.h>
#include <const.h>
#include <memory.h>

#define MM_PRO       0
#define MM_STACK     1
#define MM_CODE      2
#define MM_DATA      3

#include "const.h"

#ifdef EXTERN
#undef EXTERN
#endif

#ifndef _TABLE_H_
#define  EXTERN  extern
#else
#define EXTERN 
#endif

EXTERN struct vm_area_struct{
       i32_t  flag                      ;
       u32_t  start_address             ;
       u32_t  end_address               ;
       struct vm_area_struct *vm_next   ;
}mm_struct[MM_STRUCTS] ;

EXTERN struct vm_area_struct *vm_front  ;

void*  get_vm_struct();
void   put_vm_struct( struct vm_area_struct *ptr  );

#endif
