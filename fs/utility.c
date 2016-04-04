#include <type.h>
#include <ipc.h>
#include <com.h>
#include "h/const.h"
#include "h/inode.h"
#include "h/superblock.h"
#include "h/buffer.h"
#include "h/com.h"
#include "h/dev.h"
#include "h/fproc.h"
#include "h/glo.h"
#include "h/param.h"

//  这里的头文件其实有顺序问题 #include <stdio.h>
#include <ipc.h>


i32_t no_sys()
{
  return ;
}

// 拷贝数据  这里的拷贝是在自己的数据空间 
// 主要用于load_ram时
void copy( u8_t *dest , u8_t *src , i32_t bytes )
{
    if ( bytes <= 0 )
    {
            return ;
    }
    if ( bytes % sizeof(i32_t) == 0 && ((i32_t)dest)% sizeof(i32_t) == 0 &&
     ((i32_t)src)%sizeof(i32_t) == 0    )
    {
      register i32_t n = (bytes/sizeof(i32_t)) ;
	  register i32_t *dpi = (i32_t*)dest       ;
	  register i32_t *spi = (i32_t*)src        ;
	  do { *dpi++ = *spi++ ; }while(--n)       ;
    }
    else
    {
	  register i32_t n = bytes/sizeof(u8_t)    ;
	  register u8_t *dpc = (u8_t*)dest         ;
	  register u8_t *spc = (u8_t*)src          ;
	  do { *dpc++ = *spc++ ; }while(--n)       ;
    }
}

void panic( u8_t *str )
{
  printf( str )     ;
  while(1)          ;
}

i32_t  fetch_name( u8_t *str , i32_t len , i32_t  flag )
{
  u8_t  *sptr , *dptr ;
  i32_t erro = OK ;
  dptr  = user_path ;
  // M3表示字符串比较短 可以存放在消息包中 否则要从用户程序的地址空间拷贝
  if ( flag == M3 )
  {
       sptr  = m.m3_cal ;
       for ( ; *sptr != 0 ;  ) 
       {
           *dptr++  = *sptr++;
       }
       return OK ;
  }
  if ( len > MAX_PATH )
  {
     return FALSE  ;
  }
  erro = rw_user( dptr , who , str , len , FROM_USER  );
  return erro  ;
}


