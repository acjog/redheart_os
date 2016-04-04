#include <type.h>
#include <const.h>
#include <ipc.h>
#include <com.h>
#include <param_user.h>
#include <stdio.h>

#include "h/const.h"
#include "h/inode.h"
#include "h/superblock.h"
#include "h/buffer.h"
#include "h/com.h" 
#include "h/dev.h" 
#include "h/fproc.h"
#include "h/glo.h"

#ifdef CMD_SIZE
#undef CMD_SIZE
#endif
#define CMD_SIZE  2
struct buildin_command {
   u8_t *name    ;
   i32_t (*fn)() ;
};

i32_t fs_ls()  ;
i32_t fs_cd()  ;
//内部命令结构
struct buildin_command inner_cmd[CMD_SIZE]={
    "ls" ,fs_ls ,
    "cd" ,fs_cd 
};
//接收外部命令的缓冲
u8_t  cmdline[BUF_SIZE] ;
u8_t  *argv[MAXARGS]    ;
i32_t  argc             ;
//函数声明
i32_t  parseline( u8_t *cmdline , u8_t **argv ) ;

i32_t  do_buildin()
{
    i32_t  len  , r = FALSE  , i ;
    u8_t  *p     ;
    p      = m.SYS_ADDR1  ;
    len    = m.SYS_LEN    ;
    rw_user(cmdline,who,p,len,FROM_USER);
    argc = parseline( cmdline , argv );
    for ( i = 0 ; i < CMD_SIZE ; i++  )
    {
        if ( !strcmp( inner_cmd[i].name , argv[0] ) ) 
        {
           r =  (*inner_cmd[i].fn)() ;
           break ;
        }
    }
#ifdef _FS_DEBUG
    for ( i = 0 ; argv[i] != NULL ; i++ )
    {
       printf( "%s   ", argv[i] );
    }
#endif
    return r  ;
}

//用flag来支持命令的参数
// 0 表示不支持该选项 1表示支持小写 2表示支持大写
u8_t  ls_param[26] = { 0,0,0,0,0,0,0,0,0,0,
                       1,1,0,0,0,0,0,0,0,0,
                       0,0,0,0,0,0  };
//  10 11 
i32_t fs_ls()  
{
    i32_t size , i , n ,k, flag  = 0  , j , erro = 0  ;
    struct inode     *ip    ;
    struct buf    *bp    ;
    struct direct    *dp    ;
    for ( i = 1 ; i < argc ;  )
    {
           if ( argv[i][0] == '-' )
           {
                 for ( k = 1; argv[i][k] != 0 ; k++ )
                 {
                    switch ( argv[i][k] )
                    {
                        case 'l':
                            {
                                flag   |=  0x1 ;
                                break ;
                            }
                        case 'k':
                            {
                                flag  |=  0x2  ; 
                                break ;
                            }
                        default:
                            {
                                erro  = 1 ;
                                break ;
                            }
                    }
                 }
                 i++   ;
           }
           break ;
    }
    if ( !erro )
    {
      if ( argv[i]  == NULL )
      {
          return 0;
      }
      ip  = eat_path( argv[i] );
      if ( NULL == ip )
      {
          printf("address not find!\n");
          return FALSE ;
      }
      size  =  ip->i_size    ;
      size  =  size / sizeof( struct direct ) ;
      bp    =  get_block( ip->i_dev ,ip->i_zone[0]);
      n  = 0 ;
      for ( dp = &bp->b.b_dir[0] ; dp < &bp->b.b_dir[NR_DIR_ENTERS];dp++)
      {
         if ( dp->d_ino != 0  )
         {
             printf("%s  ",dp->d_name);
             n ++ ;
         }
      }
      printf("\n");
    }
    else
    {
      printf( "param not provide!\n");
    }
    return 0 ;
}

i32_t fs_cd()  
{
    struct inode *ip ;
    if ( argv[1] != NULL ) 
    {
       if ( !strcmp(argv[1],"./") || !strcmp(argv[1],".") ) 
       {
           return 0 ;
       }
       if ( !strcmp(argv[1],"../") || !strcmp(argv[1],"..") )
       {
           if ( !strcmp(pwd,"/") )
           {
                  fp->fp_workdir = fp->fp_rootdir ;
                  return 0 ;
           }
           pre_dir(pwd,pwd,NULL);
           ip  = eat_path(pwd);
           if ( ip != NULL )
           {
               fp->fp_workdir = ip ;
           }
           return 0 ;
       }
       ip  = eat_path( argv[1] ); 
       if ( ip != NULL  && ip->i_mode == I_DIRECTORY ) 
       {
             strcpy(pwd,argv[1]);
             fp->fp_workdir = ip ;
       }
    }
    return 0 ;
}

i32_t  parseline( u8_t *cmdline , u8_t **argv )
{
  u8_t *p0, *p1 ;
  i32_t argc = 0 , len  ;
  p0  = cmdline ;
  len = strlen( cmdline ) ; 
  if( p0[len-1] == '\n')
  {
         p0[len-1] = 0 ;
  }
  if ( p0 == NULL )
  {
    printf(" cmd wrong ");
    return  0 ;
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
  return (argc+1);
}
