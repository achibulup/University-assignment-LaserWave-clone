#ifndef SIMPLE_TEXTBOX_HPP_INCLUDED
#define SIMPLE_TEXTBOX_HPP_INCLUDED

#include "commons.hpp"
#include <SFML/Graphics.hpp>
#include <functional>

namespace LaserWave
{

class SimpleTextBox : public sf::Drawable
{
  public:
    enum Alignment {
        Left,
        Center,
        Right
    };
    using OnEnterHandler = std::function<void(sf::String&)>;
    using Self = SimpleTextBox;

    static const int NO_LIMIT = -1;

    SimpleTextBox() = default;
    SimpleTextBox(sf::Vector2f pos, const sf::Font *font, 
                  int font_size, float width = {});


    Box                    getBox() const { return this->m_box; }
    sf::Vector2f      getPosition() const { return this->m_box.getPosition(); }
    sf::Vector2f          getSize() const { return this->m_box.getSize(); }
    float                getWidth() const { return this->m_box.getWidth(); }
    float               getHeight() const { return this->m_box.getHeight(); }
    Alignment        getAlignment() const { return this->m_alignment; }
    unsigned int      getFontSize() const { return this->m_fontSize; }
    const sf::Font*       getFont() const { return this->m_font; }
    sf::Uint32           getStyle() const { return this->m_style; }
    sf::Color            getColor() const { return this->m_color; }
    int                  getLimit() const { return this->m_limit; }

    bool               isSelected() const { return this->m_selected; }

    const sf::String&        text() const { return this->m_text; }
    sf::String&              text()       { return this->m_text; }

    void setSelected(bool selected) { this->m_selected = selected; }

    void enter();
    void backspace();
    void enterText(sf::Uint32 letter);
    void respondTo(const sf::Event&, bool will_set_selected = true);

    /// setters return reference to itself to allow chaining
    Self& setBox(const Box &box) &
    { this->m_box = box; return *this; }
    Self& setPosition(sf::Vector2f pos) &
    { this->m_box.setPosition(pos); return *this; }
    Self& setSize(sf::Vector2f size) &
    { this->m_box.setSize(size); return *this; }
    Self& setWidth(float width) &
    { this->m_box.setWidth(width); return *this; }
    Self& setHeight(float height) &
    { this->m_box.setHeight(height); return *this; }
    Self& setAlignment(Alignment alignment) &
    { this->m_alignment = alignment; return *this; }
    Self& setFontSize(int font_size) &
    { this->m_fontSize = font_size; return *this; }
    Self& setText(const sf::String &text) &
    { this->m_text = text; return *this; }
    Self& setFont(const sf::Font *font) &
    { this->m_font = font; return *this; }
    Self& setStyle(sf::Uint32 style) &
    { this->m_style = style; return *this; }
    Self& setColor(sf::Color color) &
    { this->m_color = color; return *this; }
    Self& setLimit(int limit) &
    { this->m_limit = limit; return *this; }
    Self& setOnEnter(OnEnterHandler on_enter) &
    { this->m_onEnter = std::move(on_enter); return *this; }


    Self&& setBox(const Box &box) &&
    { return std::move(this->setBox(box)); }
    Self&& setPosition(sf::Vector2f pos) &&
    { return std::move(this->setPosition(pos)); }
    Self&& setSize(sf::Vector2f size) &&
    { return std::move(this->setSize(size)); }
    Self&& setWidth(float width) &&
    { return std::move(this->setWidth(width)); }
    Self&& setHeight(float height) &&
    { return std::move(this->setHeight(height)); }
    Self&& setAlignment(Alignment alignment) &&
    { return std::move(this->setAlignment(alignment)); }
    Self&& setFontSize(int font_size) &&
    { return std::move(this->setFontSize(font_size)); }
    Self&& setText(const sf::String &text) &&
    { return std::move(this->setText(text)); }
    Self&& setFont(const sf::Font *font) &&
    { return std::move(this->setFont(font)); }
    Self&& setStyle(sf::Uint32 style) &&
    { return std::move(this->setStyle(style)); }
    Self&& setColor(sf::Color color) &&
    { return std::move(this->setColor(color)); }
    Self&& setLimit(int limit) &&
    { return std::move(this->setLimit(limit)); }
    Self&& setOnEnter(OnEnterHandler on_enter) &&
    { 
        this->m_onEnter = std::move(on_enter); 
        return std::move(*this); 
    }

  private:
    void draw(sf::RenderTarget &target, sf::RenderStates = {}) const override;


    Box m_box = {};
    Alignment m_alignment = Alignment::Left;
    int m_fontSize = {};
    sf::String m_text = {};
    const sf::Font *m_font = {};
    sf::Uint32 m_style = sf::Text::Regular;
    sf::Color m_color = sf::Color::White;
    int m_limit = NO_LIMIT;
    OnEnterHandler m_onEnter = {};
    bool m_selected = false;
};

} // namespace LaserWave

#endif // SIMPLE_TEXTBOX_HPP_INCLUDED

