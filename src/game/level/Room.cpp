//
// Created by gabrys on 16.12.24.
//

#include "Room.h"

#include <fstream>

#include "../Builders.h"
#include "../Game.h"

std::vector<Room::RoomData*> Room::RoomData::allRooms;

auto Room::RoomData::setFromString(std::string const& str)-> void {
    int x = 0;
    int y = 0;
    int i = 0;
    while(str[i] != '\n') {
        switch(str[i]) {
            case 't':
                entrances[up] = true;
                break;
            case 'r':
                entrances[right] = true;
            break;
            case 'b':
                entrances[down] = true;
            break;
            case 'l':
                entrances[left] = true;
            break;
            case 'I':
                type = item;
            break;
            case 'S':
                type = starting;
            break;
            case 'B':
                type = boss;
            break;
            default:
                break;
        }
        i++;
    }
    i++;
    for(;;i++) {
        if(str[i] == '\n') {
            y++;
            i++;
            x=0;
            if(y == 8) break;
        }
        assert(x < 10 && y < 8);
        tiles[x][y] = str[i];
        x++;
    }
}

void Room::RoomData::loadAllRooms(std::string const &path) {
    std::ifstream f("res/rooms.data");

    if (!f.is_open()) {
        throw std::logic_error("Room file is missing");
    }

    std::string line;
    std::string roomData;
    while (std::getline(f, line)) {
        if(!line.empty()) {
            roomData.append(line);
            roomData.append("\n");
            continue;
        }
        if(!roomData.empty()) {
            allRooms.push_back(new RoomData(roomData));
        }
        roomData.clear();
    }

    f.close();
}

auto Room::RoomData::getTile(int x, int y) const -> char {
    return tiles[x][y];
}

auto Room::RoomData::getEntrance(Entrance e) const -> bool {
    return entrances[e];
}

auto Room::RoomData::getType() const -> RoomType {
    return type;
}

Room::RoomData::RoomData(std::string const& dataStr) {
    memset( &tiles[0][0], '0', sizeof(tiles) );
    std::fill(&entrances[0], &entrances[3], false);
    type = regular;
    setFromString(dataStr);
}

auto Room::spawnTiles() -> void {
    assert(data);
    const float TILE_SIZE = 16;
    for(int x = 0; x < 10; x++)
        for(int y = 0; y < 8; y++) {
            std::unique_ptr<Entity> entity;
            switch(data->getTile(x,y)) {
                case '1': //rock
                    entity = buildRock();
                break;
                case 'b': //bat
                    entity = buildBat();
                break;
                default:
                    continue;
            }
            entity->getAs<Entity2D>().setGlobalPos((x + 1.5f) * TILE_SIZE * Game::PIXEL_SCALE, (y + 1.5f) * TILE_SIZE * Game::PIXEL_SCALE);
            entity->initAllChildren(this);
            addChild(std::move(entity));
        }
}

std::unique_ptr<Entity> Room::create(RoomData& data) {
    auto base = Entity2D::create();

    this->data = &data;
    return std::move(base);
}
