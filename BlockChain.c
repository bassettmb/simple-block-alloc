#include <stdlib.h>
#include "BlockChain.h"
#include "BlockDesc.h"

struct BlockChain {
    struct BlockDesc *link; /* Link to first block in the chain. */
    size_t blockSize, refc; /* References to BlockChain */
};

static void BlockChain_init(struct BlockChain *bc, size_t blockSize);
static void *BlockChain_allocLarge(struct BlockChain *bc,
                                   size_t bytes, size_t align);

static void *
BlockChain_allocLarge(struct BlockChain *bc, size_t bytes, size_t align)
{
    void *mem;
    struct BlockDesc *bd, **dest;

    mem = 0;
    dest = bc->link ? &bc->link->link : &bc->link;
    bd = BlockDesc_new(*dest, bytes, align);
    if (bd) {
        *dest = bd;
        mem = BlockDesc_alloc(bd, bytes, align);
    }

    return mem;
}

void *
BlockChain_alloc(struct BlockChain *bc, size_t bytes, size_t align)
{
    void *mem;
    struct BlockDesc *bd;
    size_t need;

    mem = 0;
    bd = bc->link;
    if (bytes > bc->blockSize) {
        mem = BlockChain_allocLarge(bc, bytes, align);
    } else {
        if (!bd || BlockDesc_freeBytes(bd, align) < bytes) {
            bd = BlockDesc_new(bd, bc->blockSize, align);
            if (bd) {
                bc->link = bd;
                mem = BlockDesc_alloc(bd, bytes, align);
            }
        } else {
            mem = BlockDesc_alloc(bd, bytes, align);
        }
    }

    return mem;
}

static void
BlockChain_init(struct BlockChain *bc, size_t blockSize)
{
    bc->blockSize = blockSize;
    bc->link = 0;
}

void
BlockChain_reset(struct BlockChain *bc, size_t blockSize)
{
    BlockDesc_free(bc->link);
    BlockChain_init(bc, blockSize);
}

void
BlockChain_swap(struct BlockChain *first, struct BlockChain *second)
{
    struct BlockChain tmp;

    tmp = *first;
    *first = *second;
    *second = *first;
}

size_t
BlockChain_getBlockSize(struct BlockChain *bc)
{
    return bc->blockSize;
}

void
BlockChain_setBlockSize(struct BlockChain *bc, size_t blockSize)
{
    bc->blockSize = blockSize;
}

size_t
BlockChain_freeBytes(struct BlockChain *bc, size_t align)
{
    size_t nbytes;
    
    nbytes = 0;
    if (bc->link) {
        nbytes = BlockDesc_freeBytes(bc->link, align);
    }
    return nbytes;
}

size_t
BlockChain_canAlloc(struct BlockChain *bc, size_t size, size_t align)
{
    size_t nelem;
    
    nelem = 0;
    if (bc->link) {
        nelem = BlockDesc_canAlloc(bc->link, size, align);
    }
    return nelem;
}

void
BlockChain_retain(struct BlockChain *bc)
{
    ++bc->refc;
}

struct BlockChain *
BlockChain_new(size_t blockSize)
{
    struct BlockChain *bc;

    bc = malloc(sizeof *bc);
    if (bc) {
        BlockChain_init(bc, blockSize);
        bc->refc = 1;
    }
    return bc;
}

void
BlockChain_release(struct BlockChain *bc)
{
    if (bc && !--bc->refc) {
        BlockDesc_free(bc->link);
        free(bc);
    }
}
