//
// Created by gabrys on 25.11.24.
//

#ifndef ENTITY2D_H
#define ENTITY2D_H


#include "Entity.h"

class Entity;

class Entity2D : Entity{

    double x,y;
    double localX,localY;

    explicit Entity2D();

    public:
    auto getGlobalPos();
    auto getLocalPos();

    auto setGlobalPos(double x, double y);
    auto setLocalPos(double x, double y);
    auto dislocate(double x, double y);


    explicit Entity2D(Entity* parent, double localX, double localY);
    explicit Entity2D(Entity* parent);
};



#endif //ENTITY2D_H
