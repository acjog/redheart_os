#ifndef _TABLE_H_
#define _TABLE_H_
#endif

#include <sys.h>
#include<type.h>
#include<callnr.h>
#include "h/const.h"

// 包括全局变量 
#undef EXTERN
#define EXTERN 
#include<page.h>
#include "h/glo.h"
#include "h/mproc.h"
#include "h/mm.h"

//mm系统接受的系统调用
extern  i32_t  do_fork();
extern  i32_t  do_exec();
extern  i32_t  do_exit();
extern  i32_t  no_sys();
extern  i32_t  do_buildin();

i32_t  (*call_vector[NCALLS])()={
    no_sys   ,                     // 0 = unused 
	do_exit  ,                     // 1 = exit
	do_fork  ,                     // 2 = fork
	no_sys   ,                     // 3 = read
	no_sys   ,                     // 4 = write 
	no_sys   ,                     // 5 = open 
	no_sys   ,                     // 6 = close 
	no_sys   ,                     // 7 = wait
	no_sys   ,                     // 8 = create
	no_sys   ,                     // 9 = link 
	no_sys   ,                     // 10 = unlink 
	no_sys   ,                     // 11 = chdir 
	no_sys   ,                     // 12 = time 
	no_sys   ,                     // 13 = mknod
	no_sys   ,                     // 14 = chmod 
	no_sys   ,                     // 15 = brk 
	no_sys   ,                     // 16 = stat 
	no_sys   ,                     // 17 = lseek 
	no_sys   ,                     // 18 = getpid
	no_sys   ,                     // 19 = mount
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
	no_sys   ,                     // 30 = dup 
	no_sys   ,                     // 31 = pipe 
	no_sys   ,                     // 32 = times
	no_sys   ,                     // 33 = setgid
	no_sys   ,                     // 34 = getgid 
	no_sys   ,                     // 35 = signal 
	no_sys   ,                     // 36 = ioctl
	do_exec  ,                     // 37 = exec
	no_sys   ,                     // 38 = umask
	no_sys   ,                     // 39 = chroot
    no_sys   ,                     // 40
    no_sys   ,                     // 41
    do_buildin ,                   // 42
};


