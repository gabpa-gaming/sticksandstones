//
// Created by gabrys on 29.11.24.
//

#include "SpriteEntity.h"

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

std::string SpriteEntity::getName() const {
    return fmt::format("SpriteEntity, Id({}), At({},{}), Local({},{})",
        getId(),getGlobalPos().x,getGlobalPos().y, getLocalPos().x,getLocalPos().y);
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
auto SpriteEntity::create(float localX, float localY, std::shared_ptr<sf::Texture>const& txt) -> std::unique_ptr<Entity> {
    auto base = std::move(Entity2D::create());
    auto p = dynamic_cast<SpriteEntity*>(base.get());
    p -> setScale(3.25,3.25);
    p -> texture = txt;
    if(txt)
        p -> setTexture(*txt);
    return base;
}

std::unique_ptr<Entity> SpriteEntity::newInstanceOfThisType() {
    return std::move(std::make_unique<SpriteEntity>());
}
