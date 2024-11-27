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


    explicit Entity2D(Entity* parent, double localX, double localY);
    explicit Entity2D(Entity* parent);
};



#endif //ENTITY2D_H
