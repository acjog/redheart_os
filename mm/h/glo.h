#ifndef __GLO_H__
#define __GLO_H__

#include "elf.h"

#define PHEAD_NUM      5

EXTERN  struct mproc  *mp  ;

EXTERN i32_t  procs_in_use ;
EXTERN i32_t  who          ;
EXTERN i32_t  mm_call      ;
EXTERN i32_t  error        ;
EXTERN i32_t  dont_reply   ;


EXTERN  message  mm_in     ;
EXTERN  message  mm_out    ;
EXTERN  i32_t    result    ;


EXTERN  Elf32_Phdr  pghead[PHEAD_NUM]     ;

#endif
