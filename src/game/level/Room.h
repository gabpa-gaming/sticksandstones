//
// Created by gabrys on 16.12.24.
//

#ifndef ROOM_H
#define ROOM_H
#include "../entity/Entity2D.h"


class Room : public virtual Entity2D {
    bool entered = false;
public:
    class RoomData {
    public:
        enum RoomType {
            none,
            regular,
            starting,
            item,
            boss
        };

        enum Entrance {
            up,
            right,
            down,
            left
        };
    private:
        RoomType type;

        char tiles[10][8];

        bool entrances[4]{};

        auto setFromString(std::string const& string) -> void;
    public:
        static std::vector<RoomData*> allRooms;

        static void loadAllRooms(std::string const& path);

        [[nodiscard]]auto getTile(int x, int y) const -> char;

        [[nodiscard]] auto getEntrance(Entrance e) const -> bool;

        [[nodiscard]] auto getType() const -> RoomType;

        explicit RoomData(std::string const& dataString);
    };

    RoomData* data;

    [[nodiscard]] auto getClassName() const -> std::string override {return "Room";}

    auto spawnTiles() -> void;

    auto create(RoomData& data) -> std::unique_ptr<Entity>;
};



#endif //ROOM_H
