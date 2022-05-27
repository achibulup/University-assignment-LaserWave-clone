#ifndef COMMONS_HPP_INCLUDED
#define COMMONS_HPP_INCLUDED

#include <vector>
#include <SFML/Achibulup_dependencies/SingleAllocator.hpp>

namespace LaserWave
{

template<typename Tp>
using List = std::vector<Tp>;

template<typename Tp>
using Unique = Achibulup::SAUniquePtr<Tp>;

} // namespace LaserWave

#endif // COMMONS_HPP_INCLUDED