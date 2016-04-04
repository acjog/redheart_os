#include <type.h>
#include "h/const.h"
#include "h/inode.h"
#include "h/superblock.h"
#include "h/buffer.h"
//  这里的头文件其实有顺序问题
#include <stdio.h>

//   搜索超级块表
struct super_block* getsuper( dev1_t dev )
{
	struct super_block *sp ;
	for ( sp = &super_block[0]; sp < &super_block[NR_SUPERS] ; sp++ )
	{
		if ( sp->s_dev == dev )
		{
			return sp ;
		}
	}
	//  否则出错啦
	printf("can't find superblock for device %d\n",dev);
    panic("error");
}

#define MAP_BLOCK     3
#define START_BLOCK   2
//   0 -- inode位图 1 -- zone位图
//   查找一个空闲的区域 从origin处开始  不知有些东西老是犯糊涂 
//   比如第多少个 或者偏移多少个等 这里的origin是指第几个的意思
//   返回结果也是第几个 
bit_t  alloc_bit(struct super_block *sp , int map , bit_t origin  )
{
  block1_t  block;
  u32_t  count;
  u32_t *word_ptr, start = ( origin -1 ) / (8*sizeof(u32_t)), end = BLOCK_SIZ - sizeof(u32_t);
  u8_t  *byte_ptr;
  bit_t  ret;
  dev1_t dev = sp->s_dev;
  struct buf *bp;
  if ( map == IMAP ) 
  {
     block = START_BLOCK; 
  }   
  else if ( map == ZMAP ) 
  {
    block = START_BLOCK + 1;
  }
  bp = get_block(dev,block);
 if ( bp == NULL ) 
 {
   printf("get_block fail\n");
   exit(0);
 }
   word_ptr = (u32_t*) &(bp->b.b_bitmap[start]);
  /* travel the block  at most BLOCK_SIZ/4  times */
   for ( count = 0; count < BLOCK_SIZ/sizeof(u32_t) ; count++)
   {
    if ( *word_ptr ==(u32_t) ~0)
     {
          word_ptr++;
          if ( word_ptr > (u32_t*)&(bp->b.b_bitmap[end]) ) 
               word_ptr = (u32_t*) &(bp->b.b_bitmap[0]);
          continue;
     }
    for ( byte_ptr = (u8_t*)word_ptr; byte_ptr < (u8_t * ) (word_ptr+1); byte_ptr++ ) 
    {
      if ( *byte_ptr == (u8_t)~0 ) 
          continue; 
      else
      {
        u8_t i;
        ret = ( word_ptr - (u32_t*) &(bp->b.b_bitmap[0]) )*32;
        for ( i = 0 ; (*byte_ptr & 1<<i) ; i++) ;
        *byte_ptr = *byte_ptr | ( 1 << i );
        /* add 1 */
        ret += ( byte_ptr - (u8_t*)word_ptr)*8 + i + 1;
        bp->b_dirt = DIRTY;
        put_block(bp, MAP_BLOCK);
        return ret;       
      }
    }
  }
  put_block(bp, MAP_BLOCK);
  return 0;
}

void   free_bit(struct super_block *sp, int map, bit_t  bit_releas)
{
  dev1_t dev = sp->s_dev;
  struct buf* bp ;
  u8_t  count = ( bit_releas -1 )/8,  i = bit_releas - 1 - count*8;
  u8_t *byptr;
  block1_t block;
  if ( map == IMAP ) 
  {
    block = START_BLOCK ;
  }
  if ( map == ZMAP ) 
  {
    block = START_BLOCK + 1;
  }
  bp = get_block(dev,block);
  if ( bp == NULL )
  {
    printf("call get_block fail at super.c file\n");
    exit(0);
  }
  byptr = (u8_t *)&(bp->b.b_bitmap[count]);
  *byptr = (*byptr) & ( ~(1<<i));
  rw_block(bp, WRITING);
  put_block(bp,MAP_BLOCK);
}


void rw_super( struct  super_block *sp , i32_t rw_flag )
{
   struct buf  *bp ;
   dev1_t      dev ;
   if ( rw_flag == READING  )
   {
       dev  = sp->s_dev ;
       bp   = get_block(dev,SUPER_BLOCK);
       copy(sp,((i32_t)(bp->b.b_data)+SUPER_OFFSET ),sizeof(struct super_block));
       sp->s_dev  = dev ;
   }
   else
   {
       bp = get_block(sp->s_dev , SUPER_BLOCK );
       copy(((i32_t)(bp->b.b_data)+SUPER_OFFSET ),sp,sizeof(struct super_block));
       bp->b_dirt  = DIRTY  ;
   }
   sp->s_dirty  = CLEAN  ;
   put_block(bp,INODE_BLOCK);
}
