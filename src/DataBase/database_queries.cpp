#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include "../ShopModel/item.cpp"
#include "../ShopModel/box.cpp"
#include "../ShopModel/order.cpp"
#include "../ShopModel/filled_box.cpp"
#include "database.cpp"
#include "../Helper/helper_functions.cpp"

void UpdateItem(const uint64_t itemID, const int32_t amount) {
    std::string updateQuery = "update Item set amount = amount + " + std::to_string(amount) + " where itemID = " + std::to_string(itemID) + ";";
    NDataBase::Query(updateQuery);
}

void UpdateBox(const uint64_t boxID, const int32_t amount) {
    std::string updateQuery = "update Box set available = available + " + std::to_string(amount) + " where boxID = " + std::to_string(boxID) + ";";
    NDataBase::Query(updateQuery);
}

void InsertItem(const std::string itemName, const uint64_t weight, const uint64_t volume, const uint64_t cost, const std::string& image) {
    std::string insertQuery = "insert into Item(itemName, weight, volume, cost, amount, image) values ('" + itemName + "', " + std::to_string(weight) + ", " + std::to_string(volume) + ", " + std::to_string(cost) + ", 0, '" + image + "');";
    NDataBase::Query(insertQuery);
}

void InsertBox(const std::string boxName, const uint64_t maxWeight, const uint64_t maxVolume, const uint64_t cost, const std::string& image) {
    std::string insertQuery = "insert into Box(boxName, maxWeight, maxVolume, cost, available, image) values ('" + boxName + "', " + std::to_string(maxWeight) + ", " + std::to_string(maxVolume) + ", " + std::to_string(cost) + ", 1, '" + image + "');";
    NDataBase::Query(insertQuery);
}

std::vector<std::pair<TItem, uint32_t>> GetItems() {
    std::string getItemsQuery = "select * from Item;";
    std::vector<std::pair<TItem, uint32_t>> items;
    auto itemsRaw = NDataBase::Query(getItemsQuery);
    for (auto& dict : itemsRaw) {
        items.push_back({TItem(ToInt(dict["itemID"]), dict["itemName"], ToInt(dict["weight"]), ToInt(dict["volume"]), ToInt(dict["cost"]), dict["image"]), ToInt(dict["amount"])});
    }
    return items;
}

std::vector<TItem> GetItemsList() {
    std::string getItemsQuery = "select * from Item;";
    std::vector<TItem> items;
    auto itemsRaw = NDataBase::Query(getItemsQuery);
    for (auto& dict : itemsRaw) {
        items.push_back(TItem(ToInt(dict["itemID"]), dict["itemName"], ToInt(dict["weight"]), ToInt(dict["volume"]), ToInt(dict["cost"]), dict["image"]));
    }
    return items;
}

std::map<uint64_t, TItem> GetItemsMap() {
    std::map<uint64_t, TItem> itemsMap;
    std::vector<TItem> items = GetItemsList();
    for (const TItem& item : items) {
        itemsMap[item.ItemID] = item;
    }
    return itemsMap;
}

std::vector<std::pair<TBox, uint32_t>> GetBoxes() {
    
    std::string getBoxesQuery = "select * from Box;";
    std::vector<std::pair<TBox, uint32_t>> boxes;
    auto boxesRaw = NDataBase::Query(getBoxesQuery);
    for (auto& dict : boxesRaw) {
        boxes.push_back({TBox(ToInt(dict["boxID"]), dict["boxName"], ToInt(dict["maxWeight"]), ToInt(dict["maxVolume"]), ToInt(dict["cost"]), dict["image"]), ToInt(dict["available"])});
    }
    return boxes;
}

std::vector<TBox> GetAvailableBoxes() {
    
    std::string getBoxesQuery = "select * from Box;";
    std::vector<TBox> boxes;
    auto boxesRaw = NDataBase::Query(getBoxesQuery);
    for (auto& dict : boxesRaw) {
        if (ToInt(dict["available"])) {
            boxes.push_back(TBox(ToInt(dict["boxID"]), dict["boxName"], ToInt(dict["maxWeight"]), ToInt(dict["maxVolume"]), ToInt(dict["cost"]), dict["image"]));
        }
    }
    return boxes;
}

std::map<uint64_t, TBox> GetBoxesMap() {
    std::map<uint64_t, TBox> boxesMap;
    const auto& boxes = GetBoxes();
    for (const auto& [box, available] : boxes) {
        boxesMap[box.BoxID] = box;
    }
    return boxesMap;
}

void SaveOrder(const std::vector<std::pair<uint64_t, std::vector<uint64_t>>>& filledBoxes) {
    std::cout << "SaveOrder" << std::endl;
    const std::string insertOrderQuery = "insert into Orders(userID, orderDate) values (1, '" + CurrentDate() + "');";
    NDataBase::Query(insertOrderQuery);
    int64_t orderID = NDataBase::GetLastInsertID();
    
    for (const auto& [boxID, itemIDs] : filledBoxes) {
        const std::string insertFilledBoxQuery = "insert into FilledBox(boxID, orderID) values (" + std::to_string(boxID) + ", " + std::to_string(orderID) + ");";
        NDataBase::Query(insertFilledBoxQuery);
        int64_t filledBoxID = NDataBase::GetLastInsertID();

        for (const uint64_t& itemID : itemIDs) {
            const std::string insertItemsForFilledBoxQuery = "insert into ItemsForFilledBox(itemID, filledBoxID) values (" + std::to_string(itemID) + ", " + std::to_string(filledBoxID) + ");";
            NDataBase::Query(insertItemsForFilledBoxQuery);
        }
    }
}

std::map<uint64_t, std::string> GetUserNamesMap() {
    const std::string selectUsersQuery = "select * from Users;";
    auto usersVector = NDataBase::Query(selectUsersQuery);
    std::map<uint64_t, std::string> userNames;
    for (auto& row : usersVector) {
        userNames[ToInt(row["userID"])] = row["userName"];
    }
    return userNames;
}

std::map<uint64_t, TOrder> GetOrdersMap() {
    const std::string selectOrdersQuery = "select * from Orders;";
    auto ordersVector = NDataBase::Query(selectOrdersQuery);
    std::map<uint64_t, TOrder> orders;
    std::map<uint64_t, std::string> userNames = GetUserNamesMap();
    for (auto& row : ordersVector) {
        const uint64_t orderID = ToInt(row["orderID"]);
        const uint64_t userID = ToInt(row["userID"]);
        orders[orderID] = TOrder(orderID, userID, userNames[userID], row["orderDate"]);
    }
    return orders;
}

std::map<uint64_t, std::pair<TFilledBox, uint64_t>> GetFilledBoxesWithOrderID() {
    const std::string selectFilledBoxesQuery = "select * from FilledBox;";
    auto FilledBoxesVector = NDataBase::Query(selectFilledBoxesQuery);
    std::map<uint64_t, std::pair<TFilledBox, uint64_t>> filledBoxesWithOrderID;
    std::map<uint64_t, TBox> boxes = GetBoxesMap(); 
    for (auto& row : FilledBoxesVector) {
        filledBoxesWithOrderID[ToInt(row["filledBoxID"])] = {TFilledBox(boxes[ToInt(row["boxID"])]), ToInt(row["orderID"])};
    }
    return filledBoxesWithOrderID;
}

std::vector<TOrder> GetOrders() {
    std::map<uint64_t, TOrder> orders = GetOrdersMap();
    std::map<uint64_t, TItem> items = GetItemsMap();
    
    std::map<uint64_t, std::pair<TFilledBox, uint64_t>> filledBoxes = GetFilledBoxesWithOrderID();

    
    const std::string selectItemsForFilledBoxQuery = "select * from ItemsForFilledBox;";
    auto ItemsForFilledBoxVector = NDataBase::Query(selectItemsForFilledBoxQuery);
    for (auto& row : ItemsForFilledBoxVector) {
        filledBoxes[ToInt(row["filledBoxID"])].first.ItemIDs.push_back(ToInt(row["itemID"]));
    }

    for (const auto& [id, filledBox] : filledBoxes) {
        orders[filledBox.second].FilledBoxes.push_back(filledBox.first);
    }

    std::vector<TOrder> ans;
    ans.reserve(orders.size());
    for (const auto& order : orders) {
        ans.push_back(order.second);
    }
    return ans;
}
