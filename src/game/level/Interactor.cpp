//
// Created by gabrys on 17.12.24.
//

#include "Interactor.h"

#include "Interactible.h"
#include "../Builders.h"
#include "../Game.h"

std::vector<ItemData*> Interactor::items;

auto Interactor::interactClosest() -> void {
    auto inRange = Game::getInstance()->rectCast(collider, getAsBitMask(interactible));
    std::ranges::sort(inRange, [pos = getGlobalPos()](auto& a, auto& b) {
        auto distA = pos - a->getGlobalPos();
        auto distB = pos - b->getGlobalPos();
        return dot(distA, distA) < dot(distB, distB);
    });
    while(!inRange.empty()) {
        if(inRange[0]->getAs<Interactible>().canInteract) {
            inRange[0]->getAs<Interactible>().onInteract(*this);
            break;
        }
        inRange.erase(inRange.begin());
    }
}

auto Interactor::addItem(ItemData &item) -> bool {
    for(int i = 0; i < items.size(); i++) {
        if(items[i]==nullptr) {
           items[i] = &item;
            return true;
        }
    }
    return false;
}

auto Interactor::dropFirstItem() -> void {
    buildItemObject(*items.front(), getGlobalPos());
    items.back() = nullptr;
}

auto Interactor::dropLastItem() -> void {
    if(items.back() == nullptr) {
        return;
    }
    buildItemObject(*items.back(), getGlobalPos());
    items.back() = nullptr;
}

auto Interactor::cycleItems() -> void {
    auto first = items.begin();
    items.push_back(*first);
    items.erase(first);
}

auto Interactor::useItem(sf::Vector2i dir) -> bool {
    if(items.front() == nullptr) {
        return false;
    }
    items.front()->onUse(*items.front(), getGlobalPos(), dir,*this);
    return true;
}

auto Interactor::setInvSize(int size) -> void {
    while(size > items.size()) {
        items.push_back(nullptr);
    }
    while(size < items.size()) {
        dropLastItem();
        items.erase(items.end()-1);
    }
}

auto Interactor::create(float range) -> std::unique_ptr<Entity> {
    auto base = CollidableEntity::create(0,0,0,0,range,range);
    base->getAs<CollidableEntity>().collisionEnabled = false;
    setInvSize(6);
    return std::move(base);
}

void Interactor::setItem(int i,  ItemData & itemData) {
    items[i] = &itemData;
}
