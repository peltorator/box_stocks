#include <string>
#include <ctime>
#include <string_view>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "shop.cpp"
#include "order.cpp"
#include "button.cpp"
#include "text_field.cpp"
#include "item_tile.cpp"
#include "box_tile.cpp"
#include "filled_box_tile.cpp"
#include "database.cpp"
#include "database_queries.cpp"
#include "helper_functions.cpp"
#include "font.cpp"

using namespace std;

void AddTitle(sf::RenderWindow& window, const string& title, const float px = 50.f, const float py = 20.f, const int charSize = 24) {
    sf::Text text;
    text.setFont(NFont::font);
    text.setString(title);
    text.setCharacterSize(charSize);
    text.setFillColor(sf::Color::White);
    text.setPosition(px, py);
    window.draw(text);
}

void ShowOrder(sf::RenderWindow& window, const vector<TFilledBox>& filledBoxes, const vector<TBox>& boxes, string title = "") {
    if (title.empty()) {
        title = "You finished your purchase successfully. Thank you for using our shop. Your order will come to you in the following form:";
    }
    vector<FilledBoxTile> filledBoxTiles;
    for (size_t i = 0; i < filledBoxes.size(); i++) {
        string curImage;
        for (const auto& box : boxes) {
            if (box.BoxID == filledBoxes[i].Box.BoxID) {
                curImage = box.Image;
            }
        }
        filledBoxTiles.push_back(FilledBoxTile(250.f, 550.f, filledBoxes[i].Box.BoxName, curImage, filledBoxes[i].Items));
    }

    Button goBackButton(50.f, 700.f, 100.f, 50.f, "Go Back");
    Button leftButton(25.f, 75.f, 25.f, 25.f, "<");
    Button rightButton(1350.f, 75.f, 25.f, 25.f, ">");

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
                    if ((pageIndex + 1) * columns < filledBoxTiles.size()) {
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
        for (size_t i = 0; i < filledBoxTiles.size(); i++) {
            filledBoxTiles[i].IsPresent = false;
            if (curIndex / columns == pageIndex) {
                size_t innerIndex = curIndex % columns;
                filledBoxTiles[i].SetPosition(50.f + innerIndex * 265.f, 110.f);
                filledBoxTiles[i].IsPresent = true;
            }
            curIndex++;
        }
        if ((pageIndex + 1) * columns < curIndex) {
            rightButton.Draw(window);
        }
        for (FilledBoxTile& filledBoxTile : filledBoxTiles) {
            if (filledBoxTile.IsPresent) {
                filledBoxTile.Draw(window);
            }
        }
        window.display();
    }
}

void PrintBoxes(sf::RenderWindow& window, const vector<TFilledBox>& filledBoxes, const vector<TBox>& boxes) {
    if (filledBoxes.size() == 0) {
        Button goBackButton(50.f, 700.f, 100.f, 50.f, "Go Back");
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
        ShowOrder(window, filledBoxes, boxes);
    }
}

void DidntBuyAnything(sf::RenderWindow& window) {
    Button goBackButton(50.f, 700.f, 100.f, 50.f, "Go Back");
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
    vector<TBox> availableBoxes = GetAvailableBoxes();
    vector<pair<TItem, uint32_t>> items = GetItems();
    TShop shop(items, availableBoxes);

    vector<ItemTile> itemTiles;
    for (size_t i = 0; i < items.size(); i++) {
        itemTiles.push_back(ItemTile(250.f, 250.f, items[i].first.ItemID, items[i].first.ItemName, items[i].second, items[i].first.Image, true));
    }
    Button finishButton(1200.f, 700.f, 100.f, 50.f, "Finish Order");
    Button goBackButton(50.f, 700.f, 100.f, 50.f, "Go Back");
    Button leftButton(25.f, 75.f, 25.f, 25.f, "<");
    Button rightButton(1350.f, 75.f, 25.f, 25.f, ">");

    TextField searchField(550.f, 50.f, 300.f, 50.f, "Search");

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
                                    shop.DeleteItem(items[i].first.ItemID);
                                }
                            } else if (itemTiles[i].PlusButton.IsIn(px, py)) {
                                if (itemTiles[i].CurCnt < itemTiles[i].MaxCnt) {
                                    itemTiles[i].CurCnt++;
                                    shop.AddItem(items[i].first.ItemID);
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
            if (!StartsWith(itemTiles[i].ItemName, searchField.Label)) {
                continue;
            }
            if (curIndex / pageSize == pageIndex) {
                size_t innerIndex = curIndex % pageSize;
                itemTiles[i].SetPosition(50.f + (innerIndex % columns) * 265.f, 110.f + (innerIndex / columns) * 265.f);
                itemTiles[i].IsPresent = true;
            }
            curIndex++;
        }
        if ((pageIndex + 1) * pageSize < curIndex) {
            rightButton.Draw(window);
        }
        for (ItemTile& itemTile : itemTiles) {
            if (itemTile.IsPresent) {
                itemTile.Draw(window);
            }
        }
        window.display();
    }

    
    if (shop.OrderIsEmpty()) {
        DidntBuyAnything(window);
    } else {
        vector<TFilledBox> filledBoxes = shop.Buy();
        vector<pair<TItem, int32_t>> boughtItems;
        for (const TFilledBox& filledBox : filledBoxes) {
            for (const TItem& item : filledBox.Items) {
                boughtItems.emplace_back(item, -1);
            }
        }
        SaveOrder(filledBoxes);
        UpdateItems(boughtItems);
        PrintBoxes(window, filledBoxes, availableBoxes);
    }
}

void AdminAddDeleteItem(sf::RenderWindow& window) {
    vector<pair<TItem, uint32_t>> items = GetItems();
    vector<pair<TItem, int32_t>> newItems(items.size());
    for (size_t i = 0; i < items.size(); i++) {
        newItems[i] = {items[i].first, 0};
    }

    vector<ItemTile> itemTiles;
    for (size_t i = 0; i < items.size(); i++) {
        itemTiles.push_back(ItemTile(250.f, 250.f, items[i].first.ItemID, items[i].first.ItemName, items[i].second, items[i].first.Image, false));
    }
    Button goBackButton(50.f, 700.f, 100.f, 50.f, "Finish And Go Back");
    Button leftButton(25.f, 75.f, 25.f, 25.f, "<");
    Button rightButton(1350.f, 75.f, 25.f, 25.f, ">");
    
    TextField searchField(550.f, 50.f, 300.f, 50.f, "Search");

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
                itemTiles[i].SetPosition(50.f + (innerIndex % columns) * 265.f, 110.f + (innerIndex / columns) * 265.f);
                itemTiles[i].IsPresent = true;
            }
            curIndex++;
        }
        if ((pageIndex + 1) * pageSize < curIndex) {
            rightButton.Draw(window);
        }
        for (ItemTile& itemTile : itemTiles) {
            if (itemTile.IsPresent) {
                itemTile.Draw(window);
            }
        }
        window.display();
    }

    UpdateItems(newItems);
}

string GetItemsString(const vector<TItem>& items) {
    string ans = "Your new items:\nName\tWeight\tVolume";
    for (const TItem& item : items) {
        ans += "\n" + item.ItemName + "\t\t\t" + to_string(item.Weight) + "\t\t\t" + to_string(item.Volume);
    }
    return ans;
}

void AdminCreateItem(sf::RenderWindow& window) {
    static TItem fakeItem(0, "already exists!", 0, 0);
    Button goBackButton(50.f, 700.f, 100.f, 50.f, "Go Back");
    TextField nameField(100.f, 550.f, 100.f, 50.f, "Name");
    TextField weightField(400.f, 550.f, 100.f, 50.f, "Weight");
    TextField volumeField(700.f, 550.f, 100.f, 50.f, "Volume");
    TextField imageField(1000.f, 550.f, 100.f, 50.f, "Image Path");
    Button addButton(1250.f, 550.f, 100.f, 50.f, "Add");
    string selected = "name";
    vector<TItem> items;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace)) {
                if (selected == "name") {
                    nameField.PopChar();
                } else if (selected == "weight") {
                    weightField.PopChar();
                } else if (selected == "volume") {
                    volumeField.PopChar();
                } else if (selected == "image") {
                    imageField.PopChar();
                }
            } else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    char c = static_cast<char>(event.text.unicode);
                    if (selected == "name") {
                        nameField.AddChar(c);
                    } else if (selected == "weight") {
                        weightField.AddChar(c);
                    } else if (selected == "volume") {
                        volumeField.AddChar(c);
                    } else if (selected == "image") {
                        imageField.AddChar(c);
                    }
                }
            } else if (event.type == sf::Event::MouseButtonPressed) {
                float px = event.mouseButton.x;
                float py = event.mouseButton.y;
                if (goBackButton.IsIn(px, py)) {
                    return;
                } else if (addButton.IsIn(px, py)) {
                    string selectQuery = "select itemName from Item where itemName = '" + nameField.Label + "';";
                    auto selectResponse = NDataBase::Query(selectQuery);
                    if (!selectResponse.empty()) {
                        items.push_back(fakeItem);
                    } else {
                        TItem newItem(0, nameField.Label, ToInt(weightField.Label), ToInt(volumeField.Label));
                        items.push_back(newItem);
                        InsertItem(newItem, imageField.Label);
                    }
                    nameField.Clear();
                    weightField.Clear();
                    volumeField.Clear();
                    imageField.Clear();
                } else if (nameField.IsIn(px, py)) {
                    selected = "name";
                } else if (weightField.IsIn(px, py)) {
                    selected = "weight";
                } else if (volumeField.IsIn(px, py)) {
                    selected = "volume";
                } else if (imageField.IsIn(px, py)) {
                    selected = "image";
                }
            }
        }

        window.clear();
        AddTitle(window, "Enter items:");
        AddTitle(window, GetItemsString(items), 100.f, 100.f, 20);
        goBackButton.Draw(window);
        addButton.Draw(window);
        nameField.Draw(window);
        weightField.Draw(window);
        volumeField.Draw(window);
        imageField.Draw(window);
        window.display();
    }
}

void AdminAddDeleteBox(sf::RenderWindow& window) {
    vector<pair<TBox, uint32_t>> boxes = GetBoxes();
    vector<pair<TBox, int32_t>> newBoxes(boxes.size());
    for (size_t i = 0; i < boxes.size(); i++) {
        newBoxes[i] = {boxes[i].first, 0};
    }

    vector<BoxTile> boxTiles;
    for (size_t i = 0; i < boxes.size(); i++) {
        boxTiles.push_back(BoxTile(250.f, 250.f, boxes[i].first.BoxID, boxes[i].first.BoxName, boxes[i].second, boxes[i].first.Image));
    }
    Button goBackButton(50.f, 700.f, 100.f, 50.f, "Finish And Go Back");
    Button leftButton(25.f, 75.f, 25.f, 25.f, "<");
    Button rightButton(1350.f, 75.f, 25.f, 25.f, ">");
    
    TextField searchField(550.f, 50.f, 300.f, 50.f, "Search");

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
                boxTiles[i].SetPosition(50.f + (innerIndex % columns) * 265.f, 110.f + (innerIndex / columns) * 265.f);
                boxTiles[i].IsPresent = true;
            }
            curIndex++;
        }
        if ((pageIndex + 1) * pageSize < curIndex) {
            rightButton.Draw(window);
        }
        for (BoxTile& boxTile : boxTiles) {
            if (boxTile.IsPresent) {
                boxTile.Draw(window);
            }
        }
        window.display();
    }

    UpdateBoxes(newBoxes);
}

string GetBoxesString(const vector<TBox>& boxes) {
    string ans = "Your new boxes:\nName\tMaxWeight\tMaxVolume\tCost";
    for (const TBox& box : boxes) {
        ans += "\n" + box.BoxName + "\t\t\t" + to_string(box.MaxWeight) + "\t\t\t" + to_string(box.MaxVolume) + "\t\t\t" + to_string(box.Cost);
    }
    return ans;
}

void AdminCreateBox(sf::RenderWindow& window) {
    static TBox fakeBox(0, "already exists!", 0, 0, 0);
    Button goBackButton(50.f, 700.f, 100.f, 50.f, "Go Back");
    TextField nameField(100.f, 550.f, 100.f, 50.f, "Name");
    TextField weightField(300.f, 550.f, 100.f, 50.f, "MaxWeight");
    TextField volumeField(500.f, 550.f, 100.f, 50.f, "MaxVolume");
    TextField costField(700.f, 550.f, 100.f, 50.f, "Cost");
    TextField imageField(900.f, 550.f, 100.f, 50.f, "Image Path");
    Button addButton(1100.f, 550.f, 100.f, 50.f, "Add");
    string selected = "name";
    vector<TBox> boxes;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace)) {
                if (selected == "name") {
                    nameField.PopChar();
                } else if (selected == "weight") {
                    weightField.PopChar();
                } else if (selected == "volume") {
                    volumeField.PopChar();
                } else if (selected == "cost") {
                    costField.PopChar();
                } else if (selected == "image") {
                    imageField.PopChar();
                }
            } else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    char c = static_cast<char>(event.text.unicode);
                    if (selected == "name") {
                        nameField.AddChar(c);
                    } else if (selected == "weight") {
                        weightField.AddChar(c);
                    } else if (selected == "volume") {
                        volumeField.AddChar(c);
                    } else if (selected == "cost") {
                        costField.AddChar(c);
                    } else if (selected == "image") {
                        imageField.AddChar(c);
                    }
                }
            } else if (event.type == sf::Event::MouseButtonPressed) {
                float px = event.mouseButton.x;
                float py = event.mouseButton.y;
                if (goBackButton.IsIn(px, py)) {
                    return;
                } else if (addButton.IsIn(px, py)) {
                    if (CheckIfBoxExists(nameField.Label)) {
                        boxes.push_back(fakeBox);
                    } else {
                        TBox newBox(0, nameField.Label, ToInt(weightField.Label), ToInt(volumeField.Label), ToInt(costField.Label));
                        boxes.push_back(newBox);
                        InsertBox(newBox, imageField.Label);
                    }
                    nameField.Clear();
                    weightField.Clear();
                    volumeField.Clear();
                    costField.Clear();
                    imageField.Clear();
                } else if (nameField.IsIn(px, py)) {
                    selected = "name";
                } else if (weightField.IsIn(px, py)) {
                    selected = "weight";
                } else if (volumeField.IsIn(px, py)) {
                    selected = "volume";
                } else if (costField.IsIn(px, py)) {
                    selected = "cost";
                } else if (imageField.IsIn(px, py)) {
                    selected = "image";
                }
            }
        }

        window.clear();
        AddTitle(window, "Enter boxes:");
        AddTitle(window, GetBoxesString(boxes), 100.f, 100.f, 20);
        goBackButton.Draw(window);
        addButton.Draw(window);
        nameField.Draw(window);
        weightField.Draw(window);
        volumeField.Draw(window);
        costField.Draw(window);
        imageField.Draw(window);
        window.display();
    }

}

void AdminMode(sf::RenderWindow& window) {
    Button addItemButton(350.f, 100.f, 200.f, 100.f, "Add/Delete Items");
    Button createItemButton(350.f, 300.f, 200.f, 100.f, "Create New Item");
    Button addBoxButton(850.f, 100.f, 200.f, 100.f, "Add/Delete Boxes");
    Button createBoxButton(850.f, 300.f, 200.f, 100.f, "Create New Box");
    Button goBackButton(50.f, 700.f, 100.f, 50.f, "Go Back");

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
                } else if (createItemButton.IsIn(px, py)) {
                    AdminCreateItem(window);
                } else if (addBoxButton.IsIn(px, py)) {
                    AdminAddDeleteBox(window);
                } else if (createBoxButton.IsIn(px, py)) {
                    AdminCreateBox(window);
                } else if (goBackButton.IsIn(px, py)) {
                    return;
                }
            }
        }

        window.clear();
        AddTitle(window, "Choose action:");
        addItemButton.Draw(window);
        createItemButton.Draw(window);
        addBoxButton.Draw(window);
        createBoxButton.Draw(window);
        goBackButton.Draw(window);
        window.display();
    }
}

void ShowHistory(sf::RenderWindow& window) {
    vector<TOrder> orders = GetOrders();
    vector<Button> orderButtons(orders.size());
    for (size_t i = 0; i < orders.size(); i++) {
        orderButtons[i] = Button(0.f, 0.f, 1300.f, 50.f, "Order # " + to_string(orders[i].OrderID) + "\t\tUser: " + orders[i].UserName + "\t\tOrder Date: " + orders[i].OrderDate);
    }

    vector<TBox> availableBoxes = GetAvailableBoxes();

    Button goBackButton(50.f, 700.f, 100.f, 50.f, "Go Back");
    Button leftButton(25.f, 75.f, 25.f, 25.f, "<");
    Button rightButton(1350.f, 75.f, 25.f, 25.f, ">");

    size_t pageIndex = 0;
    const size_t rows = 13;

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
                            ShowOrder(window, orders[i].FilledBoxes, availableBoxes, orderButtons[i].Label);
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
        for (Button& orderButton : orderButtons) {
            if (orderButton.IsPresent) {
                orderButton.Draw(window);
            }
        }
        window.display();
    }
}

void ChooseMode(sf::RenderWindow& window) {
    Button adminButton(200.f, 350.f, 200.f, 100.f, "Admin");
    Button userButton(600.f, 350.f, 200.f, 100.f, "Buy");
    Button historyButton(1000.f, 350.f, 200.f, 100.f, "See History");
    Button goBackButton(50.f, 700.f, 100.f, 50.f, "Close The App");

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

int main() {
    NFont::GetFont();
    sf::RenderWindow window(sf::VideoMode(1400, 800), "Shop");
    NDataBase::Open("db.sqlite");

    ChooseMode(window);
 
    window.close();
    NDataBase::Close();
    return 0;
}
