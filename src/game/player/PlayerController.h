//
// Created by gabrys on 07.12.24.
//

#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H
#include "../IEventHandler.h"
#include "../entity/ControlledPhysicsEntity.h"
#include "../entity/TickingEntity.h"


class PlayerController : public virtual ControlledPhysicsEntity {

public:
    auto physicsUpdate(float deltaT) -> void override;

    [[nodiscard]] auto getClassName() const -> std::string override {return "PlayerController";}
};



#endif //PLAYERCONTROLLER_H
