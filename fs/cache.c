#include <type.h>
#include <const.h>
#include <com.h>
#include <ipc.h>
#include "h/const.h"
#include "h/inode.h"
#include "h/superblock.h"
#include "h/buffer.h"
//  这里的头文件其实有顺序问题
#include <stdio.h>
#include <string.h>

void rm_lru(struct buf*);
void rw_block(struct buf*,int);
void flushall(dev1_t dev);

// 从指定设备号读一个大小为4k的块
// 这个设备可能是软盘也可能是虚拟的RAM盘
struct buf*  get_block(dev1_t dev,  block1_t  block )
{

    u8_t flag = 0 ;
    struct buf *p,*pre; 
    if ( dev == NO_DEV )
    {
        printf(" get_block:dev %d is not avaiable ",dev);
        return   0;
    }
    u32_t hash_value = block & HASH_MASK;
    p = buf_hash[hash_value];
    // 首先在hash链表中寻找
    while ( p != NULL )
    {
        if ( p->b_blocknr == block && p->b_dev == dev ) 
        { 
            if ( p->b_count == 0 )
            {
                bufs_in_use++;
                rm_lru(p);
            }
            p->b_count++;
            //这里一定要从lru链表删除
//          printf("cached count:%d,bp:%d,front:%d",p->b_count,p,front);
            return  p;
        }
        p = p->b_hash_next;
    }
    if ( ( p = front ) == NULL  )
    {
        printf("No buf to use!");
        return 0;
    }
    if ( p->b_next == NULL )
    {
        printf("no buf to use P:%d,bufs_in_use:%d",p,bufs_in_use);
        while (1);
    }
    // 从hash链移除该块 并且你知道了
    // 为什么 buf_hash[0] = front这行代码的原因啦
    hash_value = p->b_blocknr & HASH_MASK;   
    pre = buf_hash[hash_value];
    if ( pre == p )
    {
        buf_hash[hash_value] = p->b_hash_next;
    }
    else
    {
        while( pre->b_hash_next != NULL )
        {
            if ( pre->b_hash_next == p )
            {
                /*  find it */
                pre->b_hash_next = p->b_hash_next;
                break;  
            }
            pre = pre->b_hash_next; 
        }
    }
    //  若只有一个块可以用时
    if ( front == rear ) 
    {
        front = NULL ;
        rear  = NULL;
    }
    else//  更新front指针的值
    {
        front = front->b_next;  
        front->b_prev = NULL;
    }
    // 若该块脏 则先写回
    if ( p->b_dirt == DIRTY  )
    {
        rw_block(p,WRITING);
        p->b_dirt = CLEAN;
    }
    // 设置值
    p->b_dev = dev;
    p->b_blocknr = block;
    rw_block(p , READING );
    p->b_count = 1;
    // 添到hash表中
    hash_value = block & HASH_MASK;
    p->b_hash_next = buf_hash[hash_value];
    buf_hash[hash_value] = p;
    bufs_in_use ++ ;
    return p;
}

// type = 0 表示放在lru表开始 1 表示放在结尾
void put_block(struct buf * bp, int type)
{
    bp->b_count--;
    struct buf *pre, *next;
    // 当计数器为0时 将该块放在lru链 以便迅速回收它
    if( bp->b_count == NO_USE )
    {
        // 若为inode块或dir块放在lru链尾,以便还要利用它
        // 让它待在lru的时间尽可能长
        if ( type == INODE_BLOCK || type == DIR_BLOCK  )
        { 
            pre = rear;
            next = NULL  ;
            bp->b_prev = pre;
            bp->b_next = next;
            if ( rear == NULL ) 
            {
                front = bp ;
                rear  = bp ;
            } 
            else
            {
               rear->b_next  = bp ;
               rear          = bp ;
            }
        }
        else
        {
            //  放在lru表头 
            pre = NULL ;
            next = front;
            bp->b_prev = pre;
            bp->b_next = next;
            if ( front == NULL ) 
            {
                rear  = bp ;
                front = bp ;
            }
            else
            {
               front->b_prev = bp ;
               front         = bp ;
            }
        }
        bufs_in_use--;
        // rw_block(bp, WRITING);
        // bp->b_dirt = CLEAN ;
    }
}

//  分配一个zone  
zone1_t  alloc_zone( dev1_t dev, u32_t z )
{
    zone1_t zone;
    struct super_block *sp = getsuper(dev);
    zone = alloc_bit(sp,ZMAP,z);
    if ( zone == NON_INODE  ) 
    {
        printf(" call alloc_bit fail !\n");
        exit(0);
    }
    return zone - 2 + sp->s_firstdatazone ;
}

//  释放一个zone 
void free_zone(dev1_t dev, u32_t z)
{
    struct super_block *sp = getsuper(dev);
    z = z + 2  - sp->s_firstdatazone ; 
    printf("free zone %d:\n",z);
    if ( z < sp->s_zsearch )
    {
        sp->s_zsearch = z ;
    }
    free_bit(sp,ZMAP,z);
}

// 读写块设备
void rw_block(struct buf *bp, int rw_flag)
{ 
    i32_t    r   ;
    file_pos pos ;
    dev1_t   dev ;  
    if ( bp->b_dev != NO_DEV )
    {
        pos = (bp->b_blocknr*BLOCK_SIZ)/SECT_SIZ ;
        r   = dev_io(rw_flag,bp->b_dev,pos,BLOCK_SIZ,FS_PROC_NR,bp->b.b_data);
        if ( r < 0 )
        {
            dev = bp->b_dev ;
            printf( "unrecovery disk error on device %d %d,block %d",(dev>>MAJOR)&BYTE,(dev>>MINMOR)&BYTE,bp->b_blocknr);
        }
    }
    bp->b_dirt  = CLEAN ;
}

// flush all block at dev 
void flushall( dev1_t dev )
{
    register struct buf *bp;
    for( bp = &buf[0]; bp < &buf[NR_BUFS]; bp++ )
    {
        if ( bp->b_dev ==  dev && bp->b_dirt == DIRTY ) 
        {
            rw_block( bp , WRITING );
        }
    }
}

// 这里可以看出用双向链表的用心良苦
void rm_lru(struct buf *bp)
{
    // 如果块脏，则回写
    struct buf *pre,*next;
    if ( bp->b_dirt == DIRTY  )
    {
        rw_block(bp,WRITING);
        bp->b_dirt = CLEAN  ;
    }
    // 修改双向链表
    pre  = bp->b_prev;
    next = bp->b_next;
    if ( pre )
    {
        pre->b_next = next;
    }
    else
    {
        front = next ;  // 前面为NULL则表示删除的为lru表头
        pre   = front;
    }
    if ( next  )
    {
        next->b_prev = pre; 
    }
    else
    {
        rear = pre;
    }
}

void zero_block( struct buf *bp )
{
    memset( bp->b.b_data, 0 , sizeof(bp->b.b_data) );
    bp->b_dirt = DIRTY;
}
