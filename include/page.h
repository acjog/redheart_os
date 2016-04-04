#ifndef ___PAGE_H__
#define __PAGE_H__

#define PAGE_SIZE      4096 
#define  PG_G       0x100
#define  PG_P    1
#define  PG_USU  4
#define  PG_USS  0
#define  PG_RWW  2
#define  PG_RWR  0
#define  PAGE_SHIT    12
#define  PMD_SHIT     22
#define  PMD_PAGES     1024
#define  PMD_PAGE_SHIT  10 

//全局变量 
//内存块计数
//相当于磁盘上超级块一样 
#define  MM_PARAM_NUM       5
#define  MM_MAP_BASE        0
#define  MM_MAP_P0          MM_MAP_BASE
#define  MM_MAP_P1          MM_MAP_P0 + 4 
#define  MM_MAP_P2          MM_MAP_P1   + 4 
#define  MM_MAP_P3          MM_MAP_P2   + 4 
#define  MM_MAP_P4          MM_MAP_P3   + 4 

#ifndef _TABLE_H_
#define  EXTERN  extern
#else
#define EXTERN 
#endif

EXTERN  u8_t*           mm_map                         ;
EXTERN  u32_t*          mm_magic                       ;
EXTERN  u32_t*          mm_map_start                   ;
EXTERN  u32_t*          mm_map_end                     ;
EXTERN  u32_t*          mm_map_search                  ;
EXTERN  u32_t*          mm_user_total                  ;


//与内存相关的定义　
#define  MEMORY_SIZE  64*1024*1024 
#define MM_PAGES   MEMORY_SIZE>>PAGE_SHIT
//定义在page_map.c的接口
i32_t set_pmd(u32_t pmd, u32_t start  , u32_t phys ,u32_t count);
i32_t unset_pmd(u32_t pmd, u32_t start  ,u32_t count , u32_t flag );
i32_t  set_page(u32_t pgd , u32_t vaddr , u32_t phys, u32_t count );
i32_t  unset_page(u32_t pgd, u32_t vaddr, u32_t count , i32_t flag );
//定义在lib/common/page_alloc.h的接口
void *get_page();
void *get_pages(u32_t );
i32_t  free_page(u32_t  pfn);
i32_t  free_pages(u32_t pfn_start ,u32_t n);
void *pfn_to_phys(u32_t pfn);
u32_t phys_to_pfn(u32_t phys);

#endif
