#ifndef  _IPC_H_
#define  _IPC_H_

#ifdef OFFSET
#include  "type.h"
#else
#include <type.h>
#endif

#ifndef _TABLE_H_
#define  EXTERN  extern
#else
#define EXTERN 
#endif

#define M_STRING  14

typedef	struct {
i32_t	m1i1,m1i2,m1i3;
u8_t	*m1p1,*m1p2,*m1p3;	
	}mess_1;

typedef	struct {
i32_t	m2i1,m2i2,m2i3;
long    m2l1,m2l2;
u8_t	*m2p1;	
	}mess_2;
	
typedef	struct {
 i32_t  m3i1 , m3i2 ;
 u8_t *m3p1 ;
 char m3cal[M_STRING]; 
}mess_3;

typedef  struct {
long m4l1 , m4l2 , m4l3 , m4l4;
}mess_4;

typedef struct {
 u8_t  m5c1, m5c2 ;
 i32_t m5i1 , m5i2 ;
 long m5l1 , m5l2 , m5l3; 
}mess_5;

typedef struct {
 i32_t m6i1 , m6i2 , m6i3 ;
 long  m6l1;
 i32_t (*m6f1)() ; 
}mess_6;

typedef	struct{
i32_t	m_source;
i32_t	m_type;
union{
	mess_1	m_m1;
	mess_2	m_m2;
	mess_3	m_m3;
	mess_4  m_m4;
	mess_5  m_m5;
	mess_6  m_m6;
	}m_u;	
	}message;

// 以下定义了一些有用的简写
#define  m1_i1  m_u.m_m1.m1i1 
#define  m1_i2  m_u.m_m1.m1i2
#define  m1_i3  m_u.m_m1.m1i3 
#define  m1_p1  m_u.m_m1.m1p1 
#define  m1_p2  m_u.m_m1.m1p2
#define  m1_p3  m_u.m_m1.m1p3

#define  m2_i1  m_u.m_m2.m2i1
#define  m2_i2  m_u.m_m2.m2i2
#define  m2_i3  m_u.m_m2.m2i3
#define  m2_l1  m_u.m_m2.m2l1
#define  m2_l2  m_u.m_m2.m2l2
#define  m2_p1  m_u.m_m2.m2p1

#define  m3_i1  m_u.m_m3.m3i1
#define  m3_i2  m_u.m_m3.m3i2
#define  m3_p1  m_u.m_m3.m3p1
#define  m3_cal m_u.m_m3.m3cal

#define  m4_l1  m_u.m_m4.m4l1
#define  m4_l2  m_u.m_m4.m4l2
#define  m4_l3  m_u.m_m4.m4l3

#define  m5_c1  m_u.m_m5.m5c1
#define  m5_c2  m_u.m_m5.m5c2
#define  m5_i1  m_u.m_m5.m5i1
#define  m5_i2  m_u.m_m5.m5i2
#define  m5_l1  m_u.m_m5.m5l1
#define  m5_l2  m_u.m_m5.m5l2
#define  m5_l3  m_u.m_m5.m5l3

#define  m6_i1  m_u.m_m6.m6i1
#define  m6_i2  m_u.m_m6.m6i2
#define  m6_i3  m_u.m_m6.m6i3
#define  m6_l1  m_u.m_m6.m6l1
#define  m6_f1  m_u.m_m6.m6f1

//异步消息机制
typedef struct asyn_mess {
    message   m ;
    struct  asyn_mess  *next ;
}asyn_mess;

#define KERNEL_MESSAGE   100 
#define ASYN_TO_MM       0
#define ASYN_TO_FS       1
//异步消息
EXTERN  asyn_mess        syscall_mess[KERNEL_MESSAGE] ;
EXTERN  asyn_mess        *fs_hptr                     ;
EXTERN  asyn_mess        *mm_hptr                     ;
EXTERN  asyn_mess        *fs_tptr                     ;
EXTERN  asyn_mess        *mm_tptr                     ;
EXTERN  asyn_mess        *asyn_front                  ; 
void        asyn_send( i32_t flag ,asyn_mess *m_ptr ) ;
void        inform()                                  ;
asyn_mess*  get_msg_buf()                             ;

#endif
