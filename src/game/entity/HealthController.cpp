//
// Created by gabrys on 13.12.24.
//

#include "HealthController.h"

#include "TickingEntity.h"


void HealthController::init(Entity* parent) {
    CollidableEntity::init(parent);
    stateMachine = getInParents<TickingEntity>();
}

void HealthController::onCollision(CollidableEntity *other) {
    CollidableEntity::onCollision(other);
    if(auto p = dynamic_cast<HealthController*>(other)) {
        takeDamage(p->contactDamage);
    }
}

auto HealthController::takeDamage(float amount) -> void {

    if(amount == 0) {
        return;
    }
    if(clock.getElapsedTime().asSeconds() < invTime) {
        return;
    }
    clock.restart();
    health -= amount;
    if(stateMachine && amount) {
        stateMachine->setStateByName("damage");
    }
    fmt::println("{} took {} dmg", getName() ,amount);
}


std::unique_ptr<Entity> HealthController::create(float x, float y, std::bitset<8> collisionMask,
    std::bitset<8> collidesWith, float width, float height, float hp,float contactDmg = 0) {
    health = hp;
    contactDamage = contactDmg;
    return CollidableEntity::create(x, y, collisionMask, collidesWith, width, height);
}
