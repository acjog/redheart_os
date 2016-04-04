#ifndef _TYPE_H_
#define _TYPE_H_
typedef unsigned int    u32_t    ; 
typedef unsigned short  u16_t    ;
typedef unsigned char   u8_t     ;
typedef struct { u32_t a, b; }      u64_t;

typedef signed   int      i32_t  ;
typedef signed   short    i16_t  ;
typedef signed   char     i8_t   ;
typedef struct { i32_t  a, b;}      i64_t;

typedef unsigned int    ino1_t    ;
typedef unsigned int    off1_t    ; 
typedef unsigned int    mask_bits ;
typedef unsigned short  dev1_t    ;
typedef unsigned short  zone1_t   ;
typedef unsigned int    bit_t     ;
typedef unsigned int    uid       ;
typedef unsigned int    pid       ;
typedef unsigned int    gid       ;
typedef unsigned int    block1_t  ;
typedef unsigned char   bit_chunk ;
typedef	unsigned int    clock_t   ;
typedef	unsigned int    time_t    ;
typedef int	        realtime  ;
typedef int             file_pos  ;
#endif
