
#include "headers.h"
#include "game/entity/Entity.h"
#include "game/entity/Entity2D.h"
#include "game/entity/CollidableEntity.h"
#include "game/entity/PhysicsEntity.h"
#include "game/Game.h"
#include "game/entity/SpriteEntity.h"
#include "game/player/PlayerController.h"


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
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 600), "Sticks and stones");
    window -> setFramerateLimit(60);

    sf::Image image;
    assert(image.loadFromFile("res/ico.png"));

    window->setIcon(16,16,image.getPixelsPtr());

    return window;
}

auto initGame(std::shared_ptr<sf::RenderWindow> w) -> void {
    auto g = Game::getInstance();

    std::vector<sf::FloatRect> r;
    auto player = (new PlayerController)->create(0,0, 0,0 , r);

    auto ticker = (new TickingEntity)->create();

    auto sprite = (new SpriteEntity)->create(0,0, loadTxt("character"), 32, 32);

    std::vector<TickingEntity::StateMachineState> states =
        {{0,20,1, "idle"},
        {1,10,1, "idle"},
        {2,10,1, "idle"},
        {3,10,1, "idle"},
        {4,10,1, "idle"},
        {5,20,1, "idle"},
        {4,10,1, "idle"},
        {3,10,1, "idle"},
        {2,10,1, "idle"},
        {1,10,-8, "idle"},
        {6,5, 1, "walk"},
        {7,5, 1, "walk"},
        {8,5, 1, "walk"},
        {9,5, 1, "walk"},
        {10,5, 1, "walk"},
        {11,5, 1, "walk"},
        {12,5, 1, "walk"},
        {13,5, -7, "walk"},};

    auto t = dynamic_cast<TickingEntity*>(ticker.get());
    t -> states = states;
    t -> setStateByName("idle");

    auto p = dynamic_cast<PlayerController*>(player.get());
    p -> speedGain = 900;
    p -> topSpeed = 150;
    ticker -> addChild(std::move(sprite));

    player -> addChild(std::move(ticker));

    Game::getInstance()->addChild(std::move(player));

    Entity2D e;

    g->gameLoop(w);
}