#include <type.h>
#include <ipc.h>
#include <com.h>

#define BUF_SIZE  100
i32_t buf_count = 0 ;
u8_t  print_buf[BUF_SIZE];

message put_msg ;

void _flush() ;

void  putc( u8_t ch )
{
  print_buf[buf_count++] = ch ;
  if ( buf_count == BUF_SIZE )
  {
	_flush();
  }
  if ( ch == '\n' )
  {
       _flush();
  }
}

void _flush()
{
    if ( buf_count == 0 )
    {
	return ;
    }
    //这里的buf_count还有区别
    put_msg.m_type      = TTY_WRITE  ;
    put_msg.TTY_PROC_NR = 2 ;
    put_msg.TTY_LINE    = 0 ;
    put_msg.TTY_ADDRESS = print_buf ;
    put_msg.TTY_COUNT   = buf_count ;
    buf_count           = 0 ;
    sendrec(TTY_TASK,&put_msg);
    return ;
}
