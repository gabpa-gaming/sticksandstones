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

    TickingEntity* stateMachine = nullptr;

    sf::Clock clock;
    public:
    float invTime = 0.f;

    float contactDamage = 0;

    auto init(Entity * parent) -> void override;

    auto getClassName() const -> std::string override {return "HealthController";}

    auto onCollision(CollidableEntity *other) -> void override;

    auto takeDamage(float amount) -> void;


    auto virtual create(float x, float y, std::bitset<8> collisionMask, std::bitset<8> collidesWith, float width, float height, float hp, float
                        contactDmg) -> std::unique_ptr<Entity>;

};



#endif //HEALTHCONTROLLER_H
