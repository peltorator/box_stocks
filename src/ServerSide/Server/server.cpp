#include <sstream>
#include <utility>

#include "../ShopSession/shop_session.cpp"
#include "../DataBase/database.cpp"
#include "../DataBase/database_queries.cpp"
#include "../../Helper/helper_functions.cpp"


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

    TShopSession* MakeShop() {
        return new TShopSession;
    }

    void DeleteShop(TShopSession* shop) {
        delete shop;
    }

    void AddItemToShop(TShopSession* shop, const uint64_t itemID) {
        shop->AddItem(itemID);
    }

    void DeleteItemFromShop(TShopSession* shop, const uint64_t itemID) {
        shop->DeleteItem(itemID);
    }
    
    const char* BuyOrderToString(TShopSession* shop) {
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

    int32_t ShopOrderIsEmpty(TShopSession* shop) {
        return shop->OrderIsEmpty();
    }

    void DBUpdateItem(const uint64_t itemID, const int32_t amount) {
        UpdateItem(itemID, amount);
    }

    void DBUpdateBox(const uint64_t boxID, const int32_t amount) {
        UpdateBox(boxID, amount);
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
