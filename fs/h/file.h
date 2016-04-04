#ifndef __FILE_H__
#define __FILE_H__

EXTERN  struct filp{
    mask_bits       filp_mode ;
	i32_t           filp_count ;
	struct  inode  *filp_ino ;
	file_pos        file_pos ;
}filp[NR_FILPS];

i32_t   get_fd( i32_t mask , i32_t *k , struct filp **fpt );
struct  filp *get_filp(    i32_t fd           )      ;
struct  filp *find_filp( struct inode *rip , i32_t bits );


#endif
