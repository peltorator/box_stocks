#pragma once

#include "filled_box.h"
#include <list>
#include <unordered_map>
#include <vector>

class TShop {
private:
    typedef std::vector<TBox> TBoxContainer;

    TBoxContainer Boxes;
    std::unordered_map<uint64_t, TItem> Items;
    std::unordered_map<uint64_t, uint32_t> AvailableAmounts;
    std::unordered_map<uint64_t, uint32_t> OrderAmounts;

    const uint64_t INF_COST = std::numeric_limits<uint64_t>::max();
public:
    TShop(const std::list<std::pair<TItem, uint32_t>> items);

    void AddItem(const uint64_t itemID);

    void DeleteItem(const uint64_t itemID);

    std::list<TFilledBox> Buy();

    void Save();

    void Load();
};
