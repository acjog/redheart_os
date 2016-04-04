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

//  这里的头文件其实有顺序问题
#include <stdio.h>
#include <ipc.h>

extern  read_write( i32_t );

i32_t  do_write()
{
  return ( read_write(WRITING));
}


struct buf *new_block( struct inode *ip , i32_t pos )
{
    return NULL ;
}
