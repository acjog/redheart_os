#include<type.h>
#include<const.h>
#include<ipc.h>
#include<com.h>
#include<sys.h>
#include<callnr.h>
#include<param_user.h>

i32_t  read( i32_t fd , u8_t *buf , i32_t bytes  )
{
    message  m ;
    m.m_type     = READ ;
    m.READ_FD    = fd   ;
    m.READ_BUF   = buf  ;
    m.READ_BYTES = bytes;
    sendrec( FS_PROC_NR , &m );
    return m.m_type ;
}
