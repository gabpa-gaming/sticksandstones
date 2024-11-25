//
// Created by gabrys on 25.11.24.
//

#ifndef GAME_H
#define GAME_H

#include "entity/Entity2D.h"

class Entity2D;

class Game : Entity2D {

    static Game *instance;

    public:
    static Game *getInstance();
    Game();
};



#endif //GAME_H
