#ifndef __FS_PARAM_H__
#define __FS_PARAM_H__ 

//这里添加系统调用的参数

#define  FD     m.m1_i1
#define  BUFFER m.m1_p1
#define  NBYTES m.m1_i2

#define  M3           1
#define  M2           2
#define  M1           3
#define  NAME         m.m1_p1
#define  NAME_LENGTH  m.m1_i1
#define  RWX_BITS     m.m1_i2
#define  PATHNAME     m.m1

#endif
