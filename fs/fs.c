// sys 
#include <type.h> 
#include <const.h>
#include <sys.h>
#include <stdio.h>
#include <callnr.h>
#include <erro.h>
#include <ipc.h>
#include <page.h>
// user
#include "h/const.h"
#include "h/glo.h"
#include "h/superblock.h"
#include "h/fproc.h"
#include "h/inode.h"
#include "h/buffer.h"
#include "h/param.h"
//  函数声明
void  fs_init();
void  get_work();
void  reply(i32_t whom , i32_t result );

void  buf_pool();
void  load_ram();
void  load_super();

// 声明系统调用数组
extern   i32_t (*call_vector[])();

int main()
{
	i32_t error ;
    printf(" fs ");
	fs_init();
	while ( TRUE  )
	{
	  get_work();
	  fp  = &fproc[who] ;
	  super_user = fp->fp_effuid == SU_UID ?TRUE : FALSE ;
	  dont_reply = FALSE ;
	  if ( fs_call < 0 || fs_call > NCALLS )
	  {
	       error = E_BAD_CALL ;
	  }
	  else 
	  {
	       error =  (*call_vector[fs_call])();
	  }
	  if ( dont_reply )
	  {
		continue  ;
	  }
	  reply(who,error);
	}
	return 0;
}

void get_work()
{
  register struct fproc  *rp ;
 // 是否有需要唤醒的用户进程
 if ( reviving != 0 )
 {
	for ( rp = &fproc[0] ; rp < &fproc[NR_PROCS] ; rp++ )
	{
		if ( rp->fp_revived == REVIVING )
		{
		   who = rp - &fproc[0] ;
		   fs_call = rp->fp_fd & BYTE  ;
		   FD      = (rp->fp_fd>>MAJOR) & BYTE ;
		   BUFFER  = fp->fp_buffer ;
		   NBYTES  = fp->fp_bytes  ;
		   rp->fp_suspended   = NOT_SUSPENDED ;
		   rp->fp_revived     = NOT_REVIVING  ;
		   reviving -- ;
		   return ;
		}
	}
 }
 receve(ANY,&m);
 who       = m.m_source ;
 fs_call   = m.m_type   ;
 fp        = &fproc[who]; 
}

void  fs_init()
{
  register  struct inode *rip ;
  i32_t i ;
  // 缓冲起始地址
  // 1M地方存放的是1M+64个内存页表
  buf = (struct buf *)(0x100000 + 4*0x11000) ;
  // 初始化
  buf_pool();
  load_ram();
  load_super();
  // 初始化MM INIT
  for ( i = 0 ; i < 3 ; i+=2 )
  {
     fp               = &fproc[i] ;
     rip              = get_inode(ROOT_DEV,ROOT_INODE);
     fp->fp_rootdir   = rip ;
     fp->fp_workdir   = rip ;
     strcpy(pwd,"/");
     fp->fp_realuid   = SYS_UID ;
     fp->fp_effuid    = SYS_UID ;
     fp->fp_realgid   = SYS_GID ;
     fp->fp_effgid    = SYS_GID ;
     fp->fp_umask     = ~0;
  }
  // 还可以在这里做一些检测
  // 唤醒init  这样做是一种暂时的办法
  m.m_type   =  SYS_REVIVE_INIT ;
  sendrec( SYS_TASK , &m );
}

void  reply( i32_t whom , i32_t result )
{
   // 发送一个reply消息
   m1.m_type = result ;
   send(whom,&m1);
}

void buf_pool()
{
    struct buf *bp ;
    front   =  &buf[0] ;
    rear    =  &buf[NR_BUFS-1];
    for ( bp = &buf[0]; bp < &buf[NR_BUFS]; bp++ )
    {
	bp->b_blocknr = NO_BLOCK ;
	bp->b_dev     = NO_DEV   ;
	bp->b_next    = bp + 1   ;
	bp->b_prev    = bp - 1   ;
    bp->b_count   = 0        ;
    }
    buf[0].b_prev  =  NIL_BUF    ;
    buf[NR_BUFS-1].b_next  = NIL_BUF ;
    buf[NR_BUFS-1].b_count = 0       ;
    // hash表
    for ( bp = &buf[0] ; bp < &buf[NR_BUFS-1]; bp++ )
    {
	  bp->b_hash_next  = bp->b_next  ;
    }
    buf_hash[NO_BLOCK & HASH_MASK ]   = front  ;
    bufs_in_use = 0 ;
}

void load_ram()
{
  struct buf *bp0 , *bp1 ;
  i32_t  count , base, i  ;
  struct super_block *sp ;
  base  =  (i32_t)buf + NR_BUFS*sizeof(struct buf) ; 
  //  这里使用移位比位与好一些
  //  因为移位是活的 而位与则固定了
  base  = (base + BLOCK_SIZ -1 ) >> PAGE_SHIT ;
  base  = base << PAGE_SHIT   ;
  count = (KERNEL_ADDR_END - base)>>PAGE_SHIT ;
  if ( count >  ((1440*1024)>>12) )
  {
       count  = (1440*1024)>>12 ;
  }
#ifdef _FS_DEBUG
  printf("base:%d,count:%d",base,count);
#endif 
  // 这个可以暂时不做
  bp0  = get_block(BOOT_DEV,SUPER_BLOCK);
  copy(super_block,((i32_t)(bp0->b.b_data)+SUPER_OFFSET ),sizeof(struct super_block));
  sp = &super_block[0] ;
  if ( sp->s_magic != SUPER_BLOCK_V3 )
  {
     printf("the fs system is not used  by system %d %d", sp->s_ninodes,sp->s_imap_blocks);
  }
  else
  {
#ifdef _FS_DEBUG
     printf( "super success ");
#endif
  }
  put_block(bp0,FULL_DATA_BLOCK);
#ifdef _DEBUG
  count  = 128  ;
#endif
  for ( i = 0 ; i < count ; i ++ )
  {
    bp0 = get_block(BOOT_DEV,i);
    bp1 = get_block(ROOT_DEV,i);
    copy(bp1->b.b_data,bp0->b.b_data,BLOCK_SIZ);
    bp1->b_dirt   = DIRTY ;
    put_block(bp0,INODE_BLOCK);
    put_block(bp1,FULL_DATA_BLOCK);
  }
  i32_t n = 0;
  struct buf *tp = front ;
  while (  tp != NULL )
  {
    n++ ;
    tp=tp->b_next ;
  }
  printf("RAM disk loaded success %d",n);
}

void  load_super()
{
  i32_t i ; 
  struct super_block  *sp ;
  // 进行超级块的读取
  for ( sp = &super_block[0] ; sp < &super_block[NR_SUPERS] ; sp ++  )
  {
      sp->s_dev  = NO_DEV  ;
  }
  sp = &super_block[0]   ;
  sp->s_dev  = ROOT_DEV  ;
  rw_super(sp,READING)   ; 
  sp->s_block_size       =  BLOCK_SIZ ;
  sp->s_inodes_per_block =  BLOCK_SIZ/sizeof(struct inode );
  sp->s_isearch          =  1;
}
