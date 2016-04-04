#ifndef __PARAM_USER_H__
#define __PARAM_USER_H__

//READ
#define READ_FD    m1_i1
#define READ_BUF   m1_p1
#define READ_BYTES m1_i2

//EXEC 
#define EXEC_BUF    m1_p1
#define EXEC_COUNT  m1_i1

//OPEN
#define OPEN_NAME  m1_p1
#define OPEN_FLAG  m1_i1
#define OPEN_LEN   m1_i2

//CLOSE
#define CLOSE_FD   m1_i2


//MKNOD
#define MKNODE_PATH m2_p1
#define MKNODE_MASK m2_i1
#define MKNODE_ZR   m2_i2
#define MKNODE_LEN  m2_i3
#define MKNODE_TYPE m2_l1

//CREAT
#define CREAT_MASK   m1_i1

//DUP
#define DUP_FD0   m1_i1
#define DUP_FD1   m1_i2

//EXIT
#define EXIT_FLAG  m1_i1

//FORK
#define FORK_PARENT m1_i1
#define FORK_CHILD  m1_i2

//SEEK
#define SEEK_FD       m1_i1
#define SEEK_OFFSET   m1_i2
#define SEEK_FLAG     m1_i3
#define CURRENT       1
#define START         2
#define END           3

#endif
