//
// Created by gabrys on 26.12.24.
//

#include "PlayerUI.h"

#include "../Game.h"


auto PlayerUI::inform(std::string text) -> void {
    informatorText.setString(text);
    informatorText.setFillColor(sf::Color::White);
    informatorSprite->setColor(sf::Color::White);
    auto rect = informatorText.getLocalBounds();
    informatorText.setOrigin(rect.left + rect.width/2.0f,rect.top  + rect.height/2.0f);
    setStateByName("inform");
}

auto PlayerUI::draw(sf::RenderWindow &w) -> void {
    w.draw(informatorText);
    w.draw(playerHpText);
}

auto PlayerUI::create() -> std::unique_ptr<Entity> {
    auto base = TickingEntity::create();

    static std::shared_ptr<sf::Font> font = std::make_unique<sf::Font>();

    playerHealthController = Game::getInstance()->getPlayer().getChildOfTypeRecursive<HealthController>();

    auto hpSprite = (new SpriteEntity)->create(32,32, loadTxt("heart"), 32,32, 10);

    auto deathScreenSprite = (new SpriteEntity)->create(GAME_WIDTH_UNSCALED / 2 * Game::PIXEL_SCALE,
        GAME_HEIGHT_UNSCALED / 2 * Game::PIXEL_SCALE, loadTxt("deathScreen"), 192,160, 20);

    this -> deathScreenSprite = &deathScreenSprite->getAs<SpriteEntity>();
    this -> deathScreenSprite->setColor(sf::Color(0));

    font->loadFromFile("res/fonts/Silkscreen-Regular.ttf");

    playerHpText.setFont(*font);
    playerHpText.setOutlineColor(sf::Color::Black);
    playerHpText.setOutlineThickness(3);
    playerHpText.setCharacterSize(32);
    playerHpText.setString(fmt::format("{}", playerHealthController->getHealth()));
    playerHpText.setOrigin(32,32);
    playerHpText.setPosition(64,48);


    auto informatorSprite = (new SpriteEntity)->create(GAME_WIDTH_UNSCALED / 2 * Game::PIXEL_SCALE,
        GAME_HEIGHT_UNSCALED / 2 * Game::PIXEL_SCALE, loadTxt("informer"), GAME_WIDTH_UNSCALED,GAME_HEIGHT_UNSCALED, 10);

    this -> informatorSprite = &informatorSprite->getAs<SpriteEntity>();

    informatorText.setFont(*font);
    informatorText.setCharacterSize(20);
    informatorText.setString("");
    informatorText.setPosition(GAME_WIDTH_UNSCALED / 2 * Game::PIXEL_SCALE,(GAME_HEIGHT_UNSCALED - 16) * Game::PIXEL_SCALE);

    base->addChild(std::move(hpSprite));
    base->addChild(std::move(informatorSprite));
    base->addChild(std::move(deathScreenSprite));

    auto updateHP = [this](TickingEntity &caller, StateMachineState &state) -> void {
        playerHpText.setString(fmt::format("{}", playerHealthController->getHealth()));
        if(!caller.isCurrentState("death") && playerHealthController->isDead()) {
            setStateByName("death");
        }
    };

    auto disappearCompletely = [this, updateHP](TickingEntity &caller, StateMachineState &state) -> void {
        sf::Color color = sf::Color(255,255,255, static_cast<sf::Uint8>(((0.f + state.tickLength - caller.tickCounter) / state.tickLength) * 255));
        this->informatorText.setFillColor(color); //How to disappear completely
        this->informatorSprite->setColor(color);
        updateHP(caller, state);
    };

    auto fadeIntoDeath = [this](TickingEntity &caller, StateMachineState &state) -> void {
        sf::Color color = sf::Color(255,255,255, static_cast<sf::Uint8>(((1.f + caller.tickCounter) / state.tickLength) * 255));
        this->deathScreenSprite->setColor(color);
        if(caller.tickCounter == state.tickLength) {
            this->deathScreenSprite->setColor(sf::Color::White);
        }
    };

    auto hideUI = [this](TickingEntity &caller, StateMachineState &state) -> void {
        this->informatorText.setFillColor(sf::Color(0));
        this->informatorSprite->setColor(sf::Color(0));
        this->playerHpText.setFillColor(sf::Color(0));
        this->playerHpText.setOutlineColor(sf::Color(0));
        this->getChildOfType<SpriteEntity>()->setColor(sf::Color(0));
    };

    states = {
        {-1,1, 0, "noting", updateHP},
        {-1,250, 1, "inform", updateHP},
        {-1,100, -2, "disappearCompletely", disappearCompletely},
        {-1,150, 1, "death", updateHP, hideUI},
        {-1,50, 1, "death", fadeIntoDeath},
        {-1,1, 0, "death"},
        {-1,1, 1, "win", updateHP, [this](auto&,auto&) {
            this -> deathScreenSprite->getAs<SpriteEntity>().setTexture(*loadTxt("winScreen"));}},
        {0,200, 1, "win",fadeIntoDeath, updateHP},
        {-1,100, 0, "win", hideUI}
    };


    return base;
}

