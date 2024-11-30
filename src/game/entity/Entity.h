//
// Created by gabrys on 25.11.24.
//

#ifndef ENTITY_H
#define ENTITY_H

#include "../../headers.h"

class Entity { //oh, the misery everybody wants to be my entity

    int id = 0;
protected:
    std::vector<std::shared_ptr<Entity>> children;
    Entity *parent = nullptr;

public:
    [[nodiscard]] auto getId() const -> int;
    auto isChildOf(const Entity *parent) const -> bool;
    auto isParentOf(const Entity* child) const -> bool;
    auto addChild(Entity& child) -> void;
    auto setParent(Entity *parent) -> void;
    auto getSiblings() const -> std::vector<Entity*>;
    [[nodiscard]] auto getHierarchy() const -> std::string;
    [[nodiscard]] virtual auto getName() const -> std::string;
    [[nodiscard]] auto getChild(int child_iter) const -> Entity*;
    [[nodiscard]] auto getChildIter() const -> int;

    auto virtual create(Entity *parent) -> Entity*;

    virtual auto IS_ROOT_FLAG() -> bool;

    virtual ~Entity() = default;

};



#endif //ENTITY_H
