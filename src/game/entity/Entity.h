//
// Created by gabrys on 25.11.24.
//

#ifndef ENTITY_H
#define ENTITY_H

#include "../../headers.h"

class Entity { //oh, the misery everybody wants to be my entity

    int id = 0;
protected:
    std::vector<Entity *> children;
    Entity *parent = nullptr;

public:
    auto getId() const -> int;
    auto isChildOf(const Entity *parent) const -> bool;
    auto isParentOf(Entity* child) const -> bool;
    auto addChild(Entity& child) -> void;
    auto removeChild(int child_iter) -> void;
    auto setParent(Entity *parent) -> void;

    [[nodiscard]]
    auto getHierarchy() const -> std::string;
    [[nodiscard]]
    virtual auto getName() const -> std::string;
    [[nodiscard]]
    auto getChild(int child_iter) const -> Entity*;


    auto virtual create(Entity *parent) -> Entity*;

    virtual auto IS_ROOT_FLAG() -> bool;

    virtual ~Entity();

};



#endif //ENTITY_H
