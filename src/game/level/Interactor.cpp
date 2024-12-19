//
// Created by gabrys on 17.12.24.
//

#include "Interactor.h"

void Interactor::setGlobalPos(float x, float y) {
    CollidableEntity::setGlobalPos(x, y);
}

auto Interactor::interactClosest() -> void {

}

auto Interactor::create(float range) -> std::unique_ptr<Entity> {

}
