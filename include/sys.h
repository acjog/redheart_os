#ifndef _SYS_H_
#define _SYS_H_ 

#include <type.h>
#include <ipc.h>
#include <com.h>

extern  void    sendrec( u32_t dest , message *ptr );
extern  void    send( u32_t dest , message *ptr );
extern  void    receve( u32_t src  , message *ptr );

//系统库
extern  i32_t   sys_fork( pid , pid, void *, void * );
extern  i32_t   sys_exit( i32_t procnr ,i32_t flag ) ;
extern  i32_t   sys_exec( i32_t procnr , u8_t *buf , i32_t count );
extern  i32_t   sys_newmem(i32_t caller ,  u8_t *buf , i32_t len ) ;
extern  i32_t   sys_copy( message *m_ptr );
extern  i32_t   sys_freemem(i32_t caller ,  u8_t *buf , i32_t len ) ;

#endif
