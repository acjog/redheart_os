#ifndef __USER_H__
#define __USER_H__

#define STDIN   0
#define STDOUT  1
#define STDERR  2

i32_t fork();
i32_t exec( u8_t*,u8_t**);
i32_t open( u8_t *, i32_t);
i32_t read( i32_t fd , u8_t *buf, i32_t bytes  );
i32_t seek( i32_t fd , u8_t *buf , i32_t flag  );
i32_t mknode( u8_t *path  , i32_t mode ,i32_t mask , i32_t zone);
i32_t dup( i32_t fd , i32_t fd1);
i32_t exit(i32_t flag) ;

i32_t  mm_exec( i32_t task , u8_t *argv[]  );

#endif 
