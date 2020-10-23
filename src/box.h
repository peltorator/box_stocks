#pragma once
#include <cstdint>

class TBox {
private:
    uint64_t BoxID;
    uint64_t MaxWeight;
    uint64_t MaxVolume;
    uint64_t Cost;
public:
    TBox(const uint64_t boxID, const uint64_t maxWeight, const uint64_t maxVolume, const uint64_t cost)
        : BoxID(boxID), MaxWeight(maxWeight), MaxVolume(maxVolume), Cost(cost) {}

    uint64_t GetBoxID() const {
        return BoxID;
    }

    uint64_t GetMaxWeight() const {
        return MaxWeight;
    }

    uint64_t GetMaxVolume() const {
        return MaxVolume;
    }

    uint64_t GetCost() const {
        return Cost;
    }
};
