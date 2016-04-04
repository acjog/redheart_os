// sys 
#include <type.h>
#include <const.h>
#include <sys.h>
#include <stdio.h>
#include <callnr.h>
#include <erro.h>
#include <ipc.h>

// user
#include "h/const.h"
#include "h/glo.h"
#include "h/fproc.h"
#include "h/inode.h"
#include "h/buffer.h"
#include "h/param.h"

void suspend( i32_t task )
{
  if ( task == XPIPE )
  {
	susp_count++;
  }
  fp->fp_suspended = SUSPENDED ;
  fp->fp_fd        = (FD << 8) | fs_call&BYTE ;
  fp->fp_buffer    = BUFFER ;
  fp->fp_bytes     = NBYTES  ;
  fp->fp_task      = -task  ;
  dont_reply       = TRUE    ;
}

void revive( i32_t proc , i32_t bytes )
{
    struct fproc *rfp ;
   rfp = &fproc[proc] ;
   rfp->fp_suspended = NOT_SUSPENDED ;
   rfp->fp_bytes     = bytes ;
   reply( proc , bytes );
}
