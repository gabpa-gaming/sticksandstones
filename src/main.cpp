
#include "headers.h"
#include "fmt/chrono.h"
#include "game/Game.h"
#include "game/entity/CollidableEntity.h"
#include "game/entity/Entity.h"
#include "game/entity/Entity2D.h"
#include "game/entity/PhysicsEntity.h"
#include "game/entity/SpriteEntity.h"
#include "game/player/PlayerController.h"


#include "game/Builders.h"

auto initGameWindow() -> std::shared_ptr<sf::RenderWindow>;

auto initGame(std::shared_ptr<sf::RenderWindow> w) -> void;

auto main() -> int {
    fmt::println("Running the game...");

    const auto window = initGameWindow();

    fmt::println("Window created");

    initGame(window);

    return 0;
}

auto initGameWindow() -> std::shared_ptr<sf::RenderWindow> {
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(576, 480), "Sticks and stones");
    //window -> setFramerateLimit(60);

    sf::Image image;
    assert(image.loadFromFile("res/ico.png"));

    window->setIcon(16,16,image.getPixelsPtr());

    return window;
}

auto initGame(std::shared_ptr<sf::RenderWindow> w) -> void {
    auto g = Game::getInstance();
    auto bgr = (new SpriteEntity())->create(288,240, loadTxt("bgr") ,192, 160,-1);

    g->addChild(std::move(bgr));
    g->addChild(std::move(buildPlayer()));

    g->addChild(std::move(buildRock()));
    g->addChild(std::move(buildBat()));
    dynamic_cast<Entity2D*>(g -> getChild(2).get()) -> setGlobalPos((192 + 32) * Game::PIXEL_SCALE/2,160 *Game::PIXEL_SCALE/2);

    g->initAllChildren(nullptr);
    g->gameLoop(w);
}