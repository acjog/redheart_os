#include<const.h>
#include<type.h>
#include<ipc.h>
#include<com.h>
#include<callnr.h>
#include<sys.h>

#include  "h/const.h"
#include "h/mproc.h"
#include "h/glo.h"

i32_t  do_exit()
{
    i32_t  i, n ;
    struct vm_area_struct *pvm , *tmp ;
     //printf( "exit..  ");
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
    n = i ;
    sys_freemem( who ,  (u8_t*)pghead , n*sizeof(Elf32_Phdr) );
    mproc[who].mp_flag  = 0 ;
    procs_in_use--  ;
    tell_fs(EXIT,0,who);
    sys_exit(who,0) ;
    dont_reply  = TRUE ;
	return 0 ;
}
