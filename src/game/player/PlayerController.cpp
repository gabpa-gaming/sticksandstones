//
// Created by gabrys on 07.12.24.
//

#include "PlayerController.h"
auto PlayerController::newInstanceOfThisType() -> std::unique_ptr<Entity> {
    return std::move(std::make_unique<PlayerController>());
}