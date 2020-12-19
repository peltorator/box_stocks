#include "../Shop/shop.cpp"
#include "../DataBase/database.cpp"
#include "../DataBase/database_queries.cpp"
#include "../../Helper/helper_functions.cpp"
#include <iostream>
#include <sstream>

std::string FilledBoxToString(const TFilledBox& filledBox) {
    std::stringstream s;
    s << filledBox.BoxID << "\n";
    s << filledBox.ItemIDs.size() << "\n";
    for (const uint64_t& itemID : filledBox.ItemIDs) {
        s << itemID << "\n";
    }
    return s.str();
}

std::string ItemToString(const TItem& item, const uint32_t amount) {
    std::stringstream s;
    s << item.ItemID << " " << item.ItemName << " " << item.Weight << " " << item.Volume << " " << item.Cost << " " << item.Image << " " << amount;
    return s.str();
}

std::string BoxToString(const TBox& box, const uint32_t amount) {
    std::stringstream s;
    s << box.BoxID << " " << box.BoxName << " " << box.MaxWeight << " " << box.MaxVolume << " " << box.Cost << " " << box.Image << " " << amount;
    return s.str();
}

std::vector<uint64_t> ParsePath(const std::string& s) {
    std::vector<uint64_t> vals;
    std::string curStr = "";
    size_t i = 1;
    while (s[i] != '/') {
        i++;
    }
    for (; i < s.size(); i++) {
        if (s[i] == '/') {
            if (!curStr.empty()) {
                vals.push_back(ToInt(curStr));
                curStr = "";
            }
        } else {
            curStr.push_back(s[i]);
        }
    }
    return vals;
}

std::vector<std::pair<uint64_t, std::vector<uint64_t>>> StringToOrder(std::string s) {
    std::cout << "StringToOrderAA" << std::endl;
    std::vector<uint64_t> vals = ParsePath(s + "/");
    for (uint64_t i : vals) {
        std::cout << "a " << i << std::endl;
    }
    std::vector<std::pair<uint64_t, std::vector<uint64_t>>> order;
    int ind = 0;
    size_t orderSize = vals[ind++];
    order.reserve(orderSize);
    for (size_t i = 0; i < orderSize; i++) {
        uint64_t boxID = vals[ind++];
        std::vector<uint64_t> itemIDs;
        size_t itemsSize = vals[ind++];
        itemIDs.reserve(itemsSize);
        for (size_t j = 0; j < itemsSize; j++) {
            itemIDs.push_back(vals[ind++]);
        }
        order.emplace_back(boxID, itemIDs);
    }
    return order;
}

std::string OrderToString(const TOrder& order) {
    std::stringstream s;
    s << order.OrderID << " " << order.UserID << " " << order.UserName << " " << order.OrderDate << "\n";
    s << order.FilledBoxes.size() << "\n";
    for (const TFilledBox& filledBox : order.FilledBoxes) {
        s << FilledBoxToString(filledBox);
    }
    return s.str();
}

extern "C" {
    void OpenDataBase() {
        NDataBase::Open("db.sqlite");
    }

    TShop* MakeShop() {
        return new TShop;
    }

    void DeleteShop(TShop* shop) {
        delete shop;
    }

    void AddItemToShop(TShop* shop, const uint64_t itemID) {
        shop->AddItem(itemID);
    }

    void DeleteItemFromShop(TShop* shop, const uint64_t itemID) {
        shop->DeleteItem(itemID);
    }
    
    const char* BuyOrderToString(TShop* shop) {
        std::vector<TFilledBox> order = (*shop).Buy();
        std::stringstream s;
        s << order.size() << "\n";
        for (size_t i = 0; i < order.size(); i++) {
            s << FilledBoxToString(order[i]);
        }
        char* chars = new char[s.str().size()];
        strcpy(chars, s.str().c_str());
        return chars;
    }

    int32_t ShopOrderIsEmpty(TShop* shop) {
        return shop->OrderIsEmpty();
    }

    void DBUpdateItem(const uint64_t itemID, const int32_t amount) {
        UpdateItem(itemID, amount);
    }

    void DBUpdateBox(const uint64_t boxID, const int32_t amount) {
        UpdateBox(boxID, amount);
    }

    void DBInsertItem(const char* itemName, const uint64_t weight, const uint64_t volume, const uint64_t cost, const char* image) {
        InsertItem(std::string(itemName), weight, volume, cost, std::string(image));
    }

    void DBInsertBox(const char* boxName, const uint64_t maxWeight, const uint64_t maxVolume, const uint64_t cost, const char* image) {
        InsertBox(std::string(boxName), maxWeight, maxVolume, cost, std::string(image));
    }

    const char* DBGetItems() {
        std::vector<std::pair<TItem, uint32_t>> items = GetItems();
        std::stringstream s;
        s << items.size() << "\n";
        for (size_t i = 0; i < items.size(); i++) {
            s << ItemToString(items[i].first, items[i].second) << "\n";
        }
        char* chars = new char[s.str().size()];
        strcpy(chars, s.str().c_str());
        return chars;
    }

    const char* DBGetBoxes() {
        std::vector<std::pair<TBox, uint32_t>> boxes = GetBoxes();
        std::stringstream s;
        s << boxes.size() << "\n";
        for (size_t i = 0; i < boxes.size(); i++) {
            s << BoxToString(boxes[i].first, boxes[i].second) << "\n";
        }
        char* chars = new char[s.str().size()];
        strcpy(chars, s.str().c_str());
        return chars;
    }
 
    void DBSaveOrder(const char* s) {
        std::cout << "DBSaveOrder" << std::endl;
        SaveOrder(StringToOrder(s));
    }

    const char* DBGetOrders() {
        std::vector<TOrder> orders = GetOrders();
        std::stringstream s;
        s << orders.size() << "\n";
        for (size_t i = 0; i < orders.size(); i++) {
            s << OrderToString(orders[i]) << "\n";
        }
        char* chars = new char[s.str().size()];
        strcpy(chars, s.str().c_str());
        return chars;
    }

    
}
