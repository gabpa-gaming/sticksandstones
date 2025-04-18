//
// Created by gabrys on 07.12.24.
//

#ifndef TICKINGENTITY_H
#define TICKINGENTITY_H
#include "Entity.h"
#include "Entity2D.h"
#include "../../headers.h"

class TickingEntity : public virtual Entity2D { //acts like a state machine for entities

public:
    int tickCounter = 0;
    int currentState = 0;

    struct StateMachineState {
        int spriteIndex = -1; //index of sprite on texture, iterating left to right, down to bottom (-1 represents no sprite change)
        int tickLength = 1;
        int nextStateOffset = 1; //after this state ends next state will be currentState + nextStateOffset by default
        std::string stateName; //used for simple state changes
        std::function<void(TickingEntity & caller, StateMachineState & stateMachineState)> perTick =
            [](TickingEntity & , StateMachineState &) {};
        std::function<void(TickingEntity & caller, StateMachineState & stateMachineState)> startOfState =
            [](TickingEntity &, StateMachineState &) {};
    };

    std::vector<StateMachineState> states;

    auto getClassName() const -> std::string override{return "TickingEntity";}

    auto virtual stateMachineTick() -> void; //ticks based on game tick rate

    auto virtual inconstantTick(float deltaT) -> void; //ticks every frame
    auto setState(int n) -> void;

    auto setStateByName(const std::string &stateName) -> bool;
    [[nodiscard]] auto isCurrentState(std::string name) const -> bool;
};



#endif //TICKINGENTITY_H
