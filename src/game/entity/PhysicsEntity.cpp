//
// Created by gabrys on 05.12.24.
//

#include "PhysicsEntity.h"

auto PhysicsEntity::getName() const -> std::string {
    return fmt::format("{}, V: ({},{}), A: ({},{})", CollidableEntity::getName(), velocity.x,velocity.y,acceleration.x,acceleration.y);
}

auto PhysicsEntity::checkPhysicsMove(sf::Vector2f moveDelta) -> std::vector<std::reference_wrapper<CollidableEntity>> {
    return std::vector<std::reference_wrapper<CollidableEntity>>();
}

auto PhysicsEntity::tryPhysicsMove(sf::Vector2f moveDelta) -> bool {
    dislocate(moveDelta.x, moveDelta.y);
    return true;
}

auto PhysicsEntity::physicsUpdate(float deltaT) -> void {
    fmt::print("PhysicsUpdate\n{}", deltaT);
    velocity += acceleration * deltaT;
    if(magnitude(velocity) > topSpeed) {
        velocity = normalize(velocity) * topSpeed;
    }
    sf::Vector2f moveDelta = velocity * deltaT;
    tryPhysicsMove(moveDelta);
}

auto PhysicsEntity::newInstanceOfThisType() -> std::unique_ptr<Entity> {
    return std::move(std::make_unique<PhysicsEntity>());
}
