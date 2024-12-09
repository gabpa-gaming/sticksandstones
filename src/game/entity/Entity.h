//
// Created by gabrys on 25.11.24.
//

#ifndef ENTITY_H
#define ENTITY_H

#include "../../headers.h"

class Entity { //oh, the misery everybody wants to be my entity

protected:
    const std::string typeName = "Entity";
    int id = -1;
    std::vector<std::unique_ptr<Entity>> children;

public:
    [[nodiscard]] auto  virtual getClassName() const -> std::string {return "Entity";}

    [[nodiscard]] auto getId() const -> int;

    [[nodiscard]] auto isParentOf(const std::unique_ptr<Entity> &child) const -> bool;

    auto virtual addChild(std::unique_ptr<Entity> child) -> void;

    [[nodiscard]] auto getHierarchy() const -> std::string;

    [[nodiscard]] virtual auto getName() const -> std::string;

    template<typename E>
    [[nodiscard]] auto getChildOfType() const -> E*;

    template<class E>
    [[nodiscard]] auto getChildOfTypeRecursive() const -> E*;

    template<class E>
    [[nodiscard]] auto getAllChildrenOfType() const -> std::vector<Entity *>;

    template<class E>
    [[nodiscard]] auto getAllChildrenOfTypeRecursive() const -> std::vector<Entity *>;

    [[nodiscard]] auto getChild(int child_iter) -> std::unique_ptr<Entity>&;

    virtual auto create() -> std::unique_ptr<Entity>;

private:

    virtual auto IS_ROOT_FLAG() -> bool;
public:
    Entity();

    virtual ~Entity();

};


#endif //ENTITY_H
