//
// Created by gabrys on 25.11.24.
//

#include "../headers.h"

#include "entity/PhysicsEntity.h"
#include "entity/CollidableEntity.h"
#include "entity/Entity.h"

#include "Game.h"


#include "Builders.h"
#include "entity/SpriteEntity.h"
#include "entity/TickingEntity.h"
#include "level/Interactor.h"
#include "level/LevelGenerator.h"

bool Game::debugModeOn = false;

float Game::PIXEL_SCALE = 3;
int Game::PHYSICS_TICK_RATE = 35;
int Game::STATE_MACHINE_TICK_RATE = 100;
std::shared_ptr<Game> Game::instance = nullptr;


auto Game::getInstance() -> std::shared_ptr<Game> {
    if(instance)
        return instance;
    resetInstance();
    return instance;
}

auto Game::chooseSaveFile(std::shared_ptr<sf::RenderWindow> window) -> bool{

    std::string info = "Please input file name for new game... \n";
    std::string input = "";
    std::string info2 = "\n ...Or select a played file by typing its name: \n";

    if(!std::filesystem::exists("sav")) {
        if(!std::filesystem::create_directory("sav")) {
            throw std::logic_error("Failed to create sav folder");
        }
    }

    std::vector<std::string> saves;
    for (const auto& entry : std::filesystem::directory_iterator("sav/")) {
        if (entry.is_regular_file()) {
            saves.push_back(entry.path().filename().string());
            info2+= entry.path().filename().string() + "\n";
        }
    }

    sf::Font font;
    if (!font.loadFromFile("res/fonts/Silkscreen-Regular.ttf")) {
        throw std::logic_error("Failed to load font");
    }

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);


    text.setString(info+input+info2);
    bool end = false;
    while (window -> isOpen() && !end) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window -> pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window -> close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    std::regex invalidChars("[<>:\"/\\|?*]");
                    if (!std::regex_search(input, invalidChars)) {
                        chosenFileName = input;
                        end = true;
                        break;
                    }
                }
            }
            if(event.type == sf::Event::TextEntered && event.text.unicode < 128) {
                if(event.text.unicode == 8) {
                    if(input.length() > 0) {
                        input.erase(input.length()-1);
                    }
                }else if(input.length() < 15) {
                    input += static_cast<char>(event.text.unicode);
                }
                text.setString(info+input+info2);
            }

        }
        window -> clear(sf::Color::Black);

        window->draw(text);

        window -> display();
    }



    if(std::ranges::find(saves, input) != saves.end()) {
        return true;
    }
    return false;
}

auto Game::initGame() -> void {
    itemPool = {"tomBlessing","meat", "dagger", "helmet", "apple"};

    auto bgr = (new SpriteEntity())->create(288,240, loadTxt("bgr") ,192, 160,-5);
    auto wallColliderR = (new CollidableEntity())->create((GAME_WIDTH_UNSCALED)* Game::PIXEL_SCALE,0,
        CollidableEntity::getAsBitMask(CollidableEntity::wall),0,
        2, 20);
    auto wallColliderL = (new CollidableEntity())->create(0,0, CollidableEntity::getAsBitMask(CollidableEntity::wall),0,
        2, 20);
    auto wallColliderT = (new CollidableEntity())->create(0,0, CollidableEntity::getAsBitMask(CollidableEntity::wall),0,
        24, 2);
    auto wallColliderB = (new CollidableEntity())->create(0,(GAME_HEIGHT_UNSCALED) * Game::PIXEL_SCALE,
        CollidableEntity::getAsBitMask(CollidableEntity::wall),0,
        24, 2);

    addChild(std::move(wallColliderB));
    addChild(std::move(wallColliderR));
    addChild(std::move(wallColliderL));
    addChild(std::move(wallColliderT));

    addChild(std::move(bgr));
    auto player = buildPlayer();
    setPlayer(*player);
    addChild(std::move(player));
    auto gen = buildGenerator();
    setLevelGenerator(gen->getAs<LevelGenerator>());
    addChild(std::move(gen));
    initAllChildren(nullptr);

    auto playerUI = (new PlayerUI)->create();
    setPlayerUI(std::move(playerUI));

    goToNextLevel();

    fmt::println("Game initialized");
    fmt::println("{}", getHierarchy());
}

auto Game::tilePosToScreenCoords(sf::Vector2f pos) -> sf::Vector2f {
    return {pos.x * PIXEL_SCALE * TILE_SIZE, pos.y * PIXEL_SCALE * TILE_SIZE};
}

auto Game::resetInstance() -> void{
    std::shared_ptr<Entity> ptr = std::move((new Game)->create());
    instance = std::dynamic_pointer_cast<Game>(ptr);
    std::random_device device;
    instance->miscRNG.seed(device());
    instance->initGame();
}

std::unique_ptr<Entity> Game::create() {
    return std::move(Entity2D::create());
}

auto Game::gameLoop(std::shared_ptr<sf::RenderWindow>const& window) -> bool {
    if(chooseSaveFile(window)) {
        loadGameFromFile();
    }
    gameClock.restart();
    //sf::Clock frameCounter;
    float lastFrameTime = 60;
    float fps;
    lastPhysicsTick = gameClock.getElapsedTime();
    window->setKeyRepeatEnabled(false);
    window->setFramerateLimit(600);
    while (window -> isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;

        while (window -> pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window -> close();
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                rPressed = true;
                rKeyPressedClock.restart();
            }
            if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::R) {
                rKeyPressedClock.restart();
                rPressed = false;
            }
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::LControl) {
                player->getChildOfTypeRecursive<Interactor>()->cycleItems();
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::R) && rKeyPressedClock.getElapsedTime().asSeconds() > 1.f && rPressed) {
            restartGame = true;
        }
        window -> clear(sf::Color(85, 85, 85));

        if(gameClock.getElapsedTime() - lastPhysicsTick >= sf::seconds(1.0 / PHYSICS_TICK_RATE)) {
            physicsUpdateAll((gameClock.getElapsedTime() - lastPhysicsTick).asSeconds());
            //fmt::println("PTick: {}", (gameClock.getElapsedTime() - lastPhysicsTick).asMilliseconds());
            lastPhysicsTick = gameClock.getElapsedTime();
        }

        if(gameClock.getElapsedTime() - lastStateMachineTick >= sf::seconds(1.0 / STATE_MACHINE_TICK_RATE)) {
            tickAll();
            //fmt::println("SMTick: {}", (gameClock.getElapsedTime() - lastStateMachineTick).asMilliseconds());
            lastStateMachineTick = gameClock.getElapsedTime();
        }

        frameUpdateAll((gameClock.getElapsedTime() - lastFrame).asSeconds());



        while(!toRemove.empty()) {
            //fmt::print("{}", getHierarchy());
            toRemove[0] -> remove();
            toRemove.erase(toRemove.begin());
            //fmt::print("{}", getHierarchy());
        }

        //fmt::print("{}", getHierarchy());

        drawFrame(window);
        playerUI->draw(*window);

        window -> display();
        lastFrame = gameClock.getElapsedTime();
        float currentTime = gameClock.getElapsedTime().asSeconds();
        fps = (1.f / (currentTime - lastFrameTime)+fps)/2;
        lastFrameTime = currentTime;
        //fmt::print("FPS : {}\n", fps);

        if(restartGame) {
            restartGame = false;
            return true;
        }
    }
    fmt::println("Game closed.");
    return false;
}

auto Game::goToNextLevel() -> void {
    if(levelGenerator->levelCounter != 3) {
        playerUI->inform(fmt::format("Abandoned cheese factory ({})", Game::getInstance()->getLevelGenerator().levelCounter+1));
        levelGenerator->generateNextLevel();
        levelGenerator->setRoomToStart();
        return;
    }

    playerUI->setStateByName("win");
}

auto Game::drawFrame(std::shared_ptr<sf::RenderWindow> window) const -> void {
    auto entities = getAllChildrenOfTypeRecursive<SpriteEntity>();
    std::vector<SpriteEntity*> drawable;

    std::ranges::transform(entities, std::back_inserter(drawable), [](Entity* spr) -> SpriteEntity* {
        return dynamic_cast<SpriteEntity*>(spr);
    });

    std::ranges::sort(drawable, [](SpriteEntity* lhs, SpriteEntity* rhs) -> bool {
        return (lhs->drawOrder != rhs->drawOrder
        ? lhs->drawOrder < rhs->drawOrder
        : lhs -> getPosition().y < rhs -> getPosition().y);
    });


    for (auto entity: drawable) {
        if(entity->getEnabled()) window -> draw(*dynamic_cast<sf::Drawable*>(entity));
    }
    if(debugModeOn) {
        auto debugColliders = getAllChildrenOfTypeRecursive<CollidableEntity>();

        std::ranges::for_each(debugColliders, [window](Entity* ent) -> void {
            auto p = dynamic_cast<CollidableEntity*>(ent);
            if(!p || !p->getEnabled()) {
                return;
            }
            sf::RectangleShape rectangle;
            rectangle.setPosition(p->collider.getPosition());
            rectangle.setSize(p->collider.getSize());
            rectangle.setOutlineThickness(1);
            rectangle.setFillColor(sf::Color(0,0,0,0));
            rectangle.setOutlineColor(sf::Color::Green);
            window->draw(rectangle);
        });
    }
}

auto Game::physicsUpdateAll(float deltaT) const -> void {
    auto physicsEntities = getAllChildrenOfTypeRecursive<PhysicsEntity>();
    for(auto entity : physicsEntities) {
        if(entity->getEnabled()) dynamic_cast<PhysicsEntity*>(entity) -> physicsUpdate(deltaT);
    }
}

auto Game::frameUpdateAll(float deltaT) const -> void {
    auto tickable = getAllChildrenOfTypeRecursive<TickingEntity>();
    for (auto entity: tickable) {
        if(entity->getEnabled()) dynamic_cast<TickingEntity*>(entity) -> inconstantTick(deltaT);
    }
}

auto Game::tickAll() const -> void {
    auto tickable = getAllChildrenOfTypeRecursive<TickingEntity>();
    for (auto entity: tickable) {
        if(entity->getEnabled()) dynamic_cast<TickingEntity*>(entity) -> stateMachineTick();
    }
}

auto Game::rectCast(sf::FloatRect rect, std::bitset<8> mask) const -> std::vector<CollidableEntity*> {
    auto collidableEntity = getAllChildrenOfTypeRecursive<CollidableEntity>();
    std::vector<CollidableEntity*> out;
    for(auto entity : collidableEntity) {
        if(!entity->getEnabled()) continue;
        auto col = dynamic_cast<CollidableEntity*>(entity);
        if(!col -> collisionEnabled || !(col -> collisionMask & mask).any()) continue;
        for (auto other: col -> getColliders()) {
            if(other.intersects(rect)) {
                out.push_back(dynamic_cast<CollidableEntity*>(entity));
                break;
            }
        }

    }
    return out;
}

auto Game::getCurrentEnemyScaling() -> float {
    return std::pow(1.15f, levelGenerator->levelCounter);
}

auto Game::getPlayer() const -> Entity & {
    return *player;
}

auto Game::getPlayerUI() const -> PlayerUI & {
    return *playerUI;
}

auto Game::setPlayer(Entity& p) -> void {
    player = &p;
}

auto Game::setPlayerUI(std::unique_ptr<Entity> ui) -> void {
    playerUI = &ui->getAs<PlayerUI>();
    addChild(std::move(ui));
}

auto Game::getLevelGenerator() const -> LevelGenerator & {
    return *levelGenerator;
}

auto Game::setLevelGenerator(LevelGenerator &p) -> void {
    levelGenerator = &p;
}

auto Game::saveGameToFile() -> void {
    std::ofstream outFile("sav/"+chosenFileName, std::ios::trunc);
    if (outFile.is_open()) {
        outFile << levelGenerator->levelCounter;
        outFile << "\n\n";
        auto playerInteractor = player->getChildOfTypeRecursive<Interactor>();
        auto playerHP = player->getChildOfTypeRecursive<HealthController>();
        auto playerController = player->getChildOfType<PlayerController>();
        for(int i = 0; i < playerInteractor->items.size(); i++) {
            auto item = playerInteractor->items[i];
            if(item) {
                outFile << item->name << "\n";
            }
            else {
                outFile<< "\n";
            }
        }
        outFile << playerHP->getHealth() << "\n";
        outFile << playerHP->armor << "\n";
        outFile << playerController->damage << "\n";
        outFile << playerController->attackSpeed << "\n";
        outFile.close();
    }
}
auto Game::loadGameFromFile() -> void {
    std::ifstream inFile("sav/"+chosenFileName);
    levelGenerator->levelCounter = 0;

    if (!inFile.is_open()) {
        throw std::logic_error("File opening error");
    }

    auto playerInteractor = player->getChildOfTypeRecursive<Interactor>();
    auto playerHP = player->getChildOfTypeRecursive<HealthController>();
    auto playerController = player->getChildOfType<PlayerController>();

    std::string line;

    std::getline(inFile, line);
    levelGenerator->levelCounter = std::stoi(line);
    std::getline(inFile, line);

    for(int i = 0; i < playerInteractor->items.size(); i++) {
        std::string item;
        std::getline(inFile, item);
        if(item != "") {
            playerInteractor->setItem(i, &ItemData::getItemData(item));
        }
        else {
            playerInteractor->setItem(i, nullptr);
        }
    }
    std::getline(inFile, line);

    playerHP->setHealth(std::stof(line));

    std::getline(inFile, line);

    playerHP->armor = std::stof(line);

    std::getline(inFile, line);

    playerController->damage = std::stof(line);

    std::getline(inFile, line);

    playerController->attackSpeed = std::stof(line);

    goToNextLevel();
    inFile.close();
}
