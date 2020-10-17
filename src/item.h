#pragma once

class TItem {
private:
    uint64 ItemID;
    uint64 Weight;
    uint64 Volume;
public:
    TItem(const uint64 itemID, const uint64 weight, const uint64 volume)
        : ItemID(itemID), Weight(weight), Volume(volume) {}

    uint32 GetItemID() const {
        return ItemID;
    }

    uint64 GetWeight() const {
        return Weight;
    }

    uint64 GetVolume() const {
        return Volume;
    }
};
