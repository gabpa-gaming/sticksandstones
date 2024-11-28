//
// Created by gabrys on 25.11.24.
//

#ifndef ENTITY2D_H
#define ENTITY2D_H


#include "Entity.h"


class Entity2D : public virtual Entity{

    double x,y;
    double localX,localY;

    public:

    auto getGlobalPos() const -> sf::Vector2<double>;
    auto getLocalPos() const-> sf::Vector2<double>;

    auto setGlobalPos(double x, double y) -> void;
    auto setLocalPos(double x, double y) -> void;
    auto dislocate(double x, double y) -> void;
    auto getName() const -> std::string override;

    auto create(Entity *parent) -> Entity2D * override;
    auto virtual create(Entity* parent, double localX, double localY) -> Entity2D*;
};



#endif //ENTITY2D_H
