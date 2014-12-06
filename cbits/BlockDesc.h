#ifndef CBITS_BLOCKDESC_H
#define CBITS_BLOCKDESC_H

#include <stdlib.h>

#define ALIGN_PADDING(size, align) (((align) - (size)) & ((align) - 1))

/*
 * A BlockDesc (Block Descriptor) is a header attached to a piece of memory in
 * a linked list of such pieces of data terminated in a null pointer.
 */
struct BlockDesc {
    char *base; /* pointer to the first byte of this block */
    char *free; /* pointer to the next unused byte in this block */
    char *lim;  /* pointer to one-past-the-end of this block */
    struct BlockDesc *link; /* pointer to the next block in a chain */
};

/*
 * Dynamically allocates a new BlockDesc and at least 'bytes' extra bytes of
 * data. The beginning of the block will be aligned as specified by 'align'.
 * The 'link' parameter will specify the next block in the 
 */
static struct BlockDesc *
BlockDesc_new(struct BlockDesc *link, size_t bytes, size_t align)
{
    struct BlockDesc *newp;
    size_t bdescSize;

    bdescSize = sizeof(struct BlockDesc) +
                    ALIGN_PADDING(sizeof(struct BlockDesc), align);

    newp = malloc(bdescSize + bytes);

    if (newp) {
        newp->base = (char *)newp + bdescSize;
        newp->free = newp->base;
        newp->lim = newp->free + bytes;
        newp->link = link;
    }

    return newp;
}

static void
BlockDesc_free(struct BlockDesc *bd)
{
    struct BlockDesc *tmp;
    while (bd) {
        tmp = bd;
        bd = bd->link;
        free(tmp);
    }
}

static inline size_t
BlockDesc_freeBytes(struct BlockDesc *bd, size_t align)
{
    return bd->lim - bd->free - ALIGN_PADDING((uintptr_t)bd->free, align);
}

static inline size_t
BlockDesc_canAlloc(struct BlockDesc *bd, size_t size, size_t align)
{
    return BlockDesc_freeBytes(bd, align) / size;
}

static inline char *
BlockDesc_alloc(struct BlockDesc *bd, size_t bytes, size_t align)
{
    void *mem;
    size_t padding;
    
    bd->free += ALIGN_PADDING((uintptr_t)bd->free, align);
    mem = bd->free;
    bd->free += bytes;
    return mem;
}

#endif /* !CBITS_BLOCKDESC_H */
