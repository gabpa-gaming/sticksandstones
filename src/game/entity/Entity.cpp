//
// Created by gabrys on 25.11.24.
//
#include "Entity.h"
#include "../Game.h"

auto Entity::IS_ROOT_FLAG() -> bool {
    return false;
}
auto Entity::getId() const -> int {
    return id;
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

auto Entity::isParentOf(Entity* child) const -> bool {
    return child->parent == this;
}

auto Entity::removeChild(const int child_iter) -> void {
    children[child_iter] -> setParent(Game::getInstance());
    children.erase(children.begin() + child_iter);
}

auto Entity::setParent(Entity *parent) -> void { //in this codebase adoption is a simple process
    if(IS_ROOT_FLAG() && parent != nullptr) {
        throw std::logic_error("Cannot set a parent to root entity.");
    }
    if (parent == nullptr && !IS_ROOT_FLAG()) {
        delete this;
        return;
    }
    if(this -> parent == parent) {
        return;
    }
    if(parent == this) {
        throw std::logic_error("An entity cannot be its own parent.");
    }
    if(parent == nullptr) { //only case for root
        return;
    }
    if(!isChildOf(parent)) {
        parent -> children.push_back(this);
        this -> parent = parent;
    }
}

auto Entity::getHierarchy() const -> std::string {
    std::string out = "\n";
    for(auto it = children.begin(); it != children.end(); it++) {
        std::string c = (it == children.end() - 1) ? "╚" : "╠";
        out += c;
        out += ((*it) -> getHierarchy());
    }
    return getName() + tabAllLines(out);
}

auto Entity::getName() const -> std::string {
    return fmt::format("Entity, Id:{}", id) ;
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

auto Entity::create(Entity *parent) -> Entity * { //this is how kids are born
    static int idCount = 0;
    this -> id = idCount++;
    this -> setParent(parent);
    return this;
}



Entity::~Entity() {
    for (auto child : children ) {
        child->setParent(nullptr);
    }
}
