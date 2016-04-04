#include <type.h>
#include <ipc.h>
#include <com.h>
#include "h/const.h"
#include "h/inode.h"
#include "h/superblock.h"
#include "h/buffer.h"
#include "h/com.h"
#include "h/dev.h"
#include "h/fproc.h"
#include "h/glo.h"

//  这里的头文件其实有顺序问题
#include <stdio.h>
#include <ipc.h>

// 内部函数声明
void  find_dev(dev1_t);

// 局部数据
message dev_mess ;
i32_t  major , minor , task ;
i32_t max_major = 3;


i32_t no_call( i32_t task_nr , message *m_ptr )
{
	m_ptr->REP_PROC_NR = OK ;
	return 0;
}

i32_t dev_open( dev1_t dev , i32_t mask )
{
    find_dev(dev);
    (*dmap[major].dmap_open)(task,&dev_mess); 
    return (dev_mess.REP_STATUS);
}

i32_t dev_close( dev1_t dev )
{
  find_dev(dev);
  (*dmap[major].dmap_close)(task,&dev_mess);
}

i32_t rw_dev( i32_t task_nr , message *m_ptr )
{
  i32_t proc_nr ;
  proc_nr =  m_ptr->PROC_NR ;
  sendrec(task_nr,m_ptr) ;
  return    OK ;
}

i32_t  rw_dev2( i32_t dummy , message *m_ptr )
{
	i32_t task_nr , major_s ;
//	major   =  (fp->fs_tty)>>MAJOR & BYTE   ;
	task_nr = dmap[major].dmap_task ;
//	m_ptr->DEVICE = (fp->fs_tty)>>MINMOR & BYTE  ;
    #ifdef _FS_DEBUG
    printf( "dev_io,task_nr:%d,m_ptr:%d,type:%d",-task_nr,(i32_t)m_ptr,m_ptr->m_type);
    #endif
	rw_dev(task_nr,m_ptr);
}

void  find_dev( dev1_t dev  )
{
  major  =  (dev>>MAJOR ) & BYTE ;
  minor  =  (dev>>MINMOR) & BYTE ;
  if ( major > max_major )
  {
      printf("no such task \n");
      return ;
  } 
  //  设备号为两个字节 高字节表示驱动低字节表示对于的设备
  task = dmap[major].dmap_task ;
  dev_mess.DEVICE = minor ;
}

i32_t  dev_io(i32_t rw_flag , dev1_t dev ,file_pos pos , i32_t nbytes , i32_t proc , u8_t *buf )
{
	// 从设备读写
	find_dev(dev);
	// 设置消息
	dev_mess.m_type  = (rw_flag == READING?DISK_READ:DISK_WRITE);
	dev_mess.DEVICE  = (dev>>MINMOR) & BYTE ;
	dev_mess.POS     = pos ;
	dev_mess.PROC_NR = proc ;
	dev_mess.ADDR    = buf  ;
	dev_mess.COUNT   = nbytes ;
	// 调用驱动
	(*dmap[major].dmap_rw)(task,&dev_mess);
	if ( dev_mess.REP_STATUS == SUSPENDED )
	{
	    suspend(task);
	}
	return (dev_mess.m_type );
}
