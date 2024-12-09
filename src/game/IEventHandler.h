//
// Created by gabrys on 08.12.24.
//

#ifndef IEVENTHANDLER_H
#define IEVENTHANDLER_H
#include "../headers.h"


class IEventHandler {
public:
    auto virtual handleEvent(sf::Event& event) -> void = 0;
};



#endif //IEVENTHANDLER_H
