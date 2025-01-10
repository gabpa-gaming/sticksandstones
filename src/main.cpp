
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
#include "game/level/Room.h"



auto initGameWindow() -> std::shared_ptr<sf::RenderWindow>;

auto startGame(std::shared_ptr<sf::RenderWindow> w) -> void;

auto main() -> int {
    fmt::println("Running the game...");

    fmt::println("Window created");

    startGame(initGameWindow());

    return 0;
}

auto initGameWindow() -> std::shared_ptr<sf::RenderWindow> {
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(
        GAME_WIDTH_UNSCALED * Game::PIXEL_SCALE, GAME_HEIGHT_UNSCALED * Game::PIXEL_SCALE),
        "Sticks and stones");
    //window -> setFramerateLimit(60);

    sf::Image image;
    assert(image.loadFromFile("res/ico.png"));

    window->setIcon(16,16,image.getPixelsPtr());

    return window;
}

auto startGame(std::shared_ptr<sf::RenderWindow> w) -> void {
    Room::RoomData::loadAllRooms("res/rooms.data");

    while(Game::getInstance()->gameLoop(w)) {
        fmt::print("Restarting game...");
        Game::resetInstance();
    }
}