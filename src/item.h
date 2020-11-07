#pragma once
#include <cstdint>
#include <string>

class TItem {
private:
    std::string ItemName;
    uint64_t ItemID;
    uint64_t Weight;
    uint64_t Volume;
    static uint64_t LastID;
public:
    TItem(const std::string& itemName, const uint64_t weight, const uint64_t volume)
        : ItemName(itemName), ItemID(LastID++), Weight(weight), Volume(volume) {}

    TItem() : ItemName(""), ItemID(0), Weight(0), Volume(0) {}

    const std::string& GetItemName() const {
        return ItemName;
    }

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
        return ItemID == other.GetItemID();
    }

    friend std::ostream& operator << (std::ostream& out, TItem item);
};

uint64_t TItem::LastID = 0;

std::ostream& operator << (std::ostream& out, TItem item) {
    out << item.ItemName;
    return out;
}
