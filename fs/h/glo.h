#ifndef __GLO_H__
#define __GLO_H__

#include <type.h>
#include <ipc.h>
#include <const.h>

EXTERN  struct fproc *fp ;
EXTERN  message  m    ;               // 接受消息
EXTERN  message  m1   ;               // 消息发出
EXTERN  i32_t    who  ;
EXTERN  i32_t    super_user ;
EXTERN  i32_t    susp_count ;
EXTERN  i32_t    fs_call ;
EXTERN  i32_t    dont_reply ;
EXTERN  i32_t    reviving   ;         // 需要唤醒的用户进程
//用户调用的路径名,由fetch_name取来，放在此。
EXTERN  u8_t     pwd[MAX_PATH_LENGTH];
EXTERN  u8_t     user_path[MAX_PATH]; 
EXTERN  i32_t    error_code ;


#endif 
