#include<type.h>
#include<ipc.h>
#include<com.h>
#include<sys.h>


i32_t  sys_exec( i32_t  pronr , u8_t *buf , i32_t count )
{
    message  m ;
    m.m_type     =  SYS_EXEC   ;
    m.SYS_PROC1  =  pronr      ;
    m.SYS_ADDR1  =  buf        ;
    m.SYS_COUNT  =  count      ;
    sendrec( SYS_TASK , &m );
    return 0;
}
