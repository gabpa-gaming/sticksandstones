//
// Created by gabrys on 07.12.24.
//

#include "PhysicalObjectController.h"

auto PhysicalObjectController::newInstanceOfThisType() -> std::unique_ptr<Entity> {
    return std::move(std::make_unique<PhysicalObjectController>());
}
