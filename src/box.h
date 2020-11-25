#pragma once
#include <cstdint>
#include <string>
#include <ostream>

class TBox {
private:
    std::string BoxName;
    uint64_t BoxID;
    uint64_t MaxWeight;
    uint64_t MaxVolume;
    uint64_t Cost;
    static uint64_t LastID;
    std::string ImagePath;
public:
    TBox(const std::string& boxName, const uint64_t maxWeight, const uint64_t maxVolume, const uint64_t cost, const std::string& imagePath = "")
        : BoxName(boxName), BoxID(LastID++), MaxWeight(maxWeight), MaxVolume(maxVolume), Cost(cost), ImagePath(imagePath) {}

    const std::string& GetBoxName() const {
        return BoxName;
    }

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

    const std::string& GetImagePath() const {
        return ImagePath;
    }

    bool operator == (const TBox& other) const {
        return BoxID == other.GetBoxID();
    }

    friend std::ostream& operator << (std::ostream& out, TBox box);
};

uint64_t TBox::LastID = 0;

std::ostream& operator << (std::ostream& out, TBox box) {
    out << box.BoxName;
    return out;
}
