//
// Created by gabrys on 25.11.24.
//

#ifndef ENTITY_H
#define ENTITY_H

#include "../../headers.h"

class Entity { //oh, the misery everybody wants to be my entity

    int id = 0;
protected:
    std::vector<std::unique_ptr<Entity>> children;
    //std::weak_ptr<Entity> parent;

public:
    [[nodiscard]] auto getId() const -> int;
    //auto isChildOf(std::unique_ptr<Entity>& parent) const -> bool;
    auto isParentOf(const std::unique_ptr<Entity> &child) const -> bool;
    auto virtual addChild(std::unique_ptr<Entity> child) -> void;
    //auto setParent(std::unique_ptr<Entity>& parent) -> void;
    //auto getSiblings() const -> std::vector<std::shared_ptr<Entity>>&;
    [[nodiscard]] auto getHierarchy() const -> std::string;
    [[nodiscard]] virtual auto getName() const -> std::string;
    [[nodiscard]] auto getChild(int child_iter) -> std::unique_ptr<Entity>&;

    [[nodiscard]] auto getChildIter() const -> int;

    virtual auto create() -> std::unique_ptr<Entity>;

private:
    virtual auto newInstanceOfThisType() -> std::unique_ptr<Entity>;

    virtual auto IS_ROOT_FLAG() -> bool;
public:
    Entity();

    virtual ~Entity();

};



#endif //ENTITY_H
