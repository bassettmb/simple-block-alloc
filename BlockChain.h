#ifndef CBITS_ALLOC_H
#define CBITS_ALLOC_H

#include <stddef.h> /* Hopefully this won't collide with cstddef. */

struct BlockChain;

/* For C++ linkage */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* TODO: comments. */

struct BlockChain *BlockChain_new(size_t blockSize);
void BlockChain_retain(struct BlockChain *bc);
void BlockChain_release(struct BlockChain *bc);
void BlockChain_reset(struct BlockChain *bc, size_t blockSize);

void *BlockChain_alloc(struct BlockChain *bc, size_t bytes, size_t align);

size_t BlockChain_getBlockSize(struct BlockChain *bc);
void BlockChain_setBlockSize(struct BlockChain *bc, size_t blockSize);
size_t BlockChain_freeBytes(struct BlockChain *bc, size_t align);
size_t BlockChain_canAlloc(struct BlockChain *bc, size_t nelem, size_t align);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !CBITS_ALLOC_H */
