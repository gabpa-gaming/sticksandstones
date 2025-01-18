//
// Created by gabrys on 18.12.24.
//

#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "../entity/ControlledPhysicsEntity.h"


class Projectile : public virtual ControlledPhysicsEntity {

    std::vector<CollidableEntity*> hit;

    bool killed = false;

public:
    int penetrateCount = 99999;
    bool penetrateWall = true;

    sf::Vector2f projectileKnockbackForce;
    float projectileKnockbackTime = 0;

    std::function<void(Projectile & caller)> onDeath =
            [](Projectile & caller) {};

    auto getClassName() const -> std::string override {return "Projectile";}

    auto onCollision(CollidableEntity *other) -> void override;

    auto kill() -> void;

    float damage;
    float lifetime;
    auto create(float x, float y, std::bitset<8> collisionMask, std::bitset<8> collidesWith, float width, float height, float life,
        float dmg, CollidableEntity *ignore) -> std::unique_ptr<Entity>;
};



#endif //PROJECTILE_H
