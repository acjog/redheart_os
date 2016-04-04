#include<type.h>
#include<const.h>
#include<ipc.h>
#include<com.h>
#include<param_user.h>
#include<erro.h>

#include "h/const.h"
#include "h/dev.h"
#include "h/inode.h"
#include "h/file.h"
#include "h/fproc.h"
#include "h/glo.h"
#include "h/param.h"
#include "h/utility.h"

struct inode *new_node( u8_t *path , i32_t mask , i32_t zone_nr );

//  这里的头文件其实有顺序问题
i32_t  do_open() 
{
    i32_t  r , fd , maska ;
    maska  = 1;
   struct filp  *fil_ptr ;
   struct inode *rip    ;
   if ( (r = fetch_name(m.OPEN_NAME, m.OPEN_LEN , M1  )) != OK )
   {
       return FALSE ;
   }
   if ( (r = get_fd(maska , &fd , &fil_ptr )) != OK  )
   {
      return r ;
   }
   if ( ( rip = (struct inode *)eat_path( user_path ) ) == NULL  )
   {
       return FALSE ;
   }
   switch ( rip->i_mode  & I_TYPE  )
   {
     case   I_DIRECTORY :
         {
             if ( maska & W_BIT )
             {
              put_inode( rip );
              return OK ;
             }
         }
     case I_CHAR_SPECIAL :
         {
             if ( fp->fs_tty == 0 )
             {
                 fp->fs_tty = rip->i_zone[0] ;
             }
             dev_open( (dev1_t)(rip->i_zone[0]) , maska ) ;
             break ;
         }
     case I_BLOCK_SPECIAL:
         {
             dev_open( rip->i_zone[0] , maska );
             break ;
         }
     default:
         {
             break ;
         }
   }
   fp->fp_filp[fd]       = fil_ptr  ;
   fil_ptr->filp_count   = 1        ;
   fil_ptr->filp_ino     = rip      ;
   fil_ptr->file_pos     = 0        ;  
   return fd  ;
}


i32_t do_close()
{
  struct filp *f ;
  struct inode *rip ;
  i32_t mode ;
  i32_t fd = m.CLOSE_FD ;
  f  = get_filp( fd ) ;
  rip = f->filp_ino   ;
  mode = rip->i_mode & I_TYPE ;
  if ( mode == I_CHAR_SPECIAL  || mode == I_BLOCK_SPECIAL   )
  {
       if ( mode == I_BLOCK_SPECIAL )
       {
           do_sync();
       }
       dev_close( (dev1_t)rip->i_zone[0] );
  }
  if ( --f->filp_count == 0 )
  {
      put_inode( rip );
  }
  fp->fp_filp[fd] = NULL ;
}


i32_t do_mknod()
{
  struct inode * ip ;
  i32_t r, mask , type   ;
  #ifdef _FS_DEBUG
  printf("mknode");
  #endif
  if ( !super_user )
  {
      return FALSE ;
  }
  if ( ( r = fetch_name(m.MKNODE_PATH,m.MKNODE_LEN , M1))!= OK )
  {
       return r ;
  }
  mask  = m.MKNODE_MASK & fp->fp_umask   ;
  type  = m.MKNODE_TYPE  ;
  ip = (struct inode *)new_node(user_path,mask,m.MKNODE_ZR) ;
  ip->i_mode  = type ;
  put_inode(ip );
  return r ;
}

i32_t do_creat()
{
  i32_t   r , fd, bits  ;
  struct filp *fil_ptr ;
  struct inode *rip ;
  bits  = (m.CREAT_MASK & fp->fp_umask )  ;
  if ( ( r = fetch_name(NAME, NAME_LENGTH , M3))!= OK )
  {
       return r ;
  }
  rip = new_node(user_path,bits,0) ;
  r   = error_code  ;
  if ( r != OK && r != EEXIST )
  {
     return r ;
  }
  if ( r == EEXIST )
  {
      switch ( rip->i_mode & I_TYPE  )
      {
           case I_DIRECTORY :
               {
                   r = EISDIR ;
                   break ;
               }
           case I_REGULAR:
               {
                   if ( ( r = forbidden( rip , W_BIT,0) ) == OK )
                   {
                        truncate( rip );
                   }
                   break ;
               }
           case I_CHAR_SPECIAL:
           case I_BLOCK_SPECIAL:
               {
                   r = dev_open( rip->i_zone[0],  W_BIT );
                   break ;
               }
      }
  }
  if ( r != OK )
  {
           put_inode( rip );
           return r ;
  }
  fp->fp_filp[fd] = fil_ptr        ;
  fil_ptr->filp_count = 1          ;
  fil_ptr->filp_ino   = rip        ;
  return fd ;
}

// seek( fd , offset , flag  )
i32_t do_seek()
{
  struct inode *iptr              ;
  struct filp *fil_ptr            ;
  i32_t  fd  , offset , flag      ;
  fd          =  m.SEEK_FD        ;
  offset      =  m.SEEK_OFFSET    ;
  flag        =  m.SEEK_FLAG      ;
  fil_ptr     = get_filp( fd )    ;
  if ( NULL == fil_ptr )
  {
      return FALSE ;
  }
  iptr  =  fil_ptr->filp_ino  ;
  switch ( flag )
  {
   case  START:
       {
           fil_ptr->file_pos  = offset   ;
           break ;
       }
   case END:
       {
           if ( iptr->i_size > offset  )
           {
                fil_ptr->file_pos  = iptr->i_size  - offset  ;
           }
           else
           {
               fil_ptr->file_pos   = iptr->i_size            ;
           }
           break ;
       }
   default:
       {
             fil_ptr->file_pos  += offset                  ;
       }
  }
}

i32_t do_dup()
{
    i32_t fd0 ,fd1 ;
    fd0 = m.DUP_FD0 ;
    fd1 = m.DUP_FD1 ;
    fp->fp_filp[fd1] = fp->fp_filp[fd0] ;
    fp->fp_filp[fd0] = NULL ;
    return OK ;
}


struct inode *new_node( u8_t *path , i32_t filetype , i32_t zone_nr )
{
  i32_t dev , r  ;
  struct inode *ip , *ldip  ;
  char  string[MAX_PATH];
  ldip  = (struct inode *)last_dir( path , string);
  if ( ldip == NULL )
  {
     return NULL ;
  }
  ip = (struct inode*)advance( ldip , string );
  if ( ip == NULL && error_code == ENOENT )
  {
           if ( ( ip = alloc_inode( ldip->i_dev) ) == NULL  )
           {
               put_inode( ldip );
               return NULL ;
           }
           ip->i_nlinks ++         ;
           ip->i_mode    =   filetype  ;
           ip->i_zone[0] = zone_nr ;
           rw_inode( ip , WRITING );
           if ( (r=search_dir(ldip,string,&ip->i_num,ENTER)) != OK )
           {
                put_inode(ldip);
                ip->i_nlinks--;
                ip->i_dirty = DIRTY ;
                put_inode( ip );
                error_code  =  r;
                return NULL ;
           }
  }
  else
  {
      if ( ip != NULL )
      {
           r = EEXIST ;
      }
      else
      {
          r  = error_code ;
      }
  }
  put_inode( ldip );
  error_code  =  r;
  return ip ;
}

i32_t truncate( struct inode *ip )
{

}

i32_t forbidden( struct inode *ip , i32_t mask , i32_t real_uid  )
{
  return OK ;
}

