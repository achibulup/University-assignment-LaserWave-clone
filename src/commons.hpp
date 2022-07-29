#ifndef COMMONS_HPP_INCLUDED
#define COMMONS_HPP_INCLUDED

#include <set>
// #include <list>
// #include <deque>
#include <vector>
#include <memory>
#include <algorithm>
#include <SFML/Achibulup_dependencies/SingleAllocator.hpp>
#include <SFML/Graphics.hpp>

namespace LaserWave
{

using Vec2 = sf::Vector2f;
using Point = Vec2;

class Box
{
  public:
    Box() = default;

    Box(Point position) noexcept;
    Box(Point position, Vec2 size);
    Box(float left, float top, float right, float bottom);
    Box(sf::FloatRect);

    float getWidth() const noexcept;
    float getHeight() const noexcept;
    Vec2 getSize() const noexcept;
    float getLeft() const noexcept;
    float getTop() const noexcept;
    float getRight() const noexcept;
    float getBottom() const noexcept;
    Point getCenter() const noexcept;
    Point getTopLeft() const noexcept;
    Point getTopRight() const noexcept;
    Point getBottomLeft() const noexcept;
    Point getBottomRight() const noexcept;
    /// get the topleft position
    Point getPosition() const noexcept;

    /// keep the other bounds
    void setLeft(float pos);
    /// keep the other bounds
    void setTop(float pos);
    /// keep the other bounds
    void setRight(float pos);
    /// keep the other bounds
    void setBottom(float pos);
    /// keep the size of the box
    void setCenter(Point pos);
    /// keep the other bounds
    void setTopRight(Point pos);
    /// keep the other bounds
    void setBottomLeft(Point pos);
    /// keep the other bounds
    void setBottomRight(Point pos);
    /// set the topleft postion
    void setPosition(Point pos);
    /// keep the position
    void setSize(Vec2 size);
    /// keep the position
    void setWidth(float width);
    /// keep the position
    void setHeight(float height);

    void move(Vec2 displacement);
    void setSizeKeepCenter(Vec2 size);
    
  private:
    Point m_topLeft = {};
    Vec2 m_size = {};
};

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








inline Box::Box(Point position) noexcept : m_topLeft(position) {}

inline Box::Box(Point position, Vec2 size) 
: m_topLeft(position), m_size(size) {}

inline Box::Box(float left, float top, float right, float bottom)
: m_topLeft(left, top), m_size(right - left, bottom - top) {}

inline Box::Box(sf::FloatRect rect)
: m_topLeft(rect.left, rect.top), m_size(rect.width, rect.height) {}


inline float Box::getWidth() const noexcept
{
    return this->m_size.x;
}
inline float Box::getHeight() const noexcept
{
    return this->m_size.y;
}
inline Vec2 Box::getSize() const noexcept
{
    return this->m_size;
}
inline float Box::getLeft() const noexcept
{
    return this->m_topLeft.x;
}
inline float Box::getTop() const noexcept
{
    return this->m_topLeft.y;
}
inline float Box::getRight() const noexcept
{
    return this->m_topLeft.x + this->m_size.x;
}
inline float Box::getBottom() const noexcept
{
    return this->m_topLeft.y + this->m_size.y;
}
inline Point Box::getCenter() const noexcept
{
    return this->m_topLeft + this->m_size / 2.f;
}
inline Point Box::getTopLeft() const noexcept
{
    return this->m_topLeft;
}
inline Point Box::getTopRight() const noexcept
{
    return {this->getRight(), this->getTop()};
}
inline Point Box::getBottomLeft() const noexcept
{
    return {this->getLeft(), this->getBottom()};
}
inline Point Box::getBottomRight() const noexcept
{
    return {this->getRight(), this->getBottom()};
}
inline Point Box::getPosition() const noexcept
{
    return this->getTopLeft();
}
inline void Box::setLeft(float pos)
{
    this->m_topLeft.x = pos;
}
inline void Box::setTop(float pos)
{
    this->m_topLeft.y = pos;
}
inline void Box::setRight(float pos)
{
    this->m_size.x = pos - this->m_topLeft.x;
}
inline void Box::setBottom(float pos)
{
    this->m_size.y = pos - this->m_topLeft.y;
}
inline void Box::setCenter(Point pos)
{
    this->m_topLeft = pos - m_size / 2.f;
}
inline void Box::setTopRight(Point pos)
{
    this->m_size.x = pos.x - this->m_topLeft.x;
    this->m_size.y += this->m_topLeft.y - pos.y;
    this->m_topLeft.y = pos.y;
}
inline void Box::setBottomLeft(Point pos)
{
    this->m_size.y = pos.y - this->m_topLeft.y;
    this->m_size.x += this->m_topLeft.x - pos.x;
    this->m_topLeft.x = pos.x;
}
inline void Box::setBottomRight(Point pos)
{
    this->m_size.x = pos.x - this->m_topLeft.x;
    this->m_size.y = pos.y - this->m_topLeft.y;
}
inline void Box::setPosition(Point pos)
{
    this->m_topLeft = pos;
}
inline void Box::setSize(Vec2 size)
{
    this->m_size = size;
}
inline void Box::setWidth(float width)
{
    this->m_size.x = width;
}
inline void Box::setHeight(float height)
{
    this->m_size.y = height;
}
inline void Box::move(Vec2 displacement)
{
    this->m_topLeft += displacement;
}
inline void Box::setSizeKeepCenter(Vec2 size)
{
    this->m_topLeft = this->getCenter() - size / 2.f;
    this->m_size = size;
}

} // namespace LaserWave

#endif // COMMONS_HPP_INCLUDED