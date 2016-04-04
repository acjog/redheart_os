#ifndef _CONST_H_
#define _CONST_H_

#define  EXTERN        extern 

#define  BLOCK_SIZ        4096
#define  ZONE_SIZ         4096
#define  SECT_SIZ         512
#define  SUPER_BLOCK_V3   0x4d5a
#define  SUPER_OFFSET     0x400
#define  NR_SUPERS        32
#define  NO_DEV           0
#define  NO_BLOCK         0

#define DIRECT_PER_BLOCK  64

#define READING           0
#define WRITING           1
#define NORMAL            0
#define INODE_BLOCK       1
#define DIR_BLOCK         2
#define MAP_BLOCK         3
#define FULL_DATA_BLOCK   4
#define B_IMAP            2
#define NO_USE            0
#define NO_DEV            0

#define DIRTY             1
#define CLEAN             0

#define NON_INODE  (ino1_t)0

#define MAX_PATH_LENGTH   128
#define FILE_NAME_MAX     60

#define DIRSIZ            60

#define PUT_DIR           0
#define REMOVE_DIR        1

#define ROOT_DEV    (dev1_t)0x100    //  这个指ram盘
#define BOOT_DEV    (dev1_t)0x200    //  这个指启动软盘
#define ROOT_INODE  (ino1_t) 1
// 这些标记 是我自己看minix3的二进制发现的 可以与它兼容
#define ROOT_DIR_TYPE      0x41ff
#define DIR_FILE_TYPE      0x41ed  
#define NORMAL_FILE_TYPE   0x81a4

#define NORMAL_TYPE        10

#ifndef NULL
#define NULL        (void*)0
#endif 

#define NR_FDS      30
#define NR_PROCS    64
#define NR_FILPS    128
#define MAX_PATH    26
// 宏定义
#define MAJOR  8
#define MINMOR 0
#define BYTE   0xFF

#define SYS_UID   0
#define SYS_GID   0

#define XPIPE     0

#define   MIN(a,b)    ((a>b)?b:a)
#define   MAX(a,b)    ((a>b)?a:b)


#define ENTER   0
#define LOOK_UP 1
#define DELETE  2

#define BUF_SIZE  256 
#define MAXARGS        10 

#endif
