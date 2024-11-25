//
// Created by gabrys on 25.11.24.
//

#ifndef ENTITY_H
#define ENTITY_H

#include "../../headers.h"

class Entity {

    int id = 0;
    Entity *parent = nullptr;
    std::vector<Entity *> children;

public:
    auto isChildOf(const Entity *parent) const -> bool;
    const auto isParentOf(Entity& child);
    auto addChild(Entity &child);
    auto removeChild(int child_iter);
    auto setParent(Entity *parent) -> void;

    Entity *getChild(int child_iter) const;
    explicit Entity(Entity *parent);
};



#endif //ENTITY_H
