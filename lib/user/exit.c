#include<type.h>
#include<const.h>
#include<ipc.h>
#include<com.h>
#include<sys.h>
#include<callnr.h>
#include<param_user.h>

i32_t  exit( i32_t flag  )
{
    message  m ;
    m.m_type     = EXIT ;
    m.EXIT_FLAG  = flag ;
    sendrec( MM_PROC_NR , &m );
    return m.m_type ;
}
