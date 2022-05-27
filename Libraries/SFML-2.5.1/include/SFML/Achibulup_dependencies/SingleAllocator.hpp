#ifndef SINGLEALLOCATOR_HPP_INCLUDED
#define SINGLEALLOCATOR_HPP_INCLUDED

#include "LIFOMemoryResource.hpp"

namespace Achibulup
{

class SingleAllocatorResource
{
  public:
    /// for each instance of this type, @a n and @a alignment have to be the same in every call to allocate()
    void* allocate(size_t n, size_t alignment = alignof(std::max_align_t))
    {
        if (n == 0) return nullptr;
        if (!m_reuse.empty()) {
          void* result = this->m_reuse.back();
          this->m_reuse.pop_back();
          return result;
        }
        return this->m_resource.allocate(n, alignment);
    }

    void deallocate(void* ptr)
    {
        if (ptr) this->m_reuse.push_back(ptr);
    }
  
  private:
    LIFOMemoryResource m_resource;
    std::vector<void*> m_reuse;
};

template<typename Tp>
class SingleAllocator
{
  public:
    using value_type = Tp;
    using is_always_equal = std::true_type;

    static Tp* allocate(std::size_t n)
    {
        if (n > 1) throw std::runtime_error("SingleAllocator can only allocate one object at a time");
        if (n == 0) return nullptr;
        return static_cast<Tp*>(
            s_sharedResource.allocate(sizeof(Tp), alignof(Tp)));
    }

    static void deallocate(void* ptr, std::size_t n)
    {
        if (n > 1) throw std::runtime_error("SingleAllocator can only deallocate one object at a time");
        if (n == 0) return;
        s_sharedResource.deallocate(ptr);
    }

    static SingleAllocatorResource s_sharedResource;
};
template<typename Tp>
SingleAllocatorResource SingleAllocator<Tp>::s_sharedResource;

template<typename Base, typename Derived>
constexpr std::ptrdiff_t BaseOffset()
{
    return reinterpret_cast<std::uintptr_t>(&static_cast<Derived*>(nullptr)->Base::m_dummy)
        - reinterpret_cast<std::uintptr_t>(&static_cast<Base*>(nullptr)->m_dummy);
}

class SADeleter
{
  public:
    constexpr SADeleter() noexcept : m_ptr(), m_source() {}
    explicit constexpr SADeleter(std::nullptr_t) noexcept : SADeleter() {}

    template<typename Tp>
    explicit constexpr SADeleter(Tp *ptr) noexcept 
    : m_ptr(ptr), m_source(&SingleAllocator<Tp>::s_sharedResource) {}

    ACHIBULUP__constexpr_fun14 SADeleter(SADeleter &&other) noexcept
    : m_ptr(other.m_ptr), m_source(other.m_source)
    {
        other.m_ptr = nullptr;
        other.m_source = nullptr;
    }

    ACHIBULUP__constexpr_fun14 SADeleter& 
    operator = (SADeleter &&other) noexcept
    {
        if (this != &other) {
          this->m_ptr = other.m_ptr;
          this->m_source = other.m_source;
          other.m_ptr = nullptr;
          other.m_source = nullptr;
        }
        return *this;
    }

    void swap(SADeleter &other) noexcept
    {
        using std::swap;
        swap(this->m_ptr, other.m_ptr);
        swap(this->m_source, other.m_source);
    }

    void release() noexcept
    {
        if (this->m_source) {
          this->m_source->deallocate(this->m_ptr);
          this->m_ptr = nullptr;
          this->m_source = nullptr;
        }
    }

  private:
    void *m_ptr;
    SingleAllocatorResource *m_source;
};

template<typename Tp>
class SAUniquePtr
{
  public:
    using value_type = Tp;

    constexpr SAUniquePtr() noexcept : m_manager(), m_ptr() {}
    constexpr SAUniquePtr(std::nullptr_t) noexcept : SAUniquePtr() {}

    explicit constexpr SAUniquePtr(Tp *ptr) noexcept
    : m_manager(ptr), m_ptr(ptr) {}

    ACHIBULUP__constexpr_fun14 SAUniquePtr(SAUniquePtr &&other) noexcept
    : m_manager(std::move(other.m_manager)), m_ptr(other.m_ptr)
    {
        other.m_ptr = {};
    }

    template<typename U>
    ACHIBULUP__constexpr_fun14 SAUniquePtr(SAUniquePtr<U>&& other) noexcept
    : SAUniquePtr()
    {
        this->m_ptr = other.release(this->m_manager);
    }

    SAUniquePtr& operator = (SAUniquePtr other) noexcept
    {
        this->swap(other);
        return *this;
    }

    ~SAUniquePtr()
    {
        this->reset();
    }

    void swap(SAUniquePtr &other) noexcept
    {
        using std::swap;
        swap(this->m_manager, other.m_manager);
        swap(this->m_ptr, other.m_ptr);
    }

    ACHIBULUP__constexpr_fun14 Tp* release() noexcept
    {
        Tp* result = this->m_ptr;
        this->m_manager = {};
        this->m_ptr = nullptr;
        return result;
    }
    ACHIBULUP__constexpr_fun14 Tp* release(SADeleter &move_to) noexcept
    {
        Tp* result = this->m_ptr;
        move_to = std::move(this->m_manager);
        this->m_ptr = nullptr;
        return result;
    }

    void reset() noexcept
    {
        this->m_manager.release();
        this->m_ptr = nullptr;
    }

    constexpr Tp* get() const noexcept
    {
        return this->m_ptr;
    }

    // ACHIBULUP__constexpr_fun14 SADeleter& getDeleter() noexcept
    // {
    //     return this->m_manager;
    // }
    // constexpr const SADeleter& getDeleter() const noexcept
    // {
    //     return this->m_manager;
    // }

    explicit constexpr operator bool () const noexcept
    {
        return this->m_ptr != nullptr;
    }

    constexpr Tp& operator * () const
    {
        return *this->m_ptr;
    }

    constexpr Tp* operator -> () const noexcept
    {
        return this->m_ptr;
    }

  private:
    SADeleter m_manager;
    Tp *m_ptr;
};


template<typename Tp, typename ...Args>
SAUniquePtr<Tp> makeSAUnique(Args&& ...args)
{
    Tp *ptr = SingleAllocator<Tp>().allocate(1);
    return SAUniquePtr<Tp>(new (ptr) Tp(std::forward<Args>(args)...));
}

} // namespace Achibulup

#endif // SINGLEALLOCATOR_HPP_INCLUDED