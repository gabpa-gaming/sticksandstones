//
// Created by gabrys on 17.12.24.
//

#include "Interactor.h"

#include "Interactible.h"
#include "../Game.h"

auto Interactor::interactClosest() -> void {
    auto inRange = Game::getInstance()->rectCast(collider, getAsBitMask(interactible));
    std::ranges::sort(inRange, [pos = getGlobalPos()](auto& a, auto& b) {
        auto distA = pos - a->getGlobalPos();
        auto distB = pos - b->getGlobalPos();
        return dot(distA, distA) < dot(distB, distB);
    });
    while(!inRange.empty()) {
        if(inRange[0]->getAs<Interactible>().canInteract) {
            inRange[0]->getAs<Interactible>().onInteract(*this);
            break;
        }
        inRange.erase(inRange.begin());
    }
}

auto Interactor::create(float range) -> std::unique_ptr<Entity> {
    auto base = CollidableEntity::create(0,0,0,0,range,range);
    base->getAs<CollidableEntity>().collisionEnabled = false;
    return base;
}
