//
// Created by gabrys on 07.12.24.
//

#include "TickingEntity.h"

#include <sys/stat.h>

#include "SpriteEntity.h"


auto TickingEntity::stateMachineTick() -> void {
    tickCounter++;

    if(tickCounter >= states[currentState].tickLength) {
        tickCounter -= states[currentState].tickLength;
        setState(currentState+states[currentState].nextStateOffset);
        return;
    }
    states[currentState].perTick(*this, states[currentState]);
}

auto TickingEntity::inconstantTick(float deltaT) -> void {
    if(states[currentState].spriteIndex != -1) {
        auto p = getChildOfTypeRecursive<SpriteEntity>();
        dynamic_cast<SpriteEntity*>(p) -> setSpriteIndex(states[currentState].spriteIndex);
    }
    if(currentState >= states.size()) {
        throw std::runtime_error(fmt::format("{} is in a wrong or bugged state",getName()));
    }
    states[currentState].startOfState(*this, states[currentState]);
}

auto TickingEntity::setState(int n) -> void {
    currentState = n;
}

auto TickingEntity::setStateByName(std::string stateName) -> void {
    if(states[currentState].stateName == stateName) {
        return;
    }
    for (int i = 0; i < states.size(); i++) {
        if(states[i].stateName == stateName) {
            setState(i);
            return;
        }
    }
    throw std::logic_error(fmt::format("{} is not a valid state name", getName()));
}
