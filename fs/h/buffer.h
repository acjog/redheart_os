#ifndef _BUFFER_H_
#define _BUFFER_H_

#define  NR_BUFS                    0x100
#define  MAX_BLOCK_SIZE             4096
#define  MAX_DIR_SIZE               64 
#define  INDIRE_BLOCK_SIZE          1024
#define  DINODE_BLOCK_SIZE          ( 4096/sizeof(struct d_inode) ) 
#define  MAX_BITMAP_BLOCK_SIZE      4096

struct buf {
    union { 
           char           b_data[MAX_BLOCK_SIZE]          ;  // 普通数据块
           struct direct  b_dir[MAX_DIR_SIZE]             ;  // 目录项块
           u32_t          b_ind[INDIRE_BLOCK_SIZE]        ;  // 间接寻址块
           struct d_inode b_dinode[DINODE_BLOCK_SIZE]     ;  // i节点块
           bit_chunk      b_bitmap[MAX_BITMAP_BLOCK_SIZE] ;  // 位图块
      }b;
    // 缓冲块的头部数据结构
    struct buf *b_next;               //  缓冲双向链表的后向指针
    struct buf *b_prev;               //  缓冲双向链表的前驱指针
    struct buf *b_hash_next;          //  hash表next指针
    block1_t    b_blocknr;            //  块号
    dev1_t      b_dev;                //  设备号 major | minor
    char        b_dirt;               //  块脏 或 块净
    char        b_count;              //  块使用计数器
};

EXTERN struct buf *front;   // 最久未使用空闲表
EXTERN struct buf *rear;    // 最近使用的空闲表尾
EXTERN int    bufs_in_use;  // 在使用的缓冲
#define NR_BUF_HASH   NR_BUFS
#define HASH_MASK    (NR_BUF_HASH -1 )
EXTERN struct buf *buf_hash[NR_BUF_HASH];

// 这里的缓冲地址是固定好的 
EXTERN  struct buf *buf;

void         put_block(struct buf*, int)   ;
void         zero_block(struct buf *)      ;
struct buf*  get_block(dev1_t,block1_t)    ;
void         rm_lru(struct buf* )          ;
void         flushall( dev1_t  )           ;
void         rw_block(struct buf *, int )  ;
void         free_zone(dev1_t , u32_t )    ;
zone1_t      alloc_zone( dev1_t, u32_t)    ;

#define  NIL_BUF   ((struct buf *)0)
// 这里的超级块偏移为0块
#define  SUPER_BLOCK      0 

#endif
