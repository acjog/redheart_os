#include<type.h>
#include<ipc.h>
#include<com.h>
#include<sys.h>

i32_t  sys_freemem( i32_t caller , u8_t *buf , i32_t len )
{
  message  m ;
  m.m_type          = SYS_FREEMEM  ;
  m.SYS_NEWMEM_PROC = caller      ;
  m.SYS_NEWMEM_BUF  = buf         ;
  m.SYS_NEWMEM_LEN  = len         ;
  sendrec(SYS_TASK , &m)          ;
  return m.m_type                 ;
}
