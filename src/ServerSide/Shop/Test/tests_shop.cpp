#define CATCH_CONFIG_MAIN
#include <algorithm>

#include "../../../../libs/catch2/catch.hpp"
#include "../shop_session.cpp"

void SortOrder(std::vector<TFilledBox>& order) {
    for (TFilledBox& filledBox : order) {
        std::sort(filledBox.ItemIDs.begin(), filledBox.ItemIDs.end());
    }
    std::sort(order.begin(), order.end(), [](const TFilledBox& firstBox, const TFilledBox& secondBox) {
        return make_pair(firstBox.BoxID, firstBox.ItemIDs) < make_pair(secondBox.BoxID, secondBox.ItemIDs);
    });
}

TEST_CASE("Empty shop", "[single-file]") {
    TShopSession shop({}, {});

    std::vector<TFilledBox> myFilledBoxes;
    std::vector<TFilledBox> algoFilledBoxes = shop.Buy();
    SortOrder(myFilledBoxes);
    SortOrder(algoFilledBoxes);

    REQUIRE(myFilledBoxes == algoFilledBoxes);
}

TEST_CASE("3 items (1, 1). Boxes (1, 1, 2) and (2, 2, 3). The second box is cheeper than two first boxes.", "[single-file]") {
    const TItem ball = TItem(1, "ball", 1, 1, 1);

    const TBox smallBox = TBox(1, "small", 1, 1, 2);
    const TBox bigBox = TBox(2, "big", 2, 2, 3);

    TShopSession shop({{ball, 3}}, {smallBox, bigBox});

    shop.AddItem(ball.ItemID, 3);

    std::vector<TFilledBox> myFilledBoxes = {
        TFilledBox(bigBox, {ball, ball}),
        TFilledBox(smallBox, {ball})
    };
    std::vector<TFilledBox> algoFilledBoxes = shop.Buy();
    SortOrder(myFilledBoxes);
    SortOrder(algoFilledBoxes);

    REQUIRE(myFilledBoxes == algoFilledBoxes);
}

TEST_CASE("3 items (1, 1). Boxes (1, 1, 2) and (2, 2, 5). The second box is more expensive than two first boxes.", "[single-file]") {
    const TItem ball = TItem(1, "ball", 1, 1, 1);

    const TBox smallBox = TBox(1, "small", 1, 1, 2);
    const TBox bigBox = TBox(2, "big", 2, 2, 5);

    TShopSession shop({{ball, 3}}, {smallBox, bigBox});
    
    shop.AddItem(ball.ItemID, 3);

    std::vector<TFilledBox> myFilledBoxes = {
        TFilledBox(smallBox, {ball}),
        TFilledBox(smallBox, {ball}),
        TFilledBox(smallBox, {ball})
    };
    std::vector<TFilledBox> algoFilledBoxes = shop.Buy();
    SortOrder(myFilledBoxes);
    SortOrder(algoFilledBoxes);

    REQUIRE(myFilledBoxes == algoFilledBoxes);
}

TEST_CASE("Same boxes with different cost.", "[single-file]") {
    const TItem ball = TItem(1, "ball", 1, 1, 1);
    
    const TBox cheepBox = TBox(1, "cheep", 2, 2, 2);
    const TBox expensiveBox = TBox(2, "expensive", 2, 2, 3);

    TShopSession shop({{ball, 5}}, {cheepBox, expensiveBox});

    shop.AddItem(ball.ItemID, 5);

    std::vector<TFilledBox> myFilledBoxes = {
        TFilledBox(cheepBox, {ball, ball}),
        TFilledBox(cheepBox, {ball, ball}),
        TFilledBox(cheepBox, {ball}),
    };
    std::vector<TFilledBox> algoFilledBoxes = shop.Buy();
    SortOrder(myFilledBoxes);
    SortOrder(algoFilledBoxes);

    REQUIRE(myFilledBoxes == algoFilledBoxes);
}

TEST_CASE("One of the items doesn't fit in any box", "[single-file]") {
    const TItem ball = TItem(1, "ball", 1, 1, 1);
    const TItem car = TItem(2, "car", 100, 100, 1);

    const TBox smallBox = TBox(1, "small", 1, 1, 2);
    const TBox bigBox = TBox(2, "big", 5, 5, 5);

    TShopSession shop({{ball, 1}, {car, 1}}, {smallBox, bigBox});

    shop.AddItem(ball.ItemID, 1);
    shop.AddItem(car.ItemID, 1);

    std::vector<TFilledBox> myFilledBoxes;
    std::vector<TFilledBox> algoFilledBoxes = shop.Buy();
    SortOrder(myFilledBoxes);
    SortOrder(algoFilledBoxes);

    REQUIRE(myFilledBoxes == algoFilledBoxes);
}

TEST_CASE("No boxes but some items", "[single-file]") {
    const TItem ball = TItem(1, "ball", 1, 1, 1);

    TShopSession shop({{ball, 1}}, {});

    shop.AddItem(ball.ItemID, 1);

    std::vector<TFilledBox> myFilledBoxes;
    std::vector<TFilledBox> algoFilledBoxes = shop.Buy();
    SortOrder(myFilledBoxes);
    SortOrder(algoFilledBoxes);

    REQUIRE(myFilledBoxes == algoFilledBoxes);
}

TEST_CASE("Add + Delete", "[single-file]") {
    const TItem ball = TItem(1, "ball", 1, 1, 1);

    const TBox smallBox = TBox(1, "small", 1, 1, 2);

    TShopSession shop({{ball, 1}}, {smallBox});

    shop.AddItem(ball.ItemID);
    shop.DeleteItem(ball.ItemID);

    REQUIRE(shop.OrderIsEmpty());
}

TEST_CASE("Restaurant test 1", "[single-file]") {
    const TItem sushi = TItem(1, "sushi", 100, 10, 1);
    const TItem pizza = TItem(2, "pizza", 300, 35, 1);
    const TItem stake = TItem(3, "stake", 300, 20, 1);

    const TBox smallBox = TBox(1, "small", 200, 8, 10);
    const TBox mediumBox = TBox(2, "medium", 200, 20, 20);
    const TBox bigBox = TBox(3, "big", 300, 30, 30);
    const TBox largeBox = TBox(4, "large", 350, 40, 40);
    const TBox extraLargeBox = TBox(5, "extra large", 400, 30, 45);
    const TBox enormousBox = TBox(6, "enormous", 600, 100, 100);

    TShopSession shop({{sushi, 4}, {pizza, 3}, {stake, 2}}, {smallBox, mediumBox, bigBox, largeBox, extraLargeBox, enormousBox});

    shop.AddItem(sushi.ItemID, 4);
    shop.AddItem(pizza.ItemID, 3);
    shop.AddItem(stake.ItemID, 2);

    std::vector<TFilledBox> myFilledBoxes = {
        TFilledBox(mediumBox, {sushi, sushi}),
        TFilledBox(mediumBox, {sushi, sushi}),
        TFilledBox(largeBox, {pizza}),
        TFilledBox(largeBox, {pizza}),
        TFilledBox(largeBox, {pizza}),
        TFilledBox(bigBox, {stake}),
        TFilledBox(bigBox, {stake}),
    };
    std::vector<TFilledBox> algoFilledBoxes = shop.Buy();
    SortOrder(myFilledBoxes);
    SortOrder(algoFilledBoxes);

    REQUIRE(myFilledBoxes == algoFilledBoxes);
}

TEST_CASE("Restaurant test 2", "[single-file]") {
    const TItem sushi = TItem(1, "sushi", 100, 10, 1);
    const TItem pizza = TItem(2, "pizza", 300, 35, 1);
    const TItem stake = TItem(3, "stake", 300, 20, 1);

    const TBox smallBox = TBox(1, "small", 200, 8, 10);
    const TBox mediumBox = TBox(2, "medium", 200, 20, 20);
    const TBox bigBox = TBox(3, "big", 300, 30, 30);
    const TBox largeBox = TBox(4, "large", 350, 40, 40);
    const TBox extraLargeBox = TBox(5, "extra large", 400, 30, 39);
    const TBox enormousBox = TBox(6, "enormous", 600, 100, 100);

    TShopSession shop({{sushi, 4}, {pizza, 3}, {stake, 2}}, {smallBox, mediumBox, bigBox, largeBox, extraLargeBox, enormousBox});

    shop.AddItem(sushi.ItemID, 4);
    shop.AddItem(pizza.ItemID, 3);
    shop.AddItem(stake.ItemID, 2);

    std::vector<TFilledBox> myFilledBoxes = {
        TFilledBox(mediumBox, {sushi, sushi}),
        TFilledBox(extraLargeBox, {stake, sushi}),
        TFilledBox(extraLargeBox, {stake, sushi}),
        TFilledBox(largeBox, {pizza}),
        TFilledBox(largeBox, {pizza}),
        TFilledBox(largeBox, {pizza}),
    };
    std::vector<TFilledBox> algoFilledBoxes = shop.Buy();
    SortOrder(myFilledBoxes);
    SortOrder(algoFilledBoxes);

    REQUIRE(myFilledBoxes == algoFilledBoxes);
}

TEST_CASE("Restaurant test 3", "[single-file]") {
    const TItem sushi = TItem(1, "sushi", 100, 10, 1);
    const TItem pizza = TItem(2, "pizza", 300, 35, 1);
    const TItem stake = TItem(3, "stake", 300, 20, 1);

    const TBox smallBox = TBox(1, "small", 200, 8, 10);
    const TBox mediumBox = TBox(2, "medium", 200, 20, 20);
    const TBox bigBox = TBox(3, "big", 300, 30, 30);
    const TBox largeBox = TBox(4, "large", 350, 40, 51);
    const TBox extraLargeBox = TBox(5, "extra large", 400, 30, 39);
    const TBox enormousBox = TBox(6, "enormous", 600, 100, 100);

    TShopSession shop({{sushi, 4}, {pizza, 3}, {stake, 2}}, {smallBox, mediumBox, bigBox, largeBox, extraLargeBox, enormousBox});

    shop.AddItem(sushi.ItemID, 4);
    shop.AddItem(pizza.ItemID, 3);
    shop.AddItem(stake.ItemID, 2);

    std::vector<TFilledBox> myFilledBoxes = {
        TFilledBox(mediumBox, {sushi, sushi}),
        TFilledBox(extraLargeBox, {stake, sushi}),
        TFilledBox(extraLargeBox, {stake, sushi}),
        TFilledBox(enormousBox, {pizza, pizza}),
        TFilledBox(largeBox, {pizza}),
    };
    std::vector<TFilledBox> algoFilledBoxes = shop.Buy();
    SortOrder(myFilledBoxes);
    SortOrder(algoFilledBoxes);

    REQUIRE(myFilledBoxes == algoFilledBoxes);
}

TEST_CASE("Restaurant test 4", "[single-file]") {
    const TItem sushi = TItem(1, "sushi", 100, 10, 1);

    const TBox smallBox = TBox(1, "small", 100, 10, 10);
    const TBox bigBox = TBox(2, "big", 400, 40, 35);

    TShopSession shop({{sushi, 100}}, {smallBox, bigBox});

    shop.AddItem(sushi.ItemID, 3);

    std::vector<TFilledBox> myFilledBoxes = {
        TFilledBox(smallBox, {sushi}),
        TFilledBox(smallBox, {sushi}),
        TFilledBox(smallBox, {sushi}),
    };
    std::vector<TFilledBox> algoFilledBoxes = shop.Buy();
    SortOrder(myFilledBoxes);
    SortOrder(algoFilledBoxes);

    REQUIRE(myFilledBoxes == algoFilledBoxes);
}

TEST_CASE("Restaurant test 5", "[single-file]") {
    const TItem sushi = TItem(1, "sushi", 100, 10, 1);

    const TBox smallBox = TBox(1, "small", 100, 10, 10);
    const TBox bigBox = TBox(2, "big", 400, 40, 35);

    TShopSession shop({{sushi, 100}}, {smallBox, bigBox});

    shop.AddItem(sushi.ItemID, 4);

    std::vector<TFilledBox> myFilledBoxes = {
        TFilledBox(bigBox, {sushi, sushi, sushi, sushi}),
    };
    std::vector<TFilledBox> algoFilledBoxes = shop.Buy();
    SortOrder(myFilledBoxes);
    SortOrder(algoFilledBoxes);

    REQUIRE(myFilledBoxes == algoFilledBoxes);
}

TEST_CASE("Restaurant test 6", "[single-file]") {
    const TItem sushi = TItem(1, "sushi", 100, 10, 1);

    const TBox smallBox = TBox(1, "small", 100, 10, 10);
    const TBox bigBox = TBox(2, "big", 400, 40, 35);

    TShopSession shop({{sushi, 100}}, {smallBox, bigBox});

    shop.AddItem(sushi.ItemID, 7);

    std::vector<TFilledBox> myFilledBoxes = {
        TFilledBox(bigBox, {sushi, sushi, sushi, sushi}),
        TFilledBox(smallBox, {sushi}),
        TFilledBox(smallBox, {sushi}),
        TFilledBox(smallBox, {sushi}),
    };
    std::vector<TFilledBox> algoFilledBoxes = shop.Buy();
    SortOrder(myFilledBoxes);
    SortOrder(algoFilledBoxes);

    REQUIRE(myFilledBoxes == algoFilledBoxes);
}

TEST_CASE("Restaurant test 7", "[single-file]") {
    const TItem sushi = TItem(1, "sushi", 100, 10, 1);

    const TBox smallBox = TBox(1, "small", 100, 10, 10);
    const TBox bigBox = TBox(2, "big", 400, 40, 35);

    TShopSession shop({{sushi, 100}}, {smallBox, bigBox});

    shop.AddItem(sushi.ItemID, 8);

    std::vector<TFilledBox> myFilledBoxes = {
        TFilledBox(bigBox, {sushi, sushi, sushi, sushi}),
        TFilledBox(bigBox, {sushi, sushi, sushi, sushi}),
    };
    std::vector<TFilledBox> algoFilledBoxes = shop.Buy();
    SortOrder(myFilledBoxes);
    SortOrder(algoFilledBoxes);

    REQUIRE(myFilledBoxes == algoFilledBoxes);
}

TEST_CASE("Restaurant test 8", "[single-file]") {
    const TItem sushi = TItem(1, "sushi", 100, 10, 1);

    const TBox smallBox = TBox(1, "small", 100, 10, 10);
    const TBox bigBox = TBox(2, "big", 400, 40, 35);

    TShopSession shop({{sushi, 100}}, {smallBox, bigBox});

    shop.AddItem(sushi.ItemID, 9);

    std::vector<TFilledBox> myFilledBoxes = {
        TFilledBox(bigBox, {sushi, sushi, sushi, sushi}),
        TFilledBox(bigBox, {sushi, sushi, sushi, sushi}),
        TFilledBox(smallBox, {sushi}),
    };
    std::vector<TFilledBox> algoFilledBoxes = shop.Buy();
    SortOrder(myFilledBoxes);
    SortOrder(algoFilledBoxes);

    REQUIRE(myFilledBoxes == algoFilledBoxes);
}

TEST_CASE("Big test 1", "[single-file]") {
    const TItem sushi = TItem(1, "sushi", 100, 10, 1);

    const TBox smallBox = TBox(1, "small", 100, 10, 10);
    const TBox bigBox = TBox(2, "big", 400, 40, 35);

    const int SUSHI_CNT = 100;

    TShopSession shop({{sushi, SUSHI_CNT}}, {smallBox, bigBox});

    shop.AddItem(sushi.ItemID, SUSHI_CNT);

    std::vector<TFilledBox> myFilledBoxes;
    for (int i = 0; i < SUSHI_CNT / 4; i++) {
        myFilledBoxes.push_back(TFilledBox(bigBox, {sushi, sushi, sushi, sushi}));
    }
    for (int i = 0; i < SUSHI_CNT % 4; i++) {
        myFilledBoxes.push_back(TFilledBox(smallBox, {sushi}));
    }

    std::vector<TFilledBox> algoFilledBoxes = shop.Buy();
    SortOrder(myFilledBoxes);
    SortOrder(algoFilledBoxes);

    REQUIRE(myFilledBoxes == algoFilledBoxes);
}

TEST_CASE("Big test 2", "[single-file]") {
    const TItem sushi = TItem(1, "sushi", 100, 10, 1);

    const TBox smallBox = TBox(1, "small", 100, 10, 10);
    const TBox bigBox = TBox(2, "big", 400, 40, 35);

    const int SUSHI_CNT = 1000;

    TShopSession shop({{sushi, SUSHI_CNT}}, {smallBox, bigBox});

    shop.AddItem(sushi.ItemID, SUSHI_CNT);

    std::vector<TFilledBox> myFilledBoxes;
    for (int i = 0; i < SUSHI_CNT / 4; i++) {
        myFilledBoxes.push_back(TFilledBox(bigBox, {sushi, sushi, sushi, sushi}));
    }
    for (int i = 0; i < SUSHI_CNT % 4; i++) {
        myFilledBoxes.push_back(TFilledBox(smallBox, {sushi}));
    }

    std::vector<TFilledBox> algoFilledBoxes = shop.Buy();
    SortOrder(myFilledBoxes);
    SortOrder(algoFilledBoxes);

    REQUIRE(myFilledBoxes == algoFilledBoxes);
}
