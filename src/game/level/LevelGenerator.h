//
// Created by gabrys on 17.12.24.
//

#ifndef LEVELGENERATOR_H
#define LEVELGENERATOR_H
#include "Room.h"
#include "../entity/Entity2D.h"

#define MAP_SIZE 35
#define TILE_SIZE 16

class LevelGenerator : public virtual Entity2D{

    sf::Vector2i currentRoomPos;

    Room::RoomData* level[35][35] = {};

    std::mt19937_64 rng;

    unsigned long generationSeed = 0;

    int levelCounter = 0;

    struct VectorHasher {
        auto operator()(const sf::Vector2i& v) const -> size_t {
            return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
        }
    };

    std::unordered_map<sf::Vector2i, Room*, VectorHasher> loadedRooms;
public:
    auto getClassName() const -> std::string override;

    auto generateNextLevel() -> void;

    auto setRoomToStart() -> void;

    auto setRoom(int x, int y) -> void;

    auto tryGenerateNextLevel() -> bool;

    auto randWalk(int (&roomArr)[MAP_SIZE][MAP_SIZE], int length, int startX, int startY, sf::Vector2i) -> bool;

    static auto findDeadEnds(int (&roomArr)[MAP_SIZE][MAP_SIZE]) -> std::vector<sf::Vector2i>;

    auto findFarthestDeadEnd(const std::vector<sf::Vector2i> &deadEnds) -> sf::Vector2i;

    auto create(unsigned long seed) ->  std::unique_ptr<Entity>;
};



#endif //LEVELGENERATOR_H
