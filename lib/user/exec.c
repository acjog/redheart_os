#include<type.h>
#include<const.h>
#include<ipc.h>
#include<com.h>
#include<sys.h>
#include<callnr.h>
#include<param_user.h>
#include<string.h>

#undef  BUF_SIZE
#define BUF_SIZE   256
u8_t    buf[BUF_SIZE] ;
i32_t  exec( u8_t *file_name , u8_t *argv[] )
{
  i32_t  count = 0     ;
  u8_t  **p            ;
  message  m           ;
  m.m_type     = EXEC  ;
  m.EXEC_BUF   = buf   ;
  p            = argv  ;
  while ( *p != NULL )
  {
      strcpy(&buf[count],*p)  ;
      count    += strlen( *p) ;
      buf[count] = ' '        ;
      p++                     ;
      count++                 ;
  }
  m.EXEC_COUNT = count        ;
  sendrec( MM_PROC_NR , &m )  ;
  return m.m_type             ;
}

//这里有一个执行内部命令的函数
i32_t  mm_exec(i32_t task  , u8_t *argv[]  )
{
  i32_t  count = -1 ;
  u8_t **p          ;
  message  m        ;
  m.m_type     = BUILD_IN_CMD ;
  p            = argv         ;
  while ( *p != NULL )
  {
      count++                 ;
      strcpy(&buf[count],*p)  ;
      count    += strlen( *p) ;
      buf[count]   =   ' '    ; //这里是补空格
      p++;
  }
  buf[count]   =    0         ;
  m.EXEC_BUF   = buf          ;
  m.EXEC_COUNT = count+1        ;
  sendrec( task , &m )        ;
  return m.m_type             ; 
}
