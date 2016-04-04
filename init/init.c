#include <type.h>
#include <const.h>
#include <sys.h>
#include <user.h>
#include <stdio.h>
#include <com.h>
#include <ipc.h>
#include <string.h>
//仅仅包含struct comand结构
#include <init.h>
//宏
#define MAXAREGS        10 
#define I_CHAR_SPECIAL  0x416d
#define BUILDIN_CMDS    4
#define INIT_BUF_SIZE   256
//全局变量
u8_t  cmdline[INIT_BUF_SIZE];
u8_t  test ;
message  m ;
u8_t  *shell_argv[MAXAREGS] ;
struct command build_cmd[BUILDIN_CMDS]={
    {  "clear" , SYS_TASK     },
    {  "cd"    , FS_PROC_NR   },
    {  "ls"    , FS_PROC_NR   },
    {  "test"  , MM_PROC_NR   }
};

//函数
void  eval(u8_t *) ;
void  parseline( u8_t *cmdline , u8_t **argv );
i32_t build_command( u8_t *cmd[] );
i32_t  mm_exec(i32_t task  , u8_t *argv[]  );
int main()
{
    i32_t  pid  , a  , b  ;
    i32_t  fd ;
    //开始使用页保护，将数据段替换出来
    test  = 1;
    printf(" this is init , haha hello buf:%d ",cmdline);
    mknode("/dev/stdin" , I_CHAR_SPECIAL , 0x1 ,0x300);
    mknode("/dev/stdout", I_CHAR_SPECIAL , 0x1 ,0x300);
    mknode("/dev/stderr", I_CHAR_SPECIAL , 0x1 ,0x300);
    open("/dev/stdin" ,0x1);
    open("/dev/stdout",0x1);
    open("/dev/stderr",0x1);
    #ifdef _INIT_DEBUG
    printf("hello world ");
    #endif
    //执行shell 
    while ( TRUE )
    {
       printf("localhost@thinks>");
       read(STDIN,cmdline, 256);
       #ifdef _INIT_DEBUG
       printf( cmdline );
       #endif
       eval( cmdline );
    }
    return 0;
}

void eval(u8_t *cmdline )
{
  i32_t pid  , r ;
  i32_t index , i,len ;
  // 检测是否为有效按键
  len = strlen( cmdline ) ; 
  if( cmdline[len-1] == '\n')
  {
         cmdline[len-1] = 0 ;
  }
  for (i=0 ; cmdline[i] != 0 ;)
  {
      if(!(is_uper(cmdline[i])||is_lower(cmdline[i])||cmdline[i]==' '||cmdline[i] == '-'||cmdline[i]=='/'||cmdline[i]=='.'))
      {
          return ;
      }
      i++ ;
  }
  parseline( cmdline , shell_argv );
  if ( shell_argv[0] == '\0' )
  {
      return ;
  }
  index = build_command(shell_argv ) ;
  //内部命令shell自己处理
  if ( index >= 0  )
  {
//     printf(" buildin cmd :%d,%s",build_cmd[index].server,shell_argv[0]);
     mm_exec( build_cmd[index].server , shell_argv );
     return ;
  }
  //外部命令
  if ( ( pid=fork()) == 0 )
  {
      if ( (r=exec(shell_argv[0], shell_argv )) < 0 ) 
      {
           exit(0) ;
      }
  }
  return ;
}

void parseline( u8_t *cmdline , u8_t **argv )
{
  u8_t *p0, *p1 ;
  i32_t argc = 0 , len  ;
  p0  = cmdline ;
  if ( p0 == NULL )
  {
    printf(" cmd wrong ");
    return ;
  }
  while( *p0 != 0 )
  {
     while( *p0 == ' ' ) 
     {
         p0++ ;
     }
     if ( *p0 == 0 )
     {
        break ;
     }
     argv[argc++] = p0 ;
     while( *p0!= ' '  && *p0 != 0 ) 
     {
         p0++;
     }
     if( *p0 == ' ' )
     {
         *p0 = 0 ;
         p0++;
     }
  }
  //补充null
  argv[argc]=0;
}

i32_t  build_command( u8_t *argv[] )
{
   i32_t i = 0 , flag  = 0  ;
   for ( i = 0 ; i < BUILDIN_CMDS ;  ++i  )
   {
     if ( !strcmp( build_cmd[i].cmdbuf , argv[0] ) )
     {
           flag = 1;
           break ;
     }
   }
   return ( flag == 1 ) ? i : -1 ;
}


