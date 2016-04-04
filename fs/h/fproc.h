#ifndef __FPROC_H__
#define __FPROC_H__

EXTERN  struct fproc{
	i32_t          fp_umask        ;
	struct  inode *fp_workdir      ;    // 当前工作目录
	struct  inode *fp_rootdir      ;    // 根目录
	struct  inode  *executable     ;    // 可执行文件节点
	struct  filp  *fp_filp[NR_FDS] ;
	uid            fp_realuid      ;
	uid            fp_effuid       ;
	gid            fp_realgid      ;
	gid            fp_effgid       ;
	i32_t          fs_tty          ;
	i32_t          fp_fd           ;     // save fd 
	u8_t*          fp_buffer       ;
	i32_t          fp_bytes        ;
	u8_t           fp_suspended    ;
	u8_t           fp_revived      ;
	u8_t           fp_task         ;    // 任务号
}fproc[NR_PROCS];
#define       NOT_SUSPENDED      0
#define       SUSPENDED          1
#define       NOT_REVIVING       0
#define       REVIVING           1

#endif 
