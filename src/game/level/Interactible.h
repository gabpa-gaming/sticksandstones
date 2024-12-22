//
// Created by gabrys on 17.12.24.
//

#ifndef INTERACTIBLE_H
#define INTERACTIBLE_H
#include "Interactor.h"
#include "../entity/CollidableEntity.h"


class Interactible : public virtual CollidableEntity {

public:
    bool canInteract = true;

    auto getClassName() const -> std::string override { return "Interactible";}

    std::function<void(Interactible & thing, Interactor & who)> eventOnInteract =
            [](Interactible & with, Interactor & who) {};

    auto virtual onInteract(Interactor & who) -> void;

    auto create(float range, std::function<void(Interactible & thing, Interactor & who)> onInteractEvent) -> std::unique_ptr<Entity>;
};

#endif //INTERACTIBLE_H
