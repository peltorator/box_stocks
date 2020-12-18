#pragma once

#include "../ShopModel/filled_box.cpp"
#include "../DataBase/database_queries.cpp"
#include <vector>
#include <random>
#include <unordered_map>

class TShop {
private:
    std::vector<std::pair<TItem, uint32_t>> RawItems;
    std::vector<TBox> Boxes;
    std::unordered_map<uint64_t, TItem> Items;
    std::unordered_map<uint64_t, uint32_t> AvailableAmounts;
    std::unordered_map<uint64_t, uint32_t> OrderAmounts;
    std::mt19937 rnd;

    const uint64_t INF_COST = std::numeric_limits<uint64_t>::max();
    const size_t PARTITIONS = 10;
    const size_t MAX_ITEMS_IN_BLOCK = 12;
public:
    TShop();

    std::vector<TBox> GetBoxes() const;

    std::vector<std::pair<TItem, uint32_t>> GetRawItems() const;

    void AddItem(const uint64_t itemID, const uint32_t amount = 1);

    void DeleteItem(const uint64_t itemID, const uint32_t amount = 1);

    std::pair<uint64_t, std::vector<TFilledBox>> PackSmall(const std::vector<TItem>& items);

    std::vector<TFilledBox> Buy();

    void Save();

    void Load();

    bool OrderIsEmpty() const;
};
