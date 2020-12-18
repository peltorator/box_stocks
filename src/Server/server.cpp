#include "shop.cpp"
#include "../DataBase/database.cpp"
#include "../DataBase/database_queries.cpp"
#include <iostream>
#include <sstream>

std::string FilledBoxToJson(const TFilledBox& filledBox) {
    std::stringstream s;
    s << filledBox.BoxID << "\n";
    s << filledBox.ItemIDs.size() << "\n";
    for (const uint64_t& itemID : filledBox.ItemIDs) {
        s << itemID << "\n";
    }
    return s.str();
}

std::string ItemToJson(const TItem& item, const uint32_t amount) {
    std::stringstream s;
    s << item.ItemID << " " << item.ItemName << " " << item.Weight << " " << item.Volume << " " << item.Cost << " " << item.Image << " " << amount;
    return s.str();
}

std::string BoxToJson(const TBox& box, const uint32_t amount) {
    std::stringstream s;
    s << box.BoxID << " " << box.BoxName << " " << box.MaxWeight << " " << box.MaxVolume << " " << box.Cost << " " << box.Image << " " << amount;
    return s.str();
}

std::vector<std::pair<uint64_t, std::vector<uint64_t>>> JsonToOrder(const std::string& s) {
    std::vector<std::pair<uint64_t, std::vector<uint64_t>>> order;
    // TODO
    return order;
}

std::string OrderToJson(const TOrder& order) {
    std::stringstream s;
    s << order.OrderID << " " << order.UserID << " " << order.UserName << " " << order.OrderDate << "\n";
    s << order.FilledBoxes.size() << "\n";
    for (const TFilledBox& filledBox : order.FilledBoxes) {
        s << FilledBoxToJson(filledBox) << "\n";
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
    
    const char* BuyOrderToJson(TShop* shop) {
        std::vector<TFilledBox> order = (*shop).Buy();
        std::stringstream s;
        s << order.size() << "\n";
        for (size_t i = 0; i < order.size(); i++) {
            s << FilledBoxToJson(order[i]) << "\n";
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
            s << ItemToJson(items[i].first, items[i].second) << "\n";
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
            s << BoxToJson(boxes[i].first, boxes[i].second) << "\n";
        }
        char* chars = new char[s.str().size()];
        strcpy(chars, s.str().c_str());
        return chars;
    }
 
    void DBSaveOrder(const char* s) {
        SaveOrder(JsonToOrder(s));
    }

    const char* DBGetOrders() {
        std::vector<TOrder> orders = GetOrders();
        std::stringstream s;
        s << orders.size() << "\n";
        for (size_t i = 0; i < orders.size(); i++) {
            s << OrderToJson(orders[i]) << "\n";
        }
        char* chars = new char[s.str().size()];
        strcpy(chars, s.str().c_str());
        return chars;
    }
}
