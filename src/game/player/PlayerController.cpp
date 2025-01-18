//
// Created by gabrys on 07.12.24.
//

#include "PlayerController.h"

#include "../Builders.h"
#include "../Game.h"
#include "../entity/HealthController.h"
#include "../entity/TickingEntity.h"
#include "../entity/Entity.h"
#include "../entity/SpriteEntity.h"
#include "../level/Interactor.h"
#include "../projectile/Projectile.h"
#include "../level/LevelGenerator.h"

auto PlayerController::physicsUpdate(float deltaT) -> void {
    direction = sf::Vector2f(0,0);
    if(getChildOfTypeRecursive<HealthController>()->isDead()) {
        ControlledPhysicsEntity::physicsUpdate(deltaT);
        return;
    }
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

    auto attackOrUseDir = sf::Vector2i();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && canAttack) {
        attackOrUseDir = {0, -1};
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && canAttack) {
        attackOrUseDir = {1, 0};
        getChildOfTypeRecursive<SpriteEntity>()->setFlipX(false);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && canAttack) {
        attackOrUseDir = {0, 1};
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && canAttack) {
        attackOrUseDir = {-1, 0};
        getChildOfTypeRecursive<SpriteEntity>()->setFlipX(true);
    }
    if(!(attackOrUseDir.x == 0 && attackOrUseDir.y == 0) && interactClock.getElapsedTime().asSeconds() > interactTime & getChildOfTypeRecursive<Interactor>()->useItem(attackOrUseDir)) {
        interactClock.restart();
    } else if(!(attackOrUseDir.x == 0 && attackOrUseDir.y == 0)) {
        canAttack = false;
        addChild(std::move(buildPlayerAttack(attackOrUseDir)));

    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && interactClock.getElapsedTime().asSeconds() > interactTime) {
        interactClock.restart();
        getChildOfTypeRecursive<Interactor>()->dropFirstItem();
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::E) && interactClock.getElapsedTime().asSeconds() > interactTime) {
        interactClock.restart();
        getChildOfTypeRecursive<Interactor>()->interactClosest();
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Grave) && interactClock.getElapsedTime().asSeconds() > interactTime) {
        interactClock.restart();
        Game::debugModeOn = !Game::debugModeOn;
    }

    auto item = getChildOfType<Interactor>()->items[0];
    if(item) {
        heldItemSprite->setTexture(*loadTxt(item->name));
    }else {
        heldItemSprite->setTexture(*loadTxt("nothing"));
    }


    ControlledPhysicsEntity::physicsUpdate(deltaT);
}

auto PlayerController::buildPlayerAttack(sf::Vector2i dir) -> std::unique_ptr<Entity> {
    auto displacement = range * sf::Vector2f{static_cast<float>(dir.x),static_cast<float>(dir.y)}*static_cast<float>(TILE_SIZE)*Game::PIXEL_SCALE;
    auto proj = buildBaseProjectile(damage, 0,"attack",
        0.75f, 0.75f,
        loadTxt("attack"), attackSpeed,
        this->parent->getChildOfTypeRecursive<HealthController>(),
        {static_cast<float>(dir.x),static_cast<float>(dir.y)}, getGlobalPos().x + displacement.x, getGlobalPos().y+displacement.y);

    auto projRef = proj->getChildOfType<Projectile>();

    projRef->onDeath = [this](Projectile& ref) {
        this->canAttack = true;
    };
    projRef->knockbackTime = 0.25f;
    projRef->knockbackForce = sf::Vector2f(dir.x * 4, dir.y * 4);
    projRef->ignoreCollisionsInMovement = true;
    auto stateMachine = dynamic_cast<TickingEntity*>(proj.get());
    const int ATTACK_FRAME_COUNT = 6;
    stateMachine->states[0].tickLength = static_cast<int>(Game::STATE_MACHINE_TICK_RATE * (attackSpeed / (ATTACK_FRAME_COUNT-1)));

    for(int i = 1; i < ATTACK_FRAME_COUNT; i++) {
        auto nextState = stateMachine->states[i-1];
        nextState.spriteIndex += 1;
        stateMachine->states.push_back(nextState);
    }

    proj->initAllChildren(this);
    return std::move(proj);
}


