//
// Created by gabrys on 12.12.24.
//

#ifndef BUILDERS_H
#define BUILDERS_H
#include <memory>

#include "entity/Entity.h"
#include "level/Room.h"
#include "entity/CollidableEntity.h"
#include "entity/TickingEntity.h"

//compose more complex entities from simpler components

auto buildPlayer() -> std::unique_ptr<Entity>;
auto buildBat() -> std::unique_ptr<Entity>;
auto buildRat() -> std::unique_ptr<Entity>;
auto buildRock() -> std::unique_ptr<Entity>;
auto buildRoom(Room::RoomData& data) -> std::unique_ptr<Entity>;
auto buildGenerator() -> std::unique_ptr<Entity>;
auto buildBaseProjectile(float damage, float speed, std::string name, float sizeX, float sizeY,
    const std::shared_ptr<sf::Texture>& texture, float life, CollidableEntity *ignore,
    sf::Vector2i orientation, float x, float y) -> std::unique_ptr<Entity>;
auto buildDoor(sf::Vector2i to, Room::RoomData::Entrance dir) -> std::unique_ptr<Entity>;



auto simpleMoveTowardsPlayer(TickingEntity& caller, TickingEntity::StateMachineState&) -> void;
auto projectileLifetimeCounter(TickingEntity& caller, TickingEntity::StateMachineState&) -> void;
auto blinker(TickingEntity& caller, TickingEntity::StateMachineState&) -> void;
#endif //BUILDERS_H


