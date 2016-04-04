#ifndef  _CALLNR_H_
#define  _CALLNR_H_
// 标记为B的表示本系统会去实现这些调用
#define NCALLS            69
#define EXIT              1           //  B
#define FORK              2           //  B
#define READ              3           //  B
#define WRITE             4           //  B
#define OPEN              5     
#define CLOSE             6
#define WAIT              7
#define CREAT             8
#define LINK              9
#define UNLINK            10
#define CHDIR             11
#define TIME              12
#define MKNOD             13
#define CHMOD             14
#define BRK               15
#define STAT              16
#define SEEK              17
#define GETPID            18
#define MOUNT             19           //  B
#define UNMOUNT           20
#define SETUID            21
#define GETUID            22
#define STIME             23
#define ALARM             24           // B
#define FSTAT             25
#define PAUSE             26
#define KILL              27
#define ACCESS            28
#define SYNC              29
#define DUP               30
#define PIPE              31
#define TIMES             32
#define SETGID            33
#define GETGID            34
#define SIGNAL            35
#define IOCTL             36
#define EXEC              37           // B
#define UMASK             38
#define CHROOT            39

#define   K_UN_PRO        40
#define   REVIVE          41
#define   BUILD_IN_CMD    42
#define   KSIG            66           // B
#define   UNPAUSE         67
#define   BRK2            68
#endif
