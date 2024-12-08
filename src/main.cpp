
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

    auto s = sprite.create(5,5, loadTxt("char"));

    std::vector<sf::FloatRect> r;

    auto c = character.create(0,0, 0, 0, r);

    dynamic_cast<PhysicsEntity*>(c.get())->acceleration = sf::Vector2f(120, 120);

    c->addChild(std::move(s));

    Game::getInstance()->addChild(std::move(c));

    Entity2D e;

    Game::getInstance()->gameLoop(w);
}