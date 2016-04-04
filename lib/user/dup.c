#include<type.h>
#include<const.h>
#include<ipc.h>
#include<com.h>
#include<sys.h>
#include<callnr.h>
#include<string.h>
#include<param_user.h>

i32_t dup( i32_t fd0 , i32_t fd1 )
{
  message m ;
  m.m_type  = DUP ;
  m.DUP_FD0 = fd0 ;
  m.DUP_FD1 = fd1 ;
  sendrec( FS_PROC_NR , &m );
}
