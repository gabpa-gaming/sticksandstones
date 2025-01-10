//
// Created by gabrys on 26.12.24.
//

#include "ItemData.h"
#include "../Builders.h"
#include "../level/Interactor.h"
#include "../Game.h"

ItemData ItemData::itemList[] = {
    ItemData(0, "rock", 5.f),
    ItemData( 0, "smallPot", 5.f, &defaultThrow, false, true),
    ItemData(0, "stick", 5.f, &defaultThrow, false, 2, true),
    ItemData( 0, "helmet", 5.f, &helmetOnUse, true),
    ItemData( 0, "dagger", 5.f, &daggerOnUse, false, false)
};

auto ItemData::defaultThrow(ItemData &item, sf::Vector2f pos, sf::Vector2i dir, Interactor &who) -> void {
    auto itemObj = buildItemObject(item,pos);
    itemObj->initAllChildren(Game::getInstance()->currentRoom);
    //auto itemProjectile
    Game::getInstance()->currentRoom->addChild(std::move(itemObj));
    who.setItem(0, item);
};

ItemData::ItemData(int spriteNumerator, std::string name, float itemDamage,
                   const std::function<void(ItemData &item, sf::Vector2f pos, sf::Vector2i dir, Interactor &who)> &onUse,
                   bool useOnPickup, bool isFragile, int piercesEnemies, bool rare, bool breaksOnThrow):
                        spriteNumerator(spriteNumerator), name(name),
                        itemDamage(itemDamage), onUse(onUse),
                        useOnPickup(useOnPickup),
                        isFragile(isFragile), piercesEnemies(piercesEnemies),
                        rare(rare), breaksOnThrow(breaksOnThrow){}

auto ItemData::getItemData(std::string name) -> ItemData & {

    ItemData* item = &itemList[0];

    for(int i = 0; i < 64; i++) {
        if(itemList[i].name == name) {
            item = &itemList[i];
            break;
        }
    }

    return *item;
}

auto ItemData::helmetOnUse(ItemData &item, sf::Vector2f pos, sf::Vector2i dir, Interactor &who) -> void {
    auto player = &Game::getInstance()->getPlayer().getAs<HealthController>();
    player->takeDamage(-5);
    player->getInParents<PlayerController>()->damage++;
}

auto ItemData::daggerOnUse(ItemData &item, sf::Vector2f pos, sf::Vector2i dir, Interactor &who) -> void {
    auto player = &Game::getInstance()->getPlayer().getAs<HealthController>();
    player->takeDamage(-5);
    player->getInParents<PlayerController>()->damage++;
}

auto ItemData::tomBlessingOnUse(ItemData &item, sf::Vector2f pos, sf::Vector2i dir, Interactor &who) -> void {
    auto player = &Game::getInstance()->getPlayer().getAs<HealthController>();
    player->takeDamage(-5);
    player->getInParents<PlayerController>()->damage++;
}


auto ItemData::meatOnUse(ItemData &item, sf::Vector2f pos, sf::Vector2i dir, Interactor &who) -> void {
    auto player = &Game::getInstance()->getPlayer().getAs<HealthController>();
    player->takeDamage(-5);
    player->getInParents<PlayerController>()->damage++;
}