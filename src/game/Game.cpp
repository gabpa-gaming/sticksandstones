//
// Created by gabrys on 25.11.24.
//

#include "../headers.h"

#include "entity/PhysicsEntity.h"
#include "entity/CollidableEntity.h"
#include "entity/Entity.h"

#include "Game.h"

#include "entity/SpriteEntity.h"
#include "entity/TickingEntity.h"

float Game::PIXEL_SCALE = 2.5;
int Game::PHYSICS_TICK_RATE = 75;
int Game::STATE_MACHINE_TICK_RATE = 100;
std::shared_ptr<Game> Game::instance = nullptr;



auto Game::getInstance() -> std::shared_ptr<Game> {
    if(instance)
        return instance;
    resetInstance();
    return instance;
}

auto Game::resetInstance() -> void{
    std::shared_ptr<Entity> ptr = std::move((new Game)->create());
    instance = std::dynamic_pointer_cast<Game>(ptr);
}

std::unique_ptr<Entity> Game::create() {
    return std::move(Entity2D::create());
}

auto Game::gameLoop(std::shared_ptr<sf::RenderWindow>const& window) -> void {
    gameClock.restart();
    lastPhysicsTick = gameClock.getElapsedTime();
    while (window -> isOpen())
    {

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;

        while (window -> pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window -> close();
        }
        window -> clear(sf::Color::Black);

        if(gameClock.getElapsedTime() - lastPhysicsTick >= sf::seconds(1.0 / PHYSICS_TICK_RATE)) {
            physicsUpdateAll((gameClock.getElapsedTime() - lastPhysicsTick).asSeconds());
            lastPhysicsTick = gameClock.getElapsedTime();
        }

        if(gameClock.getElapsedTime() - lastTick >= sf::seconds(1.0 / STATE_MACHINE_TICK_RATE)) {
            tickAll();
            lastTick = gameClock.getElapsedTime();
        }

        frameUpdateAll((gameClock.getElapsedTime() - lastFrame).asSeconds());


        fmt::print(getHierarchy());

        drawFrame(window);

        window -> display();
        lastFrame = gameClock.getElapsedTime();
    }
    fmt::println("Game closed.");
}

auto Game::drawFrame(std::shared_ptr<sf::RenderWindow> window) -> void {
    auto drawable = getAllChildrenOfTypeRecursive<SpriteEntity>();
    for (auto entity: drawable) {
        window.get() -> draw(*dynamic_cast<sf::Drawable*>(entity));
    }
}

auto Game::physicsUpdateAll(float deltaT) -> void {
    auto physicsEntities = getAllChildrenOfTypeRecursive<PhysicsEntity>();
    for(auto &entity : physicsEntities) {
        dynamic_cast<PhysicsEntity*>(entity) -> physicsUpdate(deltaT);
    }
}

auto Game::frameUpdateAll(float deltaT) -> void {
    auto tickable = getAllChildrenOfTypeRecursive<TickingEntity>();
    for (auto entity: tickable) {
        dynamic_cast<TickingEntity*>(entity) -> inconstantTick(deltaT);
    }
}

auto Game::tickAll() -> void {
    auto tickable = getAllChildrenOfTypeRecursive<TickingEntity>();
    for (auto entity: tickable) {
        dynamic_cast<TickingEntity*>(entity) -> stateMachineTick();
    }
}

auto Game::rectCast(sf::FloatRect rect, std::bitset<8> mask) -> std::vector<CollidableEntity*> {
    auto collidableEntity = getAllChildrenOfTypeRecursive<CollidableEntity>();
    std::vector<CollidableEntity*> out;
    for(auto entity : collidableEntity) {
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

bool Game::IS_ROOT_FLAG() {
    return true;
}

