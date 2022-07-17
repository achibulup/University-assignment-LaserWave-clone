#ifndef COMMONS_HPP_INCLUDED
#define COMMONS_HPP_INCLUDED

#include <set>
// #include <list>
// #include <deque>
#include <vector>
#include <memory>
#include <algorithm>
#include <SFML/Achibulup_dependencies/SingleAllocator.hpp>

namespace LaserWave
{

/// the recommended container for general purpose
template<typename Tp>
using UList = std::vector<Tp>;

template<typename Tp>
using List = std::vector<Tp>;

template<typename Tp>
using Set = std::set<Tp>;



template<typename Tp>
using Unique = Achibulup::SAUniquePtr<Tp>;


template<typename Tp>
void add(std::vector<Tp> &list, const Tp &element)
{
    list.push_back(element);
}
template<typename Tp>
void add(std::vector<Tp> &list, Tp &&element)
{
    list.push_back(std::move(element));
}
template<typename Tp, typename Pred>
int removeIf(std::vector<Tp> &list, Pred &&pred)
{
    auto it = std::remove_if(list.begin(), list.end(), std::forward<Pred>(pred));
    int removed = std::distance(it, list.end());
    list.erase(it, list.end());
    return removed;
}

// template<typename Tp>
// void add(std::list<Tp> &list, const Tp &element)
// {
//     list.push_back(element);
// }
// template<typename Tp>
// void add(std::list<Tp> &list, Tp &&element)
// {
//     list.push_back(std::move(element));
// }
// template<typename Tp, typename Pred>
// int removeIf(std::list<Tp> &list, Pred &&pred)
// {
//     int removed = 0;
//     for(auto it = list.begin(), ; it != list.end();) {
//       if(pred(*it)) {
//         it = list.erase(it);
//         ++removed;
//       }
//       else ++it;
//     }
//     return removed;
// }

// template<typename Tp>
// void add(std::set<Tp> &list, const Tp &element)
// {
//     list.insert(element);
// }
// template<typename Tp>
// void add(std::set<Tp> &list, Tp &&element)
// {
//     list.insert(std::move(element));
// }
// template<typename Tp, typename Pred>
// int removeIf(std::set<Tp> &list, Pred &&pred)
// {
//     int removed = 0;
//     for (auto it = list.begin(), last = list.end(); it != last;) {
//       if (pred(*it)) {
//         it = list.erase(it);
//         ++removed;
//       } 
//       else ++it;
//     }
//     return removed;
// }


template<typename Cont, typename Value>
int remove(Cont &list, const Value &value)
{
    return removeIf(list, [&value] (const auto &e) { return e == value; });
}


template<typename Tp, typename ...Args>
Unique<Tp> makeUnique(Args&& ...args)
{
    return Achibulup::makeSAUnique<Tp>(std::forward<Args>(args)...);
}

} // namespace LaserWave

#endif // COMMONS_HPP_INCLUDED