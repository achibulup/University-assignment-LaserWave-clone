#include "ScoreSystem.hpp"
#include <fstream>

namespace LaserWave
{

static std::ostream& operator << (std::ostream &os, const sf::String &str)
{
    for (sf::Uint32 c : str)
      os << c << ' ';
    return os << sf::Uint32();
} 
static std::istream& operator >> (std::istream &is, sf::String &str)
{
    sf::Uint32 c;
    str.clear();
    while (is >> c && c)
      str += c;
    return is;
}

/// serializes the score entry
static std::ostream& operator << (std::ostream &os, const ScoreEntry &entry)
{
    os << entry.playerName << " " << entry.score;
    return os;
}
/// deserializes the score entry
static std::istream& operator >> (std::istream &in, ScoreEntry &entry)
{
    in >> entry.playerName >> entry.score;
    return in;
}

/// serializes the leaderboard
static std::ostream& operator << (std::ostream &out, const List<ScoreEntry> &list)
{
    for (const auto &entry : list)
      out << entry << '\n';
    return out;
}
/// deserializes the score
static std::istream& operator >> (std::istream &in, List<ScoreEntry> &list)
{
    ScoreEntry tmp;
    while(in >> tmp) list.push_back(tmp);
    return in;
}




ScoreSystem::ScoreSystem(int leaderboard_size)
: m_leaderBoardMaxSize(leaderboard_size) {}
ScoreSystem::ScoreSystem(const std::string &file_path, int leaderboard_size)
: ScoreSystem(leaderboard_size)
{
    this->load();
}
ScoreSystem::~ScoreSystem()
{
    if (!this->m_fileName.empty()) this->save();
}

void ScoreSystem::setLeaderboardMaxSize(int leaderboard_size)
{
    this->m_leaderBoardMaxSize = leaderboard_size;
}

int ScoreSystem::addScore(ScoreEntry score)
{
    this->m_lastPlayerName = score.playerName;

    int rank = 1;
    auto it = this->m_leaderBoard.begin();
    auto end = this->m_leaderBoard.end();
    for (; it != end; ++it) {
      if (score < *it) ++rank;
      else break;
    }
    if (rank > this->m_leaderBoardMaxSize) 
      return -1;
    this->m_leaderBoard.insert(it, std::move(score));
    if (this->m_leaderBoard.size() > this->m_leaderBoardMaxSize)
      this->m_leaderBoard.pop_back();
    return rank;
}

sf::String ScoreSystem::getLastPlayerName() const
{
    return this->m_lastPlayerName;
}
List<ScoreEntry> ScoreSystem::getLeaderBoard() const
{
    return this->m_leaderBoard;
}

void ScoreSystem::load(const std::string &file_path)
{
    this->setSaveFile(file_path);
    this->load();
}

void ScoreSystem::setSaveFile(const std::string &file_path)
{
    this->m_fileName = file_path;
}

void ScoreSystem::save() const
{
    if (this->m_fileName.empty()) 
      throw std::runtime_error("ScoreSystem::save: save file not specified");
    this->saveAs(this->m_fileName);
}

void ScoreSystem::saveAs(const std::string &file_path) const
{
    std::ofstream save_file(file_path);
    if (!save_file)
      throw std::runtime_error("ScoreSystem::saveAs: could not open file");
    save_file << this->m_lastPlayerName << "\n" << this->m_leaderBoard;
}

void ScoreSystem::load()
{
    std::ifstream load_file(this->m_fileName);
    if (!load_file) return;
      // throw std::runtime_error("ScoreSystem::load: could not open file");
    load_file >> this->m_lastPlayerName >> this->m_leaderBoard;
    if (this->m_leaderBoard.size() > this->m_leaderBoardMaxSize)
      this->m_leaderBoard.resize(this->m_leaderBoardMaxSize);
}

} // namespace LaserWave