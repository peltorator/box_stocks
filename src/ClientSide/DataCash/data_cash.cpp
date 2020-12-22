#pragma once

#include <vector>
#include <utility>
#include <map>

#include "../../Model/box.cpp"
#include "../../Model/item.cpp"
#include "../HTTP/http_queries.cpp"

namespace NDataCash {
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

    void AddNewItem(const TItem& item) {
        AllItems.push_back(item);
        Items.emplace_back(item, 0);
        IdToItem[item.ItemID] = item;
    }

    void AddNewBox(const TBox& box) {
        AllBoxes.push_back(box);
        Boxes.emplace_back(box, 0);
        IdToBox[box.BoxID] = box;
    }

    void UpdateItem(const uint64_t itemID, const int32_t amount) {
        if (amount == 0) {
            return;
        }
        for (auto& itemAndAmount : Items) {
            if (itemAndAmount.first.ItemID == itemID) {
                itemAndAmount.second += amount;
                break;
            }
        }
    }

    void UpdateBox(const uint64_t boxID, const int32_t amount) {
        if (amount == 0) {
            return;
        }
        for (auto& boxAndAvailability : Boxes) {
            if (boxAndAvailability.first.BoxID == boxID) {
                boxAndAvailability.second += amount;
                break;
            }
        }
        for (size_t i = 0; i < AvailableBoxes.size(); i++) {
            if (AvailableBoxes[i].BoxID == boxID) {
                AvailableBoxes.erase(AvailableBoxes.begin() + i);
                break;
            }
        }
    }

    bool CheckIfItemExists(const std::string& itemName) {
        for (const TItem& item : AllItems) {
            if (item.ItemName == itemName) {
                return true;
            }
        }
        return false;
    }

    bool CheckIfBoxExists(const std::string& boxName) {
        for (const TBox& box : AllBoxes) {
            if (box.BoxName == boxName) {
                return true;
            }
        }
        return false;
    }
}
