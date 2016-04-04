#include<type.h>
#include<const.h>
#include<ipc.h>
#include<com.h>
#include<sys.h>
#include<callnr.h>
#include<stdio.h>

i32_t fork()
{
    message    m ; 
    m.m_type   =   FORK ;
    sendrec( MM_PROC_NR , &m ) ;
    return m.m_type ;
}
