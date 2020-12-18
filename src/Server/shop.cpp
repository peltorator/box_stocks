#include "shop.h"

TShop::TShop() {
    RawItems = GetItems();
    Boxes = GetAvailableBoxes();
    for (const auto& [item, amount] : RawItems) {
        Items[item.ItemID] = item;
        AvailableAmounts[item.ItemID] = amount;
    }
    rnd = std::mt19937(0);
}

std::vector<TBox> TShop::GetBoxes() const {
    return Boxes;
}

std::vector<std::pair<TItem, uint32_t>> TShop::GetRawItems() const {
    return RawItems;
}

void TShop::AddItem(const uint64_t itemID, const uint32_t amount) {
    OrderAmounts[itemID] += amount;
}

void TShop::DeleteItem(const uint64_t itemID, const uint32_t amount) {
    OrderAmounts[itemID] -= amount;
}

std::pair<uint64_t, std::vector<TFilledBox>> TShop::PackSmall(const std::vector<TItem>& items) {
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
    uint64_t cost = minCost[currentMask];

    if (cost == INF_COST) {
        return {cost, vectorOfBoxes};
    }

    while (currentMask != 0) {
        const uint64_t currentBoxID = Boxes[lastBox[currentMask].second].BoxID;
        const uint64_t boxMask = lastBox[currentMask].first;
        std::vector<uint64_t> currentBoxItemIDs;
        for (size_t bit = 0; bit < items.size(); bit++) {
            if ((boxMask >> bit) & 1) {
                currentBoxItemIDs.emplace_back(items[bit].ItemID);
            }
        }
        vectorOfBoxes.emplace_back(currentBoxID, currentBoxItemIDs);
        currentMask ^= boxMask;
    }
    return {cost, vectorOfBoxes};
}


std::vector<TFilledBox> TShop::Buy() {
    std::vector<TItem> items;
    for (const auto& [itemID, amount] : OrderAmounts) {
        for (uint32_t ind = 0; ind < amount; ind++) {
            items.push_back(TItem(Items[itemID].ItemID, Items[itemID].ItemName, Items[itemID].Weight, Items[itemID].Volume, Items[itemID].Cost));
        }
    }
    std::vector<TFilledBox> bestFilledBoxes;
    uint64_t bestCost = INF_COST;
    for (size_t partitionIndex = 0; partitionIndex < PARTITIONS; partitionIndex++) {
        shuffle(items.begin(), items.end(), rnd);
        uint64_t curCost = 0;
        std::vector<TFilledBox> curFilledBoxes;
        for (size_t i = 0; i < items.size(); i += MAX_ITEMS_IN_BLOCK) {
            std::vector<TItem> curItems;
            for (size_t j = i; j < i + MAX_ITEMS_IN_BLOCK && j < items.size(); j++) {
                curItems.push_back(items[j]);
            }
            const auto& [cost, boxes] = PackSmall(curItems);
            if (cost == INF_COST) {
                return std::vector<TFilledBox>();
            }
            curCost += cost;
            for (const TFilledBox& box : boxes) {
                curFilledBoxes.push_back(box);
            }
        }
        if (bestCost > curCost) {
            bestCost = curCost;
            bestFilledBoxes = curFilledBoxes;
        }
    }
    return bestFilledBoxes;
}

bool TShop::OrderIsEmpty() const {
    for (const auto& [itemId, amount] : OrderAmounts) {
        if (amount > 0) {
            return false;
        }
    }
    return true;
}
