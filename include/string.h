#ifndef __STRING_H__
#define __STRING_H__
// 库声明
// 使用这个库时 要注意ds=es=数据空间
#ifndef NULL
#define  NULL (void*)0
#endif

extern char* strcpy(char *dest , const char *src );
extern char * strncpy(char *dest , const char *src , int count );
extern char* strcat(char *dest, const  char *src  );
extern char * strncat(char *dest , const char *src , int count );
extern int strcmp(const  char *cs , const char *ct);
extern int strncmp(const  char * cs , const char * ct , int count );
extern int strlen(const char *s );
extern void *memcpy(void *dest , const void *src , int n );
extern void *memset(void *s , char c , int count );
extern int  is_lower( char  ch );
extern int  is_uper( char  ch );


#endif 
