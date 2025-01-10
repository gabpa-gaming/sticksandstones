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
    }
    states[currentState].perTick(*this, states[currentState]);
}

auto TickingEntity::inconstantTick(float deltaT) -> void {

}

auto TickingEntity::setState(const int n) -> void {
    currentState = n;
    if(states[currentState].spriteIndex != -1) {
        auto p = getChildOfTypeRecursive<SpriteEntity>();
        if(p) p -> setSpriteIndex(states[currentState].spriteIndex);
    }
    if(currentState >= states.size()) {
        setState(n-(states.size()));
    }
    if(currentState < 0) {
        setState(n+(states.size()));
    }
    states[currentState].startOfState(*this, states[currentState]);
}

auto TickingEntity::setStateByName(const std::string &stateName) -> bool {
    if(states[currentState].stateName == stateName) {
        return true;
    }
    for (int i = 0; i < states.size(); i++) {
        if(states[i].stateName == stateName) {
            setState(i);
            return true;
        }
    }
    return false;
}

auto TickingEntity::isCurrentState(std::string name) const -> bool {
    return states[currentState].stateName == name;
}
