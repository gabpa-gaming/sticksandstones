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
#include "level/Interactible.h"
#include "level/LevelGenerator.h"
#include "level/Room.h"
#include "projectile/Projectile.h"

auto blinker(TickingEntity& caller, TickingEntity::StateMachineState&) -> void {
    const int CHANGE_TICK_COUNT = 15;
    caller.getChildOfTypeRecursive<SpriteEntity>()->setColor((caller.tickCounter / CHANGE_TICK_COUNT % 2 == 0)
        ? sf::Color{0,0,0,0}
        : sf::Color{255,255,255,255});
}

auto simpleMoveTowardsPlayer(TickingEntity& caller, TickingEntity::StateMachineState&) -> void {
    caller.getChildOfTypeRecursive<ControlledPhysicsEntity>() -> direction
    = normalize(-caller.getChildOfTypeRecursive<HealthController>()->getGlobalPos()
        + Game::getInstance()
        ->getPlayer()
        .getChildOfTypeRecursive<HealthController>()
        ->getGlobalPos());
}
auto projectileLifetimeCounter(TickingEntity& caller, TickingEntity::StateMachineState&) -> void {
    auto proj = caller.getChildOfType<Projectile>();
    caller.dislocate(0,0); //to update collision
    proj->lifetime -= 1.f/Game::STATE_MACHINE_TICK_RATE;
    if(proj->lifetime <= 0)
        proj->kill();
}

auto buildPlayer() -> std::unique_ptr<Entity> {
    auto player = (new PlayerController)->create(0,0,
        CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::player),
        CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::wall), 0.25, 0.25);
    auto ticker = (new TickingEntity)->create();
    auto sprite = (new SpriteEntity)->create(0,0, loadTxt("character"), 32, 32);
    auto healthController = (new HealthController)->create(0,0, CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::player),
        CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::enemy),
        0.5,1, 20, 0, nullptr);
    auto interactor = (new Interactor)->create(0.5f);
    healthController->getAs<HealthController>().invTime = 0.5;
    healthController->getAs<HealthController>().onDeathEvent = [](Entity& caller) {
        caller.getInParents<TickingEntity>()->setStateByName("dead");
    };

    auto stateChanger = [](TickingEntity& caller, TickingEntity::StateMachineState&) {
        caller.setStateByName(caller.getChildOfTypeRecursive<PlayerController>() -> velocity != sf::Vector2f(0,0) ? "walk" : "idle");
    };
    static std::vector<TickingEntity::StateMachineState> states =
        {{13,50, 1, "damage", blinker},
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
        {12,1, 0, "dead"}
        };
    auto t = dynamic_cast<TickingEntity*>(ticker.get());
        t -> states = states;
    t -> setStateByName("idle");

    player->addChild(std::move(sprite));
    player->addChild(std::move(healthController));
    player->addChild(std::move(interactor));

    player->getAs<ControlledPhysicsEntity>().speedGain = 1026;
    player->getAs<ControlledPhysicsEntity>().topSpeed = 150;
    player->getAs<ControlledPhysicsEntity>().setGlobalPos(GAME_WIDTH_UNSCALED * Game::PIXEL_SCALE/2,GAME_HEIGHT_UNSCALED *Game::PIXEL_SCALE/2);
    player->getAs<ControlledPhysicsEntity>().colliderOffset = sf::Vector2f(0, 0.35);

    ticker -> addChild(std::move(player));

    return std::move(ticker);
}

auto buildBat() -> std::unique_ptr<Entity> {
    auto bat = (new ControlledPhysicsEntity)->create(0,0,
        CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::enemy), 0, 0.25, 0.25);
    auto ticker = (new TickingEntity)->create();
    auto sprite = (new SpriteEntity)->create(0,0, loadTxt("bat"), 16, 16, 3);

    auto extraRandomLength = [](TickingEntity &caller, TickingEntity::StateMachineState &state) {
        caller.tickCounter -= Game::getInstance()->miscRNG() % 225;
    };

    static std::vector<TickingEntity::StateMachineState> states =
    {{6,20,2, "damage"},
    {0,5,1, "walk", simpleMoveTowardsPlayer},
    {1,5,1, "walk", simpleMoveTowardsPlayer},
    {2,5,1, "walk", simpleMoveTowardsPlayer},
    {3,5,1, "walk", simpleMoveTowardsPlayer},
    {4,5,1, "walk", simpleMoveTowardsPlayer},
    {3,5,1, "walk", simpleMoveTowardsPlayer},
    {2,5,1, "walk", simpleMoveTowardsPlayer},
    {1,5,-7, "walk", simpleMoveTowardsPlayer},
    {5,75,-8, "idle", [](TickingEntity &, TickingEntity::StateMachineState &)->void{}, extraRandomLength}
    };

    bat -> addChild(std::move(sprite));

    auto t = dynamic_cast<TickingEntity*>(ticker.get());
    t -> states = states;


    bat-> getAs<ControlledPhysicsEntity>().speedGain = 200;
    bat-> getAs<ControlledPhysicsEntity>().topSpeed = 120;
    bat -> getAs<ControlledPhysicsEntity>().colliderOffset = sf::Vector2f(0, 0.35);

    auto health = (new HealthController)->create(0,0,
         CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::enemy),
         CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::player),
         0.5f,0.5f, 25,5, ticker.get());
    bat->addChild(std::move(health));

    ticker -> addChild(std::move(bat));
    t -> setStateByName("idle");
    return std::move(ticker);
}

auto buildRat() -> std::unique_ptr<Entity> {
    auto rat = (new ControlledPhysicsEntity)->create(0,0,
        CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::enemy),
        CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::wall), 0.25, 0.25);
    auto ticker = (new TickingEntity)->create();
    auto sprite = (new SpriteEntity)->create(0,0, loadTxt("szizur"), 32, 32);
    auto healthController = (new HealthController)->create(0,0, CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::enemy),
                                                           CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::player),
                                                           0.5,1, 18, 4, ticker.get());
    healthController->getAs<HealthController>().invTime = 0;
    auto t = dynamic_cast<TickingEntity*>(ticker.get());

    static std::vector<TickingEntity::StateMachineState> states =
        {{8,50, 7, "damage",},
        {0,7, 1, "walk", simpleMoveTowardsPlayer },
        {1,7, 1, "walk", simpleMoveTowardsPlayer},
        {2,7, 1, "walk", simpleMoveTowardsPlayer},
        {3,7, 1, "walk", simpleMoveTowardsPlayer},
        {4,7, 1, "walk", simpleMoveTowardsPlayer},
        {5,7, 1, "walk", simpleMoveTowardsPlayer},
        {6,7, 1, "walk", simpleMoveTowardsPlayer},
        {7,7, -7, "walk", simpleMoveTowardsPlayer},
    };
    t -> states = states;
    t -> setStateByName("walk");

    rat->addChild(std::move(sprite));
    rat->addChild(std::move(healthController));

    rat->getAs<ControlledPhysicsEntity>().speedGain = 450;
    rat->getAs<ControlledPhysicsEntity>().topSpeed = 165;
    //rat->getAs<ControlledPhysicsEntity>().setGlobalPos(GAME_WIDTH_UNSCALED * Game::PIXEL_SCALE/2,GAME_HEIGHT_UNSCALED *Game::PIXEL_SCALE/2);
    rat->getAs<ControlledPhysicsEntity>().colliderOffset = sf::Vector2f(0, 0.35);

    ticker -> addChild(std::move(rat));

    return std::move(ticker);
}

auto buildEmenterror() -> std::unique_ptr<Entity> {
    auto ementerror = (new ControlledPhysicsEntity)->create(0,0,
        CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::enemy),
        CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::wall), 0.25, 0.25);
    auto ticker = (new TickingEntity)->create();
    auto sprite = (new SpriteEntity)->create(0,0, loadTxt("emmenterror"), 32, 32);
    auto healthController = (new HealthController)->create(0,0, CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::enemy),
                                                           CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::player),
                                                           0.5,1, 42, 4, ticker.get());
    healthController->getAs<HealthController>().invTime = 0;
    auto t = dynamic_cast<TickingEntity*>(ticker.get());

    static std::vector<TickingEntity::StateMachineState>states = {
        {0,50, 1, "damage",},
        {0,12, 1, "walk", simpleMoveTowardsPlayer },
        {1,12, 1, "walk", simpleMoveTowardsPlayer},
        {2,12, 1, "walk", simpleMoveTowardsPlayer},
        {3,12, 1, "walk", simpleMoveTowardsPlayer},
        {4,12, 1, "walk", simpleMoveTowardsPlayer},
        {5,12, 1, "walk", simpleMoveTowardsPlayer},
        {6,12, 1, "walk", simpleMoveTowardsPlayer},
        {7,12, -7, "walk", simpleMoveTowardsPlayer},
    };
    t -> states = states;
    t -> setStateByName("walk");

    ementerror->addChild(std::move(sprite));
    ementerror->addChild(std::move(healthController));

    ementerror->getAs<ControlledPhysicsEntity>().speedGain = 450;
    ementerror->getAs<ControlledPhysicsEntity>().topSpeed = 50;
    //ementerror->getAs<ControlledPhysicsEntity>().setGlobalPos(GAME_WIDTH_UNSCALED * Game::PIXEL_SCALE/2,GAME_HEIGHT_UNSCALED *Game::PIXEL_SCALE/2);
    ementerror->getAs<ControlledPhysicsEntity>().colliderOffset = sf::Vector2f(0, 0.35);

    ticker -> addChild(std::move(ementerror));

    return std::move(ticker);
}

auto buildRock() -> std::unique_ptr<Entity> {
    auto rock = (new CollidableEntity)->create(0,0,
        CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::wall)
                    + CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::destructible),
0, 1, 1);
    auto sprite = (new SpriteEntity)->create(0,0, loadTxt("tileset") ,16, 16);
    if(Game::getInstance()->miscRNG() % 2 == 0) {
        sprite->getAs<SpriteEntity>().setSpriteIndex(1);
    }
    rock->addChild(std::move(sprite));
    return std::move(rock);
}

auto buildRoom(Room::RoomData& data) -> std::unique_ptr<Entity> {
    auto room = (new Room)->create(data);
    room->getAs<Entity2D>().setGlobalPos(GAME_WIDTH_UNSCALED * Game::PIXEL_SCALE/2,GAME_HEIGHT_UNSCALED *Game::PIXEL_SCALE/2);
    return std::move(room);
}

auto buildGenerator() -> std::unique_ptr<Entity> {
    auto levelGen = (new LevelGenerator)->create(0);

    levelGen->getAs<LevelGenerator>().generateNextLevel();
    levelGen->getAs<LevelGenerator>().setRoomToStart();
    return std::move(levelGen);
}

auto buildBaseProjectile(float damage, float speed, std::string name, float sizeX, float sizeY,
    const std::shared_ptr<sf::Texture>& texture, float life, CollidableEntity *ignore, sf::Vector2i orientation, float x, float y) -> std::unique_ptr<Entity> {

    auto projectile = (new Projectile)->create(x,y, 0,
        CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::player) + CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::enemy),
        sizeX, sizeY, life, damage, ignore);
    projectile->getAs<ControlledPhysicsEntity>().speedGain = 9999;
    projectile->getAs<ControlledPhysicsEntity>().topSpeed = speed;
    projectile->getAs<ControlledPhysicsEntity>().direction = {static_cast<float>(orientation.x), static_cast<float>(orientation.y)};

    auto sprite = (new SpriteEntity)->create(x,y,texture,16,16, 1);
    sprite->getAs<SpriteEntity>().rotate(atan2f(orientation.y, orientation.x) * 180 / std::numbers::pi);
    projectile->addChild(std::move(sprite));

    auto stateMachine = (new TickingEntity)->create();
    stateMachine->getAs<TickingEntity>().states
    = {{0, 1, 1,"normal", projectileLifetimeCounter}};
    stateMachine->addChild(std::move(projectile));

    return std::move(stateMachine);
}

auto buildDoor(sf::Vector2i to, Room::RoomData::Entrance dir) -> std::unique_ptr<Entity> {
    sf::Vector2f pos;
    switch(dir) {
        case 0: pos = sf::Vector2f(GAME_WIDTH_UNSCALED / 2.f, TILE_SIZE*0.75f) * Game::PIXEL_SCALE;
            break;
        case 1: pos = sf::Vector2f(GAME_WIDTH_UNSCALED - TILE_SIZE*0.75f,GAME_HEIGHT_UNSCALED/2.f) * Game::PIXEL_SCALE;
            break;
        case 2: pos = sf::Vector2f(GAME_WIDTH_UNSCALED / 2.f, -TILE_SIZE*0.75f + GAME_HEIGHT_UNSCALED) * Game::PIXEL_SCALE;
            break;
        case 3: pos = sf::Vector2f(TILE_SIZE*0.75f,GAME_HEIGHT_UNSCALED/2.f) * Game::PIXEL_SCALE;
            break;
    }
    auto sprite = (new SpriteEntity)->create(0,0, loadTxt("door"),32, 32,-1);
    sprite->getAs<SpriteEntity>().rotate(dir*90);
    auto interactible = (new Interactible)->create(1.25f,
        [to,pos,dir](Interactible &thing, Interactor &who) {
            Game::getInstance()->getLevelGenerator().setRoom(to.x, to.y);
            switch(dir) {
        case 0: Game::getInstance()->getPlayer().getAs<Entity2D>().dislocate(0, (GAME_HEIGHT_UNSCALED - TILE_SIZE*3.25f)*Game::PIXEL_SCALE);
            break;
        case 1: Game::getInstance()->getPlayer().getAs<Entity2D>().dislocate(-(GAME_WIDTH_UNSCALED - TILE_SIZE*3.25f)*Game::PIXEL_SCALE, 0);
            break;
        case 2: Game::getInstance()->getPlayer().getAs<Entity2D>().dislocate(0, -(GAME_HEIGHT_UNSCALED - TILE_SIZE*3.25f)*Game::PIXEL_SCALE);
            break;
        case 3: Game::getInstance()->getPlayer().getAs<Entity2D>().dislocate((GAME_WIDTH_UNSCALED - TILE_SIZE*3.25f)*Game::PIXEL_SCALE, 0);
            break;
        }
    });
    interactible->addChild(std::move(sprite));
    interactible->getAs<Entity2D>().setGlobalPos(pos.x,pos.y);
    fmt::print("{}",Game::getInstance()->getHierarchy());
    return interactible;
}

auto buildItemObject(ItemData& data, sf::Vector2f at)-> std::unique_ptr<Entity> {
    auto sprite = (new SpriteEntity)->create(at.x, at.y, loadTxt(data.name), 16, 16, -1);
    auto interactible = (new Interactible)->create(0.5f,
        [itemData = &data](Interactible &thing, Interactor &who)->  void {
            if(itemData->useOnPickup) {
                itemData->onUse(*itemData, thing.getGlobalPos(), sf::Vector2i(0,0), who);
                thing.endOfFrameRemove();
            }
            else if(who.addItem(*itemData)) {
                thing.endOfFrameRemove();
            }
        }
    );
    interactible->getAs<Entity2D>().setGlobalPos(at.x,at.y);
    interactible->addChild(std::move(sprite));
    return std::move(interactible);
}


