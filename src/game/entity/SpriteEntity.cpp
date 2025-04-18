//
// Created by gabrys on 29.11.24.
//

#include "SpriteEntity.h"
#include "../Game.h"
void SpriteEntity::draw(sf::RenderWindow *w) const {
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

auto SpriteEntity::setFlipX(bool flip) -> void {
    if(flip == this -> flipX) {
        return;
    }
    this -> flipX = flip;
    setScale(abs(getScale().x) * (flip ? -1.0f : 1.0f), getScale().y);
}

auto SpriteEntity::getFlipX() const -> bool {
    return flipX;
}

auto SpriteEntity::setFlipY(bool flip) -> void {
    if(flip == this -> flipY) {
        return;
    }
    this -> flipY = flip;
    setScale(getScale().x, abs(getScale().y) * (flip ? -1.0f : 1.0f));
}

auto SpriteEntity::getFlipY() const -> bool {
    return flipY;
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

    this -> setScale(Game::PIXEL_SCALE,Game::PIXEL_SCALE);
    this -> width = width;
    this -> height = height;
    this -> drawOrder = drawOrder;
    if(txt) {
        this -> setTexture(*txt);
        this -> setSpriteIndex(0);
    }else {
        throw std::runtime_error("Set texture please 😊😊😊");
    }

    return base;
}

void SpriteEntity::setSpriteIndex(int sprite_index) {
    int spriteCount = getTexture() -> getSize().x * getTexture() -> getSize().y / width / height;
    if(spriteCount == 0) {
        fmt::println("ERROR: Texture too big... sizing down.");
        width = 16;
        height = 16;
        setTexture(*loadTxt("msg"));
        setSpriteIndex(sprite_index);
        return;
    }
    if(sprite_index >= spriteCount) {
        setSpriteIndex(sprite_index % spriteCount);
        return;
    }
    auto rowSize = getTexture() -> getSize().x / width;
    setTextureRect(sf::IntRect(sprite_index % rowSize * width, sprite_index / rowSize * height, width, height));
    setOrigin(width/2,height/2);
    setColor({255,255,255,255});
}
