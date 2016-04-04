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

//  这里的头文件其实有顺序问题
#include <stdio.h>
#include <ipc.h>

i32_t  do_sync()
{

}

i32_t  do_exit()
{
  i32_t  i ; 
  struct fproc *fp  ;
  fp  = &fproc[who] ;
  for ( i = 0 ; i < NR_FDS  ; i++ ) 
  {
      if ( fp->fp_filp[i] != NULL )
      {
           put_inode( fp->fp_filp[i]->filp_ino );
      }
  }
  return 0 ;
}

// 复苏
i32_t  do_revive()
{
  #ifdef _FS_DEBUG
  printf( "revive");
  #endif
  if ( who > 0 )
  {
   return FALSE ;
  }
  revive( m.REP_PROC_NR , m.REP_STATUS );
  dont_reply  = TRUE ;
  return OK ;
}
