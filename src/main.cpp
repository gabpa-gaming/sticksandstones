
#include "headers.h"


auto initGameWindow()-> sf::Window*;

auto initGame();

auto main() -> int {
    fmt::println("Running the game...");

    const auto window = initGameWindow();

    fmt::println("Window created");

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