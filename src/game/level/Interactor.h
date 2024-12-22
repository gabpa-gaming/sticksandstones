//
// Created by gabrys on 17.12.24.
//

#ifndef INTERACTOR_H
#define INTERACTOR_H
#include "../entity/CollidableEntity.h"



class Interactor : public virtual CollidableEntity {
public:
    [[nodiscard]] auto getClassName() const -> std::string override { return "Interactor";}

    auto interactClosest() -> void;

    auto create(float range) -> std::unique_ptr<Entity>;

};



#endif //INTERACTOR_H
