#include <const.h>
#include <type.h>
#include <glo_var.h>
#include <memory.h>
#include <glo_fun.h>
#include <page.h>

//外部接口 
void page_init();
void *get_page();
void *get_pages(u32_t  );
i32_t  free_page(u32_t  pfn);
i32_t  free_pages(u32_t pfn_start ,u32_t n);
void   free_all_page(u32_t pgd);

void *pfn_to_phys(u32_t pfn);
u32_t phys_to_pfn(u32_t phys);

void page_init()
{
    u32_t  i ;
    // 64M内存　需要16k个页面 其中开始的64M-1M- 1M -2M 左右
    // 也就是前1k个字节作为mm与内核交换信息所用
      mm_map        = MM_MAP_BASE  ;
      mm_magic      = MM_MAP_P0 ; 
      mm_map_start  = MM_MAP_P1 ;
      mm_map_end    = MM_MAP_P2 ;
      mm_map_search = MM_MAP_P3 ;
      mm_user_total = MM_MAP_P4 ;
    if ( *mm_magic != 0x12345678 )
    {
      *mm_magic  = 0x12345678 ; 
      *mm_map_start  = 1024 ;
      *mm_map_end    = MM_PAGES ;
      *mm_map_search = 1024 ;
      *mm_user_total = *mm_map_end - *mm_map_start ;
      for ( i = *mm_map_start  ; i < *mm_map_end ; ++i )
      {
        mm_map[i] = 0 ;
      } 
    }
}

u32_t phys_to_pfn(u32_t phys)
{
    return phys>>PAGE_SHIT ;
}

void *pfn_to_phys(u32_t pfn)
{
     return pfn<<PAGE_SHIT  ;
}

void *get_page()
{
  return get_pages(1);
}

void *get_pages(u32_t n)
{
  u32_t i , j , k, search_pages =  0, total ;
  total  = *mm_user_total ; 
  for ( i = *mm_map_search ; search_pages < total  ;  )
  {
      if ( i + n - 1  < *mm_map_end  )
      {
         for ( j = i+n-1 ; mm_map[j] == 0 && j >= i ; --j ) ;
         // 找到的 
	     if ( j < i )
	     {
	        for ( k = 0 ; k < n ; ++k )
		    {
		          mm_map[i+k] = 1; 
		    }
		    *mm_map_search += n ; 
		    return pfn_to_phys(i);
	     }
         search_pages += n ;
	     i += n ;
      }
      else
      {
          search_pages += *mm_map_end - *mm_map_search ;
          i = *mm_map_start ;
          continue ;
      }
	  
  }
  return NULL ;
}

i32_t  free_page( u32_t pfn )
{
  return free_pages(pfn,1) ;
}

i32_t  free_pages( u32_t pfn_start , u32_t n )
{
   u32_t i ,pfn_end = pfn_start + n ;
   if ( pfn_end < *mm_map_end ) 
   {
     for ( i = pfn_start ; i < pfn_end && mm_map[i]!=0 ; ++i );
     if ( i < pfn_end )
     {
        return -1 ;
     }
     for ( i = pfn_start ; i < pfn_end ; ++i )
     {
        mm_map[i]-- ;
     }
     return 0 ;
   }
   return -1 ;
}

