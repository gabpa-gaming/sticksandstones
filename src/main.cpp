
#include "headers.h"
#include "game/Game.h"


auto initGameWindow()-> sf::Window*;

auto initGame(sf::Window* window) -> Game*;

auto main() -> int {
    fmt::println("Running the game...");

    const auto window = initGameWindow();

    fmt::println("Window created");

    initGame(window);

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
    }
    fmt::println("Game closed.");
    delete window;
    return 0;
}

auto initGameWindow() -> sf::Window* {
    auto window = new sf::Window(sf::VideoMode(800, 600), "Sticks and stones");
    window -> setFramerateLimit(60);

    sf::Image image;
    assert(image.loadFromFile("res/ico.png"));

    window->setIcon(16,16,image.getPixelsPtr());

    return window;
}

auto initGame(sf::Window* window) -> Game*{

    Game* game = Game::getInstance();

    auto e1 = new Entity2D(); e1 -> create(game);
    auto e2 = new Entity2D(); e2 -> create(game);
    auto e3 = new Entity2D(); e3 -> create(game);
    auto e4 = new Entity2D(); e4 -> create(game);
    auto e5 = new Entity2D(); e5 -> create(e3, 1, 0);
    auto e6 = new Entity2D(); e6 -> create(e2, 2 , 5);
    auto e7 = new Entity2D(); e7 -> create(e3, 3, 4);
    auto e8 = new Entity2D(); e8 -> create(e5, 1 , 1);
    fmt::print(game->getHierarchy());

    e5->dislocate(2,3);
    fmt::print(game->getHierarchy());
    e5->dislocate(-2,-3);
    fmt::print(game->getHierarchy());

    return game;
}