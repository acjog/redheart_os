#include<const.h>
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
#include "h/file.h"

//进程搜索到的文件描述符和文件指针放进k,fpt里
i32_t  get_fd( i32_t mask , i32_t *k , struct filp **fpt )
{
   register struct filp *f ;
   i32_t i ;
   *k = -1 ;
   for ( i = 0 ; i < NR_FDS ;  i ++ )
   {
       if ( fp->fp_filp[i] == NULL )
       {
              *k  = i ;
              break ;
       }
   }
   if ( *k < 0 )
   {
         return FALSE ;
   }
   // 注意这里并不将count置1,因为这里调用者的责任
   for ( f = &filp[0]; f < &filp[NR_FILPS] ; f++ )
   {
       if (  f->filp_count == 0  )
       {
             f->filp_mode  = mask ;
             f->file_pos   = 0    ;
             *fpt         =  f    ;
             return OK ;
       }
   }
   return FALSE ;
}

struct  filp *get_filp( i32_t fd )
{
    if ( fd < 0 || fd > NR_FDS )
    {
          return  NULL ;
    }
    return  fp->fp_filp[fd] ;
}

struct filp *find_filp( struct inode *rip , i32_t bits )
{
    register struct filp *f ;
    for ( f = &filp[0] ; f < &filp[NR_FILPS] ;  f++ )
    {
       if ( f->filp_count != 0 && f->filp_ino == rip && ( f->filp_mode & bits ) )
       {
             return f ;
       }
    }
    return NULL ;
}




