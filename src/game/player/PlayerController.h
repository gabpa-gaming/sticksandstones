//
// Created by gabrys on 07.12.24.
//

#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H
#include "../entity/PhysicalObjectController.h"
#include "../entity/TickingEntity.h"


class PlayerController : public virtual PhysicalObjectController {

public:

    [[nodiscard]] auto getClassName() const -> std::string override {return "PhysicsEntity";}
private:
    auto newInstanceOfThisType() -> std::unique_ptr<Entity> override;
};



#endif //PLAYERCONTROLLER_H
