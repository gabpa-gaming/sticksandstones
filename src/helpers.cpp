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