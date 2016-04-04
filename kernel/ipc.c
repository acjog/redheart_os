//实现异步消息，因为有时异步消息还是有必要的
#include <type.h>
#include <const.h>
#include <com.h>
#include <protect.h>
#include <memory.h>
#include <proc.h>
#include <callnr.h>
#include <glo_var.h>
#include <glo_fun.h>
#include <ipc.h>

//得到消息缓存
asyn_mess*  get_msg_buf()
{
  asyn_mess *p      ;
  if ( NULL == asyn_front )
  {
     return NULL    ;
  }
  p   = asyn_front  ;
  //确保异步消息访问正确，避免竞争
  setlock();
  asyn_front = asyn_front->next  ;
  restore();
}

//异步消息发送
void asyn_send( i32_t flag ,asyn_mess *m_ptr  )
{
  setlock();
  m_ptr->next     =   NULL ;
  if ( ASYN_TO_MM == flag )
  {
      if ( NULL == mm_hptr )
      {
         mm_hptr         = m_ptr  ;
         mm_tptr         = m_ptr  ;
         mm_hptr->next   = NULL   ;
      }
      else
      {
        mm_tptr->next    = m_ptr   ;  
        mm_tptr          = m_ptr   ;
      }
  }
  else
  {
      if ( NULL == fs_hptr )
      {
         fs_hptr         = m_ptr  ;
         fs_tptr         = m_ptr  ;
         fs_hptr->next   = NULL   ;
      }
      else
      {
        fs_tptr->next    = m_ptr   ;  
        fs_tptr          = m_ptr   ;
      }
  }
  restore();
}


//通知mm fs，内核有任务安排  这是异步消息
void inform()
{
   PROCESS *rp ;
   rp  = proc_addr(MM_PROC_NR);
   if ( (rp->p_flag & RECEVING ) && ( mm_hptr != NULL )  )
   {
      mini_send(HARDWARE,MM_PROC_NR,&(mm_hptr->m) );
      mm_hptr   = mm_hptr->next ;
      if ( NULL == mm_hptr )
      {
         mm_tptr  = NULL ;
      }
   }
   rp   = proc_addr(FS_PROC_NR) ;
   if ( ( rp->p_flag & RECEVING ) && ( fs_hptr != NULL )  )
   {
      mini_send(HARDWARE,FS_PROC_NR,&(fs_hptr->m) );
      fs_hptr   = fs_hptr->next ;
      if ( NULL == fs_hptr ) 
      {
         fs_tptr  = NULL  ;
      }
   }
}
