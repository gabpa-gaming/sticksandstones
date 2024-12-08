//
// Created by gabrys on 05.12.24.
//

#ifndef COLLIDABLEENTITY_H
#define COLLIDABLEENTITY_H

#include "Entity2D.h"
#include "../../headers.h"

class CollidableEntity : public virtual Entity2D{
    std::vector<sf::FloatRect> colliders;
public:
    std::bitset<8> collisionMask = 0; //represents object types that can this represents
    std::bitset<8> collidesWith = 0; //represents what object this will collide with

    bool collisionEnabled = true;

    enum ColliderType { //represents bit in the collision mask
        wall,
        player,
        enemy,
        item,
        destructible
    };

    [[nodiscard]] auto getClassName() const -> std::string override {return "CollidableEntity";}

    auto getColliders() -> std::vector<sf::FloatRect>;

    virtual auto onCollision(CollidableEntity& other) -> void;

private:
    auto newInstanceOfThisType() -> std::unique_ptr<Entity> override;
public:
    auto create() -> std::unique_ptr<Entity> override;

    auto create(float localX, float localY) -> std::unique_ptr<Entity> override;

    auto virtual create(float localX, float localY, std::bitset<8> collisionMask, std::bitset<8> collidesWith, std::vector<sf::FloatRect> &colliders) -> std::unique_ptr<Entity>;
};



#endif //COLLIDABLEENTITY_H
