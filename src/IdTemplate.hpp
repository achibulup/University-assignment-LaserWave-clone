#ifndef IDTEMLATE_HPP_INCLUDED
#define IDTEMLATE_HPP_INCLUDED

#include <string_view>

namespace LaserWave
{

template<class ID>
class IdTemplate
{
  public:
    IdTemplate() noexcept = default;

    explicit IdTemplate(std::string_view id_str) noexcept
    : m_id(s_idGenerator++), m_str(id_str) {}

    std::string_view getString() const noexcept
    {
        return this->m_str;
    }

    friend bool operator == (IdTemplate lhs, IdTemplate rhs) noexcept
    {
        return lhs.m_id == rhs.m_id;
    }
    friend bool operator != (IdTemplate lhs, IdTemplate rhs) noexcept
    {
        return !(lhs == rhs);
    }
    friend bool operator < (IdTemplate lhs, IdTemplate rhs) noexcept
    {
        return lhs.m_id < rhs.m_id;
    }
    friend bool operator > (IdTemplate lhs, IdTemplate rhs) noexcept
    {
        return rhs < lhs;
    }
    friend bool operator <= (IdTemplate lhs, IdTemplate rhs) noexcept
    {
        return !(rhs < lhs);
    }
    friend bool operator >= (IdTemplate lhs, IdTemplate rhs) noexcept
    {
        return !(lhs < rhs);
    }
  
  private:
    static int s_idGenerator;
    
    int m_id = -1;
    std::string_view m_str = {};
};

template<typename ID>
int IdTemplate<ID>::s_idGenerator = 0;


#ifdef DEBUG
template<class Container>
struct DebugIdInitializer
{
    DebugIdInitializer()
    {
        auto &to_container = static_cast<Container&>(*this);
        to_container.m_id = to_container.Container::getId();
    }
};

#define DEBUG_ID_VAR public: Id m_id = {};
#define INIT_DEBUG_ID(Container) , public DebugIdInitializer<Container>
#else // DEBUG
#define DEBUG_ID_VAR
#define INIT_DEBUG_ID(Container)
#endif // DEBUG


} // namespace LaserWave

#endif // IDTEMLATE_HPP_INCLUDED