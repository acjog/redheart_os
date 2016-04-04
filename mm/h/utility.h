#ifndef  __UTILITY_H__
#define  __UTILITY_H__

extern i32_t tell_fs( i32_t call, pid parent , pid child );
extern void reply( i32_t whom , i32_t result );
extern void panic( u8_t *str );
extern i32_t  rw_user( u8_t *bp , i32_t s , u8_t  *sbuf ,  i32_t count , i32_t direction   );


#endif
