#pragma once

#include "box.cpp"
#include "item.cpp"
#include <vector>
#include <ostream>

struct TFilledBox {
    TBox Box;
    std::vector<TItem> Items;
    TFilledBox(const TBox& box, const std::vector<TItem>& items = {}) : Box(box), Items(items) {}

    TFilledBox() = default;

    bool operator == (const TFilledBox& other) const {
        return Box == other.Box && Items == other.Items;
    }
};

std::ostream& operator << (std::ostream& out, TFilledBox filledBox) {
    out << "Box: " << filledBox.Box << " and Items:";
    for (const TItem& item : filledBox.Items) {
        out << " " << item;
    }
    return out;
}
