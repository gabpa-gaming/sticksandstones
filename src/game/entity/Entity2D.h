//
// Created by gabrys on 25.11.24.
//

#ifndef ENTITY2D_H
#define ENTITY2D_H


#include "Entity.h"


class Entity2D : public virtual Entity{
    float x,y;
    float localX,localY;
    float parentX,parentY;

public:

    auto getGlobalPos() const -> sf::Vector2<float>;
    auto getLocalPos() const-> sf::Vector2<float>;
    auto updateParentPos(float x, float y) -> void;
    auto addChild(std::unique_ptr<Entity> child) -> void override;

    auto updateChildrenParentPoses(float x, float y) const -> void;
    auto virtual setGlobalPos(float x, float y) -> void;
    auto virtual setLocalPos(float x, float y) -> void;
    auto dislocate(float x, float y) -> void;

    [[nodiscard]] auto getClassName() const -> std::string override {return "Entity2D";}
    [[nodiscard]] auto getName() const -> std::string override;

    auto create() -> std::unique_ptr<Entity> override;
    auto virtual create(float localX, float localY) -> std::unique_ptr<Entity>;
};



#endif //ENTITY2D_H
