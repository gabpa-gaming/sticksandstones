//
// Created by gabrys on 05.12.24.
//

#include "CollidableEntity.h"

#include "../Game.h"

int CollidableEntity::getAsBitMask(ColliderType type) {
    return std::pow<int>(2, static_cast<int>(type));
}

auto CollidableEntity::getColliders() const -> std::vector<sf::FloatRect> {
    return {collider};
}

auto CollidableEntity::setGlobalPos(float x, float y) -> void {
    Entity2D::setGlobalPos(x, y);
    collider = sf::FloatRect(getGlobalPos() + colliderOffset * (float)16*Game::PIXEL_SCALE
        - sf::Vector2f(width,height) * Game::PIXEL_SCALE * (float)8,
        Game::tilePosToScreenCoords({width, height}));
}

auto CollidableEntity::onCollision(CollidableEntity &other) -> void {
    fmt::println("{} collided with {}!", getName(), other.getName());
}

auto CollidableEntity::create()-> std::unique_ptr<Entity> {
    return CollidableEntity::create(0,0);
}

auto CollidableEntity::create(float localX, float localY) -> std::unique_ptr<Entity> {
    std::vector<sf::FloatRect> v;
    return CollidableEntity::create(localX, localY, 0, 0, 1, 1);
}

auto CollidableEntity::create(float x, float y, std::bitset<8> collisionMask, std::bitset<8> collidesWith, float width, float height) -> std::unique_ptr<Entity> {
    auto base = std::move(Entity2D::create(x, y));
    auto p = dynamic_cast<CollidableEntity*>(base.get());
    p -> collisionMask = collisionMask;
    p -> collidesWith = collidesWith;
    p -> width = width;
    p -> height = height;
    p -> collider = sf::FloatRect(getGlobalPos(), Game::tilePosToScreenCoords({width, height}));
    return base;
}
