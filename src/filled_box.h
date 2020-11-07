#pragma once

#include "box.h"
#include "item.h"
#include <vector>
#include <ostream>

class TFilledBox {
private:
    const TBox Box;
    const std::vector<TItem> Items;
public:
    TFilledBox(const TBox& box, const std::vector<TItem>& items) : Box(box), Items(items) {}

    const TBox& GetBox() const {
        return Box;
    }

    const std::vector<TItem>& GetItems() const {
        return Items;
    }

    bool operator == (const TFilledBox& other) const {
        return Box == other.GetBox() && Items == other.GetItems();
    }

    friend std::ostream& operator << (std::ostream& out, TFilledBox filledBox);
};

std::ostream& operator << (std::ostream& out, TFilledBox filledBox) {
    out << "Box: " << filledBox.Box << " and Items:";
    for (const TItem& item : filledBox.Items) {
        out << " " << item;
    }
    return out;
}
