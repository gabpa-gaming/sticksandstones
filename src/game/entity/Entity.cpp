//
// Created by gabrys on 25.11.24.
//
#include "Entity.h"
#include "../Game.h"

auto Entity::addChild(Entity& child) {
    children.push_back(&child);
}

auto Entity::removeChild(int child_iter) {
    children[child_iter] -> setParent(nullptr);
    children.erase(children.begin() + child_iter);
}

auto Entity::setParent(Entity *parent) -> void{
    if (parent == nullptr && typeid(this) != typeid(Game::getInstance())) {
        delete this;
    }
    if(this -> parent == parent) {
        return;
    }
    assert(parent != this);
    if(!isChildOf(parent)) {
        parent -> children.push_back(this);
        this -> parent = parent;
    }
}

auto Entity::getChild(int child_iter) const -> Entity * {
    if(child_iter >= children.size()) {
        return nullptr;
    }
    return children[child_iter];
}

auto Entity::isChildOf(const Entity* parent) const -> bool {
    if(parent == nullptr) {
        return false;
    }
    int i =0;
    Entity* child;
    do {
        child = parent -> getChild(i++);
        if(child == this) {
            return true;
        }
    }while(child != nullptr);
    return false;
}

Entity::Entity(Entity* parent) {
    static int idCount = 0;
    id = idCount++;
    setParent(parent);
}
