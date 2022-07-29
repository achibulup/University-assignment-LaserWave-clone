#include "SimpleTextBox.hpp"

namespace LaserWave
{


void SimpleTextBox::enter()
{
    if (this->m_onEnter) this->m_onEnter(this->text());
}

void SimpleTextBox::backspace()
{
    auto &text = this->text();
    if (!text.isEmpty())
      text.erase(text.getSize() - 1);
}

void SimpleTextBox::enterText(sf::Uint32 letter)
{
    static const sf::Uint32 BACKSPACE = '\b';
    static const sf::Uint32 ENTER = '\n';
    static const sf::Uint32 ENTER2 = '\r';
    static const sf::Uint32 TAB = '\t';

    auto &text = this->text();
    
    switch(letter) {
      case TAB:
      break;
      case ENTER:
      case ENTER2:
        this->enter();
      break;
      case BACKSPACE:
        if (!text.isEmpty())
          text.erase(text.getSize() - 1, 1);
      break;
      default:
        if (this->getLimit() == NO_LIMIT 
         || text.getSize() < this->getLimit())
          text += letter;
    }
}

void SimpleTextBox::respondTo(const sf::Event &event, bool will_set_selected)
{
    if (this->isSelected() && event.type == sf::Event::TextEntered)
      this->enterText(event.text.unicode);
    if (event.type == sf::Event::MouseButtonPressed && will_set_selected) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        
      }
    }
}

void SimpleTextBox::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::String text = this->text();
    if (this->m_selected) {
      text += '_';
    }
    sf::Text display(text, *this->getFont(), this->getFontSize());
    
    float line_length = display.getGlobalBounds().width;
    sf::Vector2f position = this->getPosition();
    if (this->getAlignment() == Alignment::Right)
      position.x += this->getWidth() - line_length;
    else if (this->getAlignment() == Alignment::Center)
      position.x += (this->getWidth() - line_length) / 2;

    display.setPosition(position);
    display.setStyle(this->getStyle());
    display.setFillColor(this->getColor());
    target.draw(display, states);
}
constexpr auto a = sizeof(sf::String);

} // namespace LaserWave