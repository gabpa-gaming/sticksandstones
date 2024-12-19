//
// Created by gabrys on 17.12.24.
//

#include "LevelGenerator.h"
#include "../Builders.h"
#include "../Game.h"

std::string LevelGenerator::getClassName() const {
    return "LevelGenerator";
}

auto LevelGenerator::generateNextLevel() -> void {
    levelCounter++;

    if(generationSeed == 0){
        std::random_device rd;
        generationSeed = rd();
    }
    rng.seed(generationSeed);
    while(!tryGenerateNextLevel());
}

auto LevelGenerator::setRoomToStart() -> void {
    for(int i = 0; i < MAP_SIZE;i++) {
        for(int j = 0; j < MAP_SIZE; j++) {
            if(level[i][j]&&level[i][j]->getType() == Room::RoomData::starting) {
                setRoom(i, j);
                return;
            }
        }
    }
}

auto LevelGenerator::setRoom(int x, int y) -> void {
    if(loadedRooms.contains({x,y})) {
        loadedRooms[currentRoom]->setEnabled(false);
        loadedRooms[{x,y}]->setEnabled(true);
        currentRoom = {x,y};
        Game::getInstance()-> currentRoom = loadedRooms[{x,y}];
        return;
    }
    if(currentRoom != sf::Vector2i{-1,-1}) {
        loadedRooms[currentRoom]->setEnabled(false);
    }
    auto room = buildRoom(*level[x][y]);
    auto roomPtr = room.get();
    addChild(std::move(room));
    loadedRooms[{x,y}] = &roomPtr->getAs<Room>();
    loadedRooms[{x,y}]-> initAllChildren(this);
    loadedRooms[{x,y}]-> spawnTiles();
    Game::getInstance()-> currentRoom = loadedRooms[{x,y}];
    currentRoom = {x,y};

}

auto LevelGenerator::tryGenerateNextLevel() -> bool {
    levelCounter = 1;
    int roomArr[MAP_SIZE][MAP_SIZE] = {{0}};

    int baseSize = 2*levelCounter + 2;

    int sizeVariation = round(1.5 * levelCounter + 1);

    int pathCount = round(sqrt(levelCounter*7));


    std::uniform_int_distribution<int> distribution(0, sizeVariation);

    while(pathCount--) {
        if(!randWalk(roomArr, baseSize + distribution(rng), MAP_SIZE/2,MAP_SIZE/2, rotate90NTimes({1,0}, distribution(rng)))) {
            return false;
        }
    }
    roomArr[MAP_SIZE/2][MAP_SIZE/2] = Room::RoomData::RoomType::starting;
    auto deadEnds = findDeadEnds(roomArr);
    if(deadEnds.size() < 3) {
        return false;
    }
    auto boss = findFarthestDeadEnd(deadEnds);
    roomArr[boss.x][boss.y] = Room::RoomData::RoomType::boss;
    erase_if(deadEnds, [boss](const auto& e) { return e.x == boss.x && e.y == boss.y; });
    roomArr[deadEnds[0].x][deadEnds[0].y] = Room::RoomData::RoomType::item;
    for(int i = 0; i < MAP_SIZE; i++) {
        for(int j = 0; j < MAP_SIZE; j++) {
            if(roomArr[i][j] == 0) {
                fmt::print(" ");
                continue;
            }
            fmt::print("{}", roomArr[i][j]);
        }
        fmt::print("\n");
    }

    for(int i = 0; i < MAP_SIZE; i++) {
        for(int j = 0; j < MAP_SIZE; j++) {
            if(roomArr[i][j] == Room::RoomData::RoomType::none) {
                continue;
            }
            auto comparer = [&,roomArr,i,j](const Room::RoomData* data) -> bool {
                return data->getType() == roomArr[i][j]
                && (roomArr[i][j+1] == Room::RoomData::RoomType::none || data->getEntrance(Room::RoomData::up))
                && (roomArr[i+1][j] == Room::RoomData::RoomType::none || data->getEntrance(Room::RoomData::right))
                && (roomArr[i][j-1] == Room::RoomData::RoomType::none || data->getEntrance(Room::RoomData::down))
                && (roomArr[i-1][j] == Room::RoomData::RoomType::none || data->getEntrance(Room::RoomData::left));
            };
            std::vector<Room::RoomData*> matching;
            std::ranges::copy_if(Room::RoomData::allRooms,std::back_inserter(matching), comparer);
            if(matching.empty()) {
                throw std::logic_error("No matching rooms found");
            }
            std::uniform_int_distribution<> dist(0,  matching.size() - 1);

            level[i][j] = matching[dist(rng)];
        }
    }

    return true;
}

auto LevelGenerator::randWalk(int (&roomArr)[MAP_SIZE][MAP_SIZE], int length, int startX, int startY, sf::Vector2i dir) -> bool {
    if(length == 0 ) {
        return true;
    }
    std::bernoulli_distribution distribDir(0.6); //change dir
    if(distribDir(rng)) {
        dir = rng()%2 == 1 ? rotate90CW(dir) : rotate90CCW(dir);
    }
    if(roomArr[startX+dir.x][startY+dir.y] != 0) {
        std::bernoulli_distribution distribChangeIfTaken(0.3);
        if(distribChangeIfTaken(rng)) {
            return randWalk(roomArr, length+1, startX, startY, dir);
        }
        dir = rng()%2 == 1 ? rotate90CW(dir) : rotate90CCW(dir);
    }
    if(startX + dir.x == MAP_SIZE || startX + dir.x == -1 || startY + dir.y == MAP_SIZE || startY + dir.y == -1 ||
        startX == -1 || startY == -1 || startX >= MAP_SIZE || startY >= MAP_SIZE) {
        return false;
    }
    for(int i = 0; i < 3; i++) {
        if(randWalk(roomArr, length-1, startX+dir.x, startY+dir.y, dir)) {
            roomArr[startX][startY] = 1;
            //roomArr[startX+dir.x][startY+dir.y] = 1;
            return true;
        }
    }
    return false;
}

auto LevelGenerator::findDeadEnds(int (&roomArr)[MAP_SIZE][MAP_SIZE]) -> std::vector<sf::Vector2i> {
    std::vector<sf::Vector2i> output;
    for(int i = 0; i < MAP_SIZE; i++) {
        for(int j = 0; j < MAP_SIZE; j++) {
            if(roomArr[i][j] == 1
                && i > 1 && i < MAP_SIZE-1 && j > 0 && j < MAP_SIZE-1) {
                int neighbours = 0;
                if(roomArr[i-1][j] != 0) {
                    neighbours += 1;
                }
                if(roomArr[i+1][j] != 0) {
                    neighbours += 1;
                }
                if(roomArr[i][j+1] != 0) {
                    neighbours += 1;
                }
                if(roomArr[i][j-1] != 0) {
                    neighbours += 1;
                }
                if(neighbours == 1) {
                    output.emplace_back(i, j);
                }
            }
        }
    }
    return output;
}

auto LevelGenerator::findFarthestDeadEnd(const std::vector<sf::Vector2i>& deadEnds) -> sf::Vector2i {
    int centerX = MAP_SIZE / 2;
    int centerY = MAP_SIZE / 2;

    sf::Vector2i farthestDeadEnd;
    double maxDistanceSquared = -1.0;

    for (const auto& deadEnd : deadEnds) {
        double distanceSquared = pow(deadEnd.x - centerX, 2) + pow(deadEnd.y - centerY, 2);

        if (distanceSquared > maxDistanceSquared) {
            maxDistanceSquared = distanceSquared;
            farthestDeadEnd = deadEnd;
        }
    }

    return farthestDeadEnd;
}

auto LevelGenerator::create(unsigned long seed) -> std::unique_ptr<Entity> {
    auto base = Entity2D::create();
    this -> generationSeed = seed;
    currentRoom = {-1,-1};
    return std::move(base);
}
