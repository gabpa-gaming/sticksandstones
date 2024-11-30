//
// Created by gabrys on 25.11.24.
//

#include "Game.h"
#include "entity/Entity.h"
#include "entity/Entity2D.h"

std::shared_ptr<Game> Game::instance;

auto Game::getInstance() -> Game* {
    if (!instance) {
        instance = std::make_shared<Game>();
        instance -> create();
    }
    return instance.get();
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

auto Game::create() -> Game * {
    Entity2D::create(nullptr);
    return this;
}

