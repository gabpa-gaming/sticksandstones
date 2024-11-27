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


Game::Game() : Entity2D(nullptr) {

}
