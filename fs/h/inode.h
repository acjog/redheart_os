#ifndef _INODE_H_
#define _INODE_H_
// 宏定义
#ifndef  DIRSIZ  
#define  DIRSIZ       60
#endif

#define FIRST_INODE   0
#define END_INODE     30
#define NR_ZONES      10
#define NR_INZONE0    7
#define NR_INZONE1    8
#define NR_INZONE2    9
#define NR_INODES     30
#define NR_INBLOCKS   1024

struct direct {
   ino1_t   d_ino;            // i节点号
   char   d_name[DIRSIZ];     // 文件名称
};

#define NR_DIR_ENTERS  BLOCK_SIZ/sizeof(struct direct)
// 这些成员有一些 并没有用到 做成这样完全是处于兼容minix系统的考虑
// 磁盘上的inode结构
struct d_inode{
  u16_t    d_mode;             // 文件类型或保护信息
  u16_t    d_nlinks;           // 多少个link指向此文件
  u16_t    d_uid;              // 拥有者用户id
  u16_t    d_gid;              // 拥有者组id
  off1_t   d_size;             // 文件大小
  time_t   d_atime;            // 最后一次访问时间
  time_t   d_mtime;            // 最后一次的修改时间
  time_t   d_ctime;            // 创建时间
  u32_t    d_zone[NR_ZONES];   // 块号以及索引块
};
// 内存中的inode结构
EXTERN struct inode{
  u16_t    i_mode;             // 文件类型或保护信息
  u16_t    i_nlinks;           // 多少个link指向此文件
  u16_t    i_uid;              // 拥有者用户id
  u16_t    i_gid;              // 拥有者组id
  off1_t   i_size;             // 文件大小
  time_t   i_atime;            // 最后一次访问时间
  time_t   i_mtime;            // 最后一次的修改时间
  time_t   i_ctime;            // 创建时间
  u32_t    i_zone[NR_ZONES];   // 块号以及索引块
  //    内存中记录的一些信息
  dev1_t   i_dev;              // 设备号
  ino1_t   i_num;              // inode号
  u32_t    i_count;            // 使用计数
  u8_t     i_dirty;            // clean or dirty 
  struct super_block *i_sp;    // 指向的超级块
}inode[NR_INODES];


#define I_CHAR_SPECIAL      0x416d
#define I_DIRECTORY         0x41ed
#define I_BLOCK_SPECIAL     0x42d6
#define I_REGULAR           4
#define I_TYPE              7

//外部使用的函数
extern  struct inode *get_inode(dev1_t , ino1_t)    ;
extern  void          put_inode(struct inode*)      ;
extern  struct inode* alloc_inode(dev1_t)           ;
extern  void          free_inode(dev1_t,ino1_t)     ;
extern  void          rw_inode(struct inode*, u8_t) ;
extern  void          dup_inode(struct inode *ip )  ;
#endif

