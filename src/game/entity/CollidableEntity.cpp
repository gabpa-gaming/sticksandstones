//
// Created by gabrys on 05.12.24.
//

#include "CollidableEntity.h"

auto CollidableEntity::getColliders() -> std::vector<sf::FloatRect> {
    return colliders;
}

auto CollidableEntity::onCollision(CollidableEntity &other) -> void {
    fmt::print("{} collided with {}!", getName(), other.getName());
}

std::unique_ptr<Entity> CollidableEntity::create() {
    return CollidableEntity::create(0,0);
}

std::unique_ptr<Entity> CollidableEntity::create(float localX, float localY) {
    std::vector<sf::FloatRect> v;
    return CollidableEntity::create(localX, localY, 0, 0, v);
}

auto CollidableEntity::create(float localX, float localY, std::bitset<8> collisionMask, std::bitset<8> collidesWith,
    std::vector<sf::FloatRect>& colliders) -> std::unique_ptr<Entity> {
    auto base = std::move(Entity2D::create(localX, localY));
    auto p = dynamic_cast<CollidableEntity*>(base.get());
    p -> collisionMask = collisionMask;
    p -> collidesWith = collidesWith;
    p -> colliders = colliders;
    return base;
}
