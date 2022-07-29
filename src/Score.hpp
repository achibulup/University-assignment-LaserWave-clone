#ifndef SCORE_HPP_INCLUDED
#define SCORE_HPP_INCLUDED

// include for sf::String
#include <SFML/Graphics/Text.hpp>

namespace LaserWave
{

using Score = std::string;

struct ScoreEntry
{
    sf::String playerName;
    Score score;
};

inline bool operator < (const ScoreEntry &lhs, const ScoreEntry &rhs)
{
    return lhs.score < rhs.score;
}



} // namespace LaserWave

#endif // SCORE_HPP_INCLUDED