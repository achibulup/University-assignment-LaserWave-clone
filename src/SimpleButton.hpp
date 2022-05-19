#ifndef SIMPLEBUTTON_HPP_INCLUDED
#define SIMPLEBUTTON_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <functional>

namespace LaserWave
{

class SimpleButton : public sf::Drawable
{
  public:
    using DrawFunctor = 
        std::function<void(sf::FloatRect, sf::RenderTarget&, sf::RenderStates)>;
    using OnClickFunctor = std::function<void()>;
    SimpleButton() : m_rect() {}
    SimpleButton(sf::FloatRect rect, DrawFunctor draw = {}, 
                 OnClickFunctor on_click = {}, DrawFunctor hover_draw = {})
    : m_rect(rect), m_defaultDraw(std::move(draw)), 
      m_hoverDraw(std::move(hover_draw)), m_onClick(std::move(on_click)) {}

    void setRect(sf::FloatRect rect) 
    { 
        this->m_rect = rect; 
    }
    void setOnClick(OnClickFunctor on_click) 
    { 
        this->m_onClick = std::move(on_click); 
    }
    void setDefaultDraw(DrawFunctor draw) 
    { 
        this->m_defaultDraw = std::move(draw); 
    }
    void setHoverDraw(DrawFunctor draw) 
    { 
        this->m_hoverDraw = std::move(draw); 
    }

    sf::FloatRect getRect() const 
    { 
        return this->m_rect; 
    }
    bool isHovered() const
    {
        auto mouse_pos = sf::Mouse::getPosition();
        return this->getRect().contains(sf::Vector2f(mouse_pos));
    }
    void onClick()
    {
        if (this->m_onClick)
          this->m_onClick();
    }

  private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        if (this->isHovered() && this->m_hoverDraw)
          this->m_hoverDraw(this->getRect(), target, states);
        else if (this->m_defaultDraw)
          this->m_defaultDraw(this->getRect(), target, states);
    }

    sf::FloatRect m_rect;
    DrawFunctor m_defaultDraw, m_hoverDraw;
    OnClickFunctor m_onClick;
};

inline sf::Sprite makeSprite(const sf::Texture *texture, sf::FloatRect rect)
{
    sf::Sprite sprite(*texture);
    sprite.setPosition(rect.left, rect.top);
    sprite.setScale(rect.width / sprite.getGlobalBounds().width,
                    rect.height / sprite.getGlobalBounds().height);
    return sprite;
}

} // namespace LaserWave

#endif // SIMPLEBUTTON_HPP_INCLUDED