#ifndef BLOCKCHAIN_HPP
#define BLOCKCHAIN_HPP

#include <cstddef>

/* Forward declaration for C struct */
struct BlockChain;

class CXXBlockChain {

public:

    explicit CXXBlockChain(std::size_t blockSize = DEFAULT_BLOCK_SIZE);
    CXXBlockChain(const CXXBlockChain &other) noexcept;
    CXXBlockChain(CXXBlockChain &&other) noexcept;
    CXXBlockChain &operator=(const CXXBlockChain &other) noexcept;
    CXXBlockChain &operator=(CXXBlockChain &&other) noexcept;
    ~CXXBlockChain(void);

    void *allocate(std::size_t bytes, std::size_t alignment);
    size_t blockSize(void) const noexcept;
    void blockSize(size_t blockSize) noexcept;
    bool operator==(const CXXBlockChain &other) const noexcept;
    bool operator!=(const CXXBlockChain &other) const noexcept;
    void swap(CXXBlockChain &other) noexcept;

private:

    static const std::size_t DEFAULT_BLOCK_SIZE = 4096;
    BlockChain *chain;

};

}

#endif // BLOCKCHAIN_HPP
