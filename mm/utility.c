#include<const.h>
#include<type.h>
#include<ipc.h>
#include<com.h>
#include<sys.h>
#include<param_user.h>

#include "h/const.h"
#include "h/param.h"
#include "h/glo.h"

i32_t no_sys()
{
       return 0 ;
}

i32_t  tell_fs( i32_t call , pid parent , pid child  )
{
       message   m        ;
       m.m_type  = call   ;
       m.FORK_PARENT  = parent ;
       m.FORK_CHILD   = child  ;
       sendrec(FS_PROC_NR , &m );
}

void  reply( i32_t whom , i32_t result )
{
   // 发送一个reply消息
   mm_out.m_type = result ;
   send(whom,&mm_out);
}

void panic( u8_t *str )
{
  printf( str  );
  while(1) ;
}

i32_t  rw_user( u8_t *bp , i32_t s , u8_t  *sbuf ,  i32_t count , i32_t direction   )
{
   message  m ;
   m.m_type = SYS_COPY ;
   if ( direction == TO_USER )
   {
     m.SYS_PROC1  =  MM_PROC_NR  ;
     m.SYS_PROC2  =  s           ;
     m.SYS_ADDR1  =  bp          ;
     m.SYS_ADDR2  =  sbuf        ;
   }
   else
   {
     m.SYS_PROC1  =  s           ;
     m.SYS_PROC2  =  MM_PROC_NR  ;
     m.SYS_ADDR1  =  sbuf        ;
     m.SYS_ADDR2  =  bp          ;
   }
   m.SYS_COUNT  =  count         ;
   sys_copy( &m );
   return m.m_type   ;
}

