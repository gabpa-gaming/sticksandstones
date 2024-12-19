//
// Created by gabrys on 17.12.24.
//

#ifndef INTERACTOR_H
#define INTERACTOR_H
#include "../entity/CollidableEntity.h"


class Interactible;

class Interactor : CollidableEntity {

    std::vector<Interactible*> inRange;

    auto setGlobalPos(float x, float y) -> void override;

    auto interactClosest() -> void;

    auto create(float range) -> std::unique_ptr<Entity>;

};



#endif //INTERACTOR_H
