#ifndef COMMONS_HPP_INCLUDED
#define COMMONS_HPP_INCLUDED

#include <vector>
#include <memory>
#include <SFML/Achibulup_dependencies/SingleAllocator.hpp>

namespace LaserWave
{

/// the recommended container for general purpose
template<typename Tp>
using List = std::vector<Tp>;

template<typename Tp>
using Unique = Achibulup::SAUniquePtr<Tp>;

template<typename Tp, typename ...Args>
Unique<Tp> makeUnique(Args&& ...args)
{
    return Achibulup::makeSAUnique<Tp>(std::forward<Args>(args)...);
}

} // namespace LaserWave

#endif // COMMONS_HPP_INCLUDED