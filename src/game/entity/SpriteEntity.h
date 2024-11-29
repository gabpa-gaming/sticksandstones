//
// Created by gabrys on 29.11.24.
//

#ifndef SPRITEENTITY_H
#define SPRITEENTITY_H

#include "../../headers.h"
#include "../entity/Entity2D.h"
class SpriteEntity : public virtual Entity2D, public virtual sf::Sprite {

    sf::Texture * texture = nullptr;
    public:
    void draw(sf::RenderWindow* w);

    auto setGlobalPos(double x, double y) -> void override;
    auto setLocalPos(double x, double y) -> void override;

    auto getName() const -> std::string override;
    auto create(Entity *parent) -> SpriteEntity * override;
    auto create(Entity *parent, double localX, double localY) -> SpriteEntity * override;
    auto virtual create(Entity *parent, double localX, double localY, sf::Texture* txt) -> SpriteEntity *;

    ~SpriteEntity() override = default;
};



#endif //SPRITEENTITY_H
