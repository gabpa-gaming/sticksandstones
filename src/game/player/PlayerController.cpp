//
// Created by gabrys on 07.12.24.
//

#include "PlayerController.h"

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
    ControlledPhysicsEntity::physicsUpdate(deltaT);
}
