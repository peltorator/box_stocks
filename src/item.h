#pragma once
#include <cstdint>

class TItem {
private:
    uint64_t ItemID;
    uint64_t Weight;
    uint64_t Volume;
public:
    TItem(const uint64_t itemID, const uint64_t weight, const uint64_t volume)
        : ItemID(itemID), Weight(weight), Volume(volume) {}

    TItem() : ItemID(0), Weight(0), Volume(0) {}

    uint64_t GetItemID() const {
        return ItemID;
    }

    uint64_t GetWeight() const {
        return Weight;
    }

    uint64_t GetVolume() const {
        return Volume;
    }

    bool operator == (const TItem& other) const {
        return ItemID == other.GetItemID() && Weight == other.GetWeight() && Volume == other.GetVolume();
    }
};
