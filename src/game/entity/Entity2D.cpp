//
// Created by gabrys on 25.11.24.
//

#include "Entity2D.h"

#include "../../headers.h"

auto Entity2D::getGlobalPos() const -> sf::Vector2<float> {
    return {x,y};
}

auto Entity2D::getLocalPos() const -> sf::Vector2<float> {
    return {localX,localY};
}

auto Entity2D::setGlobalPos(float x, float y) -> void {
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
    for (auto &child : children) {
        auto c = dynamic_cast<Entity2D*>(child.get());
        c->setLocalPos(c -> getLocalPos().x, c -> getLocalPos().y); //should work
    }
}

auto Entity2D::setLocalPos(const float x, const float y) -> void {
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

    for (int i =  0; i < children.size(); i++) {
        auto c = dynamic_cast<Entity2D*>(children[i].get());
        c->setLocalPos(c -> getLocalPos().x, c -> getLocalPos().y); //reset local pos to itself so it properly changes
    }                                                               //its global pos and updates children as well
}

auto Entity2D::dislocate(const float x, const float y) -> void {
    setGlobalPos(this->x + x, this->y + y);
}

auto Entity2D::getName() const -> std::string {
    return fmt::format("Entity2D, Id({}), At({},{}), Local({},{})",
        getId(),getGlobalPos().x,getGlobalPos().y, getLocalPos().x,getLocalPos().y);
}

auto Entity2D::create(std::unique_ptr<Entity>& parent)-> std::unique_ptr<Entity>& override : create(parent, 0,0);

auto Entity2D::create(std::unique_ptr<Entity>& parent, float localX, float localY) -> std::unique_ptr<Entity>& {
    auto base = &Entity::create(parent); //Evil pointer to poiner
    auto p = dynamic_cast<Entity2D*>(base -> get());
    p -> get() -> x = localX; p -> get() -> y = localY;
    p -> get() -> localX = localX; p -> get() -> localY = localY;
    return *base;
}

