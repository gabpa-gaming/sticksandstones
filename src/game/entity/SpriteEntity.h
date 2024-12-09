//
// Created by gabrys on 29.11.24.
//

#ifndef SPRITEENTITY_H
#define SPRITEENTITY_H

#include "../../headers.h"
#include "Entity2D.h"
#include "TickingEntity.h"

class SpriteEntity : public virtual Entity2D, public virtual sf::Sprite {

    int width = 0, height = 0;
    float flip = false;
    public:
    [[nodiscard]] auto getClassName() const -> std::string override {return "SpriteEntity";}

    void draw(sf::RenderWindow* w);

    auto setGlobalPos(float x, float y) -> void override;
    auto setLocalPos(float x, float y) -> void override;

    auto setFlip(bool flip) -> void;
    auto getFlip() -> bool;
    //auto create() -> std::unique_ptr<Entity> override;
    //auto create(float localX, float localY) -> std::unique_ptr<Entity> override;
    auto virtual create(float localX, float localY, std::shared_ptr<sf::Texture>const& txt, int width, int height) -> std::unique_ptr<Entity>;

    void setSpriteIndex(int sprite_index);
};



#endif //SPRITEENTITY_H
