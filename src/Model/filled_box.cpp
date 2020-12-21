#pragma once

#include <vector>
#include <ostream>

#include "item.cpp"
#include "box.cpp"

struct TFilledBox {
    uint64_t BoxID;
    std::vector<uint64_t> ItemIDs;
    TFilledBox(const uint64_t& boxID, const std::vector<uint64_t>& itemIDs = {}) : BoxID(boxID), ItemIDs(itemIDs) {}

    TFilledBox(const TBox& box, const std::vector<TItem>& items = {}) {
        BoxID = box.BoxID;
        ItemIDs.reserve(items.size());
        for (const TItem& item : items) {
            ItemIDs.push_back(item.ItemID);
        }
    }

    TFilledBox() = default;

    bool operator == (const TFilledBox& other) const {
        return BoxID == other.BoxID && ItemIDs == other.ItemIDs;
    }
};

std::ostream& operator << (std::ostream& out, TFilledBox filledBox) {
    out << "Box: " << filledBox.BoxID << " and Items:";
    for (const uint64_t& itemID : filledBox.ItemIDs) {
        out << " " << itemID;
    }
    return out;
}
