//
// Created by gabrys on 25.11.24.
//

#include "Game.h"
#include "entity/Entity.h"
#include "entity/Entity2D.h"

std::shared_ptr<Game> Game::instance = nullptr;

auto Game::getInstance() -> std::shared_ptr<Game> {
    if(instance)
        return instance;
    resetInstance();
    return instance;
}

auto Game::resetInstance() -> void{
    Game game;
    std::shared_ptr<Entity> ptr = std::move(game.create());
    instance = std::dynamic_pointer_cast<Game>(ptr);
}

std::unique_ptr<Entity> Game::create() {
    return std::move(Entity2D::create());
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

auto Game::newInstanceOfThisType() -> std::unique_ptr<Entity> {
    return std::move(std::make_unique<Game>());

}
