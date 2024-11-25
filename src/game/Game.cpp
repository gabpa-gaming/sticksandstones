//
// Created by gabrys on 25.11.24.
//

#include "Game.h"
#include "entity/Entity.h"

Game* Game::instance = nullptr;

Game *Game::getInstance() {
    if (instance == nullptr) {
        instance = new Game();
    }
    return Game::instance;
}

Game::Game() : Entity2D(nullptr,0,0) {

}