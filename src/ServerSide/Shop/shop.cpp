#include "shop.h"

TShop::TShop() {
    std::vector<std::pair<TItem, uint32_t>> itemAmounts = GetItems();
    _boxes = GetAvailableBoxes();
    for (const auto& [item, amount] : itemAmounts) {
        _items[item.ItemID] = item;
        _availableAmounts[item.ItemID] = amount;
    }
    _rnd = std::mt19937(0);
}

void TShop::AddItem(const uint64_t itemID, const uint32_t amount) {
    _orderAmounts[itemID] += amount;
    _availableAmounts[itemID] -= amount;
}

void TShop::DeleteItem(const uint64_t itemID, const uint32_t amount) {
    _orderAmounts[itemID] -= amount;
    _availableAmounts[itemID] += amount;
}

std::pair<uint64_t, std::vector<TFilledBox>> TShop::PackSmall(const std::vector<TItem>& items) {
    const uint64_t DP_SIZE = (1LL << items.size());
    std::vector<uint64_t> totalWeight(DP_SIZE, 0);
    std::vector<uint64_t> totalVolume(DP_SIZE, 0);
    std::vector<uint64_t> minCost(DP_SIZE, _INF_COST);
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
            for (size_t boxIndex = 0; boxIndex < _boxes.size(); boxIndex++) {
                const TBox& box = _boxes[boxIndex];
                if (totalWeight[subMask] <= box.MaxWeight && totalVolume[subMask] <= box.MaxVolume && minCost[mask ^ subMask] != _INF_COST) {
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

    if (cost == _INF_COST) {
        return {cost, vectorOfBoxes};
    }

    while (currentMask != 0) {
        const uint64_t currentBoxID = _boxes[lastBox[currentMask].second].BoxID;
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
    for (const auto& [itemID, amount] : _orderAmounts) {
        for (uint32_t ind = 0; ind < amount; ind++) {
            items.push_back(TItem(_items[itemID].ItemID, _items[itemID].ItemName, _items[itemID].Weight, _items[itemID].Volume, _items[itemID].Cost));
        }
    }
    _orderAmounts.clear();
    std::vector<TFilledBox> bestFilledBoxes;
    uint64_t bestCost = _INF_COST;
    for (size_t partitionIndex = 0; partitionIndex < _PARTITIONS; partitionIndex++) {
        shuffle(items.begin(), items.end(), _rnd);
        uint64_t curCost = 0;
        std::vector<TFilledBox> curFilledBoxes;
        for (size_t i = 0; i < items.size(); i += _MAX_ITEMS_IN_BLOCK) {
            std::vector<TItem> curItems;
            for (size_t j = i; j < i + _MAX_ITEMS_IN_BLOCK && j < items.size(); j++) {
                curItems.push_back(items[j]);
            }
            const auto& [cost, boxes] = PackSmall(curItems);
            if (cost == _INF_COST) {
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
    for (const auto& [itemId, amount] : _orderAmounts) {
        if (amount > 0) {
            return false;
        }
    }
    return true;
}
