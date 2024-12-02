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
    static auto getInstance() -> std::shared_ptr<Game>; //root game entity, is a parent to all entities

    auto updateAll() -> void;

    auto end() -> void;

    auto getName() const -> std::string override;


    static auto resetInstance() -> void;

private:
    auto IS_ROOT_FLAG() -> bool override;

    auto create() -> std::unique_ptr<Entity> override;

    auto newInstanceOfThisType() -> std::unique_ptr<Entity> override;
};



#endif //GAME_H
