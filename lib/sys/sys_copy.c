#include<type.h>
#include<ipc.h>
#include<com.h>
#include<sys.h>

i32_t sys_copy( message *m_ptr )
{
  sendrec(SYS_TASK , m_ptr );
  return m_ptr->m_type ;
}
