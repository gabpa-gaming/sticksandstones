//
// Created by gabrys on 07.12.24.
//

#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "../entity/TickingEntity.h"
#include "../entity/ControlledPhysicsEntity.h"

class PlayerController : public virtual ControlledPhysicsEntity {

    bool canAttack = true;

    sf::Clock interactClock;
    float interactTime = 0.25f;

public:
    float attackSpeed = 0.5f;
    float range = 0.65f;
    float damage = 2.75f;
    auto physicsUpdate(float deltaT) -> void override;

    auto buildPlayerAttack(sf::Vector2i dir) -> std::unique_ptr<Entity>;

    [[nodiscard]] auto getClassName() const -> std::string override {return "PlayerController";}
};



#endif //PLAYERCONTROLLER_H
