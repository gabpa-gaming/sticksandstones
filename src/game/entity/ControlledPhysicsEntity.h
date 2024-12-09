//
// Created by gabrys on 07.12.24.
//

#ifndef CONTROLLEDPHYSICSENTITY_H
#define CONTROLLEDPHYSICSENTITY_H

#include "../../headers.h"
#include "Entity.h"
#include "PhysicsEntity.h"

class ControlledPhysicsEntity : public virtual PhysicsEntity {
public:
    sf::Vector2f direction;
    float speedGain = 10;

    [[nodiscard]] auto getClassName() const -> std::string override {return "ControlledPhysicsEntity";}
protected:
    auto physicsUpdate(float deltaT) -> void override;
};



#endif //CONTROLLEDPHYSICSENTITY_H
