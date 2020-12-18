#define CATCH_CONFIG_MAIN
#include "../../../libs/catch2/catch.hpp"
#include "../shop.cpp"


TEST_CASE("Empty shop", "[single-file]") {
    TShop shop({}, {});

    std::vector<TFilledBox> myFilledBoxes;

    REQUIRE(shop.Buy() == myFilledBoxes);
}

TEST_CASE("3 items (1, 1). Boxes (1, 1, 2) and (2, 2, 3). The second box is cheeper than two first boxes.", "[single-file]") {
    const TItem ball = TItem("ball", 1, 1);

    const TBox smallBox = TBox("small", 1, 1, 2);
    const TBox bigBox = TBox("big", 2, 2, 3);

    TShop shop({{ball, 3}}, {smallBox, bigBox});

    shop.AddItem(ball.GetItemID(), 3);

    std::vector<TFilledBox> myFilledBoxes = {
        TFilledBox(bigBox, {ball, ball}),
        TFilledBox(smallBox, {ball})
    };

    REQUIRE(shop.Buy() == myFilledBoxes);
}

TEST_CASE("3 items (1, 1). Boxes (1, 1, 2) and (2, 2, 5). The second box is more expensive than two first boxes.", "[single-file]") {
    const TItem ball = TItem("ball", 1, 1);

    const TBox smallBox = TBox("small", 1, 1, 2);
    const TBox bigBox = TBox("big", 2, 2, 5);

    TShop shop({{ball, 3}}, {smallBox, bigBox});
    
    shop.AddItem(ball.GetItemID(), 3);

    std::vector<TFilledBox> myFilledBoxes = {
        TFilledBox(smallBox, {ball}),
        TFilledBox(smallBox, {ball}),
        TFilledBox(smallBox, {ball})
    };

    REQUIRE(shop.Buy() == myFilledBoxes);
}

TEST_CASE("Same boxes with different cost.", "[single-file]") {
    const TItem ball = TItem("ball", 1, 1);
    
    const TBox cheepBox = TBox("cheep", 2, 2, 2);
    const TBox expensiveBox = TBox("expensive", 2, 2, 3);

    TShop shop({{ball, 5}}, {cheepBox, expensiveBox});

    shop.AddItem(ball.GetItemID(), 5);

    std::vector<TFilledBox> myFilledBoxes = {
        TFilledBox(cheepBox, {ball, ball}),
        TFilledBox(cheepBox, {ball, ball}),
        TFilledBox(cheepBox, {ball}),
    };

    REQUIRE(shop.Buy() == myFilledBoxes);
}

TEST_CASE("One of the items doesn't fit in any box", "[single-file]") {
    const TItem ball = TItem("ball", 1, 1);
    const TItem car = TItem("car", 100, 100);

    const TBox smallBox = TBox("small", 1, 1, 2);
    const TBox bigBox = TBox("big", 5, 5, 5);

    TShop shop({{ball, 1}, {car, 1}}, {smallBox, bigBox});

    shop.AddItem(ball.GetItemID(), 1);
    shop.AddItem(car.GetItemID(), 1);

    std::vector<TFilledBox> myFilledBoxes;

    REQUIRE(shop.Buy() == myFilledBoxes);
}

TEST_CASE("No boxes but some items", "[single-file]") {
    const TItem ball = TItem("ball", 1, 1);

    TShop shop({{ball, 1}}, {});

    shop.AddItem(ball.GetItemID(), 1);

    std::vector<TFilledBox> myFilledBoxes;

    REQUIRE(shop.Buy() == myFilledBoxes);
}

TEST_CASE("Add + Delete", "[single-file]") {
    const TItem ball = TItem("ball", 1, 1);

    const TBox smallBox = TBox("small", 1, 1, 2);

    TShop shop({{ball, 1}}, {smallBox});

    shop.AddItem(ball.GetItemID());
    shop.DeleteItem(ball.GetItemID());

    REQUIRE(shop.OrderIsEmpty());
}

TEST_CASE("Restaurant test 1", "[single-file]") {
    const TItem sushi = TItem("sushi", 100, 10);
    const TItem pizza = TItem("pizza", 300, 35);
    const TItem stake = TItem("stake", 300, 20);

    const TBox smallBox = TBox("small", 200, 8, 10);
    const TBox mediumBox = TBox("medium", 200, 20, 20);
    const TBox bigBox = TBox("big", 300, 30, 30);
    const TBox largeBox = TBox("large", 350, 40, 40);
    const TBox extraLargeBox = TBox("extra large", 400, 30, 45);
    const TBox enormousBox = TBox("enormous", 600, 100, 100);

    TShop shop({{sushi, 4}, {pizza, 3}, {stake, 2}}, {smallBox, mediumBox, bigBox, largeBox, extraLargeBox, enormousBox});

    shop.AddItem(sushi.GetItemID(), 4);
    shop.AddItem(pizza.GetItemID(), 3);
    shop.AddItem(stake.GetItemID(), 2);

    std::vector<TFilledBox> myFilledBoxes = {
        TFilledBox(mediumBox, {sushi, sushi}),
        TFilledBox(mediumBox, {sushi, sushi}),
        TFilledBox(largeBox, {pizza}),
        TFilledBox(largeBox, {pizza}),
        TFilledBox(largeBox, {pizza}),
        TFilledBox(bigBox, {stake}),
        TFilledBox(bigBox, {stake}),
    };

    REQUIRE(shop.Buy() == myFilledBoxes);
}

TEST_CASE("Restaurant test 2", "[single-file]") {
    const TItem sushi = TItem("sushi", 100, 10);
    const TItem pizza = TItem("pizza", 300, 35);
    const TItem stake = TItem("stake", 300, 20);

    const TBox smallBox = TBox("small", 200, 8, 10);
    const TBox mediumBox = TBox("medium", 200, 20, 20);
    const TBox bigBox = TBox("big", 300, 30, 30);
    const TBox largeBox = TBox("large", 350, 40, 40);
    const TBox extraLargeBox = TBox("extra large", 400, 30, 39);
    const TBox enormousBox = TBox("enormous", 600, 100, 100);

    TShop shop({{sushi, 4}, {pizza, 3}, {stake, 2}}, {smallBox, mediumBox, bigBox, largeBox, extraLargeBox, enormousBox});

    shop.AddItem(sushi.GetItemID(), 4);
    shop.AddItem(pizza.GetItemID(), 3);
    shop.AddItem(stake.GetItemID(), 2);

    std::vector<TFilledBox> myFilledBoxes = {
        TFilledBox(mediumBox, {sushi, sushi}),
        TFilledBox(extraLargeBox, {stake, sushi}),
        TFilledBox(extraLargeBox, {stake, sushi}),
        TFilledBox(largeBox, {pizza}),
        TFilledBox(largeBox, {pizza}),
        TFilledBox(largeBox, {pizza}),
    };

    REQUIRE(shop.Buy() == myFilledBoxes);
}

TEST_CASE("Restaurant test 3", "[single-file]") {
    const TItem sushi = TItem("sushi", 100, 10);
    const TItem pizza = TItem("pizza", 300, 35);
    const TItem stake = TItem("stake", 300, 20);

    const TBox smallBox = TBox("small", 200, 8, 10);
    const TBox mediumBox = TBox("medium", 200, 20, 20);
    const TBox bigBox = TBox("big", 300, 30, 30);
    const TBox largeBox = TBox("large", 350, 40, 51);
    const TBox extraLargeBox = TBox("extra large", 400, 30, 39);
    const TBox enormousBox = TBox("enormous", 600, 100, 100);

    TShop shop({{sushi, 4}, {pizza, 3}, {stake, 2}}, {smallBox, mediumBox, bigBox, largeBox, extraLargeBox, enormousBox});

    shop.AddItem(sushi.GetItemID(), 4);
    shop.AddItem(pizza.GetItemID(), 3);
    shop.AddItem(stake.GetItemID(), 2);

    std::vector<TFilledBox> myFilledBoxes = {
        TFilledBox(mediumBox, {sushi, sushi}),
        TFilledBox(extraLargeBox, {stake, sushi}),
        TFilledBox(extraLargeBox, {stake, sushi}),
        TFilledBox(enormousBox, {pizza, pizza}),
        TFilledBox(largeBox, {pizza}),
    };

    REQUIRE(shop.Buy() == myFilledBoxes);
}

TEST_CASE("Restaurant test 4", "[single-file]") {
    const TItem sushi = TItem("sushi", 100, 10);

    const TBox smallBox = TBox("small", 100, 10, 10);
    const TBox bigBox = TBox("big", 400, 40, 35);

    TShop shop({{sushi, 100}}, {smallBox, bigBox});

    shop.AddItem(sushi.GetItemID(), 3);

    std::vector<TFilledBox> myFilledBoxes = {
        TFilledBox(smallBox, {sushi}),
        TFilledBox(smallBox, {sushi}),
        TFilledBox(smallBox, {sushi}),
    };

    REQUIRE(shop.Buy() == myFilledBoxes);
}

TEST_CASE("Restaurant test 5", "[single-file]") {
    const TItem sushi = TItem("sushi", 100, 10);

    const TBox smallBox = TBox("small", 100, 10, 10);
    const TBox bigBox = TBox("big", 400, 40, 35);

    TShop shop({{sushi, 100}}, {smallBox, bigBox});

    shop.AddItem(sushi.GetItemID(), 4);

    std::vector<TFilledBox> myFilledBoxes = {
        TFilledBox(bigBox, {sushi, sushi, sushi, sushi}),
    };

    REQUIRE(shop.Buy() == myFilledBoxes);
}

TEST_CASE("Restaurant test 6", "[single-file]") {
    const TItem sushi = TItem("sushi", 100, 10);

    const TBox smallBox = TBox("small", 100, 10, 10);
    const TBox bigBox = TBox("big", 400, 40, 35);

    TShop shop({{sushi, 100}}, {smallBox, bigBox});

    shop.AddItem(sushi.GetItemID(), 7);

    std::vector<TFilledBox> myFilledBoxes = {
        TFilledBox(bigBox, {sushi, sushi, sushi, sushi}),
        TFilledBox(smallBox, {sushi}),
        TFilledBox(smallBox, {sushi}),
        TFilledBox(smallBox, {sushi}),
    };

    REQUIRE(shop.Buy() == myFilledBoxes);
}

TEST_CASE("Restaurant test 7", "[single-file]") {
    const TItem sushi = TItem("sushi", 100, 10);

    const TBox smallBox = TBox("small", 100, 10, 10);
    const TBox bigBox = TBox("big", 400, 40, 35);

    TShop shop({{sushi, 100}}, {smallBox, bigBox});

    shop.AddItem(sushi.GetItemID(), 8);

    std::vector<TFilledBox> myFilledBoxes = {
        TFilledBox(bigBox, {sushi, sushi, sushi, sushi}),
        TFilledBox(bigBox, {sushi, sushi, sushi, sushi}),
    };

    REQUIRE(shop.Buy() == myFilledBoxes);
}

TEST_CASE("Restaurant test 8", "[single-file]") {
    const TItem sushi = TItem("sushi", 100, 10);

    const TBox smallBox = TBox("small", 100, 10, 10);
    const TBox bigBox = TBox("big", 400, 40, 35);

    TShop shop({{sushi, 100}}, {smallBox, bigBox});

    shop.AddItem(sushi.GetItemID(), 9);

    std::vector<TFilledBox> myFilledBoxes = {
        TFilledBox(bigBox, {sushi, sushi, sushi, sushi}),
        TFilledBox(bigBox, {sushi, sushi, sushi, sushi}),
        TFilledBox(smallBox, {sushi}),
    };

    REQUIRE(shop.Buy() == myFilledBoxes);
}
