#include <type.h>
#include <const.h>
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

i32_t  read_write( i32_t flag  );
i32_t  rw_chunk( struct inode *rip , i32_t pos , i32_t off , i32_t chunk , i32_t flag , u8_t *buf ,i32_t proc );
i32_t  rw_user( u8_t *bp , i32_t s , u8_t  *sbuf ,  i32_t count , i32_t direction   );
i32_t  read_map( struct inode *rip , i32_t pos );

i32_t  do_read()
{
  return (read_write(READING));
}


i32_t  read_write( i32_t flag  )
{
  struct inode *rip    ;
  struct filp *f       ;
  i32_t  pos ,f_size,r , num_io=0, chunk, bytes , off, fd , caller  ;
  u32_t  mode          ;
  u8_t  *buf           ;
  fd      = m.READ_FD     ;
  bytes   = m.READ_BYTES  ;
  buf     = m.READ_BUF    ;
  caller  = who           ;
  if ( bytes  <= 0 )
  {
       return  OK ;
  }
  if ( who == MM_PROC_NR  && fd &0xFFFFFFF8 )
  {
          caller = fd >> 7    ;
          fd     = fd & 0x7F  ;
  }
  if ( (f=get_filp(fd) )== NULL  )
  {
     //printf( "who:%d,call:%d",who,fs_call);
     return FALSE ;
  }
  if ( (f->filp_mode & ( flag==READING?R_BIT:W_BIT )) == 0  )
  {
     return FALSE ;
  }
  pos    = f->file_pos  ;
  rip    = f->filp_ino  ;
  f_size = rip->i_size  ;
  mode   = rip->i_mode  ;
  // 字符文件
  if ( mode == I_CHAR_SPECIAL  )
  {
     if ( (r=dev_io(flag,(dev1_t)rip->i_zone[0],pos,bytes,caller,buf))>=0) 
     {
         r  = OK ;
     }
  }
  else
  {
     while( bytes != 0 )
     {
        off   = pos  % BLOCK_SIZ    ;
        chunk = MIN(bytes,BLOCK_SIZ-off);
        if ( chunk < 0 )
        {
          chunk  = BLOCK_SIZ  - off ;
        }
        if ( flag == READING  )
        {
            if (  f_size - pos <= 0 )
            {
                 break ;
            }
            else
            {
                if ( chunk > f_size - pos )
                {
                    chunk  = f_size  - pos ;
                }
            }
        }
        r =  rw_chunk( rip , pos , off , chunk , flag , buf , caller  );
        if ( r != OK )
        {
           break ;
        }
        buf    += chunk ;
        bytes  -= chunk ;
        num_io += chunk ;
        pos    += chunk ;
     }
  }
  if ( flag == WRITING )
  {
       rip->i_dirty   = DIRTY  ;
  }
  f->file_pos  = pos  ;
  return ( r == OK ? num_io : r);
}


i32_t  rw_chunk( struct inode *rip , i32_t pos , i32_t off , i32_t chunk , i32_t flag ,u8_t *buf , i32_t proc )
{
   struct  buf *bp ;
   i32_t block_nr , direction  ;
   block_nr = read_map( rip , pos )  ; 
   bp       = get_block( rip->i_dev , block_nr );
   if ( flag == READING )
   {
       direction  = TO_USER  ;
   }
   else
   {
       direction  = FROM_USER ;
   }
   rw_user( &(bp->b.b_data[off]) , proc , buf , chunk , direction   );
   if ( flag  == WRITING  )
   {
         bp->b_dirt  = DIRTY  ;
   }
   put_block( bp , NORMAL );
#ifdef _FS_DEBUG
   printf( "rw_chunk");
#endif 
   return OK ;
}

i32_t  rw_user( u8_t *bp , i32_t s , u8_t  *sbuf ,  i32_t count , i32_t direction   )
{
   message  m ;
   m.m_type = SYS_COPY ;
   if ( direction == TO_USER )
   {
     m.SYS_PROC1  =  FS_PROC_NR  ;
     m.SYS_PROC2  =  s           ;
     m.SYS_ADDR1  =  bp          ;
     m.SYS_ADDR2  =  sbuf        ;
   }
   else
   {
     m.SYS_PROC1  =  s           ;
     m.SYS_PROC2  =  FS_PROC_NR  ;
     m.SYS_ADDR1  =  sbuf        ;
     m.SYS_ADDR2  =  bp          ;
   }
   m.SYS_COUNT  =  count         ;
   sys_copy( &m );
   return m.m_type   ;
}


i32_t  read_map( struct inode *rip , i32_t pos )
{
    struct buf *bp ; 
    i32_t  b ,  index , excess  ;
    index  = pos / BLOCK_SIZ  ;
    if ( index  < NR_INZONE0 )
    {
       return rip->i_zone[index] ;
    }
    index  -=  NR_INZONE0 ;
    if ( index < 2*NR_INBLOCKS )
    {
          excess =  index % NR_INBLOCKS ;
          index  /= NR_INBLOCKS ;
          index  = rip->i_zone[ NR_INZONE0+index] ;
    }
    else
    {
        index -= 2*NR_INBLOCKS ;
        excess = index % NR_INBLOCKS ;
        bp     = get_block(rip->i_dev , NR_INZONE2);
        b      = bp->b.b_ind[index];
        bp     = get_block(rip->i_dev,b);
        index  = bp->b.b_ind[index] ;
    }
    bp  = get_block( rip->i_dev , index );
    b   = bp->b.b_ind[excess] ;
    return b ;
}
