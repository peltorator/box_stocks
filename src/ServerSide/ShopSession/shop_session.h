#pragma once

#include <vector>
#include <random>
#include <unordered_map>
#include <utility>

#include "../../Model/filled_box.cpp"
#include "../../Model/box.cpp"
#include "../../Model/item.cpp"
#include "../DataBase/database_queries.cpp"


class TShopSession {
private:
    std::vector<TBox> _boxes;
    std::unordered_map<uint64_t, TItem> _items;
    std::unordered_map<uint64_t, uint32_t> _availableAmounts;
    std::unordered_map<uint64_t, uint32_t> _orderAmounts;
    std::mt19937 _rnd;

    const uint64_t _INF_COST = std::numeric_limits<uint64_t>::max();
    const size_t _PARTITIONS = 10;
    const size_t _MAX_ITEMS_IN_BLOCK = 12;
    
    std::pair<uint64_t, std::vector<TFilledBox>> PackSmall(const std::vector<TItem>& items, const std::set<uint64_t>& availableBoxIDs);
public:
    TShopSession(std::vector<std::pair<TItem, uint32_t>> itemAmounts = {}, std::vector<TBox> = {});

    void AddItem(const uint64_t itemID, const uint32_t amount = 1);

    void DeleteItem(const uint64_t itemID, const uint32_t amount = 1);

    std::vector<TFilledBox> Buy();

    bool OrderIsEmpty() const;
};
