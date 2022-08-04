#ifndef SINGLEALLOCATOR_HPP_INCLUDED
#define SINGLEALLOCATOR_HPP_INCLUDED

#include "LIFOMemoryResource.hpp"
#include <memory>

namespace Achibulup
{

class SingleAllocatorResource
{
  public:
    /// for each instance of this type, @a n and @a alignment have to be the same in every call to allocate()
    SingleAllocatorResource(size_t size, size_t alignment = alignof(std::max_align_t))
    : m_size(size), m_alignment(alignment) {}

    void* allocate(size_t size, size_t alignment = alignof(std::max_align_t))
    {
        if (size != this->m_size || alignment != this->m_alignment)
          throw std::runtime_error("SingleAllocatorResource::allocate(): size and alignment do not match");
        return this->allocate();
    }

    void deallocate(void* ptr, size_t size, size_t alignment = alignof(std::max_align_t))
    {
        if (size != this->m_size || alignment != this->m_alignment)
          throw std::runtime_error("SingleAllocatorResource::deallocate(): size and alignment do not match");
        return this->deallocate(ptr);
    }

    void* allocate()
    {
        if (!m_reuse.empty()) {
          void* result = this->m_reuse.back();
          this->m_reuse.pop_back();
          return result;
        }
        return this->m_resource.allocate(this->m_size, this->m_alignment);
    }

    void deallocate(void* ptr)
    {
        if (ptr) this->m_reuse.push_back(ptr);
    }
  
  private:
    LIFOMemoryResource m_resource;
    std::vector<void*> m_reuse;
    size_t m_size, m_alignment;
};

class SingleDeallocator
{
  public:
    constexpr SingleDeallocator(SingleAllocatorResource *resource = {}) noexcept
    : m_resource(resource) {}

    void operator()(void* ptr) noexcept
    {
        if (this->m_resource)
          this->m_resource->deallocate(ptr);
    }

    explicit constexpr operator bool () const noexcept
    {
        return this->m_resource;
    }

  private:
    SingleAllocatorResource *m_resource;
};


template<std::size_t size, std::size_t alignment = alignof(std::max_align_t)>
struct StaticSizeAlignMemSource
{
    static SingleAllocatorResource resource;
};
template<std::size_t size, std::size_t alignment>
SingleAllocatorResource 
StaticSizeAlignMemSource<size, alignment>::resource(size, alignment);


template<typename Tp>
class SingleAllocator
{
  public:
    using value_type = Tp;
    using is_always_equal = std::true_type;

    constexpr SingleAllocator() noexcept = default;
    template<typename U>
    constexpr SingleAllocator(const SingleAllocator<U>&) noexcept {}

    constexpr bool operator == (const SingleAllocator&) const noexcept
    {
        return true;
    }

    static Tp* allocate(std::size_t n)
    {
        if (n > 1) throw std::runtime_error("SingleAllocator can only allocate one object at a time");
        if (n == 0) return {};
        return allocate();
    }

    static void deallocate(void* ptr, std::size_t n)
    {
        if (n > 1) throw std::runtime_error("SingleAllocator can only deallocate one object at a time");
        if (n == 0) return;
        deallocate(ptr);
    }

    static Tp* allocate()
    {
        return static_cast<Tp*>(
            s_sharedResource.allocate(sizeof(Tp), alignof(Tp)));
    }

    static void deallocate(Tp* ptr)
    {
        s_sharedResource.deallocate(ptr);
    }

    static SingleDeallocator getDeallocator() noexcept
    {
        return SingleDeallocator(&s_sharedResource);
    }

  private:
    static constexpr SingleAllocatorResource &s_sharedResource 
        = StaticSizeAlignMemSource<sizeof(Tp), alignof(Tp)>::resource;
};

class SADeleter
{
    using Destroyer = void(*)(void*);

  public:
    constexpr SADeleter() noexcept : m_derived(), m_destroy() {}
    explicit constexpr SADeleter(std::nullptr_t) noexcept : SADeleter() {}

    template<typename Tp>
    explicit constexpr SADeleter(Tp *ptr) noexcept 
    : m_derived(const_cast<void*>(static_cast<const void*>(ptr))), 
      m_destroy([] (void *ptr) {
          Tp *retrieved = static_cast<Tp*>(ptr);
          destroy(retrieved);
          SingleAllocator<Tp>::deallocate(retrieved);
      }) {}

    ACHIBULUP__constexpr_fun14 SADeleter(SADeleter &&other) noexcept
    : m_derived(Move(other.m_derived)), m_destroy(Move(other.m_destroy)) {}

    ACHIBULUP__constexpr_fun14 SADeleter& 
    operator = (SADeleter &&other) noexcept
    {
        if (this != &other) {
          MoveAssign(this->m_derived, other.m_derived);
          MoveAssign(this->m_destroy, other.m_destroy);
        }
        return *this;
    }

    void swap(SADeleter &other) noexcept
    {
        using std::swap;
        swap(this->m_derived, other.m_derived);
        swap(this->m_destroy, other.m_destroy);
    }

    void operator () (const void*) noexcept
    {
        if (this->m_destroy) {
          this->m_destroy(this->m_derived);
          this->m_derived = {};
          this->m_destroy = {};
        }
    }

    void* getPtr() const noexcept
    {
        return this->m_derived;
    }

  private:
    void *m_derived;
    Destroyer m_destroy;
};



/// @brief a type of unique pointer that uses SingleAllocator to allocate memory
/// this pointer supports the same interface as std::unique_ptr
/// you can also safely upcast to a base type and destroy it without the destructor being virtual
/// instances of this type (other than nullptr) should be made with the makeSAUnique() factory function (see below)
template<typename Tp>
using SAUniquePtr = std::unique_ptr<Tp, SADeleter>;




template<typename Tp, typename ...Args>
SAUniquePtr<Tp> makeSAUnique(Args&& ...args)
{
    Tp *result_ptr = new (SingleAllocator<Tp>::allocate()) 
                         Tp(std::forward<Args>(args)...);
    return SAUniquePtr<Tp>(result_ptr, SADeleter(result_ptr));
}

} // namespace Achibulup

#endif // SINGLEALLOCATOR_HPP_INCLUDED