#ifndef ARENA_HPP
#define ARENA_HPP

#include <cstddef>
#include <memory>

#include "CXXBlockChain.hpp"

template <class T>
class Arena {

public:

    typedef T value_type;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;

    typedef std::false_type propagate_on_container_copy_assignment;
    typedef std::true_type propagate_on_container_move_assignment;
    typedef std::false_type propagate_on_container_swap;

    template <class U> struct rebind { typedef Arena<U> other; };

    Arena(void) noexcept;
    explicit Arena(size_type blockSize);
    template <class U> explicit Arena(const Arena<U> &other);

    pointer address(reference obj) const noexcept;
    const_pointer address(const_reference obj) const noexcept;
    pointer allocate(size_type nelem, const void * = 0);
    void deallocate(pointer p, size_type n) noexcept;
    size_type max_size(void) const noexcept;
    template <class Tp, class ...Args> void construct(Tp *p, Args ...args);
    template <class Tp> void destroy(Tp *p) noexcept(noexcept(p->Tp::~Tp()));
    Arena select_on_container_copy_construction(void) const;
    bool operator==(const Arena &other) const noexcept;
    bool operator!=(const Arena &other) const noexcept;

private:

    CXXBlockChain chain;

};

#include "Arena.tpp"

#endif // !ARENA_H
