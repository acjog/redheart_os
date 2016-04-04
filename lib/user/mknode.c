#include<type.h>
#include<const.h>
#include<ipc.h>
#include<com.h>
#include<sys.h>
#include<callnr.h>
#include<string.h>
#include<param_user.h>

i32_t  mknode( u8_t *path ,  i32_t mode , i32_t mask, i32_t zone  )
{
    message  m ;
    m.m_type       = MKNOD ;
    m.MKNODE_PATH  = path  ;
    m.MKNODE_MASK = mask  ;
    m.MKNODE_LEN   = strlen(path)+1;
    m.MKNODE_ZR    = zone  ;
    m.MKNODE_TYPE  = mode;
    sendrec( FS_PROC_NR , &m );
}
