#include <type.h>
#include <ipc.h>
#include <com.h>
#include <param_user.h>
#include "h/const.h"
#include "h/inode.h"
#include "h/superblock.h"
#include "h/buffer.h"
#include "h/com.h"
#include "h/dev.h"
#include "h/fproc.h"
#include "h/glo.h"
#include "h/file.h"

//  这里的头文件其实有顺序问题
#include <stdio.h>
#include <ipc.h>

i32_t  do_fork()
{
    struct fproc *rp , *rc    ;
    u8_t   *rpc , *rpp        ;
    i32_t parent  , child , i ;
    i32_t len ;
   // 共享一些东西
    parent  = m.FORK_PARENT ;
    child   = m.FORK_CHILD  ;
    rpp     = (u8_t*)&fproc[parent];
    rpc     = (u8_t*)&fproc[child] ; 
    rc      =  (u8_t*)&fproc[child] ;
    i = 0 ;
    len   = sizeof(struct fproc);
    while ( i++ < len    ) 
    {
         *rpc++  = *rpp++ ;
    }
    dup_inode( rc->fp_workdir );
    dup_inode( rc->fp_rootdir ) ;
    for ( i = 0 ; i < NR_FDS ; i ++ )
    {
         if ( fproc[i].fp_filp[i] != NULL )
         {
              dup_inode( (fproc[i].fp_filp[i])->filp_ino ) ;
         }
    }
    
    return 0 ;
}
