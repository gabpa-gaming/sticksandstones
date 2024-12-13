//
// Created by gabrys on 05.12.24.
//

#ifndef COLLIDABLEENTITY_H
#define COLLIDABLEENTITY_H

#include "Entity2D.h"
#include "../../headers.h"

class CollidableEntity : public virtual Entity2D{
public:
    std::bitset<8> collisionMask = 0; //represents object types that can this represents
    std::bitset<8> collidesWith = 0; //represents what object this will collide with

    sf::FloatRect collider;
    float width;
    float height;
    sf::Vector2f colliderOffset;
    bool collisionEnabled = true;

    enum ColliderType { //represents bit in the collision mask
        wall,
        player,
        enemy,
        item,
        destructible,
        projectile,
    };

    static int getAsBitMask(ColliderType type);

    [[nodiscard]] auto getClassName() const -> std::string override {return "CollidableEntity";}

    auto getColliders() const -> std::vector<sf::FloatRect>;

    auto setGlobalPos(float x, float y) -> void override;

    auto checkPosForCollisons() const -> std::vector<CollidableEntity *>;

    virtual auto onCollision(CollidableEntity *other) -> void;

    auto create() -> std::unique_ptr<Entity> override;

    auto create(float localX, float localY) -> std::unique_ptr<Entity> override;

    auto virtual create(float x, float y, std::bitset<8> collisionMask, std::bitset<8> collidesWith, float width, float height) -> std::unique_ptr<Entity>;
};



#endif //COLLIDABLEENTITY_H
