#ifndef SIMPLEBUTTON_HPP_INCLUDED
#define SIMPLEBUTTON_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <functional>

namespace LaserWave
{

class SimpleButton : public sf::Drawable
{
  public:
    using Drawer = 
        std::function<void(sf::FloatRect, sf::RenderTarget&, sf::RenderStates)>;
    using OnClickHandler = std::function<void()>;
    using Self = SimpleButton;

    SimpleButton() = default;
    SimpleButton(sf::FloatRect rect, Drawer draw = {}, 
                 OnClickHandler on_click = {}, Drawer hover_draw = {})
    : m_rect(rect), m_defaultDraw(std::move(draw)), 
      m_hoverDraw(std::move(hover_draw)), m_onClick(std::move(on_click)) {}


    sf::FloatRect getRect() const { return this->m_rect; }
    bool isHovered() const { return this->m_hover; }
    void onClick() 
    { 
        if (this->m_onClick) this->m_onClick(); 
    }

    
    /// the setters return reference to the button to allow chaining

    Self& setRect(sf::FloatRect rect) &
    { 
        this->m_rect = rect; 
        return *this;
    }
    Self& setOnClick(OnClickHandler on_click) &
    { 
        this->m_onClick = std::move(on_click); 
        return *this;
    }
    Self& setDefaultDraw(Drawer draw) &
    { 
        this->m_defaultDraw = std::move(draw); 
        return *this;
    }
    Self& setHoverDraw(Drawer draw) & 
    { 
        this->m_hoverDraw = std::move(draw); 
        return *this;
    }

    void setHover(bool hover)
    { 
        this->m_hover = hover; 
    }

    /// setters also have rvalue version
    Self&& setRect(sf::FloatRect rect) &&
    { 
        return std::move(this->setRect(rect));
    }
    Self&& setOnClick(OnClickHandler on_click) &&
    { 
        this->m_onClick = std::move(on_click);
        return std::move(*this);
    }
    Self&& setDefaultDraw(Drawer draw) &&
    { 
        this->m_defaultDraw = std::move(draw); 
        return std::move(*this);
    }
    Self&& setHoverDraw(Drawer draw) && 
    { 
        this->m_hoverDraw = std::move(draw); 
        return std::move(*this);
    }

  private:
    void draw(sf::RenderTarget &target, sf::RenderStates = {}) const override;

    sf::FloatRect m_rect = {};
    Drawer m_defaultDraw = {}, m_hoverDraw = {};
    OnClickHandler m_onClick = {};
    bool m_hover = false;
};

sf::Sprite makeSprite(const sf::Texture*, sf::FloatRect, 
                      sf::Color = sf::Color::White);

SimpleButton::Drawer makeDrawer(const sf::Texture*, 
                                sf::Color = sf::Color::White);

SimpleButton makeButton(const sf::Texture *texture, sf::Vector2f pos = {},
                        SimpleButton::OnClickHandler on_click = {},
                        const sf::Texture *hover_texture = {});

} // namespace LaserWave

#endif // SIMPLEBUTTON_HPP_INCLUDED