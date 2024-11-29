//
// Created by gabrys on 29.11.24.
//

#include "SpriteEntity.h"

void SpriteEntity::draw(sf::RenderWindow *w) {
    w -> draw(*this);
}

void SpriteEntity::setGlobalPos(double x, double y) {
    Entity2D::setGlobalPos(x, y);
    setPosition(x, y);
}

void SpriteEntity::setLocalPos(double x, double y) {
    Entity2D::setLocalPos(x, y);
    setPosition(getGlobalPos().x, getGlobalPos().y);
}

std::string SpriteEntity::getName() const {
    return fmt::format("SpriteEntity, Id({}), At({},{}), Local({},{})",
        getId(),getGlobalPos().x,getGlobalPos().y, getLocalPos().x,getLocalPos().y);
}

auto SpriteEntity::create(Entity *parent) -> SpriteEntity*{
    return create(parent, 0, 0);
}

auto SpriteEntity::create(Entity *parent, double localX, double localY) -> SpriteEntity* {
    return create(parent, localX, localY, nullptr);
}

auto SpriteEntity::create(Entity *parent, double localX, double localY, sf::Texture* txt) -> SpriteEntity * {
    auto ent = Entity2D::create(parent, localX, localY);

    texture = txt;
    if(txt)
        setTexture(*txt);
    return dynamic_cast<SpriteEntity*>(ent);
}
