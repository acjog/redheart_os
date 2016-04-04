/*******************************
 * fs的全局变量定义于此 
 * *****************************/
#include <type.h>
#include <com.h>
#include "h/const.h"
#include  <ipc.h>
#include  <callnr.h>

#undef  EXTERN
#define EXTERN  

#include "h/inode.h"
#include "h/superblock.h"
#include "h/buffer.h"
#include "h/dev.h"
#include "h/fproc.h"
#include "h/file.h"
#include "h/glo.h"

//  fs系统接受的系统调用
extern  i32_t  do_exit();
extern  i32_t  do_fork();
extern  i32_t  do_exec();
extern  i32_t  do_read();
extern  i32_t  do_mknod();
extern  i32_t  do_write();
extern  i32_t  do_mount();
extern  i32_t  do_open();
extern  i32_t  do_dup();
extern  i32_t  do_revive();
extern  i32_t  no_sys();
extern  i32_t  do_buildin();
extern  i32_t  do_seek() ;

i32_t  (*call_vector[NCALLS])()={
    no_sys   ,                     // 0 = unused 
	do_exit  ,                     // 1 = exit
	do_fork  ,                     // 2 = fork
	do_read  ,                     // 3 = read
	do_write ,                     // 4 = write 
	do_open  ,                     // 5 = open 
	no_sys   ,                     // 6 = close 
	no_sys   ,                     // 7 = wait
	no_sys   ,                     // 8 = create
	no_sys   ,                     // 9 = link 
	no_sys   ,                     // 10 = unlink 
	no_sys   ,                     // 11 = chdir 
	no_sys   ,                     // 12 = time 
	do_mknod ,                     // 13 = mknod
	no_sys   ,                     // 14 = chmod 
	no_sys   ,                     // 15 = brk 
	no_sys   ,                     // 16 = stat 
	do_seek  ,                     // 17 = seek 
	no_sys   ,                     // 18 = getpid
	do_mount ,                     // 19 = mount
	no_sys   ,                     // 20 = umount
	no_sys   ,                     // 21 = setuid 
	no_sys   ,                     // 22 = getgid
	no_sys   ,                     // 23 = stime
	no_sys   ,                     // 24 = alarm
	no_sys   ,                     // 25 = fstat
	no_sys   ,                     // 26 = pause 
	no_sys   ,                     // 27 = kill
	no_sys   ,                     // 28 = access
	no_sys   ,                     // 29 = sync
	do_dup   ,                     // 30 = dup 
	no_sys   ,                     // 31 = pipe 
	no_sys   ,                     // 32 = times
	no_sys   ,                     // 33 = setgid
	no_sys   ,                     // 34 = getgid 
	no_sys   ,                     // 35 = signal 
	no_sys   ,                     // 36 = ioctl
	do_exec  ,                     // 37 = exec
	no_sys   ,                     // 38 = umask
	no_sys   ,                     // 39 = chroot
    no_sys   ,                     // not used 
    do_revive,                     //41 = revive
    do_buildin   ,                 // not used
    no_sys   ,                     // not used 
    no_sys   ,                     // not used
    no_sys   ,                     // not used
    no_sys   ,                     // not used 
    no_sys   ,                     // not used
    no_sys   ,                     // not used
    no_sys   ,                     // not used 
    no_sys   ,                     // not used
    no_sys   ,                     // not used
    no_sys   ,                     // not used 
    no_sys   ,                     // not used
    no_sys   ,                     // not used
    no_sys   ,                     // not used 
    no_sys   ,                     // not used
    no_sys   ,                     // not used
    no_sys   ,                     // not used 
    no_sys   ,                     // not used
    no_sys   ,                     // not used
    no_sys   ,                     // not used 
    no_sys   ,                     // not used
    no_sys   ,                     // not used
};


extern i32_t  no_call();
extern i32_t  rw_dev();
extern i32_t  rw_dev2();
// 设备映射表
struct  dmap  dmap[]={
//  open     read/write      close      task         device  file 
    0,	     0,              0,          0,           //  not used
    no_call, rw_dev,	     no_call,    MEM_TASK,    //  /dev/mem  0x100
    no_call, rw_dev,         no_call,    FLOPPY_TASK, //  /dev/fd0  0x200
    no_call, rw_dev2,        no_call,    TTY_TASK     //  /dev/tty  0x300
};
