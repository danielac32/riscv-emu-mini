 
#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <interrupt.h>
#include <kernel.h>
#define HEAP_ALIGN 4

// Linked list of free'd memory
static alloc_t *free_blocks;
static void *heap_end;


//struct  memblk  memlist;
void    *minheap;   /* Start address of heap    */
void    *maxheap;   /* End address of heap      */



void heap_init(void *buf)
{
    struct  memblk *memptr; /* Memory block pointer */

    /* Initialize the minheap and maxheap variables */

    minheap =buf;
    /* 1024 bytes is reserved for supervise mode handling */
    maxheap = (void *)MAXADDR;

    /* Initialize the memory list as one big block */

    memlist.mnext = (struct memblk *)minheap;
    memptr = memlist.mnext;

    memptr->mnext = (struct memblk *)NULL;
    memlist.mlength = memptr->mlength =
        (uint32)maxheap - (uint32)minheap;
     
}

uint32_t heap_free(void)
{
    
  struct  memblk  *memptr;  /* Ptr to memory block    */
  uint32  free_mem;   /* Total amount of free memory  */
  free_mem = 0;
  for (memptr = memlist.mnext; memptr != NULL; memptr = memptr->mnext) {
       free_mem += memptr->mlength;
  }
  return free_mem;
}

 
/*------------------------------------------------------------------------
 *  freemem  -  Free a memory block, returning the block to the free list
 *------------------------------------------------------------------------
 */
syscall freemem(
      char      *blkaddr,   /* Pointer to memory block  */
      uint32    nbytes      /* Size of block in bytes   */
    )
{
    intmask mask;           /* Saved interrupt mask     */
    struct  memblk  *next, *prev, *block;
    uint32  top;

    mask = disable();
    if ((nbytes == 0) || ((uint32) blkaddr < (uint32) minheap)
              || ((uint32) blkaddr > (uint32) maxheap)) {
        restore(mask);
        return SYSERR;
    }

    nbytes = (uint32) roundmb(nbytes);  /* Use memblk multiples */
    block = (struct memblk *)blkaddr;

    prev = &memlist;            /* Walk along free list */
    next = memlist.mnext;
    while ((next != NULL) && (next < block)) {
        prev = next;
        next = next->mnext;
    }

    if (prev == &memlist) {     /* Compute top of previous block*/
        top = (uint32) NULL;
    } else {
        top = (uint32) prev + prev->mlength;
    }

    /* Ensure new block does not overlap previous or next blocks    */

    if (((prev != &memlist) && (uint32) block < top)
        || ((next != NULL)  && (uint32) block+nbytes>(uint32)next)) {
        restore(mask);
        return SYSERR;
    }

    memlist.mlength += nbytes;

    /* Either coalesce with previous block or add to free list */

    if (top == (uint32) block) {    /* Coalesce with previous block */
        prev->mlength += nbytes;
        block = prev;
    } else {            /* Link into list as new node   */
        block->mnext = next;
        block->mlength = nbytes;
        prev->mnext = block;
    }

    /* Coalesce with next block if adjacent */

    if (((uint32) block + block->mlength) == (uint32) next) {
        block->mlength += next->mlength;
        block->mnext = next->mnext;
    }
    restore(mask);
    return OK;
}

/*------------------------------------------------------------------------
 *  getmem  -  Allocate heap storage, returning lowest word address
 *------------------------------------------------------------------------
 */
char    *getmem(
      uint32    nbytes      /* Size of memory requested */
    )
{
    intmask mask;           /* Saved interrupt mask     */
    struct  memblk  *prev, *curr, *leftover;

    mask = disable();
    if (nbytes == 0) {
        restore(mask);
        return (char *)SYSERR;
    }

    nbytes = (uint32) roundmb(nbytes);  /* Use memblk multiples */

    prev = &memlist;
    curr = memlist.mnext;
    while (curr != NULL) {          /* Search free list */

        if (curr->mlength == nbytes) {  /* Block is exact match */
            prev->mnext = curr->mnext;
            memlist.mlength -= nbytes;
            restore(mask);
            return (char *)(curr);

        } else if (curr->mlength > nbytes) { /* Split big block */
            leftover = (struct memblk *)((uint32) curr +
                    nbytes);
            prev->mnext = leftover;
            leftover->mnext = curr->mnext;
            leftover->mlength = curr->mlength - nbytes;
            memlist.mlength -= nbytes;
            restore(mask);
            return (char *)(curr);
        } else {            /* Move to next block   */
            prev = curr;
            curr = curr->mnext;
        }
    }
    restore(mask);
    return (char *)SYSERR;
}

/*------------------------------------------------------------------------
 *  getstk  -  Allocate stack memory, returning highest word address
 *------------------------------------------------------------------------
 */
char    *getstk(
      uint32    nbytes      /* Size of memory requested */
    )
{
    intmask mask;           /* Saved interrupt mask     */
    struct  memblk  *prev, *curr;   /* Walk through memory list */
    struct  memblk  *fits, *fitsprev; /* Record block that fits */

    mask = disable();
    if (nbytes == 0) {
        restore(mask);
        return (char *)SYSERR;
    }

    nbytes = (uint32) roundmb(nbytes);  /* Use mblock multiples */

    prev = &memlist;
    curr = memlist.mnext;
    fits = NULL;
    fitsprev = NULL;  /* Just to avoid a compiler warning */

    while (curr != NULL) {          /* Scan entire list */
        if (curr->mlength >= nbytes) {  /* Record block address */
            fits = curr;        /*   when request fits  */
            fitsprev = prev;
        }
        prev = curr;
        curr = curr->mnext;
    }

    if (fits == NULL) {         /* No block was found   */
        restore(mask);
        return (char *)SYSERR;
    }
    if (nbytes == fits->mlength) {      /* Block is exact match */
        fitsprev->mnext = fits->mnext;
    } else {                /* Remove top section   */
        fits->mlength -= nbytes;
        fits = (struct memblk *)((uint32)fits + fits->mlength);
    }
    memlist.mlength -= nbytes;
    restore(mask);
    return (char *)((uint32) fits + nbytes - sizeof(uint32));
}


void *malloc(uint32 nbytes)
{
struct memblk *pmem;

    /* we don't allocate 0 bytes. */
    if (0 == nbytes)
    {
        return NULL;
    }

    /* make room for accounting info */
    nbytes += sizeof(struct memblk);

    /* acquire memory from kernel */
    pmem = (struct memblk *)getmem(nbytes);
    if (SYSERR == (uint32)pmem)
    {
        return NULL;
    }

    /* set accounting info */
    pmem->mnext = pmem;
    pmem->mlength = nbytes;

    return (void *)(pmem + 1);  /* +1 to skip accounting info */


}

void free(void *pmem)
{
    struct memblk *block;

    /* block points at the memblock we want to free */
    block = (struct memblk *)pmem;

    /* back up to accounting information */
    block--;

    /* don't memfree if we fail basic checks */
    if (block->mnext != block)
    {

        return;
    }
    
    freemem((char *)block, block->mlength);
}

#if 0
void heap_init(void *buf)
{

    heap_end = buf;
    free_blocks = 0;
}

uint32_t heap_free(void)
{
    
    uint32_t total = 0;
    // Count free'd block sizes
    for (alloc_t *node = free_blocks; node != 0; node = node->next)
        total += node->size;
    // Add remaining free memory to that total
    return total + ((uint32_t)MAXADDR - (uint32_t)heap_end);
 
}

 
void *malloc(uint32 size)
{


#if 1
     uint32  mask = disable();
 if (size == 0){
     restore(mask);
     return 0;
 }

    // Round size to an aligned value
    size = (size + sizeof(alloc_t) + HEAP_ALIGN) & ~(HEAP_ALIGN - 1);

    // Begin searching through free'd blocks to see if we can reuse some memory.
    alloc_t *node = free_blocks;
    alloc_t *prev = 0;
    while (node != 0) {
        if (node->size >= size) {
            // If we can use this chunk, remove it from the free_blocks chain
            if (prev != 0)
                prev->next = node->next;
            else
                free_blocks = node->next;
            node->next = 0;

            // If this chunk is really big, give back the extra space
            if (node->size > size + 64) { // TODO why 64..?
                alloc_t *leftover = (alloc_t *)((uint32_t)node
                    + sizeof(alloc_t) + size);
                leftover->size = node->size - size - sizeof(alloc_t);
                leftover->next = 0;
                free((uint8_t *)leftover + sizeof(alloc_t));

                node->size = size;
                restore(mask);
                return (void *)((uint8_t *)node + sizeof(alloc_t));
            }
            restore(mask);
            return (void *)((uint8_t *)node + sizeof(alloc_t));
        }

        prev = node;
        node = node->next;
    }

    // No reusable chunks, take from the end of the heap
    node = (alloc_t *)heap_end;
    node->size = size;
    node->next = 0;

    heap_end = (void *)((uint8_t *)heap_end + size);
    restore(mask);
    return (void *)((uint8_t *)node + sizeof(alloc_t)); 
    #else
    struct memblk *pmem;

    /* we don't allocate 0 bytes. */
    if (0 == size)
    {
        return NULL;
    }

    /* make room for accounting info */
    size += sizeof(struct memblk);

    /* acquire memory from kernel */
    pmem = (struct memblk *)getmem(size);
    if (SYSERR == (uint32)pmem)
    {
        return NULL;
    }

    /* set accounting info */
    pmem->mnext = pmem;
    pmem->mlength = size;

    return (void *)(pmem + 1); 
    #endif
}

void free(void *buf)
{
    #if 1
     uint32 mask = disable();
     if (buf == 0){
         restore(mask);
         return;
     }

    // Get the alloc_t structure of this chunk
    alloc_t *alloc = (alloc_t *)((uint8_t *)buf - sizeof(alloc_t));
    if (alloc->next != 0){
        restore(mask);
        return;
    }

    // Search through the free_blocks list to see if this free chunk can merge
    // into an adjacent free chunk.
    int merged = 0;
    for (alloc_t *prev = 0, *node = free_blocks; node != 0; prev = node, node = node->next) {
        // If the node after the current one is ours
        if ((uint32_t)node + sizeof(alloc_t) + node->size == (uint32_t)alloc) {
            // Merge by adding our node's size to this one
            merged |= 1;
            node->size += sizeof(alloc_t) + alloc->size;
            break;
        }
        // Or, if this current node is the one after ours
        else if ((uint32_t)buf + alloc->size == (uint32_t)node) {
            // Merge the current node into ours
            merged |= 1;
            alloc->size += sizeof(alloc_t) + node->size;
            alloc->next = node->next;

            // Take the current node's place in the free_blocks chain
            if (prev != 0)
                prev->next = alloc;
            else
                free_blocks = alloc;
            break;
        }
    }

    // If we couldn't merge, simply append to the chain
    if (merged == 0) {
        alloc->next = free_blocks;
        free_blocks = alloc;
    }
    restore(mask);
    #else
    struct memblk *block;

    /* block points at the memblock we want to free */
    block = (struct memblk *)buf;

    /* back up to accounting information */
    block--;

    /* don't memfree if we fail basic checks */
    if (block->mnext != block)
    {

        return;
    }
    
    freemem((char *)block, block->mlength);

    #endif
}
#endif

void* realloc(void* ptr, size_t size)
{
    void* new_data = NULL;

    if(size)
    {
        if(!ptr)
        {
            return malloc(size);
        }

        new_data = malloc(size);
        if(new_data)
        {
            memcpy(new_data, ptr, size); // TODO: unsafe copy...
            free(ptr); // we always move the data. free.
        }
    }

    return new_data;
}

void            *calloc(size_t nmemb, size_t size)
{
  unsigned int      s;
  char          *ptr;

  s = nmemb * size;
  if ((ptr = malloc(s)) == NULL)
    return (NULL);
  memset(ptr, 0, s);
  return (ptr);
}

 