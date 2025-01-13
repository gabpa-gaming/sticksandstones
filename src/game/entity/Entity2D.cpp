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

auto Entity2D::updateParentPos(float x, float y) -> void {
    parentX = x;
    parentY = y;
    dislocate(0,0);
}


auto Entity2D::addChild(std::unique_ptr<Entity> child) -> void {
    auto p = dynamic_cast<Entity2D*>(child.get());
    if (p != nullptr) {
        fmt::print("Adding new child\n");
        p -> updateParentPos(x,y);
    }
    return Entity::addChild(std::move(child));
}

auto Entity2D::updateChildrenParentPoses(float x, float y) const -> void {
    for (auto &child : children) {
        auto p = dynamic_cast<Entity2D*>(child.get());
        if (p != nullptr) {
            p -> updateParentPos(x,y);
        }
    }
}

auto Entity2D::setGlobalPos(float x, float y) -> void {
    this -> x = x;
    this -> y = y;
    this -> localX = x - parentX;
    this -> localY = y - parentY;
    for (auto &child : children) {
        auto c = dynamic_cast<Entity2D*>(child.get());
        if(c) {
            c->parentX = x;
            c->parentY = y;
            c->setLocalPos(c->localX,c->localY);
        }
    }
}

auto Entity2D::setLocalPos(const float x, const float y) -> void {
    setGlobalPos(x + parentX,y + parentY);
}

auto Entity2D::dislocate(const float x, const float y) -> void {
    setGlobalPos(this->x + x, this->y + y);
}

auto Entity2D::getName() const -> std::string {
    return fmt::format("{}, At({},{}), Local({},{})",
        Entity::getName(),getGlobalPos().x,getGlobalPos().y, getLocalPos().x,getLocalPos().y);
}

auto Entity2D::create() -> std::unique_ptr<Entity> {
    return Entity2D::create(0,0);
}

auto Entity2D::create( float localX, float localY) -> std::unique_ptr<Entity> {
    auto base = std::move(Entity::create());
    auto p = dynamic_cast<Entity2D*>(base.get());
    p -> x = localX; p -> y = localY;
    p -> localX = localX; p -> localY = localY;
    p -> parentX = 0;
    p -> parentY = 0;
    return base;
}


