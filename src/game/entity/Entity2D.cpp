//
// Created by gabrys on 25.11.24.
//

#include "Entity2D.h"
#include "../../headers.h"
auto Entity2D::getGlobalPos() const -> sf::Vector2<double> {
    return {x,y};
}

auto Entity2D::getLocalPos() const -> sf::Vector2<double> {
    return {localX,localY};
}

auto Entity2D::setGlobalPos(double x, double y) -> void {
    this -> x = x;
    this -> y = y;
    auto p = dynamic_cast<Entity2D*>(this->parent);
    if(p == nullptr) {
        this -> x = localX;
        this -> y = localY;
        return;
    }
    auto parent_pos = p -> getGlobalPos();
    this -> localX = parent_pos.x - x;
    this -> localY = parent_pos.y - y;
}

auto Entity2D::setLocalPos(const double x, const double y) -> void {
    localX = x;
    localY = y;
    auto p = dynamic_cast<Entity2D*>(this->parent);
    if(p == nullptr) {
        this -> x = localX;
        this -> y = localY;
        return;
    }
    auto parent_pos = p -> getGlobalPos();
    this -> x = parent_pos.x - localX;
    this -> y = parent_pos.y - localY;
}

auto Entity2D::dislocate(const double x, const double y) -> void {
    setGlobalPos(this->x + x, this->y + y);
}

Entity2D::Entity2D(Entity *parent, const double localX, const double localY): Entity(parent) {
    setParent(parent);
    setLocalPos(localX, localY);
}

Entity2D::Entity2D(Entity *parent) : Entity(parent) {
    setParent(parent);
    setLocalPos(0,0);
}
