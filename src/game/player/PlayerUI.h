//
// Created by gabrys on 26.12.24.
//

#ifndef PLAYERUI_H
#define PLAYERUI_H
#include "../entity/Entity2D.h"
#include "../entity/HealthController.h"
#include "../entity/TickingEntity.h"
#include "../entity/SpriteEntity.h"



class PlayerUI : public virtual TickingEntity{
    HealthController* playerHealthController;
    sf::Text playerHpText;
    sf::Text informatorText;

    SpriteEntity *informatorSprite;
    SpriteEntity *deathScreenSprite;



public:
    auto inform(std::string text) -> void;

    auto draw(sf::RenderWindow& w) -> void;

    auto create() -> std::unique_ptr<Entity> override;
};



#endif //PLAYERUI_H
