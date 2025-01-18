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

auto variedMoveTowardsPlayer(TickingEntity& caller, TickingEntity::StateMachineState&) -> void {
    if(Game::getInstance()->miscRNG() % 5 != 0) {
        return;
    }
    std::uniform_real_distribution<float> distribution(-0.75f, 0.75f);
    caller.getChildOfTypeRecursive<ControlledPhysicsEntity>() -> direction
    = normalize(rotateVectorThetaRadians(-caller.getChildOfTypeRecursive<HealthController>()->getGlobalPos()
        + Game::getInstance()
        ->getPlayer()
        .getChildOfTypeRecursive<HealthController>()
        ->getGlobalPos(), distribution(Game::getInstance()->miscRNG)));
}

auto projectileLifetimeCounter(TickingEntity& caller, TickingEntity::StateMachineState&) -> void {
    auto proj = caller.getChildOfType<Projectile>();
    caller.dislocate(0,0); //to update collision
    proj->lifetime -= 1.f/Game::STATE_MACHINE_TICK_RATE;
    if(proj->lifetime <= 0) {
        proj->kill();
        caller.states = {{}};
    }
}

auto buildPlayer() -> std::unique_ptr<Entity> {
    auto player = (new PlayerController)->create(0,0,
        CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::player),
        CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::wall), 0.25, 0.25);
    auto ticker = (new TickingEntity)->create();
    auto sprite = (new SpriteEntity)->create(0,0, loadTxt("character"), 32, 32);
    auto healthController = (new HealthController)->create(0,0, CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::player),
        CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::enemy),
        0.5f,0.75f, 20, 0, nullptr);
    auto interactor = (new Interactor)->create(0.75f);
    healthController->getAs<HealthController>().invTime = 0.5;
    healthController->getAs<HealthController>().onDeathEvent = [](Entity& caller) {
        caller.getInParents<TickingEntity>()->setStateByName("dead");
    };

    auto heldItemSprite = (new SpriteEntity)->create(0,0, loadTxt("nothing"), 16, 16, 1);

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

    player->getAs<PlayerController>().heldItemSprite = &heldItemSprite->getAs<SpriteEntity>();

    player->addChild(std::move(sprite));
    player->addChild(std::move(healthController));
    player->addChild(std::move(interactor));
    player->addChild(std::move(heldItemSprite));

    player->getAs<ControlledPhysicsEntity>().speedGain = 1026;
    player->getAs<ControlledPhysicsEntity>().topSpeed = 150;
    player->getAs<ControlledPhysicsEntity>().setGlobalPos(GAME_WIDTH_UNSCALED * Game::PIXEL_SCALE/2,GAME_HEIGHT_UNSCALED *Game::PIXEL_SCALE/2);
    player->getAs<ControlledPhysicsEntity>().colliderOffset = sf::Vector2f(0, 0.35);

    ticker -> addChild(std::move(player));

    return std::move(ticker);
}

auto buildBat() -> std::unique_ptr<Entity> {
    auto bat = (new ControlledPhysicsEntity)->create(0,0,
        CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::enemy), 0, 0.2f, 0.2f);
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
                                                           0.5,1, 18 * Game::getInstance()->getCurrentEnemyScaling(),
                                                           4 * Game::getInstance()->getCurrentEnemyScaling(), ticker.get());
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
        {7,200, -8, "idle"}
    };
    t -> states = states;
    t -> setStateByName("idle");

    rat->addChild(std::move(sprite));
    rat->addChild(std::move(healthController));

    rat->getAs<ControlledPhysicsEntity>().speedGain = 450;
    rat->getAs<ControlledPhysicsEntity>().topSpeed = 125;
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
                                                           0.5,1, 32 * Game::getInstance()->getCurrentEnemyScaling(),
                                                           4 * Game::getInstance()->getCurrentEnemyScaling(), ticker.get());
    healthController->getAs<HealthController>().invTime = 0;
    auto t = dynamic_cast<TickingEntity*>(ticker.get());

    static std::vector<TickingEntity::StateMachineState>states = {
        {-1,50, 1, "damage",},
        {0,12, 1, "walk", simpleMoveTowardsPlayer },
        {1,12, 1, "walk", simpleMoveTowardsPlayer},
        {2,12, 1, "walk", simpleMoveTowardsPlayer},
        {3,12, 1, "walk", simpleMoveTowardsPlayer},
        {4,12, 1, "walk", simpleMoveTowardsPlayer},
        {5,12, 1, "walk", simpleMoveTowardsPlayer},
        {6,12, 1, "walk", simpleMoveTowardsPlayer},
        {7,12, -7, "walk", simpleMoveTowardsPlayer},
        {0,250, -8, "idle"}
    };
    t -> states = states;
    t -> setStateByName("idle");

    if(Game::getInstance()->miscRNG() % 3 != 0) {
        healthController->getAs<HealthController>().onDeathEvent = [](Entity& self) {
            int i = Game::getInstance()->miscRNG() % 4 + 2;
            while (i-- > 0) {
                auto mini = buildMiniterror();
                auto pos = self.getAs<Entity2D>().getGlobalPos();
                mini->getAs<TickingEntity>().setGlobalPos(pos.x, pos.y);
                mini->initAllChildren(Game::getInstance()->currentRoom);
                Game::getInstance()->currentRoom->addChild(std::move(mini));
            }
        };
    }

    ementerror->addChild(std::move(sprite));
    ementerror->addChild(std::move(healthController));

    ementerror->getAs<ControlledPhysicsEntity>().speedGain = 450;
    ementerror->getAs<ControlledPhysicsEntity>().topSpeed = 50;
    //ementerror->getAs<ControlledPhysicsEntity>().setGlobalPos(GAME_WIDTH_UNSCALED * Game::PIXEL_SCALE/2,GAME_HEIGHT_UNSCALED *Game::PIXEL_SCALE/2);
    ementerror->getAs<ControlledPhysicsEntity>().colliderOffset = sf::Vector2f(0, 0.35);

    ticker -> addChild(std::move(ementerror));

    return std::move(ticker);
}

auto buildMiniterror() -> std::unique_ptr<Entity> {
    auto miniterror = (new ControlledPhysicsEntity)->create(0,0,
        CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::enemy),
        CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::wall), 0.25, 0.25);
    auto ticker = (new TickingEntity)->create();
    auto sprite = (new SpriteEntity)->create(0,0, loadTxt("miniterror"), 16, 16);
    auto healthController = (new HealthController)->create(0,0, CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::enemy),
                                                           CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::player),
                                                           0.25,0.25, 13* Game::getInstance()->getCurrentEnemyScaling(),
                                                           4 * Game::getInstance()->getCurrentEnemyScaling(), ticker.get());
    healthController->getAs<HealthController>().invTime = 0;
    auto t = dynamic_cast<TickingEntity*>(ticker.get());

    static std::vector<TickingEntity::StateMachineState>states = {
        {-1,50, 1, "damage",},
        {0,5, 1, "walk",},
        {1,5, 1, "walk", [](auto&,auto&) -> void{},variedMoveTowardsPlayer},
        {2,5, 1, "walk", [](auto&,auto&) -> void{},variedMoveTowardsPlayer},
        {3,5, 1, "walk", [](auto&,auto&) -> void{},variedMoveTowardsPlayer},
        {4,5, 1, "walk", [](auto&,auto&) -> void{},variedMoveTowardsPlayer},
        {5,5, 1, "walk", [](auto&,auto&) -> void{},variedMoveTowardsPlayer},
        {6,5, 1, "walk", [](auto&,auto&) -> void{},variedMoveTowardsPlayer},
        {7,5, -7, "walk", [](auto&,auto&) -> void{},variedMoveTowardsPlayer},
        {0,250, -8, "idle"}
    };
    t -> states = states;
    t -> setStateByName("walk");

    miniterror->addChild(std::move(sprite));
    miniterror->addChild(std::move(healthController));

    miniterror->getAs<ControlledPhysicsEntity>().speedGain = 450;
    miniterror->getAs<ControlledPhysicsEntity>().topSpeed = 100;
    miniterror->getAs<ControlledPhysicsEntity>().colliderOffset = sf::Vector2f(0, 0.35);

    ticker -> addChild(std::move(miniterror));

    return std::move(ticker);
}

auto buildCamembert() -> std::unique_ptr<Entity> {
    auto camembert = (new ControlledPhysicsEntity)->create(0,0,
        CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::enemy),
        CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::wall), 0.25, 0.25);
    auto ticker = (new TickingEntity)->create();
    auto sprite = (new SpriteEntity)->create(0,0, loadTxt("camembert"), 16, 16);
    auto healthController = (new HealthController)->create(0,0, CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::enemy),
                                                           CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::player),
                                                           0.5,1, 16 * Game::getInstance()->getCurrentEnemyScaling(),
                                                           4 * Game::getInstance()->getCurrentEnemyScaling(), ticker.get());
    healthController->getAs<HealthController>().invTime = 0;
    auto t = dynamic_cast<TickingEntity*>(ticker.get());

    static std::vector<TickingEntity::StateMachineState>states = {
        {0,150, 1, "idle"},
        {0,1, 1, "idle", camembertWait },
        {0,1, -1, "idle", camembertWait },
        {1,20, 1, "walk", },
        {2,20, 1, "walk", },
        {3,20, 1, "walk", },
        {1,20, 1, "walk", },
        {2,20, 1, "walk", },
        {3,20, 1, "walk", },
        {1,20, 1, "walk", },
        {2,20, 1, "walk", },
        {3,20, 1, "walk", },
        {3,20, 2, "walk", },
    };
    t -> states = states;
    t -> setStateByName("idle");

    camembert->addChild(std::move(sprite));
    camembert->addChild(std::move(healthController));

    camembert->getAs<ControlledPhysicsEntity>().speedGain = 450;
    camembert->getAs<ControlledPhysicsEntity>().topSpeed = 50;
    camembert->getAs<ControlledPhysicsEntity>().colliderOffset = sf::Vector2f(0, 0.35);

    ticker -> addChild(std::move(camembert));

    return std::move(ticker);
}

auto camembertWait(TickingEntity& caller, TickingEntity::StateMachineState&) -> void {
    auto playerPos = Game::getInstance()->getPlayer().getChildOfTypeRecursive<HealthController>()->getGlobalPos();
    auto pEntity = caller.getChildOfType<ControlledPhysicsEntity>();
    auto diff = - pEntity->getGlobalPos() + playerPos;

    pEntity->acceleration = {0,0};
    pEntity->direction = {0,0};
    pEntity->velocity = {0,0};
    pEntity->topSpeed = 0;

    if(diff.x < 20.f && diff.x > -20.f || diff.y < 20.f && diff.y > -20.f) {
        auto dir = normalize(diff);
        pEntity->topSpeed = 300;
        pEntity->direction = dir;
        caller.setStateByName("walk");
    }
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

auto buildTutorial() -> std::unique_ptr<Entity> {
    auto sprite = (new SpriteEntity)->create(GAME_WIDTH_UNSCALED * Game::PIXEL_SCALE/2,GAME_HEIGHT_UNSCALED *Game::PIXEL_SCALE/2, loadTxt("tutorial"),
        GAME_WIDTH_UNSCALED, GAME_HEIGHT_UNSCALED, -2);
    return std::move(sprite);
}

auto buildGenerator() -> std::unique_ptr<Entity> {
    auto levelGen = (new LevelGenerator)->create(0);
    return std::move(levelGen);
}

auto buildBaseProjectile(float damage, float speed, std::string name, float sizeX, float sizeY,
    const std::shared_ptr<sf::Texture>& texture, float life, CollidableEntity *ignore, sf::Vector2f orientation, float x, float y, int spriteSize) -> std::unique_ptr<Entity> {

    auto projectile = (new Projectile)->create(x,y, 0,
        CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::player) + CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::enemy),
        sizeX, sizeY, life, damage, ignore);
    projectile->getAs<ControlledPhysicsEntity>().speedGain = 9999;
    projectile->getAs<ControlledPhysicsEntity>().topSpeed = speed;
    projectile->getAs<ControlledPhysicsEntity>().direction = {orientation.x, orientation.y};

    auto sprite = (new SpriteEntity)->create(x,y,texture,spriteSize,spriteSize, 3);
    sprite->getAs<SpriteEntity>().rotate(std::atan2f(orientation.y, orientation.x) * 180 / std::numbers::pi);
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

auto buildExplosion(sf::Vector2f at) -> std::unique_ptr<Entity> {
    auto projectile = buildBaseProjectile(18* Game::getInstance()->getCurrentEnemyScaling(), 0, "boom",
        1.5,1.5,loadTxt("boom"), 0, nullptr, {0,0},at.x,at.y,32);
    projectile->getAs<TickingEntity>().states = {
        {0,10, 1},
        {1,10, 1},
        {2,10, 1},
        {3,10, 1},
        {4,10, 1},
        {5,10, 1},
        {6,10, 1},
        {7,10, 1},
        {8,10, 1},
        {9,10, 1},
        {10,10, 1},
        {11,10, 1},
        {12,10, 1},
        {13,10, 1},
        {14,10, 1},
        {15,10, 1},
        {15,1000, 1, "end", projectileLifetimeCounter},
    };
    auto physicsObj = projectile->getChildOfType<ControlledPhysicsEntity>();
    physicsObj -> collidesWith = physicsObj->collidesWith.to_ulong()+CollidableEntity::getAsBitMask(CollidableEntity::destructible);
    physicsObj -> onCollisionEvent = [](CollidableEntity *other, CollidableEntity *me) -> void{
        if(other->collisionMask.to_ulong() & CollidableEntity::getAsBitMask(CollidableEntity::destructible)) {
            other->endOfFrameRemove();
        }
    };
    return projectile;
}

auto buildSirMozzarella() -> std::unique_ptr<Entity> {
    auto mozzarella = (new ControlledPhysicsEntity)->create(0,0,
        CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::enemy), 0, 2.f, 2.f);
    auto ticker = (new TickingEntity)->create();
    auto sprite = (new SpriteEntity)->create(0,0, loadTxt("sirMozzarella"), 64, 64, 3);
    auto health = (new HealthController)->create(0,0,
         CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::enemy),
         CollidableEntity::getAsBitMask(CollidableEntity::ColliderType::player),
         2.f,2.f, 150 * Game::getInstance()->getCurrentEnemyScaling(),8 * Game::getInstance()->getCurrentEnemyScaling(), ticker.get());

    health->getAs<HealthController>().onDeathEvent = [](Entity& me) -> void {
        auto boom = buildExplosion(me.getAs<HealthController>().getGlobalPos());
        boom->initAllChildren(Game::getInstance()->currentRoom);
        Game::getInstance()->currentRoom->addChild(std::move(boom));
        auto exit = buildItemObject(ItemData::getItemData("exit"), me.getAs<HealthController>().getGlobalPos());
        exit->initAllChildren(Game::getInstance()->currentRoom);
        Game::getInstance()->currentRoom->addChild(std::move(exit));
    };

    auto stopMoving = [](TickingEntity &caller, TickingEntity::StateMachineState &state)->void {
        caller.getChildOfType<ControlledPhysicsEntity>() -> direction = {0,0};
        caller.getChildOfType<ControlledPhysicsEntity>() -> velocity = {0,0};
    };

    auto shootMissile = [](TickingEntity &caller, TickingEntity::StateMachineState &state)->void {
        auto hp = caller.getChildOfTypeRecursive<HealthController>();
        auto playerDir = normalize(-caller.getGlobalPos() + Game::getInstance()->getPlayer().getChildOfType<PlayerController>()->getGlobalPos());
        fmt::print("{},{}", hp->getGlobalPos().x,hp->getGlobalPos().y);
        auto missile = buildBaseProjectile(9 * Game::getInstance()->getCurrentEnemyScaling(),
            150 * Game::getInstance()->getCurrentEnemyScaling(), "chees", 0.25f,0.25f, loadTxt("cheeseArrow"), 5, hp,
            {playerDir.x,playerDir.y}, hp->getGlobalPos().x, hp->getGlobalPos().y);
        missile->getChildOfType<ControlledPhysicsEntity>()->collidesWith = CollidableEntity::getAsBitMask(CollidableEntity::player);
        missile->getChildOfType<ControlledPhysicsEntity>()->ignoreCollisionsInMovement = true;
        missile->initAllChildren(Game::getInstance()->currentRoom);
        Game::getInstance()->currentRoom->addChild(std::move(missile));
    };

    auto spawnMinis = [](TickingEntity &caller, TickingEntity::StateMachineState &state)->void {
        auto hp = caller.getChildOfTypeRecursive<HealthController>();
        auto amount = Game::getInstance()->miscRNG() % 5 + 2;
        while(amount--) {
            auto mini = buildMiniterror();
            mini->getAs<Entity2D>().setGlobalPos(hp->getGlobalPos().x,hp->getGlobalPos().y);
            mini->initAllChildren(Game::getInstance()->currentRoom);
            Game::getInstance()->currentRoom->addChild(std::move(mini));
        }
    };

    static std::vector<TickingEntity::StateMachineState> states =
    {
    {0,20,1, "walk", simpleMoveTowardsPlayer},
    {5,20,1, "walk", simpleMoveTowardsPlayer},
    {0,20,1, "walk", simpleMoveTowardsPlayer},
    {5,20,1, "walk", simpleMoveTowardsPlayer},
    {0,20,1, "walk", simpleMoveTowardsPlayer},
    {5,20,-5, "walk", mozzarellaStateChanger},
    {1,7,1, "shoot", [](auto&,auto&)->void{}, shootMissile},
    {0,50,1, "idle", stopMoving},
    {5,50,-1, "idle", mozzarellaStateChanger},
    {1,200,-2, "spawn", stopMoving, spawnMinis},
    };

    mozzarella -> addChild(std::move(sprite));

    auto t = dynamic_cast<TickingEntity*>(ticker.get());
    t -> states = states;


    mozzarella -> getAs<ControlledPhysicsEntity>().speedGain = 90;
    mozzarella -> getAs<ControlledPhysicsEntity>().topSpeed = 90 * Game::getInstance()->getCurrentEnemyScaling();
    mozzarella-> getAs<ControlledPhysicsEntity>().colliderOffset = sf::Vector2f(0, 0);


    mozzarella -> addChild(std::move(health));

    ticker -> addChild(std::move(mozzarella));
    t -> setStateByName("idle");

    //Game::getInstance()->getPlayerUI().inform("Skull guy VS. Sir Mozarella!! Fight!");

    return std::move(ticker);
}

auto mozzarellaStateChanger(TickingEntity &caller, TickingEntity::StateMachineState &state)->void {
    bool changeState = Game::getInstance()->miscRNG() % 2 == 0;
    if(!changeState) {
        return;
    }
    int roll = Game::getInstance()->miscRNG() % 100;
    if(state.stateName == "walk") {
        caller.setStateByName(roll <= 25 ? "spawn" : "shoot");
    }
    if(state.stateName == "idle") {
        caller.setStateByName(roll <= 30 ? "walk" : "shoot");
    }
}