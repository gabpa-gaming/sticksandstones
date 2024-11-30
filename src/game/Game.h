//
// Created by gabrys on 25.11.24.
//

#ifndef GAME_H
#define GAME_H

#include "entity/Entity.h"
#include "entity/Entity2D.h"

class Game : public virtual Entity2D {

    static std::shared_ptr<Game> instance;

    public:
    static auto getInstance() -> Game*;

    auto updateAll() -> void;

    auto end() -> void;

    auto getName() const -> std::string override;

    auto IS_ROOT_FLAG() -> bool override;

    auto create() -> Game*;//root game entity, is a parent to all entities
};



#endif //GAME_H
