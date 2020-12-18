#pragma once
#include <vector>
#include <map>
#include "../ShopModel/filled_box.cpp"
#include "../HTTP/http_queries.cpp"

namespace NDataProvider {
    std::vector<TItem> AllItems;
    std::vector<TBox> AllBoxes;

    std::vector<std::pair<TItem, uint32_t>> Items;
    std::vector<std::pair<TBox, uint32_t>> Boxes;

    std::vector<TBox> AvailableBoxes;

    std::map<uint64_t, TItem> IdToItem;
    std::map<uint64_t, TBox> IdToBox;

    void LoadItems() {
        Items = NHttp::GetItems();
        AllItems.clear();
        IdToItem.clear();
        AllItems.reserve(Items.size());
        for (const auto& [item, amount] : Items) {
            IdToItem[item.ItemID] = item;
            AllItems.emplace_back(item);
        }
    }

    void LoadBoxes() {
        Boxes = NHttp::GetBoxes();
        AllBoxes.clear();
        IdToBox.clear();
        AvailableBoxes.clear();
        AllBoxes.reserve(Boxes.size());
        for (const auto& [box, amount] : Boxes) {
            AllBoxes.emplace_back(box);
            IdToBox[box.BoxID] = box;
            if (amount > 0) {
                AvailableBoxes.emplace_back(box);
            }
        }
    }
}
