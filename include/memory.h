/* memory.h - roundmb, truncmb, freestk */
#include <kernel.h>
#include <stdint.h>
typedef struct {
	uint32_t size;
	void *next;
} __attribute__ ((packed)) alloc_t;

void heap_init(void *buf);
uint32_t heap_free(void);
 
/* Added by linker */
extern unsigned int __stack_size;
extern	unsigned char	_text;			/* Start of text segment	*/
extern	unsigned char	_etext;			/* End of text segment		*/
extern	unsigned char	_sdata;			/* Start of data segment	*/
extern	unsigned char	_edata;			/* End of data segment		*/
extern	unsigned char	_sbss;			/* Start of bss segment		*/
extern	unsigned char	_ebss;			/* End of bss segment		*/
extern	unsigned int	_end;			/* End of program		*/
#define MAXADDR 0x8001d4c0//0x80000000 + 128000//0x8001e460//0x80055730//0x20055730 //350k
//extern	unsigned int	MAXADDR;
#define ALIGN(to, x)  ((x+(to)-1) & ~((to)-1))

 
#define	roundmb(x)	(char *)( (7 + (uint32)(x)) & (~7) )
#define	truncmb(x)	(char *)( ((uint32)(x)) & (~7) )

/*----------------------------------------------------------------------
 *  freestk  --  Free stack memory allocated by getstk
 *----------------------------------------------------------------------
 */
#define	freestk(p,len)	freemem((char *)((uint32)(p)		\
				- ((uint32)roundmb(len))	\
				+ (uint32)sizeof(uint32)),	\
				(uint32)roundmb(len) )

struct	memblk	{			/* See roundmb & truncmb	*/
	struct	memblk	*mnext;		/* Ptr to next free memory blk	*/
	uint32	mlength;		/* Size of blk (includes memblk)*/
	};
extern	struct	memblk	memlist;	/* Head of free memory list	*/


extern	void	*minheap;		/* Start of heap		*/
extern	void	*maxheap;		/* Highest valid heap address	*/
