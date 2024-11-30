
#include "headers.h"
#include "game/Game.h"
#include "game/entity/SpriteEntity.h"


auto initGameWindow()-> sf::RenderWindow*;

auto initGame(sf::Window* window) -> Game*;

auto main() -> int {
    fmt::println("Running the game...");

    const auto window = initGameWindow();

    fmt::println("Window created");

    initGame(window);
    fmt::print(Game::getInstance()->getHierarchy());
    sf::Texture txt;
    txt.loadFromFile("res/char.png");
    sf::Sprite sprite;

    auto spr = new SpriteEntity();
    spr->create(Game::getInstance()->getChild(2)->getChild(0), 0,0, &txt);
    fmt::print(Game::getInstance()->getHierarchy());
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
        window -> draw(*spr);
        window -> display();
        sf::sleep(sf::milliseconds(100));
        dynamic_cast<Entity2D*>(Game::getInstance()->getChild(2))->dislocate(1,1);
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

auto initGame(sf::Window* window) -> Game*{

    Game* game = Game::getInstance();

    auto e1 = std::make_shared<Entity2D>(); e1 -> create(game);
    auto e2 = std::make_shared<Entity2D>(); e2 -> create(game);
    auto e3 = std::make_shared<Entity2D>(); e3 -> create(game);
    auto e4 = std::make_shared<Entity2D>(); e4 -> create(game);
    auto e5 = std::make_shared<Entity2D>(); e5 -> create(e3.get(), 1, 0);
    auto e6 = std::make_shared<Entity2D>(); e6 -> create(e2.get(), 2 , 5);
    auto e7 = std::make_shared<Entity2D>(); e7 -> create(e3.get(), 3, 4);
    auto e8 = std::make_shared<Entity2D>(); e8 -> create(e5.get(), 1 , 1);
    e5->dislocate(2,3);


    return game;
}