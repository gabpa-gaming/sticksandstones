//
// Created by gabrys on 12.12.24.
//

#ifndef BUILDERS_H
#define BUILDERS_H
#include <memory>

#include "entity/Entity.h"

auto buildPlayer() -> std::unique_ptr<Entity>;
auto buildBat() -> std::unique_ptr<Entity>;
auto buildRock() -> std::unique_ptr<Entity>;

#endif //BUILDERS_H

