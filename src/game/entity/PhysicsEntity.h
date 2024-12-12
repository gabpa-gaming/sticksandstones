//
// Created by gabrys on 05.12.24.
//

#ifndef PHYSICSENTITY_H
#define PHYSICSENTITY_H

#include "../../headers.h"

#include "CollidableEntity.h"

class PhysicsEntity : public virtual CollidableEntity {

public:
    float topSpeed = 150;

    sf::Vector2f velocity;
    sf::Vector2f acceleration;

    [[nodiscard]] auto getClassName() const -> std::string override {return "PhysicsEntity";}

    auto getName() const -> std::string override;

    auto checkPhysicsMove(sf::Vector2f moveDelta) -> std::vector<CollidableEntity *>; //check if moving would result in collison
                                                                                                            //if so return colliding entities
    auto tryPhysicsMove(sf::Vector2f moveDelta) -> bool; //returns true if move was successful and no colliders are intersecting

    auto virtual physicsUpdate(float deltaT) -> void; //moves entity based on velocity, changes velocity based on the circumstances

    ~PhysicsEntity() = default;
};



#endif //PHYSICSENTITY_H
