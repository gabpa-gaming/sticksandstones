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
    auto p = dynamic_cast<HealthController*>(other);
    if(p) {
        takeDamage(p->contactDamage);
    }
}

auto HealthController::takeDamage(float amount) -> void {
    health -= amount;
    if(stateMachine && amount>0) {
        stateMachine->setStateByName("damage");
    }
    fmt::println("{} took {} dmg", getName() ,health);
}


std::unique_ptr<Entity> HealthController::create(float x, float y, std::bitset<8> collisionMask,
    std::bitset<8> collidesWith, float width, float height, float hp,float contactDmg = 0) {
    health = hp;
    return CollidableEntity::create(x, y, collisionMask, collidesWith, width, height);
}
