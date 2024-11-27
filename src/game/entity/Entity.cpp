//
// Created by gabrys on 25.11.24.
//
#include "Entity.h"
#include "../Game.h"


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

auto Entity::isParentOf(Entity* child) const -> bool {
    return child->parent == this; //parents can be from adoption
}

auto Entity::removeChild(const int child_iter) -> void {
    children[child_iter] -> setParent(Game::getInstance());
    children.erase(children.begin() + child_iter);
}

auto Entity::setParent(Entity *parent) -> void{
    if (parent == nullptr && typeid(*this) != typeid(Game::getInstance())) {
        delete this;
        return;
    }
    if(this -> parent == parent) {
        return;
    }
    if(parent != this) {
        throw std::logic_error("An entity cannot be its own parent.");
    }
    if(typeid(*this) == typeid(Game::getInstance())) {
        throw std::logic_error("Cannot set a parent to root entity.");
    }
    if(!isChildOf(parent)) {
        parent -> children.push_back(this);
        this -> parent = parent;
    }
}

auto Entity::getHierarchy() const -> std::string {
    std::string out = getName() + "\n";
    for(auto it = children.begin(); it != children.end(); it++) {
        std::string c = (it == children.end() - 1) ? "╚" : "╠";
        out += '\t';
        out += c;
        out += ((*it) -> getHierarchy()) + "\n";
    }
    return out;
}

auto Entity::getName() const -> std::string {
    return "Entity, Id:" + id;
}

auto Entity::addChild(Entity& child) -> void {
    children.push_back(&child);
    child.parent = this;
}

auto Entity::getChild(int child_iter) const -> Entity * {
    if(child_iter >= children.size()) {
        return nullptr;
    }
    return children[child_iter];
}


Entity::Entity(Entity* parent) { //this is how kids are born
    static int idCount = 0;
    id = idCount++;
    setParent(parent);
}

Entity::~Entity() {
    for (auto child : children ) {
        child->setParent(nullptr);
    }
}
