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
}


void Entity2D::addChild(std::unique_ptr<Entity> &child) {
    Entity::addChild(std::move(child));
    auto p = dynamic_cast<Entity2D*>(child.get());
    if (p != nullptr) {
        p -> updateParentPos(x,y);
    }
}

auto Entity2D::updateChildrenParentPoses(float x, float y) -> void {
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
        c->setLocalPos(c -> getLocalPos().x, c -> getLocalPos().y); //should work
    }
    updateChildrenParentPoses(x,y);
}

auto Entity2D::setLocalPos(const float x, const float y) -> void {
    localX = x;
    localY = y;
    this -> x = parentX + localX;
    this -> y = parentY + localY;

    for (int i =  0; i < children.size(); i++) {
        auto c = dynamic_cast<Entity2D*>(children[i].get());
        c->setLocalPos(c -> getLocalPos().x, c -> getLocalPos().y); //reset local pos to itself so it properly changes
    }
    //its global pos and updates children as well
    updateChildrenParentPoses(this->x,this->y);
}

auto Entity2D::dislocate(const float x, const float y) -> void {
    setGlobalPos(this->x + x, this->y + y);
}

auto Entity2D::getName() const -> std::string {
    return fmt::format("Entity2D, Id({}), At({},{}), Local({},{})",
        getId(),getGlobalPos().x,getGlobalPos().y, getLocalPos().x,getLocalPos().y);
}

auto Entity2D::create() -> std::unique_ptr<Entity> {
    return create(0,0);
}

auto Entity2D::create( float localX, float localY) -> std::unique_ptr<Entity> { //ugly, fix maybe ////or never 😈😈😈😈 //////This comment has its own comment ////////This too //////////This too
    auto base = std::move(Entity::create());                                                        //^ an example of infinite recursion
    auto p = dynamic_cast<Entity2D*>(base.get());
    p -> x = localX; p -> y = localY;
    p -> localX = localX; p -> localY = localY;
    return base;
}

auto Entity2D::newInstanceOfThisType() -> std::unique_ptr<Entity> {
    return std::move(std::make_unique<Entity2D>());
}

