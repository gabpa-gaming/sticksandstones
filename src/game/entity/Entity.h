//
// Created by gabrys on 25.11.24.
//

#ifndef ENTITY_H
#define ENTITY_H

#include "../../headers.h"

class Entity { //hierarchy system

protected:
    int id = -1;
    std::vector<std::unique_ptr<Entity>> children;

    bool initialized = false; //all entities should get initialized using init(parent) after creation to set references to other entities in hierarchy
    Entity* parent = nullptr;
    bool enabled = true; //game loop won't iterate over disabled entities

public:

    [[nodiscard]] auto virtual getClassName() const -> std::string {return "Entity";}

    auto getEnabled() -> bool;

    auto setEnabled(bool enabled) -> void;

    auto virtual init(Entity * parent) -> void; //set parent and sibling references before adding to hierarchy

    auto initAllChildren(Entity* parent) -> void; //same as init but also recursively for all children

    template<typename E>
    auto getAs() -> E&;

    [[nodiscard]] auto getId() const -> int;

    auto virtual addChild(std::unique_ptr<Entity> child) -> void;

    [[nodiscard]] auto getHierarchy() const -> const std::string;

    [[nodiscard]] virtual auto getName() const -> std::string;

    template<typename E>
    [[nodiscard]] auto getChildOfType() const -> E*;

    template<class E>
    [[nodiscard]] auto getChildOfTypeRecursive() const -> E*;

    template<class E>
    [[nodiscard]] auto getAllChildrenOfType() const -> std::vector<Entity *>;

    template<class E>
    [[nodiscard]] auto getAllChildrenOfTypeRecursive() const -> std::vector<Entity *>;

    template<typename E>
    [[nodiscard]] auto getInParents() -> E*;

    virtual auto remove() -> void; //only use outside of loops, use endOfFrameRemove() instead for safe removal

    auto endOfFrameRemove() -> void;

    virtual auto create() -> std::unique_ptr<Entity>; //factory function used to set base values and prepare entity to be added to hierarchy

    Entity();

    virtual ~Entity();

};


#endif //ENTITY_H
