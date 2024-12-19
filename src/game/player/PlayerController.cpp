//
// Created by gabrys on 07.12.24.
//

#include "PlayerController.h"

#include "../Builders.h"
#include "../Game.h"
#include "../entity/HealthController.h"
#include "../entity/TickingEntity.h"
#include "../entity/Entity.h"
#include "../projectile/Projectile.h"

auto PlayerController::physicsUpdate(float deltaT) -> void {
    direction = sf::Vector2f(0,0);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        direction.y += -1;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        direction.x += -1;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        direction.y += 1;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        direction.x += 1;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && canAttack) {
        canAttack = false;
        Game::getInstance()->currentRoom->addChild(std::move(buildPlayerAttack({0, 1})));
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && canAttack) {
        canAttack = false;
        Game::getInstance()->currentRoom->addChild(std::move(buildPlayerAttack({1, 0})));
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && canAttack) {
        canAttack = false;
        Game::getInstance()->currentRoom->addChild(std::move(buildPlayerAttack({0, -1})));
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && canAttack) {
        canAttack = false;
        Game::getInstance()->currentRoom->addChild(std::move(buildPlayerAttack({-1, 0})));
    }
    ControlledPhysicsEntity::physicsUpdate(deltaT);

}

auto PlayerController::buildPlayerAttack(sf::Vector2i dir) -> std::unique_ptr<Entity> {
    auto proj = buildBaseProjectile(4, 0,"attack",
        0.75f, 0.75f,
        loadTxt("attack"), attackSpeed,
        this->parent->getChildOfTypeRecursive<HealthController>(), dir, getGlobalPos().x, getGlobalPos().y);

    proj->getChildOfType<Projectile>()->onDeath = [this](Projectile& ref) {
        this->canAttack = true;
    };
    auto stateMachine = dynamic_cast<TickingEntity*>(proj.get());
    const int ATTACK_FRAME_COUNT = 6;
    stateMachine->states[0].tickLength = static_cast<int>(Game::STATE_MACHINE_TICK_RATE * (attackSpeed / (ATTACK_FRAME_COUNT-1)));

    for(int i = 1; i < ATTACK_FRAME_COUNT; i++) {
        auto nextState = stateMachine->states[i-1];
        nextState.spriteIndex += 1;
        stateMachine->states.push_back(nextState);
    }
    proj->initAllChildren(Game::getInstance()->currentRoom);
    return std::move(proj);
}
