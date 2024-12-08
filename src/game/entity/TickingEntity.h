//
// Created by gabrys on 07.12.24.
//

#ifndef TICKINGENTITY_H
#define TICKINGENTITY_H
#include "Entity.h"
#include "../../headers.h"

class TickingEntity : public virtual Entity {

    int tickCounter = 0;
public:
    int currentState = 0;

    struct StateMachineState {
        int spriteIndex = -1; //index of sprite on texture, iterating left to right, down to bottom (-1 represents no sprite change)
        int tickLength = 1;
        int nextStateOffset = 1; //after this state ends next state will be currentState + nextStateOffset by default
        std::function<void(TickingEntity const& caller, StateMachineState const& stateMachineState)> perTick;
        std::function<void(TickingEntity const& caller, StateMachineState const& stateMachineState)> startOfState;
    };

    std::vector<StateMachineState> states;

    auto getClassName() const -> std::string override{return "TickingEntity";}

    auto newInstanceOfThisType() -> std::unique_ptr<Entity> override;

    auto virtual stateMachineTick() -> void; //ticks based on game tick rate

    auto virtual inconstantTick(float deltaT) -> void; //ticks every frame
};



#endif //TICKINGENTITY_H
