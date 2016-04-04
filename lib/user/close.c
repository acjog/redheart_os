#include<type.h>
#include<const.h>
#include<ipc.h>
#include<com.h>
#include<sys.h>
#include<callnr.h>
#include<param_user.h>
#include<string.h>

i32_t  close(  i32_t fd   )
{
    message  m ;
    m.m_type    = CLOSE            ;
    m.CLOSE_FD  = fd               ;
    sendrec( FS_PROC_NR , &m )     ;
}
