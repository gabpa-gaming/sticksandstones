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
    collider = sf::FloatRect(getGlobalPos() + colliderOffset * 16.f *Game::PIXEL_SCALE
        - sf::Vector2f(width,height) * Game::PIXEL_SCALE * 8.f,
        Game::tilePosToScreenCoords({width, height}));
    auto col = checkPosForCollisons();
    while(!col.empty()) {
        col[0]->onCollision(this);
        col.erase(col.begin());
    }
}

auto CollidableEntity::checkPosForCollisons() const -> std::vector<CollidableEntity *> {
    auto movedCollider = sf::FloatRect(collider.getPosition(), collider.getSize());
    auto collisions = Game::getInstance()->rectCast(movedCollider, collidesWith);
    for (int i = 0; i < collisions.size(); i++) {
        if(collisions[i] == this) {
            collisions.erase(collisions.begin() + i);
            break;
        }
    }
    return collisions;
}

auto CollidableEntity::onCollision(CollidableEntity *other) -> void {
    //fmt::println("{} collided with {}!", getName(), other->getName());
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
    this -> collisionMask = collisionMask;
    this -> collidesWith = collidesWith;
    this -> width = width;
    this -> height = height;
    collider = sf::FloatRect(getGlobalPos(), Game::tilePosToScreenCoords({width, height}));
    return base;
}
