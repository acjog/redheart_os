#ifndef __COM_H__
#define __COM_H__
//#include <ipc.h>

//#undef  REP_PROC_NR 
//#define REP_PROC_NR       m2_i1
//#define REP_STATUS        m2_i3
// 这里的参数都是统一的 因为软盘
// Mem和tty都是这样
#define DEVICE            m2_i1
#define PROC_NR	          m2_i2
#define	COUNT	          m2_i3
#define	POS		  m2_l1
#define	ADDR	          m2_p1


#endif 
