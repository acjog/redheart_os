#include<type.h>
#include<const.h>
#include<callnr.h>
#include<erro.h>
#include<ipc.h>
#include<com.h>
#include<sys.h>
#include<param_user.h>
#include<stdio.h>
#include<user.h>
#include <k_mem.h>

#include "h/const.h"
#include "h/glo.h"
#include "h/mproc.h"
#include "h/param.h"
#include "h/utility.h"
#include "h/elf.h"

#ifdef CMD_SIZE
#undef CMD_SIZE
#endif
#define CMD_SIZE       1
#define MAXARGS        10
#define STACK_SIZE     256
#define POINTER_SIZE   4

struct buildin_command {
   u8_t *name    ;
   i32_t (*fn)() ;
};

i32_t mm_test()  ;

//内部命令结构
struct buildin_command inner_cmd[CMD_SIZE]={
    { "test" ,mm_test }
};

//接收外部命令的缓冲
u8_t  cmdline[BUF_SIZE]           ;
u8_t  stack_buf[STACK_SIZE]       ;
u8_t  filepath[MM_FILE_PATH_LEN]  ;

//函数声明
i32_t parseline( u8_t *cmdline , u8_t **argv )              ;
i32_t build_stack(i32_t argc,u8_t *buf,u8_t *argv[],i32_t top) ;
void  load_seg( i32_t fd , Elf32_Phdr *p)                   ;

i32_t  do_exec()
{
    Elf32_Ehdr  header              ;
    u8_t  *p , *argv[MAXARGS]       ;
    i32_t  magic , i , n , *pint    ;
    i32_t fd,len,count,stack_top      ;
    u8_t *buf                         ;
    struct mproc  *mp  ;
    struct  vm_area_struct  *pvm , *tmp ;
    buf   = mm_in.EXEC_BUF          ;
    count = mm_in.EXEC_COUNT        ; 
#ifdef _MM_DEBUG
    printf( "mm exec .. "  )        ;
#endif
    strcpy(filepath,"/bin/")                  ;
    mp       =  &mproc[who]  ;
    p     = &filepath[ strlen(filepath) ]     ;
    rw_user(cmdline,who,buf,count,FROM_USER)  ;
    count = parseline(cmdline , argv )        ;
    strcpy(p , argv[0] )                      ;
    argv[0]   = filepath        ;
    fd   = open( argv[0] , W_BIT|R_BIT  ) ;
    if ( fd < 0 )
    {
       return FALSE  ;
    }
    read( fd , (u8_t*)&header , sizeof(header))  ;
    // 检查是否为可执行文件
    magic  =  *(i32_t*)(header.e_ident )  ;
    if ( magic != ELF_MAGIC  )
    {
        result = E_BAD_ELF ;
        return result           ;
    }
    //释放内存虚拟地址空间
    i= 0 ;
    for ( pvm = mp->mm ; pvm != NULL ;  ) 
    {
        pghead[i].p_vaddr = pvm->start_address  ;
        pghead[i].p_memsz = pvm->end_address - pvm->start_address;
        i++ ;
        tmp  = pvm ;
        pvm = pvm->vm_next ;
        put_vm_struct(tmp);
    }
    mp->mm = NULL ;
    n = i ;
    sys_freemem( who ,  (u8_t*)pghead , n*sizeof(Elf32_Phdr) );
    // 相当于组建向内核发送的报文
    n  = header.e_phnum        ;
    n  --   ;
    len = 0 ;
    for ( i = 0 ; i < n ; i ++ )
    {
       seek( fd , sizeof(Elf32_Ehdr)+i*sizeof(Elf32_Phdr) , START );
       read( fd , (u8_t*)(&pghead[i]) , sizeof ( pghead ) ) ;
       pvm = get_vm_struct();
       pvm->start_address  = pghead[i].p_vaddr ;
       pvm->end_address    = pghead[i].p_vaddr + pghead[i].p_memsz ;
       pvm->vm_next = NULL ;
       if ( mp->mm == NULL )
       {
           mp->mm = pvm ;
       }
       else
       {
           pvm->vm_next = mp->mm ;
           mp->mm = pvm ;
       }
    }
    //这里为了性能考虑 让内核来做内存分配的事务 
    sys_newmem( who ,  (u8_t*)pghead , n*sizeof(Elf32_Phdr) ) ;
    // 加载段
    for ( i = 0 ; i < n ; i ++ )
    {
      load_seg( fd , &pghead[i] ) ;
    }
    // 调用内核将内存分配好 回收旧的
    // 拷贝数据过去
    stack_top  = HIGH_MEM   ;
    len  = build_stack( count , stack_buf, argv , stack_top   ) ;
    sys_exec( who , stack_buf , len  );
	return OK ;
}

i32_t  do_buildin()
{
    i32_t  len  , r = FALSE  , i , count ;
    u8_t  *p  ,  *argv[MAXARGS]          ;
    p      = mm_in.SYS_ADDR1             ;
    len    = mm_in.SYS_LEN               ;
    rw_user(cmdline,who,p,len,FROM_USER) ;
    count  = parseline( cmdline , argv ) ;
    for ( i = 0 ; i < CMD_SIZE ; i++  )
    {
        if ( !strcmp( inner_cmd[i].name , argv[0] ) ) 
        {
           r =  (*inner_cmd[i].fn)() ;
        }
    }
#ifdef _MM_DEBUG
    for ( i = 0 ; argv[i] != NULL ; i++ )
    {
       printf( "%s  ", argv[i] );
    }
#endif
    return r  ;
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
  return  argc ;
}

i32_t build_stack( i32_t argc ,u8_t *buf ,  u8_t *argv[] , i32_t top )
{
   i32_t len,i , *pi , size  ;
   u8_t   *pch  ;
   len     =   sizeof(argc)*2 + argc * POINTER_SIZE   ;
   pi      =   (i32_t*)buf  ;
   pch     =   buf  ;
   *pi++   =   argc ;
   *pi++   =   8    ;
   for ( i = 0 ; i < argc ; i ++ )
   {
      *pi++    =   len ;
       pch     =   buf + len  ;
       strcpy( pch , argv[i] ); 
       len     += strlen( argv[i] ) + 1;
   }
   top   -=  len ;
   pi      =   (i32_t*)buf  ;
   pi++ ;                // argc 
   *pi  = *pi  + top ;   // argv 
   pi++   ;
   for ( i= 0 ; i < argc  ; i++  )
   {
        *pi  = *pi  + top ; 
        pi++ ;
   }
   return  len  ;
}

void load_seg(i32_t fd ,Elf32_Phdr *p )
{
  // 这里len携带很多信息
  i32_t len , newfd ;
  seek(fd,p->p_offset,START) ;
  newfd    =  who << 7 | fd         ;
  len      = ( p->p_filesz + 0xFFF ) & 0xFFFFF000 ;
  read(newfd,(u8_t*)p->p_vaddr , len ) ;
  return  ;
}


i32_t  mm_test()
{
  printf("mm_test");
  return OK ;
}


