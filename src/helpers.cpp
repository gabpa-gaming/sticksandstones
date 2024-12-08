//
// Created by gabrys on 28.11.24.
//

#include "headers.h"

auto tabAllLines(const std::string& input) -> std::string {
    std::istringstream stream(input);
    std::ostringstream result;
    std::string line;

    while (std::getline(stream, line)) {
        result << "\t" << line << "\n";
    }

    return result.str();
}

auto magnitude(sf::Vector2f v) -> float {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

auto normalize(sf::Vector2f v) -> sf::Vector2f {
    return v / magnitude(v);
}

auto loadTxt(std::string name) -> std::shared_ptr<sf::Texture> {
    static std::map<std::string, std::shared_ptr<sf::Texture>> textures; //textures are loaded when needed, then exist until the program ends
    textures.insert(std::make_pair(name, std::make_shared<sf::Texture>()));

    if(!textures.at(name)->loadFromFile(fmt::format("res/{}.png", name))) {
        throw std::runtime_error(fmt::format("Failed to load texture from file res/{}.png", name));
    }
    return textures.at(name);
}
