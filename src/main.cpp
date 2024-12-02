
#include "headers.h"
#include "game/Game.h"
#include "game/entity/SpriteEntity.h"


auto initGameWindow()-> sf::RenderWindow*;

auto initGame(sf::Window* window) -> std::shared_ptr<Game>;

auto main() -> int {
    fmt::println("Running the game...");

    const auto window = initGameWindow();

    fmt::println("Window created");

    initGame(window);
    fmt::print(Game::getInstance()->getHierarchy());
    sf::Texture txt;
    txt.loadFromFile("res/char.png");
    sf::Sprite sprite;

    SpriteEntity character;
    Game::getInstance()
    ->getChild(1)
    ->addChild(character.create(5,5,
        std::make_shared<sf::Texture>(txt)));
    fmt::print(Game::getInstance()->getHierarchy());

    auto spr = &Game::getInstance()->getChild(1)->getChild(0);

    while (window -> isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;

        while (window -> pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window -> close();
        }
        window -> clear(sf::Color::Black);
        window -> draw(*dynamic_cast<SpriteEntity*>(spr->get()));
        window -> display();
        sf::sleep(sf::milliseconds(100));
        //dynamic_cast<Entity2D*>(Game::getInstance()->getChild(2))->dislocate(1,1);
    }
    fmt::println("Game closed.");
    delete window;
    return 0;
}

auto initGameWindow() -> sf::RenderWindow* {
    auto window = new sf::RenderWindow(sf::VideoMode(800, 600), "Sticks and stones");
    window -> setFramerateLimit(60);

    sf::Image image;
    assert(image.loadFromFile("res/ico.png"));

    window->setIcon(16,16,image.getPixelsPtr());

    return window;
}

auto initGame(sf::Window* window) -> std::shared_ptr<Game>{

    Entity2D e;
    auto ent = e.create();
    Game::getInstance()->addChild(ent);
    auto ent2 = e.create();
    Game::getInstance()->addChild(ent2);

    return Game::getInstance();
}