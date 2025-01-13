//
// Created by gabrys on 25.11.24.
//
#include "Entity.h"

#include "HealthController.h"
#include "SpriteEntity.h"
#include "../Game.h"
#include "../level/Interactible.h"
#include "../level/LevelGenerator.h"
#include "../player/PlayerController.h"
#include "../level/Room.h"
#include "../projectile/Projectile.h"



Entity::Entity() {
    //fmt::println("An entity was created", Entity::getName());
}

Entity::~Entity() {
    //fmt::println("An entity was deleted", Entity::getName());
}

auto Entity::getEnabled() -> bool {
    if(enabled && parent) {
        return parent -> getEnabled();
    }

    return enabled;
}

auto Entity::setEnabled(bool enabled) -> void {
    this -> enabled = enabled;
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
auto Entity::getAs()  -> E & {
    static_assert(std::is_base_of_v<Entity, E>, "E must derive from Entity");
    return dynamic_cast<E&>(*this);
}

template<typename E>
auto Entity::getChildOfType() const -> E* { //returns the first one in hierarchy, null if not found
    static_assert(std::is_base_of_v<Entity, E>, "E must derive from Entity");
    for (auto& child : children) {
        auto p = dynamic_cast<E*>(child.get());
        if (p) {
            return p;
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

auto Entity::getHierarchy() const -> const std::string {
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
    return fmt::format("{}{}{}, Id:{}", enabled ? "" : "DISABLED ", initialized ? "":"UNINITIALIZED ", getClassName(), id) ;
}

auto Entity::addChild(std::unique_ptr<Entity> child) -> void {
    children.push_back(std::move(child));
}

auto Entity::getParent() -> Entity * {
    return parent;
}

auto Entity::remove() -> void {
    int i = 0;
    for(; i < parent->children.size(); i++) {
        if(parent->children[i].get() == this) break;
    }
    parent->children.erase(parent->children.begin() + i);
}

auto Entity::endOfFrameRemove() -> void {
    Game::getInstance()->toRemove.push_back(this);
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
template auto Entity::getChildOfTypeRecursive<ControlledPhysicsEntity>() const -> ControlledPhysicsEntity*;
template auto Entity::getChildOfTypeRecursive<Interactor>() const -> Interactor*;

template auto Entity::getChildOfType<TickingEntity>() const -> TickingEntity*;
template auto Entity::getChildOfType<ControlledPhysicsEntity>() const -> ControlledPhysicsEntity*;
template auto Entity::getChildOfType<PhysicsEntity>() const -> PhysicsEntity*;
template auto Entity::getChildOfType<Projectile>() const -> Projectile*;
template auto Entity::getChildOfType<Interactor>() const -> Interactor*;

template auto Entity::getAs<Entity2D>() -> Entity2D&;
template auto Entity::getAs<HealthController>() -> HealthController&;
template auto Entity::getAs<ControlledPhysicsEntity>() -> ControlledPhysicsEntity&;
template auto Entity::getAs<Room>() -> Room&;
template auto Entity::getAs<LevelGenerator>() -> LevelGenerator&;
template auto Entity::getAs<Projectile>() -> Projectile&;
template auto Entity::getAs<TickingEntity>() -> TickingEntity&;
template auto Entity::getAs<SpriteEntity>() -> SpriteEntity&;
template auto Entity::getAs<Interactible>() -> Interactible&;
template auto Entity::getAs<Interactor>() -> Interactor&;
template auto Entity::getAs<CollidableEntity>() -> CollidableEntity&;
template auto Entity::getAs<PlayerUI>() -> PlayerUI&;

template auto Entity::getInParents<TickingEntity>() -> TickingEntity*;
template auto Entity::getInParents<PlayerController>() -> PlayerController*;
template auto Entity::getInParents<HealthController>() -> HealthController*;