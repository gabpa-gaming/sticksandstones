//
// Created by gabrys on 25.11.24.
//
#include "Entity.h"
#include "../Game.h"

auto Entity::IS_ROOT_FLAG() -> bool {
    return false;
}

Entity::Entity() {
    fmt::print("Entity {} created", getName());
}

auto Entity::getId() const -> int {
    return id;
}
/*
auto Entity::isChildOf(std::unique_ptr<Entity>& parent) const -> bool {
    return this -> parent.lock().get() == parent.get();
}
*/
auto Entity::isParentOf(const std::unique_ptr<Entity>& child) const -> bool {
    for (auto &kid: children) {
        if(kid == child) {
            return true;
        }
    }
    return false;
}
/*
auto Entity::setParent(std::unique_ptr<Entity>& parent) -> void { //in this codebase adoption is a simple process
    if(IS_ROOT_FLAG() && parent != nullptr) {
        throw std::logic_error("Cannot set a parent to root entity.");
    }
    if (parent == nullptr && !IS_ROOT_FLAG()) {
        int iter = getChildIter();
        if(iter != -1) {
            this->parent.lock()->children.erase(this -> children.begin() + iter);
        }
        this->parent.reset();
        return;
    }
    if(this -> parent.lock().get() == parent.lock) {
        return;
    }
    if(parent == this) {
        throw std::logic_error("An entity cannot be its own parent.");
    }
    if(parent == nullptr) { //only case for root
        return;
    }
    if(this -> parent == nullptr) {
        parent -> children.push_back(std::make_shared<Entity>(std::move(*this)));
        this -> parent = parent;
        return;
    }
    if(!isChildOf(parent)) {
        int iter = getChildIter();
        auto const ref = std::move(parent->children[iter]);
        parent -> children.push_back(std::make_shared<Entity>(*ref));
        parent -> children.erase(children.begin() + iter);
        this -> parent = parent;
    }
}
/*
auto Entity::getSiblings() const -> std::vector<Entity *> {
    std::vector<Entity *> siblings;
    if(parent == nullptr) {
        throw std::logic_error("Root cant have siblings.");
    }
    for(auto it = parent -> children.begin(); it != parent -> children.end(); it++) {
        siblings.push_back(it->get());
    }
    return siblings;
}
*/
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

auto Entity::addChild(std::unique_ptr<Entity>& child) -> void {
    auto p = std::move(child);
    children.push_back(p);
}

auto Entity::getChild(int child_iter) -> std::unique_ptr<Entity>& {
    if(child_iter >= children.size()) {
        throw std::logic_error(fmt::format("Entity doesnt have at least {} children", child_iter+1)); //call police
    }
    return (children[child_iter]);
}
/*
auto Entity::getChildIter() const -> int {
    if(parent == nullptr){
        return -1;
    }
    for (int i = 0; i < parent -> children.size(); i++) {
        if(parent -> children[i].get() == this) {
            return i;
        }
    }
    return -1;
}
*/
auto Entity::create(std::unique_ptr<Entity>& parent) -> std::unique_ptr<Entity>& { //this is how kids are born
    auto p = std::make_unique<Entity>(*this);
    static int idCount = 0;
    p->id = idCount++;
    parent -> addChild(p);
    return p;
}