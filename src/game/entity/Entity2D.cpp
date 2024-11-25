//
// Created by gabrys on 25.11.24.
//

#include "Entity2D.h"

auto Entity2D::setGlobalPos(double x, double y) {

}

auto Entity2D::setLocalPos(double x, double y) {
}

auto Entity2D::dislocate(double x, double y) {
}

Entity2D::Entity2D(Entity *parent, double localX, double localY): Entity(parent) {
    setParent(parent);
    setLocalPos(localX, localY);
    this->localX = localX;
    this->localY = localY;
}

Entity2D::Entity2D(Entity *parent) : Entity(parent) {
    setParent(parent);
    this->localX = 0;
    this->localY = 0;
}
