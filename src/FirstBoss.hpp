#ifndef FIRST_BOSS_HPP_INCLUDED
#define FIRST_BOSS_HPP_INCLUDED

#include "Boss.hpp"

namespace LaserWave
{

class FirstBoss : public Boss INIT_DEBUG_ID(FirstBoss)
{
    using HitboxShape = RegularHitboxConvex;

  public:
    using Hitbox = BasicHitboxRef<HitboxShape>;

    static const Entity::Id ID;

    FirstBoss(Point pos, const Entity *player);

    Entity::Id getId() const noexcept override { return ID; }

    Point getCenter() const noexcept override;
    Hitbox getHitbox() const noexcept;

    sf::Color getSignatureColor() const noexcept final override;
    sf::Color getColor() const noexcept;

    void update(sf::Time dt) override;

    void getHit(Vec2 direction = {}) override;

    int getHealth() const noexcept override;

  protected:
    void draw(sf::RenderTarget&, sf::RenderStates = {}) const override;
    Entity::Hitbox v_getHitbox() const noexcept override;

    void jump();
    float getJumpProbability() const;

  private:

    enum class Phase 
    {
        ENTERING,
        ACTIVE,
        UPON_DEATH
    };
    enum class State 
    {
        NORMAL
    };


    BasicHitbox<HitboxShape> m_hitbox;
    Phase m_phase = Phase::ACTIVE;
    sf::Time m_jumpTimer = sf::Time::Zero;
    State m_state = State::NORMAL;
    sf::Time m_hitTimer = sf::Time::Zero;
    const Entity *m_target;
    int m_health;
};

} // namespace LaserWave

#endif // FIRST_BOSS_HPP_INCLUDED