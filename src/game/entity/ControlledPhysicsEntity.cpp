//
// Created by gabrys on 07.12.24.
//

#include "ControlledPhysicsEntity.h"

#include "SpriteEntity.h"
#include "../../headers.h"

void ControlledPhysicsEntity::physicsUpdate(float deltaT) {
    if(magnitude(direction) > 0.0f) {
        acceleration = speedGain * direction;
    }
    else {
        auto b = velocity;
        auto m = magnitude(velocity);
        acceleration = sf::Vector2f(0,0);
        velocity -= (m > 1 ? normalize(velocity) : velocity) * deltaT * speedGain;
        if(magnitude(b - velocity) <= magnitude(b)) {
            velocity = sf::Vector2f(0,0);
        }
    }
    bool flip = velocity.x > 0;
    if(velocity != sf::Vector2f(0,0)) {

        getChildOfTypeRecursive<SpriteEntity>() -> setFlip(velocity.x < 0);
    }
    PhysicsEntity::physicsUpdate(deltaT);
}
