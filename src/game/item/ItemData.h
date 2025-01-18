//
// Created by gabrys on 26.12.24.
//

#ifndef ITEMDATA_H
#define ITEMDATA_H
#include "../../headers.h"

class Interactor;

struct ItemData {
    static ItemData itemList[];

    const int spriteNumerator;
    const std::string name;
    const float itemDamage;
    const std::function<void(ItemData &item, sf::Vector2f pos, sf::Vector2i dir, Interactor &who)> onUse;
    const bool useOnPickup;
    const bool isFragile;
    const int piercesEnemies;
    const bool rare;
    const bool breaksOnThrow;
    const std::function<void(ItemData &item, sf::Vector2f pos)> onKill;

    static auto defaultThrow(ItemData &item, sf::Vector2f pos, sf::Vector2i dir, Interactor &who) -> void;

    explicit ItemData(int spriteNumerator, std::string name, float itemDamage = 4.f,
             const std::function<void(ItemData &item, sf::Vector2f pos, sf::Vector2i dir, Interactor &who)> &onUse = defaultThrow,
             bool useOnPickup = false, bool isFragile = false, int piercesEnemies = 99, bool rare = false, bool breaksOnThrow = true,
             std::function<void(ItemData &item, sf::Vector2f pos)> onKill = [](ItemData &item, sf::Vector2f pos) {});


    static auto getItemData(std::string name) -> ItemData &;

    static auto meatOnUse(ItemData &item, sf::Vector2f pos, sf::Vector2i dir, Interactor &who) -> void;

    static auto appleOnUse(ItemData &item, sf::Vector2f pos, sf::Vector2i dir, Interactor &who) -> void;

    static auto exitOnUse(ItemData &item, sf::Vector2f pos, sf::Vector2i dir, Interactor &who) -> void;

    static auto bombOnUse(ItemData &item, sf::Vector2f pos, sf::Vector2i dir, Interactor &who) -> void;

    static auto helmetOnUse(ItemData &item, sf::Vector2f pos, sf::Vector2i dir, Interactor &who) -> void;

    static auto daggerOnUse(ItemData &item, sf::Vector2f pos, sf::Vector2i dir, Interactor &who) -> void;

    static auto tomBlessingOnUse(ItemData &item, sf::Vector2f pos, sf::Vector2i dir, Interactor &who) -> void;
};



#endif //ITEMDATA_H
