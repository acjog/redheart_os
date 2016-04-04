#include<type.h>
#include<const.h>
#include<ipc.h>
#include<com.h>
#include<sys.h>
#include<callnr.h>
#include<param_user.h>
#include<string.h>

i32_t  open( u8_t *file_name , i32_t flag   )
{
    message  m                           ;
    m.m_type     = OPEN                  ;
    m.OPEN_NAME  = file_name             ;
    m.OPEN_FLAG  = flag                  ;
    m.OPEN_LEN   = strlen( file_name )+1 ;
    sendrec( FS_PROC_NR , &m )           ;
    return  m.m_type ;
}
