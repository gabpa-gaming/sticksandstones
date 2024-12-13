//
// Created by gabrys on 12.12.24.
//

#include "Game.h"
#include "entity/SpriteEntity.h"
#include "player/PlayerController.h"
#include "entity/PhysicsEntity.h"
#include "entity/Entity.h"

#include "Builders.h"

#include "entity/HealthController.h"

auto blinker = [](TickingEntity& caller, TickingEntity::StateMachineState&) {
    const int CHANGE_TICK_COUNT = 5;
    caller.getChildOfType<SpriteEntity>()->setColor((caller.tickCounter / CHANGE_TICK_COUNT % 2 == 0)
        ? sf::Color{0,0,0,0}
        : sf::Color{255,255,255,255});
};

auto buildPlayer() -> std::unique_ptr<Entity> {
    auto player = (new PlayerController)->create(0,0,
        CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::player),
        CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::wall), 0.25, 0.25);
    auto ticker = (new TickingEntity)->create();
    auto sprite = (new SpriteEntity)->create(0,0, loadTxt("character"), 32, 32);

    auto healthController = (new HealthController)->create(0,0, CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::player),
                                                           0, 1,1, 20, 0);

    auto stateChanger = [](TickingEntity& caller, TickingEntity::StateMachineState&) {
        caller.setStateByName(caller.getChildOfTypeRecursive<PlayerController>() -> velocity != sf::Vector2f(0,0) ? "walk" : "idle");
    };
    auto t = dynamic_cast<TickingEntity*>(ticker.get());
        t -> states =
        {{12,50, 1, "damage", blinker},
        {0,20,1, "idle", stateChanger},
        {1,10,1, "idle", stateChanger},
        {2,10,1, "idle", stateChanger},
        {3,10,1, "idle", stateChanger},
        {4,10,-4, "idle", stateChanger},
        {5,7, 1, "walk", stateChanger},
        {6,7, 1, "walk", stateChanger},
        {7,7, 1, "walk", stateChanger},
        {8,7, 1, "walk", stateChanger},
        {9,7, 1, "walk", stateChanger},
        {10,7, 1, "walk", stateChanger},
        {11,7, -6, "walk", stateChanger},
        };
    t -> setStateByName("idle");

    auto p = dynamic_cast<PlayerController*>(player.get());
    p -> speedGain = 1026;
    p -> topSpeed = 150;

    ticker -> addChild(std::move(player));

    p -> addChild(std::move(sprite));
    p -> addChild(std::move(healthController));
    p -> setGlobalPos(192 * Game::PIXEL_SCALE/2,160 *Game::PIXEL_SCALE/2);


    p -> colliderOffset = sf::Vector2f(0, 0.35);
    return std::move(ticker);
}

auto buildBat() -> std::unique_ptr<Entity> {
    auto bat = (new ControlledPhysicsEntity)->create(0,0,
        CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::enemy), 0, 0.25, 0.25);
    auto ticker = (new TickingEntity)->create();
    auto sprite = (new SpriteEntity)->create(0,0, loadTxt("bat"), 16, 16);

    auto stateChanger = [](TickingEntity& caller, TickingEntity::StateMachineState&) {
        caller.setStateByName(caller.getChildOfType<PhysicsEntity>() -> velocity != sf::Vector2f(0,0) ? "sleep" : "fly");
    };
    std::vector<TickingEntity::StateMachineState> states =
    {{5,10,0, "damage"},
    {5,1,0, "sleep", stateChanger},
    {0,5,1, "fly", stateChanger},
    {1,5,1, "fly", stateChanger},
    {2,5,1, "fly", stateChanger},
    {3,5,1, "fly", stateChanger},
    {4,5,1, "fly", stateChanger},
    {3,5,1, "fly", stateChanger},
    {2,5,1, "fly", stateChanger},
    {1,5,-7, "fly", stateChanger},
    };

    auto t = dynamic_cast<TickingEntity*>(ticker.get());
    t -> states = states;
    t -> setStateByName("fly");

    auto p = dynamic_cast<ControlledPhysicsEntity*>(bat.get());
    p -> speedGain = 1026;
    p -> topSpeed = 150;
    bat -> addChild(std::move(sprite));


    auto health = (new HealthController)->create(0,0,
        CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::projectile),
        CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::player),
        1,1, 20,5);


    ticker -> addChild(std::move(bat));

    p -> addChild(std::move(health));

    p -> setGlobalPos(192 * Game::PIXEL_SCALE/2 - 64,160 *Game::PIXEL_SCALE/2);
    p -> colliderOffset = sf::Vector2f(0, 0.35);
    return std::move(ticker);
}

auto buildRock() -> std::unique_ptr<Entity> {
    auto rock = (new CollidableEntity)->create(0,0,
        CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::wall)
                    + CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::destructible),
0, 1, 1);
    auto rSprite = (new SpriteEntity)->create(0,0, loadTxt("tileset") ,16, 16);

    rock->addChild(std::move(rSprite));
    return std::move(rock);
}
