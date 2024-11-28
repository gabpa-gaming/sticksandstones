//
// Created by gabrys on 25.11.24.
//

#include "Game.h"
#include "entity/Entity.h"
#include "entity/Entity2D.h"

Game* Game::instance = nullptr;

Game* Game::getInstance() {
    if (instance == nullptr) {
        instance = new Game();
        instance -> create();
    }
    return Game::instance;
}

auto Game::updateAll() -> void {
    ////todo
}

auto Game::end() -> void {
    for (auto child : children) {
        child -> setParent(nullptr);
    }
    //bye
    delete instance;
} //make sure it doesnt fuck up in some way //todo

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

