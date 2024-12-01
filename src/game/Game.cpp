//
// Created by gabrys on 25.11.24.
//

#include "Game.h"
#include "entity/Entity.h"
#include "entity/Entity2D.h"

std::shared_ptr<Game> Game::instance;

auto Game::getInstance() -> std::shared_ptr<Game> {
    if(instance)
        return instance;
    resetInstance();
    return instance;
}

auto Game::resetInstance() -> void{
    std::shared_ptr<Entity> ptr = std::move(Entity2D::create());
    instance = std::dynamic_pointer_cast<Game>(ptr);
}

auto Game::updateAll() -> void {
    ////todo
}

std::string Game::getName() const {
    return fmt::format("RootGameEntity, Id:{}, Pos: {},{}",getId(),getGlobalPos().x,getGlobalPos().y);
}

bool Game::IS_ROOT_FLAG() {
    return true;
}


