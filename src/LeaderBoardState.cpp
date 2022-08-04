#include "State.hpp"
#include "stateRequests.hpp"
#include "assets.hpp"
#include "draw.hpp"
#include "constants.hpp"
#include "SimpleGUI.hpp"
#include "ScoreSystem.hpp"


namespace LaserWave
{

class LeaderBoardState : public State
{
  public:
    static const State::Id ID;

    explicit LeaderBoardState(GameDataRef);
    LeaderBoardState(const LeaderBoardState&) = delete;
    void operator = (LeaderBoardState) = delete;
    ~LeaderBoardState();

    State::Id getId() const noexcept override { return ID; }

    void update(sf::Time dt, EventManager&) override;
    void render() const override;

  private:
    sf::Text m_title;
    SimpleGUI m_GUI;
};
const State::Id LeaderBoardState::ID ("LeaderBoardState");
const State::Id LEADERBOARDSTATE_ID = LeaderBoardState::ID;


template StateRequest makePushRequest<LeaderBoardState>();



LeaderBoardState::LeaderBoardState(GameDataRef data)
: State (data), m_title () 
{
    this->m_title.setString("LEADERBOARD");
    this->m_title.setFont(this->getAssets().getFont(LEADERBOARD_FONT));
    this->m_title.setCharacterSize(TITLE_FONT_SIZE);
    this->m_title.setPosition(
        getCenterX(this->getWindow(), this->m_title),
        TITLE_Y
    );
    this->m_title.setFillColor(LEADERBOARD_COLOR);

    SimpleButton back_button = makeButton(
        &this->getAssets().getTexture(BACK_BUTTON),
        EXIT_BUTTON_POS,
        [this] {
            this->addStateRequest(makePopRequest());
        },
        &this->getAssets().getTexture(BACK_BUTTON_HOVER)
    );
    this->m_GUI.addButton(std::move(back_button));
}

LeaderBoardState::~LeaderBoardState() = default;

void LeaderBoardState::update(sf::Time, EventManager &em)
{
    if (this->m_GUI.respondToEvent(em)) {}
    else if (em.isKeyPressed(sf::Keyboard::Escape)) {
      this->addStateRequest(makePopRequest());
    }
    else if (em.isMouseButtonPressed(sf::Mouse::Right)) {
      this->addStateRequest(makePopRequest());
    }
}

void LeaderBoardState::render() const
{
    this->getWindow().clear(BACKGROUND_COLOR);

    this->getWindow().draw(this->m_title);
    sf::Text name;
    sf::Text score;
    name.setFont(this->getAssets().getFont(NAME_FONT));
    name.setCharacterSize(CLICK_FONT_SIZE);
    name.setFillColor(sf::Color::Yellow);

    score.setFont(this->getAssets().getFont(SCORE_FONT));
    score.setCharacterSize(CLICK_FONT_SIZE);
    score.setFillColor(sf::Color::Yellow);

    auto leaderboard = this->getScoreSystem().getLeaderBoard();
    for (int i = 0; i < leaderboard.size(); ++i) {
      auto &&entry = leaderboard[i];
      name.setString(entry.playerName.isEmpty() ? 
          "<Anonymous>" : entry.playerName);
      name.setPosition(LEADERBOARD_NAME_X, 300 + 70 * i);
      score.setString(entry.score);
      score.setPosition(LEADERBOARD_SCORE_X, 300 + 70 * i);
      this->getWindow().draw(name);
      this->getWindow().draw(score);
    }

    this->m_GUI.drawTo(this->getWindow());
}

} // namespace LaserWave