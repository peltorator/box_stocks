#pragma once

#include "box.h"
#include "item.h"
#include <vector>

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
};
