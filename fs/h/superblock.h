#ifndef __SUPERBLOCK_H__
#define __SUPERBLOCK_H__
/* although i don't need so much, it is also compatible with minix 
 * file system, and some type are defined in types.h, but i am happy 
 * that  some types are already defined  in linux. :) . finally, the 
 *  disk layout is:
 *   Item             #blocks
 *   boot   block       1
 *   unused block       1  ( for parameters that os used)
 *   super  block       1  (offset 1kb)
 *   inode  map         s_imap_blocks
 *   zone   map         s_zmap_blocks
 *   inodes            (s_ninodes+"inodes per block"-1)/"inodes per block"
 *   unused           whatever is need to fill out the current zone (4kb)
 *   data  zone       (s_zones-s_firstdatazone)<< s_log_zone_size
 */
#define IMAP  0
#define ZMAP  1
#define START_BLOCK  2
#define spsiz_disk  ( sizeof(ino1_t)+ 2*sizeof(zone1_t)+5*sizeof(u16_t)\
+sizeof(off1_t)+sizeof(u32_t) )

EXTERN struct super_block {
  ino1_t   s_ninodes        ;          //  已使用的节点个数
  zone1_t  s_nzones         ;          //  整个块设备的大小
  u16_t    s_imap_blocks    ;          //  i节点位图的大小 
  u16_t    s_zmap_blocks    ;          //  zone位图大小 
  zone1_t  s_firstdatazone  ;          //  第一个数据区域
  u16_t    s_log_zone_size  ;          //  log2 blocks/zones 
  u16_t    s_pad            ;          //  未被使用
  off1_t   s_max_size       ;          //  最大文件大小
  u32_t    s_zones          ;          //  number f zones
  u16_t    s_magic          ;          //  魔数
  //  以下内容存在于内存中
  u8_t                  s_dirty ;
  i16_t                 s_pad2;
  u16_t                 s_block_size;
  u8_t                  s_disk_version;
  struct inode          *s_isup;
  struct inode          *s_imount;
  u32_t                 s_inodes_per_block;
  dev1_t                s_dev;               // 设备号
  i32_t                 s_rd_only;
  i32_t                 s_native;
  i32_t                 s_version;
  i32_t                 s_ndzones;
  i32_t                 s_nindirs;
  bit_t                 s_isearch;        /* could use    */
  bit_t                 s_zsearch;        /* could use    */
}super_block[NR_SUPERS];

extern struct super_block * getsuper(dev1_t );
extern bit_t  alloc_bit(struct super_block *, int , bit_t);
extern void   free_bit(struct super_block *, int , bit_t);
extern void   rw_super( struct super_block *sp , i32_t rw_flag );

#endif
