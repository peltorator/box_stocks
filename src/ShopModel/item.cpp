#pragma once
#include <cstdint>
#include <string>

struct TItem {
    uint64_t ItemID;
    std::string ItemName;
    uint64_t Weight;
    uint64_t Volume;
    std::string Image;

    TItem(const uint64_t& itemID, const std::string& itemName, const uint64_t weight, const uint64_t volume, const std::string& image = "")
        : ItemID(itemID), ItemName(itemName), Weight(weight), Volume(volume), Image(image) {}

    TItem() = default;

    bool operator == (const TItem& other) const {
        return ItemID == other.ItemID;
    }
};

std::ostream& operator << (std::ostream& out, TItem item) {
    out << item.ItemName;
    return out;
}
