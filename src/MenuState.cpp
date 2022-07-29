#include "State.hpp"
#include "SimpleGUI.hpp"
#include "Enemies.hpp"

#include "assets.hpp"
#include "constants.hpp"
#include "stateRequests.hpp"
#include "randoms.hpp"

namespace LaserWave
{

class MenuState : public State INIT_DEBUG_ID(MenuState)
{
  public:
    static const State::Id ID;

    explicit MenuState(GameDataRef data);
    MenuState(const MenuState&) = delete;
    void operator = (MenuState) = delete;
    ~MenuState();

    virtual State::Id getId() const noexcept override
    {
        return ID;
    }

    void update(sf::Time dt, EventManager &event) override;
    void render() const override;

    void asTopState() override;

  private:
    void randomlySpawnEnemies();
    Point getRandomSpawnPosition() const;

    sf::Text m_title;
    sf::Text m_click_to_play_message;
    sf::Time m_message_timer = sf::Time::Zero;
    SimpleGUI m_GUI;
    const sf::Cursor *m_cursor = {};
    Enemies m_enemies;
};
const State::Id MenuState::ID ("MenuState");
const State::Id MENUSTATE_ID = MenuState::ID;


template StateRequest makePushRequest<MenuState>();



sf::Color calcClickMessageColor(sf::Time time);

MenuState::MenuState(GameDataRef data)
: State (data)
{
    auto &asset = this->getAssets();


    this->m_title.setString(TITLE);
    this->m_title.setFont(asset.getFont(TITLE_FONT));
    this->m_title.setCharacterSize(TITLE_FONT_SIZE);
    this->m_title.setPosition(
        getCenterX(this->getWindow(), this->m_title),
        TITLE_Y
    );
    this->m_title.setFillColor(TITLE_COLOR);

    this->m_click_to_play_message.setString(CLICK_MESSAGE);
    this->m_click_to_play_message.setFont(asset.getFont(CLICK_FONT));
    this->m_click_to_play_message.setCharacterSize(CLICK_FONT_SIZE);
    this->m_click_to_play_message.setPosition(
        getCenterX(this->getWindow(), this->m_click_to_play_message),
        CLICK_Y
    );
    this->m_click_to_play_message.setFillColor(
        calcClickMessageColor(this->m_message_timer));

    this->m_cursor = &asset.getCursor(DEFAULT_CURSOR);

    SimpleButton highscore_button = makeButton(
        &this->getAssets().getTexture(HIGHSCORE_BUTTON), 
        HIGHSCORE_BUTTON_POS,
        [this] {
            this->addStateRequest(makePushRequest<LeaderBoardState>());
        },
        &this->getAssets().getTexture(HIGHSCORE_BUTTON_HOVER)
    );
    SimpleButton exit_button = makeButton(
        &this->getAssets().getTexture(EXIT_BUTTON), 
        EXIT_BUTTON_POS,
        [this] {
            this->addStateRequest(makeClearRequest());
        },
        &this->getAssets().getTexture(EXIT_BUTTON_HOVER)
    );

    this->m_GUI.addButton(std::move(highscore_button));
    this->m_GUI.addButton(std::move(exit_button));
}

MenuState::~MenuState() = default;


void MenuState::update(sf::Time dt, EventManager &event)
{ 
    this->m_message_timer += dt;
    this->m_click_to_play_message.setFillColor(
        calcClickMessageColor(this->m_message_timer));

    this->m_enemies.update(dt);

    this->randomlySpawnEnemies();

    if (this->m_GUI.respondToEvent(event)) {}
    else if (event.isMouseButtonPressed(sf::Mouse::Left)) {
      auto play_request = makeRequests(makePushRequest<PlayingState>());
      this->addStateRequest(makePushRequest<WipeTransitionState>(
          std::move(play_request)));
      this->pause();
    }
}

void MenuState::render() const
{
    auto &window = this->getWindow();

    window.clear(BACKGROUND_COLOR);
    this->m_enemies.drawTo(window);
    window.draw(this->m_title);
    window.draw(this->m_click_to_play_message);
    this->m_GUI.drawTo(window);
}

void MenuState::asTopState()
{
    this->getWindow().setMouseCursor(*this->m_cursor);
    this->getWindow().setMouseCursorVisible(true);
    this->m_message_timer = sf::Time::Zero;
}


void MenuState::randomlySpawnEnemies()
{
    if (decide(0.005f))
      this->m_enemies.addRandomEnemy(
          this->getRandomSpawnPosition(), {
          randFloat(0, this->getWindow().getSize().x),
          randFloat(0, this->getWindow().getSize().y)
        });
}

Point MenuState::getRandomSpawnPosition() const
{
    ///get a random position outside the window
    const float TOP_Y = -ENEMY_FILTER_MARGIN.y;
    const float BOTTOM_Y = this->getWindow().getSize().y + ENEMY_FILTER_MARGIN.y;
    const float LEFT_X = -ENEMY_FILTER_MARGIN.x;
    const float RIGHT_X = this->getWindow().getSize().x + ENEMY_FILTER_MARGIN.x;

    ///decide wich side to spawn on
    enum Side : int {
        TOP_OR_LEFT,
        BOTTOM_OR_RIGHT
    };
    Side side = Side(randInt(0, 1));
    Vec2 res;
    switch (side) {
      case TOP_OR_LEFT:
        res = {LEFT_X, TOP_Y};
      break;
      case BOTTOM_OR_RIGHT:
        res = {RIGHT_X, BOTTOM_Y};
      break;
    }

    ///get a random position on the side
    float UNROLL_SIDES_LENGTH = (BOTTOM_Y - TOP_Y) + (RIGHT_X - LEFT_X);
    float pos = randFloat(0.f, UNROLL_SIDES_LENGTH, 4096);
    if (pos < (BOTTOM_Y - TOP_Y))
      res.y = TOP_Y + pos;
    else
      res.x = LEFT_X + (pos - (BOTTOM_Y - TOP_Y));

    return res;
}

sf::Color calcClickMessageColor(sf::Time time)
{
    using std::cos;
    sf::Color color = CLICK_COLOR;
    color.a = 255 * (0.4 + 0.2 * cos(time.asSeconds() * 2));
    return color;
}

} // namespace LaserWave