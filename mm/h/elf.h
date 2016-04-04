#ifndef __ELF_H__
#define __ELF_H__
#include<type.h>
/* macro*/
#define EI_MAG0		0		/* File identification byte 0 index */
#define ELFMAG0		0x7f		/* Magic number byte 0 */

#define EI_MAG1		1		/* File identification byte 1 index */
#define ELFMAG1		'E'		/* Magic number byte 1 */

#define EI_MAG2		2		/* File identification byte 2 index */
#define ELFMAG2		'L'		/* Magic number byte 2 */

#define EI_MAG3		3		/* File identification byte 3 index */
#define ELFMAG3		'F'		/* Magic number byte 3 */

#define ELF_MAGIC  0x464c457f
/* The ELF file header.  This appears at the start of every ELF file.  */

#define EI_NIDENT  16

//这里直接拷贝/usr/include/elf.h部分内容
/* Type for a 16-bit quantity.  */
typedef u16_t Elf32_Half;
typedef u16_t Elf64_Half;

/* Types for signed and unsigned 32-bit quantities.  */
typedef u32_t Elf32_Word;
typedef	i32_t  Elf32_Sword;
typedef u32_t Elf64_Word;
typedef	i32_t  Elf64_Sword;

/* Types for signed and unsigned 64-bit quantities.  */
typedef u64_t Elf32_Xword;
typedef	i64_t  Elf32_Sxword;
typedef u64_t Elf64_Xword;
typedef	i64_t  Elf64_Sxword;

/* Type of addresses.  */
typedef u32_t Elf32_Addr;
typedef u64_t Elf64_Addr;

/* Type of file offsets.  */
typedef u32_t Elf32_Off;
typedef u64_t Elf64_Off;

/* Type for section indices, which are 16-bit quantities.  */
typedef u16_t Elf32_Section;
typedef u16_t Elf64_Section;

/* Type for version symbol information.  */
typedef Elf32_Half Elf32_Versym;
typedef Elf64_Half Elf64_Versym;


/* Program segment header.  */

typedef struct
{
  Elf32_Word	p_type;			/* Segment type */
  Elf32_Off	    p_offset;		/* Segment file offset */
  Elf32_Addr	p_vaddr;		/* Segment virtual address */
  Elf32_Addr	p_paddr;		/* Segment physical address */
  Elf32_Word	p_filesz;		/* Segment size in file */
  Elf32_Word	p_memsz;		/* Segment size in memory */
  Elf32_Word	p_flags;		/* Segment flags */
  Elf32_Word	p_align;		/* Segment alignment */
} Elf32_Phdr;

typedef struct
{
  unsigned char	e_ident[EI_NIDENT];	/* Magic number and other info */
  Elf32_Half	e_type;			/* Object file type */
  Elf32_Half	e_machine;		/* Architecture */
  Elf32_Word	e_version;		/* Object file version */
  Elf32_Addr	e_entry;		/* Entry point virtual address */
  Elf32_Off	    e_phoff;		/* Program header table file offset */
  Elf32_Off	    e_shoff;		/* Section header table file offset */
  Elf32_Word	e_flags;		/* Processor-specific flags */
  Elf32_Half	e_ehsize;		/* ELF header size in bytes */
  Elf32_Half	e_phentsize;	/* Program header table entry size */
  Elf32_Half	e_phnum;		/* Program header table entry count */
  Elf32_Half	e_shentsize;		/* Section header table entry size */
  Elf32_Half	e_shnum;		/* Section header table entry count */
  Elf32_Half	e_shstrndx;		/* Section header string table index */
} Elf32_Ehdr;


#endif
