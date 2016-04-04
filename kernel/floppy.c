#include <glo_fun.h>
#include <const.h>
#include <glo_var.h>
#include <com.h>

#define SECT_PER_CYL     18
//0x12 
#define HEADS            2
#define RESULTS          8
#define FLOPPY_NUM       2
#define MAX_ERRORS       10 

#define DCR         0x3F7
#define DOR         0x3F2 
#define FDC_STATUS  0x3F4 
#define FDC_DATA    0x3F5

#define DMA_ADDR    0x4
#define DMA_COUNT   0x5

#define DMA_READ    0x46
#define DMA_WRITE   0x4A

#define DMA_M1      0xB
#define DMA_M2      0xC
#define DMA_INT     0xA

#define MASTER        0x80
#define DIRECTION     0x40
#define CTL_BUSY      0x10 
#define CTL_READY     0x10
#define CTL_ACCEPTING 0x80

#define FDC_SENSE     0x8
#define FDC_SEEK      0xF
#define FDC_READ      0xE6
#define FDC_WRITE     0xC5
#define FDC_RECALIBRATE  0x7
#define FDC_SPECIFY   0x3
#define SPEC1         0xCF 
#define SPEC2         0x6

#define MOTOR_OFF     180
#define MOTOR_START   30

#define  GAP          0x1B
#define  DTL          0xFF

#define ENABLE_INT    0xC

#define  UNCALIBRATED  0
#define  CALIBRATED    1

#define  CHANGE       0xC0 

#define MOTOR_RUNNING 0xFF 

struct floppy {
     i32_t  fl_opcode ;
     i32_t  fl_curcyl ;
     i32_t  fl_pronr ;
     i32_t  fl_drive ;
     i32_t  fl_cylinder ;
     i32_t  fl_sector ;
     i32_t  fl_head ;
     i32_t  fl_count ;
     void   *fl_address ;
     i32_t  fl_results[RESULTS];
     i32_t  fl_calibrate ;
}floppy[FLOPPY_NUM] ;

u8_t*  floppy_driver_buf ;

void init_floppy();
i32_t  do_rdwt( message * ) ;
void   reset() ;
void   fdc_out ( i32_t ) ;
i32_t  fdc_results( struct floppy *);
void   dma_setup(struct floppy * );
i32_t  start_motor( struct floppy *);
i32_t  seek( struct floppy *);
i32_t  transfer( struct floppy *);
i32_t  recalibrate( struct floppy * ) ;
i32_t  stop_motor();
i32_t  send_mess() ;
void   clock_mess( i32_t , i32_t  (*fun)()) ;
static message floppy_mess;

i32_t  need_reset   ;   // 指明是否需要重置
i32_t  motor_status ;   // 表示马达的状态
i32_t  motor_goal   ;   // 对马达的操作
i32_t  device       ;   // 软盘设备号
i32_t  start_motor_flag   ;   //马达是否开启 开启置1关闭时置0 

i32_t  opcode ,reply;

void floppy_task()
{
	i32_t    r  ,  caller ;
	init_floppy();	
    printf(" floppy_task " );
    // 这里不能跨越64k的边界
    floppy_driver_buf  = 0x4000 ;
	while ( TRUE )
	{
	   receve(ANY, &floppy_mess);	
	   caller = floppy_mess.m_source ;
	   opcode  = floppy_mess.m_type ;
       reply   = FALSE  ;
   	   switch ( opcode     )
	   {
		case DISK_READ :
			{
				r  = do_rdwt ( &floppy_mess) ;
				break ;
			}
		case DISK_WRITE :
			{
				r  = do_rdwt ( &floppy_mess ) ;
				break ;
			}
		default :
			{
				printf(" haha I am coming ~ %d", caller);
				break ;
			}
	   }
       if ( reply  )
       {
	     floppy_mess.m_type   = TASK_REPLY ;
         send( caller , &floppy_mess);	   
       }
	}
	return ;
}


i32_t do_rdwt ( message *m_ptr ) 
{
   i32_t   sector , errors  , r;
   struct floppy *fp ;
   device  = m_ptr->FL_DEVICE ;
   reply   = TRUE  ;
   if ( device < 0 || device > FLOPPY_NUM ) 
   {
	   return 0 ;
   }
   //  计算参数
   //  这里的扇区计算公式 要好好看清 注意sector的值是从0开始算起
   //  计算才不会有问题的
   sector           = m_ptr->FL_SECTOR ;
   fp = &floppy[device];
   fp->fl_opcode = ( (opcode ==DISK_READ )?DISK_READ:DISK_WRITE);
   // 这里没有对地址进行转化
   fp->fl_address   =   m_ptr->FL_ADDR ;
   fp->fl_pronr     =   m_ptr->FL_PRONR ;
   fp->fl_drive     =   device ;
   fp->fl_cylinder  =   sector / ( HEADS * SECT_PER_CYL ) ; 
   sector           =   sector % (HEADS * SECT_PER_CYL );
   fp->fl_head      =   sector / SECT_PER_CYL ;
   fp->fl_sector    =   sector % SECT_PER_CYL ;
   fp->fl_sector++ ;
   errors = 0 ;
   //  连续尝试10次
   while ( errors < MAX_ERRORS ) 
   {
     errors ++ ;
     // 是否需要重置
     if ( need_reset )
     {
         reset();
     }
     // 开启马达
     start_motor(fp);
     // 设置dma芯片
     dma_setup(fp);
     // 寻址
     r = seek( fp );
     if ( 0 == r )
     {
//       printf ( " billy " ) ;
     }
     else
     {
       printf ( " min " ) ;
     }
     if ( r != OK )
     {
	     printf( " floppy continue " );
	     continue ;
     }
     //  传送
     r  = transfer(fp);
     if ( r == OK )
     {
//	     printf( " command success " ) ;
	     break ;
     }
     printf( " transfer wrong " );
     return -1;
   }
//   printf( " error:%d , need_reset:%d " ,errors, need_reset );
   motor_goal  = 0xc ;
   // 定时关闭马达
   clock_mess( MOTOR_OFF, stop_motor);
   return 0 ;
}

i32_t transfer( struct floppy *fp )
{
  i32_t op , r , i ;
  if  ( fp->fl_calibrate == UNCALIBRATED )
  {
	 printf( " transfer calibrate " ) ;
	 return  -1  ;
  }
  // 命令 这里的值 是我观察bochs的io调试端口发现的
  // 我也没有去查找资料就直接抄了 
  op = (  (opcode==DISK_READ) ?FDC_READ :FDC_WRITE) ;
  fdc_out ( op )                                ;
  fdc_out ( (fp->fl_head << 2 ) | fp->fl_drive) ;
  fdc_out ( fp->fl_cylinder )                   ;
  fdc_out ( fp->fl_head )                       ;
  fdc_out ( fp->fl_sector)                      ;
  fdc_out ( 0x2  ) ;
  fdc_out ( SECT_PER_CYL             )                   ; //扇区大小
  fdc_out ( GAP )                               ;
  fdc_out ( DTL )                               ;
  //printf ( "device:%d,address:%d,cylinder:%d,head:%d,\
sector:%d ",fp->fl_drive,fp->fl_address,fp->fl_cylinder,fp->fl_head\
	     ,fp->fl_sector ) ;
  //  每次读完要求重置 
  fp->fl_calibrate = UNCALIBRATED ;
  if ( need_reset ) 
  {
	  printf ( " transfer need_reset " ) ;
	  return  -1 ;
  }
  unlock();
  receve( HARDWARE  , &floppy_mess)             ;
  io_delay();
  io_delay();
 // fdc_out( FDC_SENSE ) ;  //检测中断状态
  r = fdc_results( fp )                         ; 
  if ( r <  0 ) 
  {
	 return r                               ;
  }
  for ( i = 0 ; i < r  ; i++)
  {
//    printf( " %d ", fp->fl_results[i]  );
  }
  //  还有一些对结果的值进行判断 看是否运行正确
  //  将结果复制到指定位置
  if ( ((u32_t)(fp->fl_address) + 0x1000 > 0x100000) && fp->fl_opcode == DISK_READ )
  {
    MemCpy(floppy_driver_buf , fp->fl_address , 0x1000 );
  }
  return 0 ;
}

i32_t seek( struct floppy *fp )
{
  i32_t r ;
  if ( fp->fl_calibrate == UNCALIBRATED )
  {
      if ( ( r = recalibrate(fp) ) < 0  )
      {
	      return -1 ;
      }
      // 这里没有利用电梯算法 直接每次都重置
  }
  if ( fp->fl_curcyl == fp->fl_cylinder )
  {
	  return  0;
  }
  fdc_out( FDC_SEEK );
  fdc_out( (fp->fl_head << 2 |  fp->fl_drive));
  fdc_out( fp->fl_cylinder ) ;
  if ( need_reset )
  {
	  printf( " seek wrong " );
	  return  -1 ;
  }
  receve( HARDWARE , &floppy_mess );
  fdc_out( FDC_SENSE );
  r = fdc_results( fp );
  if ( r < 0  )
  {
	  if ( recalibrate ( fp ) != 0  )
	  {
		  return -1 ;
	  }
  }
 // printf( " seek success " );
  return 0 ;
}

i32_t  fdc_results( struct floppy *fp )
{
  i32_t i = 0  , j , m  ; 
  i32_t  status  ;
  for ( m = 0 ; m < 10000 ; m++) 
  {
   	 status = port_in(FDC_STATUS);     //读状态寄存器
	 status = status & ( MASTER | DIRECTION | CTL_BUSY ) ;
	 if ( MASTER  == status ) 
	 {
		return i ;
	 }
	 if ( ( MASTER | DIRECTION | CTL_BUSY ) == status ) 
	 {
	  //  结果出错
	   if ( i > 7  ) 
	   {
		printf( " wrong  i :%d" ,i  );
                break ;
	   }	   
	   status = port_in( FDC_DATA  ) ;
	   fp->fl_results[i]  = status  ;
	   i ++ ;
	   for ( j = 0 ; j < 10 ; j ++ ) ;
	 }
  }
  #ifdef _KERNEL_DEBUG
  printf(" need reset %d ", m);
  #endif
  need_reset = 1 ;
  return -1 ;
}

i32_t  stop_motor()
{
        if ( !start_motor_flag )
	{
             return 0;
	}
	port_out( DOR , motor_goal) ;
	motor_status = motor_goal ;
	start_motor_flag  = 0 ;
//	printf( " stop_motor " );
}

i32_t  start_motor( struct floppy * fp )
{
 i32_t motor_bit , m , running ;
// if ( start_motor_flag )
// {
//   return 0 ;
// }
 setlock();
 // 这里表示DOR字节高四位选择电机
 motor_bit =  1 << ( fp->fl_drive + 4 ) ;
 motor_goal =  motor_bit | ENABLE_INT | fp->fl_drive  ;
 port_out( DOR , motor_goal);
 for ( m = 1000 ; m > 0 ; m -- ) ;
 unlock();
 clock_mess( MOTOR_START , send_mess);
 //printf( " start motor %d ", motor_goal );
 receve( CLOCK_TASK , &floppy_mess);
 // printf( "start motor  ");
 // 置马达开启标志位1 
 start_motor_flag   =  1 ;
 for  ( m = 0 ; m <= 3 ; m ++ ) 
 {
   fp->fl_calibrate  = UNCALIBRATED ;
   fp->fl_curcyl     = -1 ;
 }
// printf( " floppy receve form clock_task:%d ",need_reset  );
}


void clock_mess( i32_t ticks , i32_t (*fun)() )
{
  floppy_mess.m_type        = SET_ALARM ;
  floppy_mess.CLOCK_PROC_NR = FLOPPY_TASK ;
  floppy_mess.DELTA_TICKS  = ticks ;
  floppy_mess.FUNC_TO_CALL = fun ;
 // printf( "clock_mess " );
  sendrec( CLOCK_TASK  , &floppy_mess ) ;
  //printf( "rc" );
}

i32_t  send_mess()
{
  floppy_mess.m_type  = MOTOR_RUNNING ;
  send( FLOPPY_TASK , &floppy_mess);
  //printf( "send to floppy ");
}

i32_t  recalibrate( struct floppy *fp )
{
  i32_t r , i ;
  if ( 0 == start_motor )
  {
 	 start_motor(fp);
  }
  fdc_out( FDC_RECALIBRATE);
  //printf( " need_reset:%d ",need_reset);
  fdc_out( fp->fl_drive );
 // printf( " need_reset:%d ",need_reset);
  if ( need_reset )
  {
	  printf ( " recalibrate wrong:%d ",need_reset ) ;
	  return   -1  ;
  }
  receve( HARDWARE , &floppy_mess );
  fdc_out( FDC_SENSE );
  r = fdc_results(fp);
  //对一些特征值判断
  // 这里的值参考linux完全注释0.11p393
  if ( r < 0  || fp->fl_results[0]&0x10 == 0x10
              || fp->fl_results[0] &0xc0 == 0x80 )
  {
     printf( " calibrate wrong " );
     need_reset = 1 ; //要复位
     fp->fl_curcyl = -1 ;
  }
  else
  {
     fp->fl_calibrate = CALIBRATED ;
     fp->fl_curcyl    = 0 ;
  //   printf( "calibrate :" );
  //   for ( i = 0 ; i < r   ; i ++  ) 
 //    {
//	printf( " %d " , fp->fl_results[i] );
  //   }
     return 0 ;
  }
}

void dma_setup( struct floppy *fp )
{
  i32_t mode ,user_addr;
  i32_t  low_addr , high_addr , top_addr ;
  //  这里还只是一个内核空间的代码段 并没有进行分离
  user_addr  = (i32_t) fp->fl_address ;
  //读两个扇区是否超过1M的地址处
  //因为DMA要求必须在1M以内 寻址
 mode = (( opcode ==DISK_READ )?DMA_READ : DMA_WRITE );
  if ( user_addr+0x1000 > 0x100000 ) 
  {
        if ( DMA_WRITE == mode ) 
	{
           MemCpy((void*)user_addr , floppy_driver_buf , 0x1000 ) ;
	}
	user_addr = ( i32_t )floppy_driver_buf  ;
  }
 // printf("the address is %d",user_addr);
  low_addr   =  (i32_t) (user_addr >> 0 ) & 0xFF ; 
  high_addr  =  (i32_t) (user_addr >> 8 ) & 0xFF ;
  top_addr   =  (i32_t) (user_addr >> 16) & 0xF  ;
  setlock() ;
  //  0x10 单通道屏蔽寄存器
  //  用3位 ： 0~1位表示DMA通道选择 位2表示屏蔽
  port_out (DMA_INT , 4|2 ) ;
  io_delay();
//  port_out (DMA_M2 , 0    ) ;
  port_out( DMA_ADDR , low_addr) ;
  io_delay();
  port_out( DMA_ADDR , high_addr);
  io_delay();
  //port_out( DMA_M2 , 0 ) ;
  port_out( DMA_COUNT , 0xff )  ;
  io_delay();
  port_out( DMA_COUNT , 0xf ) ;  //每次传送8个扇区
  io_delay();
  port_out( DMA_M2 , mode ) ;
  io_delay();
  port_out( DMA_M1 , mode ) ;
  io_delay();
  // DMA支持20位地址 高四位写入0x81端口的页面寄存器
  port_out( 0x81 ,     top_addr ) ;
  io_delay();
  //  恢复屏蔽寄存器
  port_out( DMA_INT ,0|2 );
  io_delay();
  restore();
//  printf( " set dma success " );
}

#define MAX_FDC_RETRY  10000

void fdc_out( i32_t cmd ) 
{
  i32_t retries = MAX_FDC_RETRY , m , r ;
  if ( need_reset ) 
  {
	  return ;
  }
  while ( retries-- > 0 ) 
  {
     r = port_in(FDC_STATUS) ;  // 获取软盘的状态
     r &= ( MASTER | DIRECTION ) ;
     if (r != CTL_ACCEPTING  )
	    continue ;
//    printf( " ready ... " ); 
    port_out(FDC_DATA , cmd );
    for ( m = 10 ; m > 0 ; m -- ) ;
    return ;
  }
  printf( " fdc_out %d wrong ", cmd  );
  need_reset = 1 ;
}
void reset()
{
  i32_t  r , i , status  ; 
  struct floppy *fp ;
  need_reset = 0 ;
  setlock();
  port_out(DOR , 0x0 );
  io_delay() ;
  port_out(DOR , 0xC ) ;    //  启动软驱A和准许DMA
  io_delay();
  unlock();
  receve( HARDWARE , &floppy_mess ) ;
  printf( "harware floppy ");
  fp = &floppy[device] ;    // 这里默认指
  fp->fl_results[0] = 0 ;
  fdc_out( FDC_SENSE ) ;  //检测中断状态
  for ( i = 10 ; i > 0 ; i-- ) ; //少量延时
  r = fdc_results( fp );
  if (  r < 0    )
  {
	printf( " FDC won't reset " );
	return ;
  }
  status = fp->fl_results[0] & 0xFF ;
  if ( CHANGE != status )
  {
	  printf(" status not change ");
	  return ;
  }
  printf( " %d reset result:%d, %d",r ,fp->fl_results[0],fp->fl_results[1]);
  //  重置成功  设置FDC参数
  fdc_out( FDC_SPECIFY ) ; 
  fdc_out( SPEC1       ) ;
  fdc_out( SPEC2       ) ;
  port_out(DCR  , 1    ) ;  //数据传送速率  300kb/s
  io_delay();
  printf( " reset success:%d ",need_reset );
  return ;
}


void init_floppy()
{
 i32_t  i ;
 device = 0 ;  //表示第一台的软盘驱动器
 need_reset = 1; 
 opcode = -1 ;
 for ( i = 3 ; i >= 0 ; i -- ) 
 {
    floppy[i].fl_curcyl = - 1 ; 
 }
 set_vec(0x26, (u32_t*)floppyhandler,0,0);
 enable_int(6);
 return ;
}
