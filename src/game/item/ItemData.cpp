//
// Created by gabrys on 26.12.24.
//

#include "ItemData.h"

#include "../Builders.h"
#include "../level/Interactor.h"
#include "../Game.h"
#include "../projectile/Projectile.h"

ItemData ItemData::itemList[] = {
    ItemData(0, "rock", 5.f),
    ItemData( 0, "smallPot", 9.f, &defaultThrow, false, true, 0),
    ItemData(0, "stick", 7.f, &defaultThrow, false, true, 4),
    ItemData( 0, "helmet", 5.f, &helmetOnUse, true),
    ItemData( 0, "dagger", 5.f, &daggerOnUse, true, false),
    ItemData( 0, "apple", 5.f, &appleOnUse, true, false),
    ItemData( 0, "bomb", 5.f, &bombOnUse, false, false),
    ItemData( 0, "tomBlessing", 5.f, &tomBlessingOnUse, true, false),
    ItemData( 0, "meat", 5.f, &meatOnUse, true, false),
    ItemData( 0, "exit", 5.f, &exitOnUse, true, false),
};

auto ItemData::defaultThrow(ItemData &item, sf::Vector2f pos, sf::Vector2i dir, Interactor &who) -> void {
    auto itemProjectile = buildBaseProjectile(item.itemDamage, 200, "thrownItem",
        0.5f, 0.5f, loadTxt(item.name), 3, who.getParent()->getChildOfType<HealthController>(),
        {static_cast<float>(dir.x), static_cast<float>(dir.y)}, pos.x, pos.y);
    itemProjectile->initAllChildren(Game::getInstance()->currentRoom);
    auto projRef = itemProjectile->getChildOfType<Projectile>();
    if(!item.isFragile) {
        projRef -> onDeath = [&item](Projectile & proj) -> void {
            auto itemObj = buildItemObject(item, proj.getGlobalPos());
            itemObj->initAllChildren(Game::getInstance()->currentRoom);
            (Game::getInstance()->currentRoom)->addChild(std::move(itemObj));
        };
    }
    else {
        projRef -> onDeath = [&item](Projectile & proj) -> void {
            item.onKill(item, proj.getGlobalPos());
        };
    }
    projRef -> penetrateCount = item.piercesEnemies;
    projRef -> penetrateWall = false;
    projRef -> collidesWith = projRef -> collidesWith.to_ulong() + 1;
    projRef -> ignoreCollisionsInMovement = true;
    (Game::getInstance()->currentRoom)->addChild(std::move(itemProjectile));
    who.setItem(0, nullptr);
};

ItemData::ItemData(int spriteNumerator, std::string name, float itemDamage,
                   const std::function<void(ItemData &item, sf::Vector2f pos, sf::Vector2i dir, Interactor &who)> &onUse,
                   bool useOnPickup, bool isFragile, int piercesEnemies, bool rare, bool breaksOnThrow, std::function<void(ItemData &item, sf::Vector2f pos)> onKill):
                        spriteNumerator(spriteNumerator), name(name),
                        itemDamage(itemDamage), onUse(onUse),
                        useOnPickup(useOnPickup),
                        isFragile(isFragile), piercesEnemies(piercesEnemies),
                        rare(rare), breaksOnThrow(breaksOnThrow), onKill(onKill){}

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
    auto player = Game::getInstance()->getPlayer().getChildOfTypeRecursive<HealthController>();
    player->takeDamage(-3);
    player->getParent()->getChildOfType<HealthController>()->armor+=2;
    Game::getInstance()->getPlayerUI().inform("Helmet! +2 DEF +3 HP");
}

auto ItemData::daggerOnUse(ItemData &item, sf::Vector2f pos, sf::Vector2i dir, Interactor &who) -> void {
    auto player = Game::getInstance()->getPlayer().getChildOfTypeRecursive<HealthController>();
    player->getInParents<PlayerController>()->attackSpeed *= 0.6f;
    Game::getInstance()->getPlayerUI().inform("Dagger! 40% ATK speed increase!");
}

auto ItemData::tomBlessingOnUse(ItemData &item, sf::Vector2f pos, sf::Vector2i dir, Interactor &who) -> void {
    auto player = Game::getInstance()->getPlayer().getChildOfTypeRecursive<HealthController>();
    player->takeDamage(-5);
    player->getInParents<PlayerController>()->damage++;
    player->getInParents<PlayerController>()->attackSpeed *= 0.7f;
    Game::getInstance()->getPlayerUI().inform("Holy Blessing! All stats up!");
}


auto ItemData::meatOnUse(ItemData &item, sf::Vector2f pos, sf::Vector2i dir, Interactor &who) -> void {
    auto player = Game::getInstance()->getPlayer().getChildOfTypeRecursive<HealthController>();
    player->takeDamage(-8);
    player->getInParents<PlayerController>()->damage += 2;
    Game::getInstance()->getPlayerUI().inform("Meat! +8 HP +2 DMG");
}

auto ItemData::appleOnUse(ItemData &item, sf::Vector2f pos, sf::Vector2i dir, Interactor &who) -> void {
    auto player = Game::getInstance()->getPlayer().getChildOfTypeRecursive<HealthController>();
    player->takeDamage(-16);
    Game::getInstance()->getPlayerUI().inform("Apple! +16 HP");
}

auto ItemData::exitOnUse(ItemData &item, sf::Vector2f pos, sf::Vector2i dir, Interactor &who) -> void {
    Game::getInstance()->saveGameToFile();
    Game::getInstance()->goToNextLevel();
}

auto ItemData::bombOnUse(ItemData &item, sf::Vector2f pos, sf::Vector2i dir, Interactor &who) -> void {
    auto bomb = buildBaseProjectile(0, 200, "bomb", 0.25f,0.25f, loadTxt("bomb"),
        0, who.getParent()->getChildOfType<HealthController>(),
        {static_cast<float>(dir.x), static_cast<float>(dir.y)}, pos.x, pos.y);
    bomb->getAs<TickingEntity>().states = {
        {0, 25, 1, "exploding"},
        {1, 25, 1, "exploding"},
        {2, 25, 1, "exploding"},
        {3, 25, 1, "exploding"},
        {4, 25, 1, "exploding"},
        {5, 25, 1, "exploding"},
        {6, 25, 1, "exploding"},
        {7, 25, 1, "exploding"},
        {8, 75, 1, "exploding"},
        {8, 25, 1, "exploding",projectileLifetimeCounter,}
    };


    auto bombRef = bomb->getChildOfType<ControlledPhysicsEntity>();

    bombRef->collidesWith = ControlledPhysicsEntity::getAsBitMask(CollidableEntity::enemy) + ControlledPhysicsEntity::getAsBitMask(CollidableEntity::wall);

    bombRef->onCollisionEvent = [](CollidableEntity* other, CollidableEntity* me) {
        me->getAs<ControlledPhysicsEntity>().direction = {0,0};
        me->getAs<ControlledPhysicsEntity>().velocity= {0,0};
    };

    bombRef->getAs<Projectile>().onDeath = [](Projectile& me) {
        auto boom = buildExplosion({me.getGlobalPos().x, me.getGlobalPos().y});
        boom->initAllChildren(Game::getInstance()->currentRoom);
        Game::getInstance()->currentRoom->addChild(std::move(boom));
    };

    bomb->initAllChildren(Game::getInstance()->currentRoom);

    Game::getInstance()->currentRoom->addChild(std::move(bomb));

    who.setItem(0, nullptr);

    Game::getInstance()->getPlayerUI().inform("Fire in the hole!");
}