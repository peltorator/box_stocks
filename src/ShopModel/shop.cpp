#include "shop.h"
#include <iostream>

TShop::TShop(const std::vector<std::pair<TItem, uint32_t>>& items, const std::vector<TBox>& boxes) {
    for (const auto& [item, amount] : items) {
        Items[item.ItemID] = item;
        AvailableAmounts[item.ItemID] = amount;
    }
    Boxes = boxes;
}

void TShop::AddItem(const uint64_t itemID, const uint64_t amount) {
    for (uint64_t i = 0; i < amount; i++) {
        OrderAmounts[itemID]++;
    }
}

void TShop::DeleteItem(const uint64_t itemID, const uint64_t amount) {
    for (uint64_t i = 0; i < amount; i++) {
        OrderAmounts[itemID]--;
    }
}

std::vector<TFilledBox> TShop::Buy() {
    std::vector<TItem> items;
    for (const auto& [itemID, amount] : OrderAmounts) {
        for (uint32_t ind = 0; ind < amount; ind++) {
            items.push_back(Items[itemID]);
        }
    }
    const uint64_t DP_SIZE = (1LL << items.size());
    std::vector<uint64_t> totalWeight(DP_SIZE, 0);
    std::vector<uint64_t> totalVolume(DP_SIZE, 0);
    std::vector<uint64_t> minCost(DP_SIZE, INF_COST);
    std::vector<std::pair<uint64_t, size_t>> lastBox(DP_SIZE);
    minCost[0] = 0;

    for (uint64_t mask = 0; mask < DP_SIZE; mask++) {
        for (size_t bit = 0; bit < items.size(); bit++) {
            if ((mask >> bit) & 1) {
                totalWeight[mask] += items[bit].Weight;
                totalVolume[mask] += items[bit].Volume;
            }
        }
        for (uint64_t subMask = mask; subMask > 0; subMask = (subMask - 1) & mask) {
            for (size_t boxIndex = 0; boxIndex < Boxes.size(); boxIndex++) {
                const TBox& box = Boxes[boxIndex];
                if (totalWeight[subMask] <= box.MaxWeight && totalVolume[subMask] <= box.MaxVolume && minCost[mask ^ subMask] != INF_COST) {
                    uint64_t newCost =  minCost[mask ^ subMask] + box.Cost;
                    if (minCost[mask] > newCost) {
                        minCost[mask] = newCost;
                        lastBox[mask] = {subMask, boxIndex};
                    }
                }
            }
        }
    }

    std::vector<TFilledBox> vectorOfBoxes;
    uint64_t currentMask = DP_SIZE - 1;

    if (minCost[currentMask] == INF_COST) {
        return vectorOfBoxes;
    }

    while (currentMask != 0) {
        const TBox currentBox = Boxes[lastBox[currentMask].second];
        const uint64_t boxMask = lastBox[currentMask].first;
        std::vector<TItem> currentBoxItems;
        for (size_t bit = 0; bit < items.size(); bit++) {
            if ((boxMask >> bit) & 1) {
                currentBoxItems.emplace_back(items[bit]);
            }
        }
        vectorOfBoxes.emplace_back(currentBox, currentBoxItems);
        currentMask ^= boxMask;
    }
    return vectorOfBoxes;
}

bool TShop::OrderIsEmpty() const {
    for (const auto& [itemId, amount] : OrderAmounts) {
        if (amount > 0) {
            return false;
        }
    }
    return true;
}
