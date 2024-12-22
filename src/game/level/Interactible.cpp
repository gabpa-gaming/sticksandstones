//
// Created by gabrys on 17.12.24.
//

#include "Interactible.h"


auto Interactible::onInteract(Interactor &who) -> void {
    eventOnInteract(*this, who);
}

auto Interactible::create(float range,
    std::function<void(Interactible &thing, Interactor &who)> onInteractEvent) -> std::unique_ptr<Entity> {
    auto base = CollidableEntity::create(0,0,getAsBitMask(interactible),0,range,range);
    eventOnInteract = onInteractEvent;
    return base;
}
