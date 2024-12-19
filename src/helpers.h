//
// Created by gabrys on 28.11.24.
//

#ifndef HELPERS_H
#define HELPERS_H

#include "headers.h"

auto tabAllLines(const std::string& input) -> std::string;

auto magnitude(sf::Vector2f v) -> float;

auto normalize(sf::Vector2f v) -> sf::Vector2f;

auto loadTxt(std::string name) -> std::shared_ptr<sf::Texture>;

auto rotate90CCW(sf::Vector2i v) -> sf::Vector2i;

auto rotate90CW(sf::Vector2i v) -> sf::Vector2i;

auto rotate90NTimes(sf::Vector2i v, int n) -> sf::Vector2i;

#endif //HELPERS_H
