#pragma once
#include <cstdint>
#include <string>
#include <ostream>

class TBox {
private:
    uint64_t BoxID;
    std::string BoxName;
    uint64_t MaxWeight;
    uint64_t MaxVolume;
    uint64_t Cost;
    std::string Image;
public:
    TBox(const uint64_t& boxID, const std::string& boxName, const uint64_t maxWeight, const uint64_t maxVolume, const uint64_t cost, const std::string& image = "")
        : BoxID(boxID), BoxName(boxName), MaxWeight(maxWeight), MaxVolume(maxVolume), Cost(cost), Image(image) {}

    TBox() = default;

    uint64_t GetBoxID() const {
        return BoxID;
    }

    const std::string& GetBoxName() const {
        return BoxName;
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

    const std::string& GetImage() const {
        return Image;
    }

    bool operator == (const TBox& other) const {
        return BoxID == other.GetBoxID();
    }

    friend std::ostream& operator << (std::ostream& out, TBox box);
};

std::ostream& operator << (std::ostream& out, TBox box) {
    out << box.BoxName;
    return out;
}
