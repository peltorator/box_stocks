#pragma once
#include <cstdint>
#include <string>

class TItem {
private:
    uint64_t ItemID;
    std::string ItemName;
    uint64_t Weight;
    uint64_t Volume;
    std::string Image;
public:
    TItem(const uint64_t& itemID, const std::string& itemName, const uint64_t weight, const uint64_t volume, const std::string& image = "")
        : ItemID(itemID), ItemName(itemName), Weight(weight), Volume(volume), Image(image) {}

    TItem() = default;

    uint64_t GetItemID() const {
        return ItemID;
    }

    const std::string& GetItemName() const {
        return ItemName;
    }

    uint64_t GetWeight() const {
        return Weight;
    }

    uint64_t GetVolume() const {
        return Volume;
    }

    const std::string& GetImage() const {
        return Image;
    }

    bool operator == (const TItem& other) const {
        return ItemID == other.GetItemID();
    }

    friend std::ostream& operator << (std::ostream& out, TItem item);
};

std::ostream& operator << (std::ostream& out, TItem item) {
    out << item.ItemName;
    return out;
}
