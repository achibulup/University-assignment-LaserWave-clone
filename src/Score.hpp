#ifndef SCORE_HPP_INCLUDED
#define SCORE_HPP_INCLUDED

// include for sf::String
#include <SFML/Graphics/Text.hpp>

namespace LaserWave
{

using Score = std::string;

inline std::string toString(Score score)
{
    return std::move(score);
}

} // namespace LaserWave

#endif // SCORE_HPP_INCLUDED