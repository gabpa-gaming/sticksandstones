//
// Created by gabrys on 29.11.24.
//

#ifndef SPRITEENTITY_H
#define SPRITEENTITY_H

#include "../../headers.h"
#include "../entity/Entity2D.h"
class SpriteEntity : public virtual Entity2D, public virtual sf::Sprite {

    std::shared_ptr<sf::Texture> texture = nullptr;
    public:
    void draw(sf::RenderWindow* w);

    auto setGlobalPos(float x, float y) -> void override;
    auto setLocalPos(float x, float y) -> void override;

    auto getName() const -> std::string override;
    //auto create() -> std::unique_ptr<Entity> override;
    //auto create(float localX, float localY) -> std::unique_ptr<Entity> override;
    auto virtual create(float localX, float localY, std::shared_ptr<sf::Texture>const& txt) -> std::unique_ptr<Entity>;

    ~SpriteEntity() override = default;

    auto newInstanceOfThisType() -> std::unique_ptr<Entity> override;
};



#endif //SPRITEENTITY_H
