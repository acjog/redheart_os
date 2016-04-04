#include<type.h>
#include<ipc.h>
#include<com.h>
#include<sys.h>

i32_t sys_exit( i32_t procnr,   i32_t flag  )
{
  message  m ;
  m.m_type      = SYS_XIT  ;
  m.SYS_EXIT_F  = flag     ;
  m.SYS_PROC2   = procnr   ;
  sendrec(SYS_TASK , &m );
  return m.m_type ;
}
