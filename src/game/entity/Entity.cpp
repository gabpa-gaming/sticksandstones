//
// Created by gabrys on 25.11.24.
//
#include "Entity.h"

#include "HealthController.h"
#include "SpriteEntity.h"
#include "../Game.h"
#include "../player/PlayerController.h"

auto Entity::IS_ROOT_FLAG() -> bool {
    return false;
}

Entity::Entity() {
    fmt::println("{} created", Entity::getName());
}

Entity::~Entity() {
    fmt::println("{} deleted", Entity::getName());
}

auto Entity::init(Entity *parent) -> void {
    initialized = true;
    this->parent = parent;
    fmt::println("{} initialized", Entity::getName());
}

auto Entity::initAllChildren(Entity *parent) -> void {
    init(parent);
    for (auto &child : children) {
        child->initAllChildren(this);
    }
}

auto Entity::getId() const -> int {
    return id;
}

template<typename E>
auto Entity::getAs() -> Entity & {
    static_assert(std::is_base_of_v<Entity, E>, "E must derive from Entity");
    return static_cast<E&>(*this);
}

template<typename E>
auto Entity::getChildOfType() const -> E* { //returns the first one in hierarchy, null if not found
    static_assert(std::is_base_of_v<Entity, E>, "E must derive from Entity");
    for (auto& child : children) {
        auto p = dynamic_cast<E*>(child.get());
        if (p) {
            return p; // Found a child of type C
        }
    }
    return nullptr;
}

template<typename E>
auto Entity::getChildOfTypeRecursive() const -> E* { //returns the first one in hierarchy, null if not found
    static_assert(std::is_base_of_v<Entity, E>, "E must derive from Entity");
    auto c = getChildOfType<E>();
    if(!c) {
        for (auto& child : children) {
            c = child.get()->getChildOfTypeRecursive<E>();
            if(c) {
                break;
            }
        }
    }
    return c;
}

template<typename E>
auto Entity::getAllChildrenOfType() const -> std::vector<Entity*> {
    static_assert(std::is_base_of_v<Entity, E>, "E must derive from Entity");
    std::vector<Entity*> out;
    for (auto& child : children) {
        auto p = dynamic_cast<E*>(child.get());
        if(p) {
            out.push_back(p);
        }
    }
    return out;
}

template<typename E>
auto Entity::getAllChildrenOfTypeRecursive() const -> std::vector<Entity*> {
    static_assert(std::is_base_of_v<Entity, E>, "E must derive from Entity");
    std::vector<Entity*> out;
    for (auto& child : children) {
        auto p = dynamic_cast<E*>(child.get());
        if(p) {
            out.push_back(p);

        }
        auto a = child -> getAllChildrenOfTypeRecursive<E>();
        out.insert(out.end(), a.begin(), a.end());
    }
    return out;
}

template<typename E>
auto Entity::getInParents() -> E* {
    static_assert(std::is_base_of_v<Entity, E>, "E must derive from Entity");
    auto p = dynamic_cast<E*>(this->parent);
    if(p) {
        return p;
    }
    if(parent == nullptr) {
        throw std::logic_error("Entity not found in parents");
    }
    return parent->getInParents<E>();
}

auto Entity::isParentOf(const std::unique_ptr<Entity>& child) const -> bool {
    for (auto &kid: children) {
        if(kid == child) {
            return true;
        }
    }
    return false;
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
    if(id == -1) {
        return "Uninitialized entity";
    }
    return fmt::format("{}, Id:{}", getClassName(), id) ;
}

auto Entity::addChild(std::unique_ptr<Entity> child) -> void {
    children.push_back(std::move(child));
}

auto Entity::getChild(int child_iter) -> std::unique_ptr<Entity>& {
    if(child_iter >= children.size()) {
        throw std::logic_error(fmt::format("Entity doesnt have at least {} children", child_iter+1));
    }
    return (children[child_iter]);
}

auto Entity::create() -> std::unique_ptr<Entity> { //this is how kids are born
    auto p = std::unique_ptr<Entity>(this);
    static int idCount = 0;
    p->id = idCount++;
    return std::move(p);
}

template auto Entity::getAllChildrenOfTypeRecursive<PhysicsEntity>() const -> std::vector<Entity*>;
template auto Entity::getAllChildrenOfTypeRecursive<CollidableEntity>() const -> std::vector<Entity*>;
template auto Entity::getAllChildrenOfTypeRecursive<SpriteEntity>() const -> std::vector<Entity*>;
template auto Entity::getAllChildrenOfTypeRecursive<TickingEntity>() const -> std::vector<Entity*>;

template auto Entity::getChildOfTypeRecursive<SpriteEntity>() const -> SpriteEntity*;
template auto Entity::getChildOfTypeRecursive<TickingEntity>() const -> TickingEntity*;
template auto Entity::getChildOfTypeRecursive<HealthController>() const -> HealthController*;
template auto Entity::getChildOfTypeRecursive<PlayerController>() const -> PlayerController*;
template auto Entity::getChildOfType<TickingEntity>() const -> TickingEntity*;
template auto Entity::getChildOfType<ControlledPhysicsEntity>() const -> ControlledPhysicsEntity*;
template auto Entity::getChildOfType<PhysicsEntity>() const -> PhysicsEntity*;

template auto Entity::getInParents<TickingEntity>() -> TickingEntity*;