#pragma once

class TBox {
private:
    uint64 BoxID;
    uint64 MaxWeight;
    uint64 MaxVolume;
    uint64 Cost;
public:
    Box(const uint64 boxID, const uint64 maxWeight, const uint64 maxVolume, const uint64 cost)
        : BoxID(boxID), MaxWeight(maxWeight), MaxVolume(maxVolume), Cost(cost) {}

    uint64 GetBoxID() const {
        return BoxID;
    }

    uint64 GetMaxWeight() const {
        return MaxWeight;
    }

    uint64 GetMaxVolume() const {
        return MaxVolume;
    }

    uint64 GetCost() const {
        return Cost;
    }
};
