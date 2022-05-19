#include "LaserBeam.hpp"
#include "constants.hpp"

namespace LaserWave
{

LaserBeam::LaserBeam(sf::Vector2f start, sf::Vector2f end)
: m_start(start), m_angle(toAngle(end - start)), m_length(abs(end - start)) {}

void LaserBeam::update(sf::Time dt)
{
    this->m_timeleft -= dt;
    if (this->m_timeleft <= sf::Time::Zero)
      this->m_expired = true;
}

void LaserBeam::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    float interpolation = this->m_timeleft.asSeconds() / LASER_BEAM_LIFETIME;

    /// draw a rectangle and a circle
    sf::Color beam_color = LASER_BEAM_COLOR;
    beam_color.a = lerp(225, 0, 0.7, 0.4, interpolation);
    sf::RectangleShape rect;
    rect.setSize({this->m_length, LASER_WIDTH});
    rect.setOrigin({0, LASER_WIDTH / 2.f});
    rect.setRotation(this->m_angle.asDegrees());
    rect.setPosition(this->m_start);
    rect.setFillColor(beam_color);

    sf::Color circle_color = LASER_BEAM_COLOR;
    circle_color.a = lerp(225, 0, 0.8, 0, interpolation);
    sf::CircleShape circle;
    circle.setRadius(FIRECIRCLE_RADIUS * (1 + 0.5f * (1 - interpolation)));
    circle.setOrigin({circle.getRadius(), circle.getRadius()});
    circle.setPosition(this->m_start);
    circle.setFillColor(circle_color);

    target.draw(rect, states);
    target.draw(circle, states);
}

} // namespace LaserWave