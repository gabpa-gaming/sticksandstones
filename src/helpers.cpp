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

auto dot(sf::Vector2f v1, sf::Vector2f v2) -> float {
    return v1.x * v2.x + v1.y * v2.y;
}

auto normalize(sf::Vector2f v) -> sf::Vector2f {
    return v / magnitude(v);
}

auto loadTxt(std::string name) -> std::shared_ptr<sf::Texture> {

    static std::map<std::string, std::shared_ptr<sf::Texture>> textures; //textures are loaded when needed

    if(textures.contains(name)) {
        return textures[name];
    }

    textures[name] = std::make_shared<sf::Texture>();

    textures[name]->loadFromFile(fmt::format("res/{}.png", name));

    if(textures[name]->loadFromFile(fmt::format("res/{}.png", name))) {
        return textures[name];
    }
    if(name =="msg") {
        throw std::logic_error("Even the missing texture is missing...");
    }
    return loadTxt("msg");
}

auto rotate90CCW(sf::Vector2i v) -> sf::Vector2i {
    return {-v.y, v.x};
}

auto rotate90CW(sf::Vector2i v) -> sf::Vector2i{
    return {v.y, -v.x};
}

auto rotate90NTimes(sf::Vector2i v, int n) -> sf::Vector2i {
    if(n > 0) {
        return rotate90NTimes( rotate90CW(v), n - 1);
    }
    if(n < 0) {
        return rotate90NTimes( rotate90CCW(v), n + 1);
    }
    return v;
}
