//
// Created by gabrys on 07.12.24.
//

#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "../entity/TickingEntity.h"
#include "../entity/ControlledPhysicsEntity.h"

class Projectile;

class PlayerController : public virtual ControlledPhysicsEntity {

    bool canAttack = true;

public:
    float attackSpeed = 0.5f;
    float range = 0.65f;
    auto physicsUpdate(float deltaT) -> void override;

    auto buildPlayerAttack(sf::Vector2i dir) -> std::unique_ptr<Entity>;

    [[nodiscard]] auto getClassName() const -> std::string override {return "PlayerController";}
};



#endif //PLAYERCONTROLLER_H
