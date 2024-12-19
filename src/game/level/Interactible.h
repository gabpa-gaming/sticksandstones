//
// Created by gabrys on 17.12.24.
//

#ifndef INTERACTIBLE_H
#define INTERACTIBLE_H
#include "../entity/CollidableEntity.h"


class Interactible : CollidableEntity {

    bool canInteract = true;

    auto virtual onInteract() -> void;

    auto create(float range) -> std::unique_ptr<Entity>;
};



#endif //INTERACTIBLE_H
