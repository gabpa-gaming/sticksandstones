//
// Created by gabrys on 29.11.24.
//

#include "SpriteEntity.h"
#include "../Game.h"
void SpriteEntity::draw(sf::RenderWindow *w) {
    w -> draw(*this);
}

void SpriteEntity::setGlobalPos(float x, float y) {
    Entity2D::setGlobalPos(x, y);
    setPosition(getGlobalPos().x, getGlobalPos().y);
}

void SpriteEntity::setLocalPos(float x, float y) {
    Entity2D::setLocalPos(x, y);
    setPosition(getGlobalPos().x, getGlobalPos().y);
}

auto SpriteEntity::setFlip(bool flip) -> void {
    if(flip == this -> flip) {
        return;
    }
    this -> flip = flip;
    setScale(abs(getScale().x) * (flip ? -1.0 : 1.0), getScale().y);
}

auto SpriteEntity::getFlip() -> bool {
    return flip;
}

/*
auto SpriteEntity::create() -> std::unique_ptr<Entity> {
    return create(0, 0);
}
*/
/*
auto SpriteEntity::create(float localX, float localY) -> std::unique_ptr<Entity> {
    return create(localX, localY, std::make_unique<sf::Texture>());
}
*/
auto SpriteEntity::create(float localX, float localY, std::shared_ptr<sf::Texture>const& txt, int width, int height, int drawOrder) -> std::unique_ptr<Entity> {
    auto base = std::move(Entity2D::create(localX, localY));
    auto p = dynamic_cast<SpriteEntity*>(base.get());
    p -> setScale(Game::PIXEL_SCALE,Game::PIXEL_SCALE);
    p -> width = width;
    p -> height = height;
    p -> drawOrder = 0;
    if(txt) {
        p -> setTexture(*txt);
        p -> setSpriteIndex(0);
    }else {
        throw std::runtime_error("Set texture please 😊😊😊");
    }

    return base;
}

void SpriteEntity::setSpriteIndex(int sprite_index) {
    auto rowSize = getTexture() -> getSize().x / width;
    setTextureRect(sf::IntRect(sprite_index % rowSize * width, sprite_index / rowSize * height, width, height));
    setOrigin(width/2,height/2);
    setColor({255,255,255,255});
}
