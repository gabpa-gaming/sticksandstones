//
// Created by gabrys on 07.12.24.
//

#include "TickingEntity.h"

#include <sys/stat.h>

#include "SpriteEntity.h"

auto TickingEntity::newInstanceOfThisType() -> std::unique_ptr<Entity> {
    return std::move(std::make_unique<TickingEntity>());
}

auto TickingEntity::stateMachineTick() -> void {
    tickCounter++;

    if(tickCounter >= states[currentState].tickLength) {
        tickCounter -= states[currentState].tickLength;

        currentState += states[currentState].nextStateOffset;
        if(states[currentState].spriteIndex != -1) {
            auto p = getChildOfTypeRecursive<SpriteEntity>();
            dynamic_cast<SpriteEntity*>(p) -> setSpriteIndex(states[currentState].spriteIndex);
        }
        if(currentState >= states.size()) {
            throw std::runtime_error(fmt::format("{} is in a wrong or bugged state",getName()));
        }
        states[currentState].startOfState(*this, states[currentState]);

        return;
    }
    states[currentState].perTick(*this, states[currentState]);
}

auto TickingEntity::inconstantTick(float deltaT) -> void {

}

auto TickingEntity::setStateByName(std::string stateName) -> void {
    for (int i = 0; i < states.size(); i++) {
        if(states[i].stateName == stateName) {
            currentState = i;
            return;
        }
    }
    throw std::logic_error(fmt::format("{} is not a valid state name", getName()));
}
