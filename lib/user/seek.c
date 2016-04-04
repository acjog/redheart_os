#include<type.h>
#include<const.h>
#include<ipc.h>
#include<com.h>
#include<sys.h>
#include<callnr.h>
#include<param_user.h>

i32_t  seek( i32_t fd , i32_t offset , i32_t flag )
{
   message m ;
   m.m_type       =  SEEK    ;
   m.SEEK_FD      =  fd      ;
   m.SEEK_OFFSET  =  offset  ;
   m.SEEK_FLAG    =  flag    ;
   sendrec(FS_PROC_NR , &m);
   return m.m_type ;
}
