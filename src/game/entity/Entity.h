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
    explicit Entity(Entity *parent);
    Entity()=default; //why is this needed?????? Bro cant even be private 😭😭 or fails build in Game.cpp
    virtual ~Entity();

};



#endif //ENTITY_H
