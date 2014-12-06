#include <cstddef>
#include <limits>
#include <memory>
#include <new>
#include <utility>

namespace cs251 {

template <class T>
Arena<T>::Arena(void) noexcept {}

template <class T>
Arena<T>::Arena(size_type blockSize) : chain(blockSize) {}

template <class T>
template <class U>
Arena<T>::Arena(const Arena<U> &other) :
    chain(other.chain.blockSize()) {
}

template <class T>
typename Arena<T>::pointer Arena<T>::address(reference obj) const noexcept {
    return std::addressof(obj);
}

template <class T>
typename Arena<T>::const_pointer
Arena<T>::address(const_reference obj) const noexcept {
    return std::addressof(obj);
}

template <class T>
typename Arena<T>::pointer Arena<T>::allocate(size_type nelem, const void *) {
    if (nelem > max_size()) {
        throw std::bad_alloc();
    }
    return static_cast<pointer>(
        chain.allocate(nelem * sizeof(value_type), alignof(value_type)));
}

template <class T>
void Arena<T>::deallocate(pointer p, size_type n) noexcept { /* nop */ }

template <class T>
typename Arena<T>::size_type Arena<T>::max_size(void) const noexcept {
    /*
     * This overshoots because of BlockChain and underlying allocator
     * overhead, but it's a good a guess as we can get.
     *
     * Basically, we want to avoid overflowing size_type.
     */
    return std::numeric_limits<size_type>::max() / sizeof(value_type);
}

template <class T>
template <class Tp, class ...Args>
void Arena<T>::construct(Tp *p, Args ...args) {
    ::new(static_cast<void *>(p)) Tp(std::forward<Args>(args)...);
}

template <class T>
template <class Tp>
void Arena<T>::destroy(Tp *p) noexcept(noexcept(p->Tp::~Tp())) {
    p->~Tp();
}

template <class T>
Arena<T> Arena<T>::select_on_container_copy_construction(void) const {
    return Arena(chain.blockSize());
}

template <class T>
bool Arena<T>::operator==(const Arena &other) const noexcept {
    return chain == other.chain;
}

template <class T>
bool Arena<T>::operator!=(const Arena &other) const noexcept {
    return !(*this == other);
}

}
