#ifndef CONVEX_ENTITY_HPP_INCLUDED
#define CONVEX_ENTITY_HPP_INCLUDED

#include "commons.hpp"
#include <SFML/Graphics.hpp>

namespace LaserWave
{

sf::RenderStates operator * (const sf::RenderStates&, const sf::Transform&);
sf::RenderStates& operator *= (sf::RenderStates&, const sf::Transform&);

struct Appearance
{
    sf::Color fillColor = sf::Color::White;
    sf::Color outlineColor = sf::Color::Black;
    float outlineThickness = 0.f;

    Appearance() = default;
    Appearance(sf::Color fill) noexcept : fillColor(fill) {}
    Appearance(sf::Color fill, sf::Color outline, float thick) noexcept
        : fillColor(fill), outlineColor(outline), outlineThickness(thick) {}
};

void apply(sf::Shape&, Appearance);
void apply(sf::Text&, Appearance);

class UseAppearance
{
  public:
    sf::Color getFillColor() const noexcept 
    { return this->m_app.fillColor; }
    sf::Color getOutlineColor() const noexcept 
    { return this->m_app.outlineColor; }
    float getOutlineThickness() const noexcept 
    { return this->m_app.outlineThickness; }
    
    void setFillColor(sf::Color color) noexcept 
    { this->m_app.fillColor = color; }
    void setOutlineColor(sf::Color color) noexcept 
    { this->m_app.outlineColor = color; }
    void setOutlineThickness(float thickness) noexcept 
    { this->m_app.outlineThickness = thickness; }

    Appearance getAppearance() const noexcept { return this->m_app; }
    void setAppearance(const Appearance &app) noexcept { this->m_app = app; }

  private:
    Appearance m_app;
};

class Rect : public sf::Drawable, public sf::Transformable, public UseAppearance
{
  public:
    Rect() = default;
    Rect(Vec2 size) : m_size(size) {}
    Rect(Box box) : m_size(box.getSize())
    {
        this->setPosition(box.getPosition());
    }

    Vec2 getSize() const noexcept { return this->m_size; }
    void setSize(Vec2 size) { this->m_size = size; }

  private:
    void draw(sf::RenderTarget&, sf::RenderStates = {}) const override;

    Vec2 m_size = {};
};

class IConvexPolygon;
class Circle;

void draw(sf::RenderTarget&, const IConvexPolygon&, 
          Appearance, sf::RenderStates = {});
void draw(sf::RenderTarget&, const Circle&, 
          Appearance, sf::RenderStates = {});

} // namespace LaserWave

#endif // CONVEX_ENTITY_HPP_INCLUDED