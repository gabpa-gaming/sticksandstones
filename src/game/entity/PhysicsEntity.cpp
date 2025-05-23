//
// Created by gabrys on 05.12.24.
//

#include "PhysicsEntity.h"
#include "../Game.h"

auto PhysicsEntity::getName() const -> std::string {
    return fmt::format("{}, V: ({},{}), A: ({},{})", CollidableEntity::getName(), velocity.x,velocity.y,acceleration.x,acceleration.y);
}

auto PhysicsEntity::getKnockedBack(sf::Vector2f force, float time) -> void {
    knockbackForce = force;
    knockbackTime = time;
    velocity = {0,0};
}

auto PhysicsEntity::checkPhysicsMove(sf::Vector2f moveDelta) -> std::vector<CollidableEntity *> {
    auto movedCollider = sf::FloatRect(collider.getPosition() + moveDelta, collider.getSize());
    auto collisions = Game::getInstance()->rectCast(movedCollider, collidesWith);
    for (int i = 0; i < collisions.size(); i++) {
        if(collisions[i] == this) {
            collisions.erase(collisions.begin() + i);
            break;
        }
    }
    return collisions;
}

auto PhysicsEntity::tryPhysicsMove(sf::Vector2f moveDelta) -> bool {
    auto cols = checkPhysicsMove(moveDelta);
    if(cols.empty() || ignoreCollisionsInMovement) {
        while(ignoreCollisionsInMovement && !cols.empty()) {
            onCollision(cols[0]);
            cols.erase(cols.begin());
        }
        dislocate(moveDelta.x, moveDelta.y);
        return true;
    }
    while(!cols.empty()) {
        onCollision(cols[0]);
        cols.erase(cols.begin());
    }
    auto colsXIncrement = checkPhysicsMove({moveDelta.x,0});

    if(colsXIncrement.empty()) {
        dislocate(moveDelta.x, 0);
        return false;
    }

    auto colsYIncrement = checkPhysicsMove({0,moveDelta.y});

    if(colsYIncrement.empty()) {
        dislocate(0, moveDelta.y);
    }

    return false;
}

auto PhysicsEntity::physicsUpdate(float deltaT) -> void {
    velocity += acceleration * deltaT;
    if(magnitude(velocity) > topSpeed) {
        velocity = normalize(velocity) * topSpeed;
    }
    sf::Vector2f moveDelta = velocity * deltaT;
    if(knockbackTime > 0) {
        moveDelta += knockbackForce * knockbackTime;
    }

    tryPhysicsMove(moveDelta);
}

