//
// Created by gabrys on 25.11.24.
//

#ifndef GAME_H
#define GAME_H

#include "entity/PhysicsEntity.h"
#include "entity/Entity.h"
#include "entity/Entity2D.h"
#include "entity/CollidableEntity.h"
#include "level/LevelGenerator.h"
#include "level/Room.h"
#include "player/PlayerController.h"

#define GAME_WIDTH_UNSCALED 192
#define GAME_HEIGHT_UNSCALED 160

class Game : public virtual Entity2D {

    static std::shared_ptr<Game> instance;



    sf::Time lastPhysicsTick;
    sf::Time lastStateMachineTick;
    sf::Time lastFrame;
    sf::Clock gameClock;

    Entity * player = nullptr;

    LevelGenerator * levelGenerator = nullptr;


    public:
    static bool debugModeOn;

    static float PIXEL_SCALE;

    static int PHYSICS_TICK_RATE;
    static int STATE_MACHINE_TICK_RATE;

    Room * currentRoom = nullptr;

    std::vector<Entity *> toRemove;

    static auto getInstance() -> std::shared_ptr<Game>; //root game entity, is a parent to all entities

    static auto tilePosToScreenCoords(sf::Vector2f pos) -> sf::Vector2f;

    auto gameLoop(std::shared_ptr<sf::RenderWindow> const &window) -> void;

    auto drawFrame(std::shared_ptr<sf::RenderWindow> window) const -> void;

    auto physicsUpdateAll(float deltaT) const -> void;

    auto frameUpdateAll(float deltaT) const -> void;

    auto tickAll() const -> void;

    [[nodiscard]] auto rectCast(sf::FloatRect rect, std::bitset<8> mask) const -> std::vector<CollidableEntity*>;

    [[nodiscard]] auto getPlayer() const -> Entity &;

    auto setPlayer(Entity &p) -> void;

    [[nodiscard]] auto getLevelGenerator() const -> LevelGenerator&;

    auto setLevelGenerator(LevelGenerator &p) -> void;

    [[nodiscard]] auto getClassName() const -> std::string override {return "RootGameEntity";}

    static auto resetInstance() -> void;

private:
    auto create() -> std::unique_ptr<Entity> override;

};



#endif //GAME_H
