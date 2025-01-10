//
// Created by gabrys on 13.12.24.
//

#ifndef HEALTHCONTROLLER_H
#define HEALTHCONTROLLER_H
#include "CollidableEntity.h"
#include "Entity.h"
#include "TickingEntity.h"


class HealthController : public virtual CollidableEntity {

    float health = 20;

    bool dead = false;

    TickingEntity* stateMachine = nullptr;

    sf::Clock clock;
public:

    Entity * topParentRef = nullptr; //on death will be removed from hierarchy

    std::function<void(Entity&)> onDeathEvent = [&](Entity& e) {};

    float invTime = 0.f;

    float contactDamage = 0;

    auto onDeath() -> void;

    auto init(Entity * parent) -> void override;

    auto getClassName() const -> std::string override {return "HealthController";}

    auto onCollision(CollidableEntity *other) -> void override;

    auto takeDamage(float amount) -> void;

    auto getHealth() -> float;

    auto isDead() -> bool;

    auto virtual create(float x, float y, std::bitset<8> collisionMask, std::bitset<8> collidesWith, float width, float height, float hp, float
                        contactDmg, Entity *topParent) -> std::unique_ptr<Entity>;

};



#endif //HEALTHCONTROLLER_H
