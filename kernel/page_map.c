#include<type.h>
#include<const.h>
#include<page.h>

//设置一个页目录下的页映射
i32_t set_pmd(u32_t pmd, u32_t start  , u32_t phys ,u32_t count)
{
   u32_t i , j ,offset , addr;
   pmd  &=  0xfffff000 ;
   if ( pmd == NULL || phys == NULL || start + count > 1024 ) 
   {
     return -1 ;
   }
   // 这里默认会做4k对齐 
   addr = phys&0xfffff000 ;
   offset = start*4 ;
   for ( i = 0 ; i < count ; ++i )
   {
     *(u32_t*)(pmd+offset) = addr |PG_P|PG_RWW ;
	 offset += 4 ;
	 addr += 4096 ;
   }
   return 0 ;
}

//清除一个页目录下的页映射
i32_t unset_pmd(u32_t pmd, u32_t start  ,u32_t count, u32_t flag )
{
   u32_t i ,offset , addr;
   pmd  &=  0xfffff000 ;
   if ( pmd == NULL || start + count > 1024 )
   {
	  return -1 ;
   }
   offset = start*4 ;
   for ( i = 0 ; i < count ; ++i ) 
   {
        addr = *(u32_t*)(pmd+offset) ;
        //页目录的内存释放由内核管理　
        if (flag )
        {
          free_page(addr>>PAGE_SHIT);
        }
        *(u32_t*)addr = 0 ;
        offset +=  4 ;
   }
   return 0 ;
}

i32_t  set_page(u32_t pgd , u32_t vaddr , u32_t phys, u32_t count )
{
  u32_t i , j ,offset, pmd , n , pages ,start ;
  pgd  &= 0xfffff000 ;
  for ( i=0 ; i < count ; ++i )
  {
     offset  = vaddr>>PMD_SHIT ;
     start   = (vaddr>>PAGE_SHIT)&0x3ff ;
     if ( *(u32_t*)(pgd+offset*4)&PG_P == 0  )
     {
        *(u32_t*)(pgd+offset*4)  = (u32_t)get_page();
     }
     pmd = *(u32_t*)(pgd+offset*4);
     set_pmd(pmd,start,phys,1);
     vaddr += PAGE_SIZE ;
     phys  += PAGE_SIZE ;
  }
  return  0 ; 
}

i32_t  unset_page(u32_t pgd, u32_t vaddr, u32_t count , i32_t flag )
{
   u32_t i , j  , pmd , n , offset , pages , left , start ;  
   vaddr &= ~PAGE_SHIT ;
   pgd  &=  0xfffff000 ;
   count = count>>PAGE_SHIT;
   for (i=0 ; i <= count ; ++i )
   {
       offset   = vaddr>>PMD_SHIT ;
       start   = (vaddr>>PAGE_SHIT)&0x3ff ;
       left    = PMD_PAGES - start ;
       if ( count <  left )
       {
           left = count  ;
       }
       pmd = *(u32_t*)(pgd+offset*4);
       pages = left ;
       count -= pages ;
       vaddr +=  left<<PAGE_SHIT ;
       unset_pmd(pmd,start,pages,flag);
   }
   return  0 ;
}
