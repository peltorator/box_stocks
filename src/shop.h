#pragma once

#include "filled_box.h"
#include <list>
#include <unordered_map>

class TShop {
private:
    typedef TBoxContainer std::list<Box>;

    TBoxConteiner Boxes;
    std::unordered_map<uint64, Item> Items;
    std::unordered_map<uint64, uint32> AvailableAmounts;
    std::unordered_map<uint64, uint32> OrderAmounts;
public:
    TShop(const std::list<std::pair<Item, uint32>> items) {
        for (const auto& [item, amount] : items) {
            Items[item.ItemID] = item;
            AvailableAmounts[item.ItemID] = amount;
        }
    }

    void AddItem(const uint64 itemID);

    void DeleteItem(consts uint64 itemID);

    std::list<TFilledBox> Buy();

    void Save();

    void Load();
};
