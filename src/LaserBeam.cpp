#include "LaserBeam.hpp"
#include "constants.hpp"
#include "draw.hpp"

namespace LaserWave
{

LaserBeam::LaserBeam(sf::Vector2f start, sf::Vector2f end)
: Particle(sf::seconds(LASER_BEAM_LIFETIME)), m_start(start), 
  m_angle(toAngle(end - start)), m_length(abs(end - start)) {}

void LaserBeam::onUpdate(sf::Time dt) {}

void LaserBeam::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    float interpolation = this->getTimeLeft().asSeconds() / LASER_BEAM_LIFETIME;

    sf::Uint8 beam_a = lerp(225, 0, 0.7, 0.4, interpolation);

    if (LASER_BEAM_OUTLINE_WIDTH) {
      sf::Color beam_outline = LASER_BEAM_OUTLINE_COLOR;//{225, 205, 77};//{225, 145,77};
      beam_outline.a = beam_a;
      Rect outline;
      outline.setSize({this->m_length, LASER_WIDTH});
      outline.setOrigin({0, LASER_WIDTH / 2.f});
      outline.setRotation(this->m_angle.asDegrees());
      outline.setPosition(this->m_start);
      outline.setFillColor(beam_outline);
      target.draw(outline, states);
    }
    /// draw a rectangle and a circle
    sf::Color beam_color = LASER_BEAM_COLOR;
    beam_color.a = beam_a;
    Rect beam;
    beam.setSize({this->m_length, LASER_WIDTH - 2 * LASER_BEAM_OUTLINE_WIDTH});
    beam.setOrigin({0, beam.getSize().y / 2.f});
    beam.setRotation(this->m_angle.asDegrees());
    beam.setPosition(this->m_start);
    beam.setFillColor(beam_color);


    sf::Uint8 circle_a = lerp(225, 0, 0.8, 0, interpolation);
    sf::Color circle_color = LASER_BEAM_CIRCLE_COLOR;
    circle_color.a = circle_a;
    Circle circle;
    circle.setRadius(FIRECIRCLE_RADIUS * (1 + 0.5f * (1 - interpolation)));
    circle.setCenter(this->m_start);
    // circle.setOutlineThickness(1);
    // circle.setOutlineColor(LASER_BEAM_OUTLINE_COLOR * sf::Color(255, 255, 255, circle_a));

    target.draw(beam, states);
    LaserWave::draw(target, circle, circle_color, states);
}

} // namespace LaserWave