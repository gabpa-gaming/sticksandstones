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
    this -> localX = x - parent_pos.x;
    this -> localY = y - parent_pos.y;
    for (auto child : children) {
        auto c = dynamic_cast<Entity2D*>(child);
        c->setLocalPos(c -> getLocalPos().x, c -> getLocalPos().y); //should work
    }
}

auto Entity2D::setLocalPos(const double x, const double y) -> void {
    localX = x;
    localY = y;
    auto p = dynamic_cast<Entity2D*>(this->parent);
    if(p == nullptr) {
        this -> x = localX;
        this -> y = localY;
    }
    else {
        auto parent_pos = p -> getGlobalPos();
        this -> x = parent_pos.x + localX;
        this -> y = parent_pos.y + localY;
    }

    for (auto child : children) {
        auto c = dynamic_cast<Entity2D*>(child);
        c->setLocalPos(c -> getLocalPos().x, c -> getLocalPos().y); //reset local pos to itself so it properly changes
    }                                                               //its global pos and updates children as well
}

auto Entity2D::dislocate(const double x, const double y) -> void {
    setGlobalPos(this->x + x, this->y + y);
}

auto Entity2D::getName() const -> std::string {
    return fmt::format("Entity2D, Id({}), At({},{}), Local({},{})",
        getId(),getGlobalPos().x,getGlobalPos().y, getLocalPos().x,getLocalPos().y);
}

auto Entity2D::create(Entity *parent)-> Entity2D* {
    Entity::create(parent);
    this -> setLocalPos(0,0);
    return this;
}

auto Entity2D::create(Entity *parent, double localX, double localY) -> Entity2D * {
    Entity::create(parent);
    this -> setParent(parent);
    this -> setLocalPos(localX, localY);
    return this;
}

