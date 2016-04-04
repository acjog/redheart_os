#include "h/mm.h"

void*  get_vm_struct();
void   put_vm_struct( struct vm_area_struct *ptr  );


void*  get_vm_struct()
{
   struct vm_area_struct  *tmp ;
   if ( vm_front != NULL )
   {
       tmp =   vm_front ;
       vm_front  = vm_front->vm_next ;
   }
   return tmp ;
}

void  put_vm_struct( struct vm_area_struct *ptr  )
{
    ptr->vm_next = vm_front ;
    vm_front = ptr ;
}
