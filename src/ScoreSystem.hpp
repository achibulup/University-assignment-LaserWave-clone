#ifndef SCORESYSTEM_HPP_INCLUDED
#define SCORESYSTEM_HPP_INCLUDED

#include "commons.hpp"
#include "Score.hpp"

#include <string>

namespace LaserWave
{

#define LEADERBOARD_SIZE 10

class ScoreSystem
{
  public:
    ScoreSystem(int leaderboard_size = LEADERBOARD_SIZE);
    ScoreSystem(const std::string &file_path, 
                int leaderboard_size = LEADERBOARD_SIZE);
    ~ScoreSystem();

    void setLeaderboardMaxSize(int leaderboard_size);

    /// returns the rank of the added score
    /// if the score is not in the leaderboard, returns -1
    int addScore(ScoreEntry score);

    /// returns the rank of the added score
    //int addScore(const char *name, int score);

    sf::String getLastPlayerName() const;
    List<ScoreEntry> getLeaderBoard() const;

    void load(const std::string &file_path);

    void setSaveFile(const std::string &file_path);
    void save() const;
    /// WARNING: this function will not change the this' save file
    void saveAs(const std::string &file_path) const;

  private:
    void load();

    std::string m_fileName;
    sf::String m_lastPlayerName;
    List<ScoreEntry> m_leaderBoard;
    int m_leaderBoardMaxSize;
};

}

#endif // SCORESYSTEM_HPP_INCLUDED