//
// Created by gabrys on 13.12.24.
//

#include "HealthController.h"

#include "TickingEntity.h"


auto HealthController::onDeath() -> void {
    dead = true;
    onDeathEvent(*this);
    if(topParentRef) topParentRef->endOfFrameRemove();
}


void HealthController::init(Entity* parent) {
    CollidableEntity::init(parent);
    stateMachine = getInParents<TickingEntity>();
}

void HealthController::onCollision(CollidableEntity *other) {
    CollidableEntity::onCollision(other);
    if(auto p = dynamic_cast<HealthController*>(other)) {
        p -> takeDamage(contactDamage);
    }
}

auto HealthController::takeDamage(float amount) -> void {
    if(amount == 0 || dead) {
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
    fmt::println("{} took {} dmg, new health: {}", getName() ,amount, health);
    if(health <= 0) {
        onDeath();
    }
}

auto HealthController::getHealth() -> float {
    return health;
}

auto HealthController::isDead() -> bool {
    return dead;
}

std::unique_ptr<Entity> HealthController::create(float x, float y, std::bitset<8> collisionMask,
                                                 std::bitset<8> collidesWith, float width, float height, float hp,float contactDmg = 0, Entity* topParent = nullptr) {
    health = hp;
    contactDamage = contactDmg;
    topParentRef = topParent;
    return CollidableEntity::create(x, y, collisionMask, collidesWith, width, height);
}
