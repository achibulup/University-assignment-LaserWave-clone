#ifndef MENUSTATE_HPP_INCLUDED
#define NEMUSTATE_HPP_INCLUDED

#include "State.hpp"
#include "SimpleGUI.hpp"

namespace LaserWave
{

class MenuState : public State
{
  public:
    static const State::Id ID;

    explicit MenuState(GameDataRef data);
    MenuState(const MenuState&) = delete;
    void operator = (MenuState) = delete;
    ~MenuState();

    virtual State::Id getId() const override
    {
        return ID;
    }

    void update(sf::Time dt, EventManager &event) override;
    void render() const override;

    void asTopState() override;

  private:
    sf::Text m_title;
    sf::Text m_click_to_play_message;
    sf::Time m_message_timer = sf::Time::Zero;
    SimpleGUI m_GUI;
    const sf::Cursor *m_cursor;
};

} // namespace LaserWave

#endif // MENUSTATE_HPP_INCLUDED