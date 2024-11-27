//
// Created by gabrys on 25.11.24.
//

#ifndef GAME_H
#define GAME_H

#include "entity/Entity.h"
#include "entity/Entity2D.h"

class Game : public virtual Entity2D {

    static Game* instance; //raw pointer shouldnt be a problem right???

    public:
    static Game* getInstance();

    auto updateAll() -> void;

    auto end() -> void;

    explicit Game(); //root game entity, is a parent to all entities
};



#endif //GAME_H
