//
// Created by gabrys on 25.11.24.
//

#include "../headers.h"

#include "entity/PhysicsEntity.h"
#include "entity/CollidableEntity.h"
#include "entity/Entity.h"

#include "Game.h"


#include "Builders.h"
#include "entity/SpriteEntity.h"
#include "entity/TickingEntity.h"
#include "level/Interactor.h"
#include "level/LevelGenerator.h"

bool Game::debugModeOn = false;

float Game::PIXEL_SCALE = 3;
int Game::PHYSICS_TICK_RATE = 35;
int Game::STATE_MACHINE_TICK_RATE = 100;
std::shared_ptr<Game> Game::instance = nullptr;


auto Game::getInstance() -> std::shared_ptr<Game> {
    if(instance)
        return instance;
    resetInstance();
    return instance;
}

auto Game::initGame() -> void {
    auto bgr = (new SpriteEntity())->create(288,240, loadTxt("bgr") ,192, 160,-5);
    auto wallColliderR = (new CollidableEntity())->create((GAME_WIDTH_UNSCALED)* Game::PIXEL_SCALE,0,
        CollidableEntity::getAsBitMask(CollidableEntity::wall),0,
        2, 20);
    auto wallColliderL = (new CollidableEntity())->create(0,0, CollidableEntity::getAsBitMask(CollidableEntity::wall),0,
        2, 20);
    auto wallColliderT = (new CollidableEntity())->create(0,0, CollidableEntity::getAsBitMask(CollidableEntity::wall),0,
        24, 2);
    auto wallColliderB = (new CollidableEntity())->create(0,(GAME_HEIGHT_UNSCALED) * Game::PIXEL_SCALE,
        CollidableEntity::getAsBitMask(CollidableEntity::wall),0,
        24, 2);

    addChild(std::move(wallColliderB));
    addChild(std::move(wallColliderR));
    addChild(std::move(wallColliderL));
    addChild(std::move(wallColliderT));

    addChild(std::move(bgr));
    auto player = buildPlayer();
    setPlayer(*player);
    addChild(std::move(player));
    auto gen = buildGenerator();
    setLevelGenerator(gen->getAs<LevelGenerator>());
    addChild(std::move(gen));
    initAllChildren(nullptr);

    auto playerUI = (new PlayerUI)->create();
    setPlayerUI(std::move(playerUI));
    fmt::println("Game initialized");
    fmt::println("{}", getHierarchy());
}

auto Game::tilePosToScreenCoords(sf::Vector2f pos) -> sf::Vector2f {
    return {pos.x * PIXEL_SCALE * TILE_SIZE, pos.y * PIXEL_SCALE * TILE_SIZE};
}

auto Game::resetInstance() -> void{
    std::shared_ptr<Entity> ptr = std::move((new Game)->create());
    instance = std::dynamic_pointer_cast<Game>(ptr);
    std::random_device device;
    instance->miscRNG.seed(device());
    instance->initGame();
}

std::unique_ptr<Entity> Game::create() {
    return std::move(Entity2D::create());
}

auto Game::gameLoop(std::shared_ptr<sf::RenderWindow>const& window) -> bool {
    gameClock.restart();
    //sf::Clock frameCounter;
    float lastFrameTime = 60;
    float fps;
    lastPhysicsTick = gameClock.getElapsedTime();
    window->setKeyRepeatEnabled(false);
    window->setFramerateLimit(600);
    while (window -> isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;

        while (window -> pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window -> close();
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                rPressed = true;
                rKeyPressedClock.restart();
            }
            if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::R) {
                rKeyPressedClock.restart();
                rPressed = false;
            }
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::LControl) {
                player->getChildOfTypeRecursive<Interactor>()->cycleItems();
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::R) && rKeyPressedClock.getElapsedTime().asSeconds() > 1.f && rPressed) {
            restartGame = true;
        }
        window -> clear(sf::Color(85, 85, 85));

        if(gameClock.getElapsedTime() - lastPhysicsTick >= sf::seconds(1.0 / PHYSICS_TICK_RATE)) {
            physicsUpdateAll((gameClock.getElapsedTime() - lastPhysicsTick).asSeconds());
            //fmt::println("PTick: {}", (gameClock.getElapsedTime() - lastPhysicsTick).asMilliseconds());
            lastPhysicsTick = gameClock.getElapsedTime();
        }

        if(gameClock.getElapsedTime() - lastStateMachineTick >= sf::seconds(1.0 / STATE_MACHINE_TICK_RATE)) {
            tickAll();
            //fmt::println("SMTick: {}", (gameClock.getElapsedTime() - lastStateMachineTick).asMilliseconds());
            lastStateMachineTick = gameClock.getElapsedTime();
        }

        frameUpdateAll((gameClock.getElapsedTime() - lastFrame).asSeconds());



        while(!toRemove.empty()) {
            //fmt::print("{}", getHierarchy());
            toRemove[0] -> remove();
            toRemove.erase(toRemove.begin());
            //fmt::print("{}", getHierarchy());
        }

        //fmt::print("{}", getHierarchy());

        drawFrame(window);
        playerUI->draw(*window);

        window -> display();
        lastFrame = gameClock.getElapsedTime();
        float currentTime = gameClock.getElapsedTime().asSeconds();
        fps = (1.f / (currentTime - lastFrameTime)+fps)/2;
        lastFrameTime = currentTime;
        //fmt::print("FPS : {}\n", fps);

        if(restartGame) {
            restartGame = false;
            return true;
        }
    }
    fmt::println("Game closed.");
    return false;
}

auto Game::drawFrame(std::shared_ptr<sf::RenderWindow> window) const -> void {
    auto entities = getAllChildrenOfTypeRecursive<SpriteEntity>();
    std::vector<SpriteEntity*> drawable;

    std::ranges::transform(entities, std::back_inserter(drawable), [](Entity* spr) -> SpriteEntity* {
        return dynamic_cast<SpriteEntity*>(spr);
    });

    std::ranges::sort(drawable, [](SpriteEntity* lhs, SpriteEntity* rhs) -> bool {
        return (lhs->drawOrder != rhs->drawOrder
        ? lhs->drawOrder < rhs->drawOrder
        : lhs -> getPosition().y < rhs -> getPosition().y);
    });


    for (auto entity: drawable) {
        if(entity->getEnabled()) window -> draw(*dynamic_cast<sf::Drawable*>(entity));
    }
    if(debugModeOn) {
        auto debugColliders = getAllChildrenOfTypeRecursive<CollidableEntity>();

        std::ranges::for_each(debugColliders, [window](Entity* ent) -> void {
            auto p = dynamic_cast<CollidableEntity*>(ent);
            if(!p || !p->getEnabled()) {
                return;
            }
            sf::RectangleShape rectangle;
            rectangle.setPosition(p->collider.getPosition());
            rectangle.setSize(p->collider.getSize());
            rectangle.setOutlineThickness(1);
            rectangle.setFillColor(sf::Color(0,0,0,0));
            rectangle.setOutlineColor(sf::Color::Green);
            window->draw(rectangle);
        });
    }
}

auto Game::physicsUpdateAll(float deltaT) const -> void {
    auto physicsEntities = getAllChildrenOfTypeRecursive<PhysicsEntity>();
    for(auto entity : physicsEntities) {
        if(entity->getEnabled()) dynamic_cast<PhysicsEntity*>(entity) -> physicsUpdate(deltaT);
    }
}

auto Game::frameUpdateAll(float deltaT) const -> void {
    auto tickable = getAllChildrenOfTypeRecursive<TickingEntity>();
    for (auto entity: tickable) {
        if(entity->getEnabled()) dynamic_cast<TickingEntity*>(entity) -> inconstantTick(deltaT);
    }
}

auto Game::tickAll() const -> void {
    auto tickable = getAllChildrenOfTypeRecursive<TickingEntity>();
    for (auto entity: tickable) {
        if(entity->getEnabled()) dynamic_cast<TickingEntity*>(entity) -> stateMachineTick();
    }
}

auto Game::rectCast(sf::FloatRect rect, std::bitset<8> mask) const -> std::vector<CollidableEntity*> {
    auto collidableEntity = getAllChildrenOfTypeRecursive<CollidableEntity>();
    std::vector<CollidableEntity*> out;
    for(auto entity : collidableEntity) {
        if(!entity->getEnabled()) continue;
        auto col = dynamic_cast<CollidableEntity*>(entity);
        if(!col -> collisionEnabled || !(col -> collisionMask & mask).any()) continue;
        for (auto other: col -> getColliders()) {
            if(other.intersects(rect)) {
                out.push_back(dynamic_cast<CollidableEntity*>(entity));
                break;
            }
        }

    }
    return out;
}

auto Game::getPlayer() const -> Entity & {
    return *player;
}

auto Game::setPlayer(Entity& p) -> void {
    player = &p;
}

auto Game::setPlayerUI(std::unique_ptr<Entity> ui) -> void {
    playerUI = &ui->getAs<PlayerUI>();
    addChild(std::move(ui));
}

auto Game::getLevelGenerator() const -> LevelGenerator & {
    return *levelGenerator;
}

auto Game::setLevelGenerator(LevelGenerator &p) -> void {
    levelGenerator = &p;
}

