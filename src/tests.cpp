#define CATCH_CONFIG_MAIN
#include "../catch2/catch.hpp"
#include "shop.h"
#include "shop.cpp"


TEST_CASE("Empty shop", "[single-file]") {
    TShop shop({}, {});

    REQUIRE(shop.Buy().size() == 0);
}

TEST_CASE("3 items (1, 1). Boxes (1, 1, 2) and (2, 2, 3). Second box is cheeper than two first boxes.", "[single-file]") {
    TShop shop({{TItem(1, 1, 1), 3}}, {TBox(1, 1, 1, 2), TBox(2, 2, 2, 3)});
    shop.AddItem(1);
    shop.AddItem(1);
    shop.AddItem(1);
    std::vector<TFilledBox> myFilledBoxes = {TFilledBox(TBox(2, 2, 2, 3), {TItem(1, 1, 1), TItem(1, 1, 1)}), TFilledBox(TBox(1, 1, 1, 2), {TItem(1, 1, 1)})};

    REQUIRE(shop.Buy() == myFilledBoxes);
}

TEST_CASE("3 items (1, 1). Boxes (1, 1, 2) and (2, 2, 5). Second box is more expensive than two first boxes.", "[single-file]") {
    TShop shop({{TItem(1, 1, 1), 3}}, {TBox(1, 1, 1, 2), TBox(2, 2, 2, 5)});
    shop.AddItem(1);
    shop.AddItem(1);
    shop.AddItem(1);
    std::vector<TFilledBox> myFilledBoxes = {TFilledBox(TBox(1, 1, 1, 2), {TItem(1, 1, 1)}), TFilledBox(TBox(1, 1, 1, 2), {TItem(1, 1, 1)}), TFilledBox(TBox(1, 1, 1, 2), {TItem(1, 1, 1)})};

    REQUIRE(shop.Buy() == myFilledBoxes);
}
