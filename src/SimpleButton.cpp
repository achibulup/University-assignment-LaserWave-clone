#include "SimpleButton.hpp"

namespace LaserWave
{

void SimpleButton::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (this->isHovered() && this->m_hoverDraw)
      this->m_hoverDraw(this->getRect(), target, states);
    else if (this->m_defaultDraw)
      this->m_defaultDraw(this->getRect(), target, states);
}

sf::Sprite makeSprite(const sf::Texture *texture, sf::FloatRect rect, 
                      sf::Color color)
{
    sf::Sprite sprite(*texture);
    sprite.setPosition(rect.left, rect.top);
    sprite.setScale(rect.width / sprite.getGlobalBounds().width,
                    rect.height / sprite.getGlobalBounds().height);
    sprite.setColor(color);
    return sprite;
}

SimpleButton::Drawer makeDrawer(const sf::Texture *texture, sf::Color color)
{
    return [texture, color](sf::FloatRect rect, sf::RenderTarget &target,
                            sf::RenderStates states) 
        {
            target.draw(makeSprite(texture, rect, color), states);
        };
}

SimpleButton makeButton(const sf::Texture *texture, sf::Vector2f pos,
                        SimpleButton::OnClickHandler on_click,
                        const sf::Texture *hover_texture)
{
    sf::Vector2f size = sf::Vector2f(texture->getSize());
    return SimpleButton(sf::FloatRect(pos, size), makeDrawer(texture),
                        std::move(on_click), makeDrawer(hover_texture));
}

} // namespace LaserWave