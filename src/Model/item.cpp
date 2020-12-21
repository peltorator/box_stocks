#pragma once
#include <cstdint>
#include <string>

struct TItem {
    uint64_t ItemID;
    std::string ItemName;
    uint64_t Weight;
    uint64_t Volume;
    uint64_t Cost;
    std::string Image;

    TItem(const uint64_t& itemID, const std::string& itemName, const uint64_t weight, const uint64_t volume, const uint64_t cost, const std::string& image = "")
        : ItemID(itemID), ItemName(itemName), Weight(weight), Volume(volume), Cost(cost), Image(image) {}

    TItem() = default;

    bool operator == (const TItem& other) const {
        return ItemID == other.ItemID;
    }
};

std::ostream& operator << (std::ostream& out, TItem item) {
    out << item.ItemName;
    return out;
}
