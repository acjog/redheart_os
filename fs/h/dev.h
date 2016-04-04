#ifndef __DEV_H_
#define __DEV_H_

#include <type.h>

extern   struct dmap{
     i32_t  (*dmap_open)();
     i32_t  (*dmap_rw)();
     i32_t  (*dmap_close)();
     i32_t  dmap_task;
}dmap[];


i32_t dev_open( dev1_t dev , i32_t flag );
i32_t dev_close( dev1_t dev ) ;
i32_t dev_io(i32_t rw_flag , dev1_t dev ,file_pos pos , i32_t nbytes , i32_t proc , u8_t *buf );

#endif
