
#include "headers.h"
#include "game/entity/Entity.h"
#include "game/entity/Entity2D.h"
#include "game/entity/CollidableEntity.h"
#include "game/entity/PhysicsEntity.h"
#include "game/Game.h"
#include "game/entity/SpriteEntity.h"


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
    Game::getInstance();

    PhysicsEntity character;
    SpriteEntity sprite;
    TickingEntity ticker;

    auto s = sprite.create(5,5, loadTxt("character"), 32, 32);

    auto sm = ticker.create();

    std::vector<TickingEntity::StateMachineState> states =
        {{0,5,1, "idle"},
        {1,5,1},
        {2,5,1},
        {3,5,1},
        {4,5,1},
        {3,5,1},
        {2,5,1},
        {1,5,-7},};

    dynamic_cast<TickingEntity*>(sm.get()) -> states = states;

    std::vector<sf::FloatRect> r;

    auto c = character.create(0,0, 0, 0, r);

    dynamic_cast<PhysicsEntity*>(c.get())->acceleration = sf::Vector2f(120, 120);

    c->addChild(std::move(s));

    sm -> addChild(std::move(c));

    Game::getInstance()->addChild(std::move(sm));

    Entity2D e;

    Game::getInstance()->gameLoop(w);
}