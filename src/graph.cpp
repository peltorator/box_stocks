#include "shop.cpp"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
//#include <SFML/Image.hpp>
#include "database.cpp"

using namespace std;

const string ADMIN = "ADMIN";
const string USER = "USER";
const string PASSWORD = "isaf27";
sf::Font font;

struct Button {
    float x;
    float y;
    float dx;
    float dy;
    string label;

    Button() = default;

    Button(float x, float y, float dx, float dy, string label)
        : x(x)
        , y(y)
        , dx(dx)
        , dy(dy)
        , label(label) {}

    void Draw(sf::RenderWindow& window) {
        sf::RectangleShape rectangle(sf::Vector2f(dx, dy));
        rectangle.setPosition(x, y);
        rectangle.setFillColor(sf::Color::White);

        sf::Text text;
        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(18);
        text.setCharacterSize(min(1.0, dx / text.getLocalBounds().width * 0.9) * 18.0);
        text.setFillColor(sf::Color::Black);
        float cx = x + dx * 0.5 - text.getLocalBounds().width * 0.5;
        float cy = y + dy * 0.5 - text.getLocalBounds().height * 0.5;
        text.setPosition(cx, cy);
        
        window.draw(rectangle);
        window.draw(text);
    }

    bool IsIn(float px, float py) {
        return x <= px && px <= x + dx && y <= py && py <= y + dy;
    }
};

struct TextField {
    float x;
    float y;
    float dx;
    float dy;
    string name;
    string label;
    bool needPop;

    TextField() = default;

    TextField(float x, float y, float dx, float dy, string name)
        : x(x)
        , y(y)
        , dx(dx)
        , dy(dy)
        , name(name)
        , label("")
        , needPop(false) {}

    void Draw(sf::RenderWindow& window) {
        sf::RectangleShape rectangle(sf::Vector2f(dx, dy));
        rectangle.setPosition(x, y);
        rectangle.setFillColor(sf::Color::White);

        sf::Text text;
        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(18);
        text.setCharacterSize(min(1.0, dx / text.getLocalBounds().width * 0.9) * 18.0);
        text.setFillColor(sf::Color::Black);
        float cx = x + dx * 0.1;
        float cy = y + dy * 0.5 - text.getLocalBounds().height * 0.5;
        text.setPosition(cx, cy);

        sf::Text nameText;
        nameText.setFont(font);
        nameText.setString(name);
        nameText.setCharacterSize(18);
        nameText.setCharacterSize(min(1.0, dx / nameText.getLocalBounds().width * 0.9) * 18.0);
        nameText.setFillColor(sf::Color::White);
        float cx2 = x + dx * 0.5 - nameText.getLocalBounds().width * 0.5;
        float cy2 = y - dy * 0.2 - nameText.getLocalBounds().height;
        nameText.setPosition(cx2, cy2);
        
        window.draw(rectangle);
        window.draw(text);
        window.draw(nameText);
    }

    bool IsIn(float px, float py) {
        return x <= px && px <= x + dx && y <= py && py <= y + dy;
    }

    void AddChar(const char& c) {
        label.push_back(c);
    }

    void PopChar() {
        if (!label.empty() && needPop) {
            label.pop_back();
        }
        needPop = !needPop;
    }

    void Clear() {
        label.clear();
    }
};


void AddTitle(sf::RenderWindow& window, const string& title, const float px = 50.f, const float py = 20.f, const int charSize = 24) {
    sf::Text text;
    text.setFont(font);
    text.setString(title);
    text.setCharacterSize(charSize);
    text.setFillColor(sf::Color::White);
    //text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    text.setPosition(px, py);
    window.draw(text);
}

string ChooseMode(sf::RenderWindow& window) {
    string mode = "";
    Button adminButton(350.f, 350.f, 200.f, 100.f, "Admin");
    Button userButton(850.f, 350.f, 200.f, 100.f, "User");
    while (window.isOpen() && mode.empty()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                float px = event.mouseButton.x;
                float py = event.mouseButton.y;

                if (adminButton.IsIn(px, py)) {
                    mode = ADMIN;
                    break;
                }
                if (userButton.IsIn(px, py)) {
                    mode = USER;
                    break;
                }
            }
        }

        window.clear();
        AddTitle(window, "Choose mode:");
        adminButton.Draw(window);
        userButton.Draw(window);
        window.display();
    }
    return mode;
}

uint64_t ToInt(const string& s) {
    uint64_t ans = 0;
    for (const char c : s) {
        if (c >= '0' && c <= '9') {
            ans = ans * 10LL + (c - '0');
        } else {
            return 0;
        }
    }
    return ans;
}


string GetItemsString(const vector<pair<TItem, uint32_t>>& items) {
    string ans = "Your items:\nName\tWeight\tVolume\tQuantity";
    for (const auto& [item, cnt] : items) {
        ans += "\n" + item.GetItemName() + "\t\t\t" + to_string(item.GetWeight()) + "\t\t\t" + to_string(item.GetVolume()) + "\t\t\t" + to_string(cnt);
    }
    return ans;
}

vector<pair<TItem, uint32_t>> GetItems(sf::RenderWindow& window) {
    vector<pair<TItem, uint32_t>> ans;
    Button finishButton(1200.f, 700.f, 100.f, 50.f, "Finish");
    TextField nameField(100.f, 550.f, 100.f, 50.f, "Name");
    TextField weightField(400.f, 550.f, 100.f, 50.f, "Weight");
    TextField volumeField(700.f, 550.f, 100.f, 50.f, "Volume");
    TextField quantityField(1000.f, 550.f, 100.f, 50.f, "Quantity");
    Button addButton(1250.f, 550.f, 100.f, 50.f, "Add");
    string selected = "name";
    bool quit = false;
    
    while (window.isOpen() && !quit) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace)) {
                cout << "backspace" << endl;
                if (selected == "name") {
                    nameField.PopChar();
                } else if (selected == "weight") {
                    weightField.PopChar();
                } else if (selected == "volume") {
                    volumeField.PopChar();
                } else if (selected == "quantity") {
                    quantityField.PopChar();
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
                    } else if (selected == "quantity") {
                        quantityField.AddChar(c);
                    }
                }
            } else if (event.type == sf::Event::MouseButtonPressed) {
                float px = event.mouseButton.x;
                float py = event.mouseButton.y;
                if (finishButton.IsIn(px, py)) {
                    quit = true;
                    break;
                } else if (addButton.IsIn(px, py)) {
                     ans.push_back({TItem(nameField.label, ToInt(weightField.label), ToInt(volumeField.label)), ToInt(quantityField.label)});
                     nameField.Clear();
                     weightField.Clear();
                     volumeField.Clear();
                     quantityField.Clear();
                } else if (nameField.IsIn(px, py)) {
                    selected = "name";
                } else if (weightField.IsIn(px, py)) {
                    selected = "weight";
                } else if (volumeField.IsIn(px, py)) {
                    selected = "volume";
                } else if (quantityField.IsIn(px, py)) {
                    selected = "quantity";
                }
            }
        }

        window.clear();
        AddTitle(window, "Enter items:");
        AddTitle(window, GetItemsString(ans), 100.f, 100.f, 20);
        finishButton.Draw(window);
        addButton.Draw(window);
        nameField.Draw(window);
        weightField.Draw(window);
        volumeField.Draw(window);
        quantityField.Draw(window);
        window.display();
    }
    return ans;
}

string GetBoxesString(const vector<TBox>& boxes) {
    string ans = "Your boxes:\nName\tMaxWeight\tMaxVolume\tCost";
    for (const TBox& box : boxes) {
        ans += "\n" + box.GetBoxName() + "\t\t\t" + to_string(box.GetMaxWeight()) + "\t\t\t" + to_string(box.GetMaxVolume()) + "\t\t\t" + to_string(box.GetCost());
    }
    return ans;
}

vector<TBox> GetBoxes(sf::RenderWindow& window) {
    vector<TBox> ans;
    Button finishButton(1200.f, 700.f, 100.f, 50.f, "Finish");
    TextField nameField(100.f, 550.f, 100.f, 50.f, "Name");
    TextField weightField(400.f, 550.f, 100.f, 50.f, "MaxWeight");
    TextField volumeField(700.f, 550.f, 100.f, 50.f, "MaxVolume");
    TextField costField(1000.f, 550.f, 100.f, 50.f, "Cost");
    Button addButton(1250.f, 550.f, 100.f, 50.f, "Add");
    string selected = "name";
    bool quit = false;
    
    while (window.isOpen() && !quit) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) {
                if (selected == "name") {
                    nameField.PopChar();
                } else if (selected == "weight") {
                    weightField.PopChar();
                } else if (selected == "volume") {
                    volumeField.PopChar();
                } else if (selected == "cost") {
                    costField.PopChar();
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
                    }
                }
            } else if (event.type == sf::Event::MouseButtonPressed) {
                float px = event.mouseButton.x;
                float py = event.mouseButton.y;
                if (finishButton.IsIn(px, py)) {
                    quit = true;
                    break;
                } else if (addButton.IsIn(px, py)) {
                     ans.push_back(TBox(nameField.label, ToInt(weightField.label), ToInt(volumeField.label), ToInt(costField.label)));
                     nameField.Clear();
                     weightField.Clear();
                     volumeField.Clear();
                     costField.Clear();
                } else if (nameField.IsIn(px, py)) {
                    selected = "name";
                } else if (weightField.IsIn(px, py)) {
                    selected = "weight";
                } else if (volumeField.IsIn(px, py)) {
                    selected = "volume";
                } else if (costField.IsIn(px, py)) {
                    selected = "cost";
                }
            }
        }

        window.clear();
        AddTitle(window, "Enter Boxes:");
        AddTitle(window, GetBoxesString(ans), 100.f, 100.f, 20);
        finishButton.Draw(window);
        addButton.Draw(window);
        nameField.Draw(window);
        weightField.Draw(window);
        volumeField.Draw(window);
        costField.Draw(window);
        window.display();
    }
    return ans;
}

const string SETTINGS_PATH = "settings.txt";

string GetImage(string filename) {
    return filename;//"image...";
}


void FillSettings(sf::RenderWindow& window, const vector<pair<TItem, uint32_t>>& items, const vector<TBox>& boxes, TDataBase& dataBase) {
    for (const auto& [item, amount] : items) {
        string findQuery = "select amount from Item where itemName = " + item.GetItemName() + ";";
        auto result = dataBase.Query(findQuery);
        uint32_t totalAmount = amount;
        if (!result.empty()) {
            totalAmount += ToInt(result[0]["amount"]);
        }

        string deleteQuery = "delete from Item where itemName = '" + item.GetItemName() + "';";
        dataBase.Query(deleteQuery);

        string itemAddQuery = "insert into Item(itemName, weight, volume, amount, image) values ('" + item.GetItemName() + "', " + to_string(item.GetWeight()) + ", " + to_string(item.GetVolume()) + ", " + to_string(totalAmount) + ", '" + GetImage("../images/" + item.GetItemName()) + "');";
        dataBase.Query(itemAddQuery);
    }

    for (const TBox& box : boxes) {
        string findQuery = "select boxName from Box where boxName = '" + box.GetBoxName() + "';";
        auto result = dataBase.Query(findQuery);
        if (result.empty()) {
            string boxAddQuery = "insert into Box(boxName, maxWeight, maxVolume, cost, image) values ('" + box.GetBoxName() + "', " + to_string(box.GetMaxWeight()) + ", " + to_string(box.GetMaxVolume()) + ", " + to_string(box.GetCost()) + ", '" + GetImage("../images/" + box.GetBoxName()) + "');";
            dataBase.Query(boxAddQuery);
        }
    }

    Button finishButton(1200.f, 700.f, 100.f, 50.f, "Finish");
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                float px = event.mouseButton.x;
                float py = event.mouseButton.y;
                if (finishButton.IsIn(px, py)) {
                    window.close();
                }
            }
        }
        window.clear();
        AddTitle(window, "Settings are saved. Have a nice day!");
        finishButton.Draw(window);
        window.display();
    }
}

string DecodeImage(const string &s) {
    return s;
    string ans;
    for (size_t i = 0; i < s.size(); i += 8) {
        char c = 0;
        for (size_t j = i; j < i + 8; j++) {
            c <<= 1;
            c += (s[j] - '0');
        }
        ans.push_back(c);
    }
    return ans;
}


pair<pair<vector<pair<TItem, uint32_t>>, vector<string>>, pair<vector<TBox>, vector<string>>> GetSettings(sf::RenderWindow& window, TDataBase& dataBase) {
    string getItemsQuery = "select * from Item;";
    vector<pair<TItem, uint32_t>> items;
    vector<string> itemImages;
    auto itemsRaw = dataBase.Query(getItemsQuery);
    for (auto& dict : itemsRaw) {
        items.push_back({TItem(dict["itemName"], ToInt(dict["weight"]), ToInt(dict["volume"])), ToInt(dict["amount"])});
        itemImages.push_back(DecodeImage(dict["image"]));
    }

    string getBoxesQuery = "select * from Box;";
    vector<TBox> boxes;
    vector<string> boxImages;
    auto boxesRaw = dataBase.Query(getBoxesQuery);
    for (auto& dict : boxesRaw) {
        boxes.push_back(TBox(dict["boxName"], ToInt(dict["maxWeight"]), ToInt(dict["maxVolume"]), ToInt(dict["cost"])));
        boxImages.push_back(DecodeImage(dict["image"]));
    }

    return {{items, itemImages}, {boxes, boxImages}};
}

struct ItemTile {
    float x;
    float y;
    float dx;
    float dy;
    string name;
    uint32_t cnt;
    uint32_t maxcnt;
    Button minusButton;
    Button plusButton;
    sf::Texture pictureTexture;

    /*class MyStream : public sf::InputStream {
        string Str;
        int64_t StrSize;
        int64_t CurPos;
    public:
        MyStream(const string& str) : Str(str), StrSize(static_cast<int64_t>(str.size())), CurPos(0) {}

        int64_t read(char* data, int64_t size) {
            int64_t ind = 0;
            while (ind < size && CurPos < StrSize) {
                data[ind++] = Str[CurPos];
            }
            return ind;
        }

        int64_t seek(int64_t position) {
            CurPos = min(position, StrSize);
            return CurPos;
        }

        int64_t tell() {
            return CurPos;
        }

        int64_t getSize() {
            return StrSize;
        }
    };*/

    ItemTile() = default;

    ItemTile(float curx, float cury, float curdx, float curdy, string curname, uint32_t curmaxcnt, const string img) {
        x = curx;
        y = cury;
        dx = curdx;
        dy = curdy;
        name = "Name: " + curname;
        cnt = 0;
        maxcnt = curmaxcnt;
        minusButton = Button(x + 0.65 * dx, y + 0.1 * dy, 0.05 * dx, 0.8 * dy, "-");
        plusButton = Button(x + 0.75 * dx, y + 0.1 * dy, 0.05 * dx, 0.8 * dy, "+");

        pictureTexture.loadFromFile(img);
    }

    void Draw(sf::RenderWindow& window) {
        sf::RectangleShape rectangle(sf::Vector2f(dx, dy));
        rectangle.setPosition(x, y);
        rectangle.setFillColor(sf::Color::White);

        sf::Text text;
        text.setFont(font);
        text.setString(name);
        text.setCharacterSize(18);
        text.setCharacterSize(min(1.0, dx / text.getLocalBounds().width * 0.9) * 18.0);
        text.setFillColor(sf::Color::Black);
        float cx = x + dx * 0.1;
        float cy = y + dy * 0.5 - text.getLocalBounds().height * 0.5;
        text.setPosition(cx, cy);

        sf::Text cntText;
        cntText.setFont(font);
        cntText.setString(to_string(cnt) + " / " + to_string(maxcnt));
        cntText.setCharacterSize(18);
        cntText.setCharacterSize(min(1.0, dx / cntText.getLocalBounds().width * 0.9) * 18.0);
        cntText.setFillColor(sf::Color::Black);
        cntText.setPosition(x + 0.85 * dx, y + dy * 0.5 - cntText.getLocalBounds().height * 0.5);

        sf::Sprite pictureSprite;
        pictureSprite.setTexture(pictureTexture);
        pictureSprite.setPosition(x + dx * 0.4, y + 0.05 * dy);
        pictureSprite.scale(0.9 * dy / pictureSprite.getLocalBounds().height, 0.9 * dy / pictureSprite.getLocalBounds().height);
        
        window.draw(rectangle);
        window.draw(text);
        window.draw(cntText);
        window.draw(pictureSprite);
        minusButton.Draw(window);
        plusButton.Draw(window);
    }
};

void SelectItems(sf::RenderWindow& window, TShop& shop, vector<pair<TItem, uint32_t>>& items, vector<string>& images) {
    vector<pair<TItem, uint32_t>> remainingItems = items;
    vector<ItemTile> itemTiles;
    for (size_t i = 0; i < items.size(); i++) {
        itemTiles.push_back(ItemTile(50.f, 60.f * i + 100.f, 1300.f, 50.f, items[i].first.GetItemName(), items[i].second, images[i]));
    }
    Button finishButton(1200.f, 700.f, 100.f, 50.f, "Finish");

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

                if (finishButton.IsIn(px, py)) {
                    quit = true;
                    break;
                } else {
                    for (size_t i = 0; i < items.size(); i++) {
                        if (itemTiles[i].minusButton.IsIn(px, py)) {
                            if (itemTiles[i].cnt > 0) {
                                itemTiles[i].cnt--;
                                remainingItems[i].second++;
                                shop.DeleteItem(items[i].first.GetItemID());
                            }
                        } else if (itemTiles[i].plusButton.IsIn(px, py)) {
                            if (itemTiles[i].cnt < itemTiles[i].maxcnt) {
                                itemTiles[i].cnt++;
                                remainingItems[i].second--;
                                shop.AddItem(items[i].first.GetItemID());
                            }
                        }
                    }
                }
            }
        }

        window.clear();
        AddTitle(window, "Select Items you want to buy:");
        finishButton.Draw(window);
        for (ItemTile& itemTile : itemTiles) {
            itemTile.Draw(window);
        }
        window.display();
    }
}

struct FilledBoxTile {
    float x;
    float y;
    float dx;
    float dy;
    string boxName;
    vector<Button> itemsButtons;
    sf::Texture pictureTexture;

    FilledBoxTile() = default;

    FilledBoxTile(float curx, float cury, float curdx, float curdy, string curBoxName, vector<TItem> items, const string& img) {
        x = curx;
        y = cury;
        dx = curdx;
        dy = curdy;
        boxName = "Box name: " + curBoxName;
        for (size_t i = 0; i < items.size(); i++) {
            itemsButtons.push_back(Button(x + dx / 7 * i + 100.f,  y + 0.1 * dy, dx / 7, 0.8 * dy, items[i].GetItemName()));
        }

        pictureTexture.loadFromFile(img);
    }

    void Draw(sf::RenderWindow& window) {
        sf::RectangleShape rectangle(sf::Vector2f(dx, dy));
        rectangle.setPosition(x, y);
        rectangle.setFillColor(sf::Color::White);

        sf::Text text;
        text.setFont(font);
        text.setString(boxName);
        text.setCharacterSize(18);
        text.setCharacterSize(min(1.0, dx / text.getLocalBounds().width * 0.9) * 18.0);
        text.setFillColor(sf::Color::White);
        float cx = x + dx * 0.5 - text.getLocalBounds().width * 0.5;
        float cy = y - dy * 0.2 - text.getLocalBounds().height;
        text.setPosition(cx, cy);

        sf::Sprite pictureSprite;
        pictureSprite.setTexture(pictureTexture);
        pictureSprite.setPosition(x + 10.f, y + 0.05 * dy);
        pictureSprite.scale(0.9 * dy / pictureSprite.getLocalBounds().height, 0.9 * dy / pictureSprite.getLocalBounds().height);

        window.draw(rectangle);
        window.draw(pictureSprite);
        window.draw(text);
        for (Button& button : itemsButtons) {
            button.Draw(window);
        }
    }
};

void PrintBoxes(sf::RenderWindow& window, const vector<TFilledBox>& filledBoxes, const vector<TBox>& boxes, const vector<string>& images) {
    if (filledBoxes.size() == 0) {
        Button finishButton(1200.f, 700.f, 100.f, 50.f, "Finish");
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
                    if (finishButton.IsIn(px, py)) {
                        quit = true;
                        break;
                    }
                }
            }

            window.clear();
            AddTitle(window, "Your order can't be packed because one of your items doesn't fit in any of our boxes. We are sorry.");
            finishButton.Draw(window);
            window.display();
        }
    } else {
        Button finishButton(1200.f, 700.f, 100.f, 50.f, "Finish");
        bool quit = false;
        vector<FilledBoxTile> filledBoxTiles;
        for (size_t i = 0; i < filledBoxes.size(); i++) {
            string curImage;
            for (size_t j = 0; j < boxes.size(); j++) {
                if (boxes[j].GetBoxID() == filledBoxes[i].GetBox().GetBoxID()) {
                    curImage = images[j];
                }
            }
            filledBoxTiles.push_back(FilledBoxTile(50.f, 80.f * i + 150, 1300.f, 50.f, filledBoxes[i].GetBox().GetBoxName(), filledBoxes[i].GetItems(), curImage));
        }

        while (window.isOpen() && !quit) {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed) {
                    window.close();
                } else if (event.type == sf::Event::MouseButtonPressed) {
                    float px = event.mouseButton.x;
                    float py = event.mouseButton.y;
                    if (finishButton.IsIn(px, py)) {
                        quit = true;
                        break;
                    }
                }
            }

            window.clear();
            AddTitle(window, "You finished your purchase successfully. Thank you for using our shop. Your order will come to you in the following form:");
            finishButton.Draw(window);
            for (FilledBoxTile& filledBoxTile : filledBoxTiles) {
                filledBoxTile.Draw(window);
            }
            window.display();
        }
    }
}

sf::Font GetFont() {
    sf::Font font;
    font.loadFromFile("arial.ttf");
    return font;
}

void DidntBuyAnything(sf::RenderWindow& window) {
    Button finishButton(1200.f, 700.f, 100.f, 50.f, "Finish");
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                float px = event.mouseButton.x;
                float py = event.mouseButton.y;
                if (finishButton.IsIn(px, py)) {
                    window.close();
                }
            }
        }
        window.clear();
        AddTitle(window, "You didn't buy anything. We hope you'll like something next time!");
        finishButton.Draw(window);
        window.display();
    }
}

int main() {
    font = GetFont();
    sf::RenderWindow window(sf::VideoMode(1400, 800), "Shop");
    TDataBase dataBase("db.sqlite");

    if (ChooseMode(window) == "ADMIN") {
        vector<pair<TItem, uint32_t>> items = GetItems(window);
        vector<TBox> boxes = GetBoxes(window);
        FillSettings(window, items, boxes, dataBase);
    } else {
        auto [items, boxes] = GetSettings(window, dataBase);
        TShop shop(items.first, boxes.first);
        SelectItems(window, shop, items.first, items.second);
        if (shop.OrderIsEmpty()) {
            DidntBuyAnything(window);
        } else {
            vector<TFilledBox> filledBoxes = shop.Buy();
            PrintBoxes(window, filledBoxes, boxes.first, boxes.second);
        }
    }

    dataBase.Close();
    return 0;
}
