//
// Created by gabrys on 17.12.24.
//

#ifndef INTERACTOR_H
#define INTERACTOR_H
#include "../entity/CollidableEntity.h"

#include "../item/ItemData.h"

class Interactor : public virtual CollidableEntity {

public:

    [[nodiscard]] auto getClassName() const -> std::string override { return "Interactor";}


    static std::vector<ItemData *> items;

    auto interactClosest() -> void;

    auto addItem(ItemData &item) -> bool;

    auto dropFirstItem() -> void;

    auto dropLastItem() -> void;

    auto cycleItems() -> void;

    auto useItem(sf::Vector2i dir) -> bool;

    auto setInvSize(int size) -> void;

    auto create(float range) -> std::unique_ptr<Entity>;

    void setItem(int i, ItemData &itemData);
};



#endif //INTERACTOR_H
