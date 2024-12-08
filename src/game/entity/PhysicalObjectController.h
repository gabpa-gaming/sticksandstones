//
// Created by gabrys on 07.12.24.
//

#ifndef PHYSICALOBJECTCONTROLLER_H
#define PHYSICALOBJECTCONTROLLER_H

#include "../../headers.h"
#include "Entity.h"

class PhysicalObjectController : Entity {
    public:
    [[nodiscard]] auto getClassName() const -> std::string override {return "PhysicalObjectController";}
    auto newInstanceOfThisType() -> std::unique_ptr<Entity> override;
};



#endif //PHYSICALOBJECTCONTROLLER_H
