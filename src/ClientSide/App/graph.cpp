#include <string>
#include <utility>
#include <ctime>
#include <string_view>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "../../../libs/easylogging/easylogging++.cc"

#include "../../Model/order.cpp"
#include "../../Model/filled_box.cpp"
#include "../../Model/box.cpp"
#include "../../Model/item.cpp"
#include "../GraphicControls/button.cpp"
#include "../GraphicControls/text_field.cpp"
#include "../GraphicControls/item_tile.cpp"
#include "../GraphicControls/box_tile.cpp"
#include "../GraphicControls/filled_box_tile.cpp"
#include "../GraphicControls/font.cpp"
#include "../../Helper/helper_functions.cpp"
#include "../HTTP/http_queries.cpp"
#include "../DataCash/data_cash.cpp"

INITIALIZE_EASYLOGGINGPP


void AddTitle(sf::RenderWindow& window, const std::string& title, const float px = 50.f, const float py = 20.f, const int charSize = 24) {
    sf::Text text;
    text.setFont(NFont::font);
    text.setString(title);
    text.setCharacterSize(charSize);
    text.setFillColor(sf::Color::White);
    text.setPosition(px, py);
    window.draw(text);
}

void ShowOrder(sf::RenderWindow& window, const std::vector<TFilledBox>& filledBoxes, std::string title = "") {
    if (title.empty()) {
        title = "You finished your purchase successfully. Thank you for using our shop. Your order will come to you in the following form:";
    }
    std::vector<TFilledBoxTile> filledTBoxTiles;
    for (size_t i = 0; i < filledBoxes.size(); i++) {
        std::string curImage = NDataCash::IdToBox[filledBoxes[i].BoxID].Image;
        filledTBoxTiles.push_back(TFilledBoxTile(250.f, 550.f, filledBoxes[i].BoxID, curImage, filledBoxes[i].ItemIDs));
    }
    uint64_t orderTotalCost = 0;
    for (const TFilledBox& filledBox : filledBoxes) {
        orderTotalCost += NDataCash::IdToBox[filledBox.BoxID].Cost;
        for (const uint64_t &itemID : filledBox.ItemIDs) {
            orderTotalCost += NDataCash::IdToItem[itemID].Cost;
        }
    }

    TButton goBackButton(50.f, 700.f, 100.f, 50.f, "Go Back");
    TButton leftButton(25.f, 75.f, 25.f, 25.f, "<");
    TButton rightButton(1350.f, 75.f, 25.f, 25.f, ">");

    size_t pageIndex = 0;
    const size_t columns = 5;

    bool quit = false;
    while (window.isOpen() && !quit) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                float px = event.mouseButton.x;
                float py = event.mouseButton.y;

                if (goBackButton.IsIn(px, py)) {
                    return;
                } else if (leftButton.IsIn(px, py)) {
                    if (pageIndex > 0) {
                        pageIndex--;
                    }
                } else if (rightButton.IsIn(px, py)) {
                    if ((pageIndex + 1) * columns < filledTBoxTiles.size()) {
                        pageIndex++;
                    }
                }
            }
        }

        window.clear();
        AddTitle(window, title);
        goBackButton.Draw(window);
        if (pageIndex != 0) {
            leftButton.Draw(window);
        }
        size_t curIndex = 0;
        for (size_t i = 0; i < filledTBoxTiles.size(); i++) {
            filledTBoxTiles[i].IsPresent = false;
            if (curIndex / columns == pageIndex) {
                size_t innerIndex = curIndex % columns;
                filledTBoxTiles[i].SetPosition(50.f + innerIndex * 265.f, 110.f);
                filledTBoxTiles[i].IsPresent = true;
            }
            curIndex++;
        }
        if ((pageIndex + 1) * columns < curIndex) {
            rightButton.Draw(window);
        }
        for (TFilledBoxTile& filledTBoxTile : filledTBoxTiles) {
            if (filledTBoxTile.IsPresent) {
                filledTBoxTile.Draw(window);
            }
        }

        sf::Text totalCostText;
        totalCostText.setFont(NFont::font);
        totalCostText.setString("Order Cost: " + std::to_string(orderTotalCost) + " Dollars");
        totalCostText.setCharacterSize(24);
        totalCostText.setFillColor(sf::Color::White);
        totalCostText.setPosition(200.f, 710.f);

        window.draw(totalCostText);

        window.display();
    }
}

void PrintBoxes(sf::RenderWindow& window, const std::vector<TFilledBox>& filledBoxes) {
    if (filledBoxes.size() == 0) {
        TButton goBackButton(50.f, 700.f, 100.f, 50.f, "Go Back");
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed) {
                    window.close();
                } else if (event.type == sf::Event::MouseButtonPressed) {
                    float px = event.mouseButton.x;
                    float py = event.mouseButton.y;
                    if (goBackButton.IsIn(px, py)) {
                        return;
                    }
                }
            }

            window.clear();
            AddTitle(window, "Your order can't be packed because one of your items doesn't fit in any of our boxes. We are sorry.");
            goBackButton.Draw(window);
            window.display();
        }
    } else {
        ShowOrder(window, filledBoxes);
    }
}

void DidntBuyAnything(sf::RenderWindow& window) {
    TButton goBackButton(50.f, 700.f, 100.f, 50.f, "Go Back");
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                float px = event.mouseButton.x;
                float py = event.mouseButton.y;
                if (goBackButton.IsIn(px, py)) {
                    return;
                }
            }
        }
        window.clear();
        AddTitle(window, "You didn't buy anything. We hope you'll like something next time!");
        goBackButton.Draw(window);
        window.display();
    }
}

void UserMode(sf::RenderWindow& window) {
    const std::vector<TBox>& availableBoxes = NDataCash::AvailableBoxes;
    const std::vector<std::pair<TItem, uint32_t>>& items = NDataCash::Items;

    std::vector<TItemTile> itemTiles;
    for (size_t i = 0; i < items.size(); i++) {
        itemTiles.push_back(TItemTile(250.f, 250.f, items[i].first, items[i].second, true));
    }
    TButton finishButton(1200.f, 700.f, 100.f, 50.f, "Finish Order");
    TButton goBackButton(50.f, 700.f, 100.f, 50.f, "Go Back");
    TButton leftButton(25.f, 75.f, 25.f, 25.f, "<");
    TButton rightButton(1350.f, 75.f, 25.f, 25.f, ">");

    TTextField searchField(550.f, 50.f, 300.f, 50.f, "Search");

    size_t pageIndex = 0;
    const size_t rows = 2, columns = 5;
    const size_t pageSize = rows * columns;

    bool quit = false;
    while (window.isOpen() && !quit) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace)) {
                searchField.PopChar();
                pageIndex = 0;
            } else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    char c = static_cast<char>(event.text.unicode);
                    searchField.AddChar(c);
                    pageIndex = 0;
                }
            } else if (event.type == sf::Event::MouseButtonPressed) {
                float px = event.mouseButton.x;
                float py = event.mouseButton.y;

                if (goBackButton.IsIn(px, py)) {
                    return;
                } else if (finishButton.IsIn(px, py)) {
                    quit = true;
                    break;
                } else if (leftButton.IsIn(px, py)) {
                    if (pageIndex > 0) {
                        pageIndex--;
                    }
                } else if (rightButton.IsIn(px, py)) {
                    if ((pageIndex + 1) * pageSize < itemTiles.size()) {
                        pageIndex++;
                    }
                } else {
                    for (size_t i = 0; i < items.size(); i++) {
                        if (itemTiles[i].IsPresent) {
                            if (itemTiles[i].MinusButton.IsIn(px, py)) {
                                if (itemTiles[i].CurCnt > 0) {
                                    itemTiles[i].CurCnt--;
                                    NDataCash::UpdateItem(items[i].first.ItemID, -1);
                                    NHttp::DeleteItem(items[i].first.ItemID);
                                }
                            } else if (itemTiles[i].PlusButton.IsIn(px, py)) {
                                if (itemTiles[i].CurCnt < itemTiles[i].MaxCnt) {
                                    itemTiles[i].CurCnt++;
                                    NDataCash::UpdateItem(items[i].first.ItemID, 1);
                                    NHttp::AddItem(items[i].first.ItemID);
                                }
                            }
                        }
                    }
                }
            }
        }

        window.clear();
        AddTitle(window, "Select Items you want to buy:");
        finishButton.Draw(window);
        searchField.Draw(window);
        goBackButton.Draw(window);
        if (pageIndex != 0) {
            leftButton.Draw(window);
        }
        size_t curIndex = 0;
        for (size_t i = 0; i < itemTiles.size(); i++) {
            itemTiles[i].IsPresent = false;
            if (itemTiles[i].MaxCnt == 0 || !StartsWith(itemTiles[i].ItemName, searchField.Label)) {
                continue;
            }
            if (curIndex / pageSize == pageIndex) {
                size_t innerIndex = curIndex % pageSize;
                itemTiles[i].SetPosition(50.f + (innerIndex % columns) * 265.f, 150.f + (innerIndex / columns) * 265.f);
                itemTiles[i].IsPresent = true;
            }
            curIndex++;
        }
        if ((pageIndex + 1) * pageSize < curIndex) {
            rightButton.Draw(window);
        }
        for (TItemTile& itemTile : itemTiles) {
            if (itemTile.IsPresent) {
                itemTile.Draw(window);
            }
        }
        window.display();
    }

    
    if (NHttp::OrderIsEmpty()) {
        DidntBuyAnything(window);
    } else {
        std::vector<TFilledBox> filledBoxes = NHttp::ShopBuy();
        PrintBoxes(window, filledBoxes);
    }
}

void AdminAddDeleteItem(sf::RenderWindow& window) {
    const std::vector<std::pair<TItem, uint32_t>>& items = NDataCash::Items;
    std::vector<std::pair<uint64_t, int32_t>> newItems(items.size());
    for (size_t i = 0; i < items.size(); i++) {
        newItems[i] = {items[i].first.ItemID, 0};
    }

    std::vector<TItemTile> itemTiles;
    for (size_t i = 0; i < items.size(); i++) {
        itemTiles.push_back(TItemTile(250.f, 250.f, items[i].first, items[i].second, false));
    }
    TButton goBackButton(50.f, 700.f, 100.f, 50.f, "Finish And Go Back");
    TButton leftButton(25.f, 75.f, 25.f, 25.f, "<");
    TButton rightButton(1350.f, 75.f, 25.f, 25.f, ">");
    
    TTextField searchField(550.f, 50.f, 300.f, 50.f, "Search");

    size_t pageIndex = 0;
    const size_t rows = 2, columns = 5;
    const size_t pageSize = rows * columns;

    bool quit = false;
    while (window.isOpen() && !quit) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace)) {
                searchField.PopChar();
                pageIndex = 0;
            } else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    char c = static_cast<char>(event.text.unicode);
                    searchField.AddChar(c);
                    pageIndex = 0;
                }
            } else if (event.type == sf::Event::MouseButtonPressed) {
                float px = event.mouseButton.x;
                float py = event.mouseButton.y;

                if (goBackButton.IsIn(px, py)) {
                    quit = true;
                    break;
                } else if (leftButton.IsIn(px, py)) {
                    if (pageIndex > 0) {
                        pageIndex--;
                    }
                } else if (rightButton.IsIn(px, py)) {
                    if ((pageIndex + 1) * pageSize < itemTiles.size()) {
                        pageIndex++;
                    }
                } else {
                    for (size_t i = 0; i < items.size(); i++) {
                        if (itemTiles[i].IsPresent) {
                            if (itemTiles[i].MinusButton.IsIn(px, py)) {
                                if (itemTiles[i].MaxCnt > 0) {
                                    itemTiles[i].MaxCnt--;
                                    newItems[i].second--;
                                }
                            } else if (itemTiles[i].PlusButton.IsIn(px, py)) {
                                itemTiles[i].MaxCnt++;
                                newItems[i].second++;
                            }
                        }
                    }
                }
            }
        }

        window.clear();
        AddTitle(window, "Change items amounts:");
        goBackButton.Draw(window);
        searchField.Draw(window);
        if (pageIndex != 0) {
            leftButton.Draw(window);
        }
        size_t curIndex = 0;
        for (size_t i = 0; i < itemTiles.size(); i++) {
            itemTiles[i].IsPresent = false;
            if (!StartsWith(itemTiles[i].ItemName, searchField.Label)) {
                continue;
            }
            if (curIndex / pageSize == pageIndex) {
                size_t innerIndex = curIndex % pageSize;
                itemTiles[i].SetPosition(50.f + (innerIndex % columns) * 265.f, 150.f + (innerIndex / columns) * 265.f);
                itemTiles[i].IsPresent = true;
            }
            curIndex++;
        }
        if ((pageIndex + 1) * pageSize < curIndex) {
            rightButton.Draw(window);
        }
        for (TItemTile& itemTile : itemTiles) {
            if (itemTile.IsPresent) {
                itemTile.Draw(window);
            }
        }
        window.display();
    }

    for (const auto [itemID, amount] : newItems) {
        NDataCash::UpdateItem(itemID, amount);
    }
    NHttp::UpdateItems(newItems);
}

void AdminAddDeleteBox(sf::RenderWindow& window) {
    const std::vector<std::pair<TBox, uint32_t>>& boxes = NDataCash::Boxes;
    std::vector<std::pair<uint64_t, int32_t>> newBoxes(boxes.size());
    for (size_t i = 0; i < boxes.size(); i++) {
        newBoxes[i] = {boxes[i].first.BoxID, 0};
    }

    std::vector<TBoxTile> boxTiles;
    for (size_t i = 0; i < boxes.size(); i++) {
        boxTiles.push_back(TBoxTile(250.f, 250.f, boxes[i].first.BoxID, boxes[i].first.BoxName, boxes[i].second, boxes[i].first.Image));
    }
    TButton goBackButton(50.f, 700.f, 100.f, 50.f, "Finish And Go Back");
    TButton leftButton(25.f, 75.f, 25.f, 25.f, "<");
    TButton rightButton(1350.f, 75.f, 25.f, 25.f, ">");
    
    TTextField searchField(550.f, 50.f, 300.f, 50.f, "Search");

    size_t pageIndex = 0;
    const size_t rows = 2, columns = 5;
    const size_t pageSize = rows * columns;

    bool quit = false;
    while (window.isOpen() && !quit) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace)) {
                searchField.PopChar();
                pageIndex = 0;
            } else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    char c = static_cast<char>(event.text.unicode);
                    searchField.AddChar(c);
                    pageIndex = 0;
                }
            } else if (event.type == sf::Event::MouseButtonPressed) {
                float px = event.mouseButton.x;
                float py = event.mouseButton.y;

                if (goBackButton.IsIn(px, py)) {
                    quit = true;
                    break;
                } else if (leftButton.IsIn(px, py)) {
                    if (pageIndex > 0) {
                        pageIndex--;
                    }
                } else if (rightButton.IsIn(px, py)) {
                    if ((pageIndex + 1) * pageSize < boxTiles.size()) {
                        pageIndex++;
                    }
                } else {
                    for (size_t i = 0; i < boxes.size(); i++) {
                        if (boxTiles[i].IsPresent) {
                            if (boxTiles[i].AvailableButton.IsIn(px, py)) {
                                if (boxTiles[i].Available) {
                                    boxTiles[i].Available = false;
                                    newBoxes[i].second--;
                                } else {
                                    boxTiles[i].Available = true;
                                    newBoxes[i].second++;
                                }
                            }
                        }
                    }
                }
            }
        }

        window.clear();
        AddTitle(window, "Change Boxes Availability:");
        goBackButton.Draw(window);
        searchField.Draw(window);
        if (pageIndex != 0) {
            leftButton.Draw(window);
        }
        size_t curIndex = 0;
        for (size_t i = 0; i < boxTiles.size(); i++) {
            boxTiles[i].IsPresent = false;
            if (!StartsWith(boxTiles[i].BoxName, searchField.Label)) {
                continue;
            }
            if (curIndex / pageSize == pageIndex) {
                size_t innerIndex = curIndex % pageSize;
                boxTiles[i].SetPosition(50.f + (innerIndex % columns) * 265.f, 150.f + (innerIndex / columns) * 265.f);
                boxTiles[i].IsPresent = true;
            }
            curIndex++;
        }
        if ((pageIndex + 1) * pageSize < curIndex) {
            rightButton.Draw(window);
        }
        for (TBoxTile& boxTile : boxTiles) {
            if (boxTile.IsPresent) {
                boxTile.Draw(window);
            }
        }
        window.display();
    }

    NHttp::UpdateBoxes(newBoxes);
    for (auto [boxID, amount] : newBoxes) {
        NDataCash::UpdateBox(boxID, amount);
    }
}

void AdminMode(sf::RenderWindow& window) {
    TButton addItemButton(350.f, 350.f, 200.f, 100.f, "Add/Delete Items");
    TButton addBoxButton(850.f, 350.f, 200.f, 100.f, "Add/Delete Boxes");
    TButton goBackButton(50.f, 700.f, 100.f, 50.f, "Go Back");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                float px = event.mouseButton.x;
                float py = event.mouseButton.y;

                if (addItemButton.IsIn(px, py)) {
                    AdminAddDeleteItem(window);
                } else if (addBoxButton.IsIn(px, py)) {
                    AdminAddDeleteBox(window);
                } else if (goBackButton.IsIn(px, py)) {
                    return;
                }
            }
        }

        window.clear();
        AddTitle(window, "Choose action:");
        addItemButton.Draw(window);
        addBoxButton.Draw(window);
        goBackButton.Draw(window);
        window.display();
    }
}

void ShowHistory(sf::RenderWindow& window) {
    std::vector<TOrder> orders = NHttp::GetOrders();
    std::vector<TButton> orderButtons(orders.size());
    for (size_t i = 0; i < orders.size(); i++) {
        orderButtons[i] = TButton(0.f, 0.f, 1300.f, 50.f, "Order # " + std::to_string(orders[i].OrderID) + "\t\tUser: " + orders[i].UserName + "\t\tOrder Date: " + orders[i].OrderDate);
    }

    std::vector<TBox>& availableBoxes = NDataCash::AvailableBoxes;

    TButton goBackButton(50.f, 700.f, 100.f, 50.f, "Go Back");
    TButton leftButton(25.f, 60.f, 25.f, 25.f, "<");
    TButton rightButton(1350.f, 60.f, 25.f, 25.f, ">");

    size_t pageIndex = 0;
    const size_t rows = 8;

    bool quit = false;
    while (window.isOpen() && !quit) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                float px = event.mouseButton.x;
                float py = event.mouseButton.y;

                if (goBackButton.IsIn(px, py)) {
                    return;
                } else if (leftButton.IsIn(px, py)) {
                    if (pageIndex > 0) {
                        pageIndex--;
                    }
                } else if (rightButton.IsIn(px, py)) {
                    if ((pageIndex + 1) * rows < orderButtons.size()) {
                        pageIndex++;
                    }
                } else {
                    for (size_t i = 0; i < orders.size(); i++) {
                        if (orderButtons[i].IsPresent && orderButtons[i].IsIn(px, py)) {
                            ShowOrder(window, orders[i].FilledBoxes, orderButtons[i].Label);
                            break;
                        }
                    }
                }
            }
        }

        window.clear();
        AddTitle(window, "This Is History Of Your Orders. You Can Select Any Of Them.");
        goBackButton.Draw(window);
        if (pageIndex != 0) {
            leftButton.Draw(window);
        }
        size_t curIndex = 0;
        for (size_t i = 0; i < orderButtons.size(); i++) {
            orderButtons[i].IsPresent = false;
            if (curIndex / rows == pageIndex) {
                size_t innerIndex = curIndex % rows;
                orderButtons[i].SetPosition(50.f, 100.f + innerIndex * 75.f);
                orderButtons[i].IsPresent = true;
            }
            curIndex++;
        }
        if ((pageIndex + 1) * rows < curIndex) {
            rightButton.Draw(window);
        }
        for (TButton& orderButton : orderButtons) {
            if (orderButton.IsPresent) {
                orderButton.Draw(window);
            }
        }
        window.display();
    }
}

void ChooseMode(sf::RenderWindow& window) {
    TButton adminButton(200.f, 350.f, 200.f, 100.f, "Admin");
    TButton userButton(600.f, 350.f, 200.f, 100.f, "Buy");
    TButton historyButton(1000.f, 350.f, 200.f, 100.f, "See History");
    TButton goBackButton(50.f, 700.f, 100.f, 50.f, "Close The App");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                float px = event.mouseButton.x;
                float py = event.mouseButton.y;

                if (adminButton.IsIn(px, py)) {
                    AdminMode(window);
                }
                else if (userButton.IsIn(px, py)) {
                    UserMode(window);
                } else if (historyButton.IsIn(px, py)) {
                    ShowHistory(window);
                } else if (goBackButton.IsIn(px, py)) {
                    return;
                }
            }
        }
        window.clear();
        AddTitle(window, "Choose mode:");
        adminButton.Draw(window);
        userButton.Draw(window);
        historyButton.Draw(window);
        goBackButton.Draw(window);
        window.display();
    }
}

int main(int argc, char* argv[]) {
    START_EASYLOGGINGPP(argc, argv);
    el::Configurations conf("client_logging_config.cfg");
    el::Loggers::reconfigureAllLoggers(conf);

    NFont::LoadFont();

    NDataCash::LoadItems();
    NDataCash::LoadBoxes();

    sf::RenderWindow window(sf::VideoMode(1400, 800), "Shop");
    ChooseMode(window);
    window.close();
    return 0;
}
