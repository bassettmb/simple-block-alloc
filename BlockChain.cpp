#include <new>
#include <utility>

#include "BlockChain.h"
#include "cbits/BlockChain.h"

CXXBlockChain::CXXBlockChain(std::size_t blockSize) {
    chain = CXXBlockChain_new(blockSize);
    if (!chain) {
        throw std::bad_alloc();
    }
}

CXXBlockChain::CXXBlockChain(const CXXBlockChain &other) noexcept {
    CXXBlockChain_retain(other.chain);
    chain = other.chain;
}

CXXBlockChain::CXXBlockChain(CXXBlockChain &&other) noexcept :
    chain(other.chain) {
    other.chain = 0;
}

CXXBlockChain &CXXBlockChain::operator=(const CXXBlockChain &other) noexcept {
    if (this != &other) {
        CXXBlockChain tmp(other);
        swap(tmp);
    }
    return *this;
}

CXXBlockChain &CXXBlockChain::operator=(CXXBlockChain &&other) noexcept {
    swap(other);
    return *this;
}

CXXBlockChain::~CXXBlockChain(void) {
    CXXBlockChain_release(chain);
}

void *CXXBlockChain::allocate(std::size_t bytes, std::size_t alignment) {

    void *mem = CXXBlockChain_alloc(chain, bytes, alignment);

    if (!mem) {
        throw std::bad_alloc();
    }

    return mem;
}

size_t CXXBlockChain::blockSize(void) const noexcept {
    return CXXBlockChain_getBlockSize(chain);
}

void CXXBlockChain::blockSize(size_t blockSize) noexcept {
    CXXBlockChain_setBlockSize(chain, blockSize);
}

bool CXXBlockChain::operator==(const CXXBlockChain &other) const noexcept {
    return chain == other.chain;
}

bool CXXBlockChain::operator!=(const CXXBlockChain &other) const noexcept {
    return !(*this == other);
}

void CXXBlockChain::swap(CXXBlockChain &other) noexcept {
    std::swap(chain, other.chain);
}
