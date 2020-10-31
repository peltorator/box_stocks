#pragma once

#include "box.h"
#include "item.h"
#include <vector>

class TFilledBox {
private:
    typedef std::vector<TItem> TItemContainer;

    TBox Box;
    TItemContainer Items;
public:
    TFilledBox(const TBox& box, const TItemContainer& items) : Box(box), Items(items) {}

    TBox GetBox() const {
        return Box;
    }

    const TItemContainer& GetItems() const {
        return Items;
    }

    bool operator == (const TFilledBox& other) const {
        return Box == other.GetBox() && Items == other.GetItems();
    }
};
