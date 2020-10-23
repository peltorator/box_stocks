#include "shop.h"

TShop::TShop(const std::list<std::pair<TItem, uint32_t>> items) {
    for (const auto& [item, amount] : items) {
        Items[item.GetItemID()] = item;
        AvailableAmounts[item.GetItemID()] = amount;
    }
}

void TShop::AddItem(const uint64_t itemID) {
    OrderAmounts[itemID]++;
}

void TShop::DeleteItem(const uint64_t itemID) {
    OrderAmounts[itemID]--;
}

std::list<TFilledBox> TShop::Buy() {
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

    for (uint64_t mask = 0; mask < DP_SIZE; mask++) {
        for (size_t bit = 0; bit < items.size(); bit++) {
            if ((mask >> bit) & 1) {
                totalWeight[mask] += items[bit].GetWeight();
                totalVolume[mask] += items[bit].GetVolume();
            }
        }
        for (uint64_t subMask = mask; ; subMask = (subMask - 1) & mask) {
            for (size_t boxIndex = 0; boxIndex < Boxes.size(); boxIndex++) {
                const TBox& box = Boxes[boxIndex];
                if (totalWeight[subMask] <= box.GetMaxWeight() && totalVolume[subMask] <= box.GetMaxVolume()) {
                    uint64_t newCost =  minCost[mask ^ subMask] + box.GetCost();
                    if (minCost[mask] > newCost) {
                        minCost[mask] = newCost;
                        lastBox[mask] = {subMask, boxIndex};
                    }
                }
            }
            if (subMask == 0) {
                break;
            }
        }
    }

    std::list<TFilledBox> listOfBoxes;
    uint64_t currentMask = DP_SIZE - 1;

    if (minCost[currentMask] == INF_COST) {
        return listOfBoxes;
    }

    while (currentMask != 0) {
        const TBox currentBox = Boxes[lastBox[currentMask].second];
        const uint64_t BoxMask = lastBox[currentMask].first;
        std::list<TItem> currentBoxItems;
        for (size_t bit = 0; bit < items.size(); bit++) {
            if ((currentMask >> bit) & 1) {
                currentBoxItems.emplace_back(items[bit]);
            }
        }
        listOfBoxes.emplace_back(currentBox, currentBoxItems);
    }
    return listOfBoxes;
}
