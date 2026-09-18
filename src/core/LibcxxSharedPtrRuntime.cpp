#include <memory>

namespace std
{

__shared_count::~__shared_count()
{
}

void __shared_count::__add_shared()
{
    ++__shared_owners_;
}

bool __shared_count::__release_shared()
{
    --__shared_owners_;
    if (__shared_owners_ == -1)
    {
        __on_zero_shared();
        return true;
    }
    return false;
}

__shared_weak_count::~__shared_weak_count()
{
}

void __shared_weak_count::__add_shared()
{
    __shared_count::__add_shared();
}

void __shared_weak_count::__add_weak()
{
    ++__shared_weak_owners_;
}

void __shared_weak_count::__release_shared()
{
    if (__shared_count::__release_shared())
    {
        __release_weak();
    }
}

void __shared_weak_count::__release_weak()
{
    --__shared_weak_owners_;
    if (__shared_weak_owners_ == -1)
    {
        __on_zero_shared_weak();
    }
}

__shared_weak_count *__shared_weak_count::lock() noexcept
{
    if (__shared_owners_ == -1)
    {
        return 0;
    }
    ++__shared_owners_;
    return this;
}

const void *__shared_weak_count::__get_deleter(const type_info &) const noexcept
{
    return 0;
}

} // namespace std
