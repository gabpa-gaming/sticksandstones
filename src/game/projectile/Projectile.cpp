//
// Created by gabrys on 18.12.24.
//

#include "Projectile.h"

#include "../entity/HealthController.h"

void Projectile::onCollision(CollidableEntity *other) {
    if(std::ranges::find(hit, other) != hit.end()) {
        return;
    }
    auto hp = dynamic_cast<HealthController*>(other);
    if(hp) {
        hp->takeDamage(damage);
    }
    hit.push_back(other);
    ControlledPhysicsEntity::onCollision(other);
}

auto Projectile::kill() -> void {
    onDeath(*this);
    parent->endOfFrameRemove();
}

auto Projectile::create(float x, float y, std::bitset<8> collisionMask, std::bitset<8> collidesWith, float width,
                        float height, float life, float dmg, CollidableEntity *ignore) -> std::unique_ptr<Entity> {
    auto base = ControlledPhysicsEntity::create(x,y,collisionMask,collidesWith,width,height);
    if(ignore != nullptr) {
        hit.push_back(ignore);
    }
    damage = dmg;
    lifetime = life;
    return std::move(base);
}
