#include <type.h>
#include <ipc.h>
#include <com.h>
#include <erro.h>
#include <string.h>

#include "h/const.h"
#include "h/inode.h"
#include "h/superblock.h"
#include "h/buffer.h"
#include "h/com.h"
#include "h/dev.h"
#include "h/fproc.h"
#include "h/glo.h"

//这里的头文件其实有顺序问题
#include <stdio.h>
#include <ipc.h>

void  get_name(u8_t *, u8_t *, u8_t *);
struct inode *last_dir( char *, char*);
struct inode *advance(struct inode*, char*);
ino1_t look_up( struct buf *, char*);

//本文件使用变量
char oldpath[MAX_PATH_LENGTH],name[FILE_NAME_MAX];

struct inode*  eat_path(char *path )
{
    struct inode *ldip,*ip ;
    char *ch_ptr;
    u8_t finding = 1 ;    /* keep finding */
    memset(oldpath,0,sizeof(oldpath));
    strcpy(oldpath,path);
    memset(name , 0 , sizeof(name) );
    ldip = last_dir(oldpath,name);
    if ( ldip == NULL )
    {
        printf("the path wrong!\n");
        return ldip;
    }
    ip = ldip ;
    if ( *name != 0 )
    {
       ip  = advance(ldip , name );
    }
    put_inode( ldip );
    return ip; 
}

struct inode* last_dir(char *path, char *str)
{
    struct inode *rip ,*new_ip , *tmpip ;
    /*  current path directory */
    rip  = ( *path == '/' ) ? fp->fp_rootdir : fp->fp_workdir ;
    new_ip = rip  ;
    dup_inode( rip ) ;
    tmpip = rip ;
    while ( TRUE )
    {
       get_name( path , path , str );
       if ( *path == 0 )
       {
           break ;
       }
       new_ip = advance( tmpip , str );
       if ( new_ip == NULL )
       {
          break  ;
       }
       tmpip    = new_ip              ;
    }
    put_inode(rip);
    return  new_ip   ;
}

ino1_t look_up(struct buf* bp, char *fname)
{
    struct direct *dir_ptr;
    u8_t i; 
    for ( i = 0 ; i < DIRECT_PER_BLOCK ; i++  )
    {
        dir_ptr = & (bp->b.b_dir[i]);
        if ( !strcmp( fname , dir_ptr->d_name) )
        {
            return dir_ptr->d_ino;
        }
    }
    return 0; 
}

/* according the inode number to find the file named by  filename */
struct inode* advance(struct inode* ip,  char* filename)
{
    struct inode *inode_ptr;
    u32_t   zone, i, j, ii, jj, n, m;
    ino1_t  inum;
    struct buf *bp0, *bp1, *bp2;
    struct direct *dp;
    if ( ip->i_mode == NORMAL_FILE_TYPE )  /* not directory */
    {
        return 0;
    } 
    if ( ip->i_zone[9] != 0 ) 
    {
        bp0 = get_block(ip->i_dev, ip->i_zone[9]);
        for ( ii = 0 ; ii < 1024 ; ii ++ ) 
        {
            if ( bp0->b.b_ind[ii] != 0 )
            {
                bp1 =  get_block(ip->i_dev, bp0->b.b_ind[ii] );
                for ( jj = 0 ; jj < 1024 ; jj++ ) 
                {
                    if ( bp1->b.b_ind[jj] != 0 )
                    {
                        bp2 = get_block(ip->i_dev, bp1->b.b_ind[jj] );
                        for ( i = 0; i < 16 ; i++ )
                        {
                            dp = &(bp2->b.b_dir[i]);         
                            if ( !strcmp(dp->d_name, filename) )
                            {
                                inode_ptr = get_inode(ip->i_dev, dp->d_ino);
                                return inode_ptr ;
                            }
                        }
                        put_block(bp2, INODE_BLOCK);
                    }
                }
                put_block(bp1, INODE_BLOCK);
            }
        }
        put_block(bp0,INODE_BLOCK );
    }
    else if ( ip->i_zone[7] != 0 )
    {
        n = 1;
        if ( ip->i_zone[8] != 0 )
        {
            n = 2;
        }
        for ( ii = 0 ; ii < n ; ii++ )
        {
            bp0 = get_block(ip->i_dev, ip->i_zone[7+ii]);
            for( jj = 0 ; jj < 1024 ; jj++ )
            {
                if ( bp1->b.b_ind[jj] != 0 ) 
                {
                    bp1 = get_block(ip->i_dev, bp1->b.b_ind[jj]);
                    for ( i = 0 ; i < 16 ; i++ )
                    {
                        dp = &(bp1->b.b_dir[i]);
                        if ( !strcmp(dp->d_name, filename) )
                        { 
                            inode_ptr = get_inode(ip->i_dev, dp->d_ino);
                            return inode_ptr ;
                        }
                    }
                    put_block(bp0,INODE_BLOCK);
                }
            }
        }
    }
    else if ( ip->i_zone[0] != 0 )
    {
        for ( ii = 0 ; ii < 7 ; ii++ ) 
        {
            if ( ip->i_zone[ii] != 0 )
            {
                bp0 = get_block(ip->i_dev, ip->i_zone[ii]);
                for ( i = 0 ; i < 16 ; i++ ) 
                {
                    dp = &(bp0->b.b_dir[i]);
                    if ( !strcmp(dp->d_name, filename) )
                    {
                        inode_ptr = get_inode(ip->i_dev, dp->d_ino);
                        return inode_ptr ;
                    }
                }
                put_block(bp0, INODE_BLOCK);
            }
        }
    }
    else
    {
        return 0 ;
    }
    error_code  = ENOENT ;
    return 0;
}

/***************************************************************************
 * Description:
 *           eg:  oldpath- /usr/bin  newpath  /bin  filename usr 
 *           test: 0 , '///' , '//usr'
 * Author  : billy
 * Language: C
 ***************************************************************************/
void get_name( u8_t *oldpath , u8_t *newpath , u8_t *filename )
{
  u8_t *sptr , *dptr , *nptr ;
  sptr      = oldpath  ;
  if ( *sptr == 0 )
  {
     *filename = 0 ;
     *newpath = 0 ;
     return ;
  }
  for (;*sptr == '/'; sptr++ ) ;    // 去掉多余的'/'
  // 拷贝名字
  dptr      = filename ;
  for ( ; *sptr != 0 && *sptr != '/' ; )
  {
        *dptr++ = *sptr++ ;
  }
  *dptr = 0 ;
  dptr = newpath ;
  //没有结束
  if ( *sptr != 0 )
  {  
     sptr++ ;
  }
  for ( ; *sptr != 0 ; ) 
  {
     *dptr++ = *sptr++ ;
  }
  *dptr = 0 ;
  return ;
}

void pre_dir( char *path,  char *predir, char *filename )
{
    char *src, *dest;
    int  len , i ;
    if ( path == NULL || *path == '\0')
    {
         return ;
    }
    /* copy the string from src to dest */

    if ( path != predir )
    {
      src = path ;
      dest = predir;
      for ( ; *src  ; )
      {
        *dest++ = *src++;
      }
      *dest = 0;
    }
    /*  remove the the first */
    len = strlen(path);
    i = len ;
    src = predir + i - 1;
    if ( *src == '/' )
    {
        *src = 0 ; 
        src--;
    }
    for ( ; *src != '/' && src >= predir ;  src--);
    /* check if the pre directory is currentdir */
    src++;
    if ( filename != NULL )
    {
      for ( ;  *src ;  )
      {
        *filename++ = *src;
        *src++ = 0;
      }
      *filename = 0;      
    }
    *src  = 0 ;
    return ;
}

i32_t search_dir( struct inode *ldir_ptr , u8_t *str , i32_t *numb , i32_t flag )
{
    struct buf  *bp ;
    i32_t bits , pos ,b , old_slots, new_slots ,e_hit, r ;
    struct direct *dp ;
    //  如果不是目录则退出
    if ( ldir_ptr->i_mode != I_DIRECTORY )
    {
       return ENOTDIR ;
    }
    bits  = ( flag == LOOK_UP ? X_BIT : W_BIT|X_BIT );
    if ( (r=forbidden(ldir_ptr,bits,0))!=OK)
    {
        return r ;
    }
    // 目录的大小 当删除一个目录项时并不减小其大小 
    old_slots  = ldir_ptr->i_size / sizeof(struct direct) ;
    new_slots  = 0 ;
    e_hit      = FALSE  ;   
    for ( pos = 0 ; pos < ldir_ptr->i_size ; pos += BLOCK_SIZ )
    {
        b  = read_map( ldir_ptr, pos  );
        bp = get_block(ldir_ptr->i_dev, b);
        for ( dp = &bp->b.b_dir[0] ; dp < &bp->b.b_dir[NR_DIR_ENTERS];dp++)
        {
            if ( ++new_slots > old_slots )
            {
                   if ( flag == ENTER  )
                   {
                         e_hit = TRUE ;
                   }
                   break ;
            }
            if(flag!=ENTER&&dp->d_ino!=0&& !strcmp(dp->d_name,str))
            {
               if ( flag == DELETE )
               {
                  dp->d_ino  =   0 ;
                  bp->b_dirt = DIRTY ;
               }
               else
               {
                 *numb = dp->d_ino ;
               }
               put_block( bp , DIR_BLOCK);
               return OK ;
            }
            if ( flag == ENTER && dp->d_ino == 0 )
            {
               e_hit  = TRUE ;
               break ;
            }
        }
        if ( e_hit )
        {
            break ;
        }
        put_block( bp ,DIR_BLOCK );
    }
    if ( flag != ENTER )
    {
            return ENOENT ;
    }
    if ( e_hit == FALSE )
    {
          new_slots++ ;
          if ( (bp= (struct buf*)new_block( (struct inode *)ldir_ptr,ldir_ptr->i_size))==NULL)
          {
              return error_code ;
          }
          dp = &bp->b.b_dir[0] ;
    }
    copy( dp->d_name,str,strlen(str));
    dp->d_ino    =  *numb ;
    bp->b_dirt    =  DIRTY  ;
    put_block(bp,DIR_BLOCK);
    ldir_ptr->i_dirty  = DIRTY ;
    if ( new_slots > old_slots )
    {
          ldir_ptr->i_size  = new_slots * sizeof(struct direct) ;
    }
    return OK ;
}


