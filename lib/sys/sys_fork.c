#include<type.h>
#include<ipc.h>
#include<com.h>
#include<sys.h>

i32_t  sys_fork( pid parent , pid child , void *dir_base ,void *page_frame )
{
   message     m                   ;
   m.SYS_PROC1      = parent       ;
   m.SYS_PROC2      = child        ;
   m.m_type         = SYS_FORKED   ;
   sendrec( SYS_TASK , &m );
   return 0;
}
