#pragma once

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include "item.cpp"
#include "box.cpp"
#include "filled_box.cpp"
#include "database.cpp"
#include "helper_functions.cpp"

std::string GetImageFromDB(const std::string &s) {
    std::string bytes;
    bytes.reserve(s.size() >> 3);
    for (size_t i = 0; i < s.size(); i += 8) {
        char c = 0;
        for (int j = i; j < i + 8; j++) {
            c = (c << 1) | (s[j] - '0');
        }
        bytes.push_back(c);
    }
    return bytes;
}

std::string GetImageBytes(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    std::string bytes((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::string ans;
    ans.reserve(bytes.size() << 3);
    for (size_t i = 0; i < bytes.size(); i++) {
        for (int j = 7; j >= 0; j--) {
            ans.push_back(((bytes[i] >> j) & 1) + '0');
        }
    }
    return ans;
}

void UpdateItems(const std::vector<std::pair<TItem, int32_t>>& items) {
    for (const auto& [item, amount] : items) {
        std::string updateQuery = "update Item set amount = amount + " + std::to_string(amount) + " where itemID = " + std::to_string(item.ItemID) + ";";
        NDataBase::Query(updateQuery);
    }
}

void UpdateBoxes(const std::vector<std::pair<TBox, int32_t>>& boxes) {
    for (const auto& [box, amount] : boxes) {
        std::string updateQuery = "update Box set available = available + " + std::to_string(amount) + " where boxID = " + std::to_string(box.BoxID) + ";";
        NDataBase::Query(updateQuery);
    }
}

void InsertItem(const TItem& item, const std::string& imagePath) {
    std::string insertQuery = "insert into Item(itemName, weight, volume, amount, image) values ('" + item.ItemName + "', " + std::to_string(item.Weight) + ", " + std::to_string(item.Volume) + ", 0, '" + GetImageBytes(imagePath) + "');";
    NDataBase::Query(insertQuery);
}

void InsertBox(const TBox& box, const std::string& imagePath) {
    std::string insertQuery = "insert into Box(boxName, maxWeight, maxVolume, cost, image) values ('" + box.BoxName + "', " + std::to_string(box.MaxWeight) + ", " + std::to_string(box.MaxVolume) + ", " + std::to_string(box.Cost) + ", 1, '" + GetImageBytes(imagePath) + "');";
    NDataBase::Query(insertQuery);
}

std::vector<std::pair<TItem, uint32_t>> GetItems() {
    std::string getItemsQuery = "select * from Item;";
    std::vector<std::pair<TItem, uint32_t>> items;
    auto itemsRaw = NDataBase::Query(getItemsQuery);
    for (auto& dict : itemsRaw) {
        items.push_back({TItem(ToInt(dict["itemID"]), dict["itemName"], ToInt(dict["weight"]), ToInt(dict["volume"]), GetImageFromDB(dict["image"])), ToInt(dict["amount"])});
    }
    return items;
}

std::vector<TItem> GetItemsList() {
    std::string getItemsQuery = "select * from Item;";
    std::vector<TItem> items;
    auto itemsRaw = NDataBase::Query(getItemsQuery);
    for (auto& dict : itemsRaw) {
        items.push_back(TItem(ToInt(dict["itemID"]), dict["itemName"], ToInt(dict["weight"]), ToInt(dict["volume"]), GetImageFromDB(dict["image"])));
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
        boxes.push_back({TBox(ToInt(dict["boxID"]), dict["boxName"], ToInt(dict["maxWeight"]), ToInt(dict["maxVolume"]), ToInt(dict["cost"]), GetImageFromDB(dict["image"])), ToInt(dict["available"])});
    }
    return boxes;
}

std::vector<TBox> GetAvailableBoxes() {
    
    std::string getBoxesQuery = "select * from Box;";
    std::vector<TBox> boxes;
    auto boxesRaw = NDataBase::Query(getBoxesQuery);
    for (auto& dict : boxesRaw) {
        if (ToInt(dict["available"])) {
            boxes.push_back(TBox(ToInt(dict["boxID"]), dict["boxName"], ToInt(dict["maxWeight"]), ToInt(dict["maxVolume"]), ToInt(dict["cost"]), GetImageFromDB(dict["image"])));
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

bool CheckIfBoxExists(const std::string& boxName) {
    std::string selectQuery = "select boxName from Box where boxName = '" + boxName + "';";
    auto selectResponse = NDataBase::Query(selectQuery);
    return !selectResponse.empty();

}

void SaveOrder(const std::vector<TFilledBox>& filledBoxes) {
    const std::string insertOrderQuery = "insert into Orders(userID, orderDate) values (1, '" + currentDate() + "');";
    NDataBase::Query(insertOrderQuery);
    int64_t orderID = NDataBase::GetLastInsertID();
    
    for (const TFilledBox& filledBox : filledBoxes) {
        const std::string insertFilledBoxQuery = "insert into FilledBox(boxID, orderID) values (" + std::to_string(filledBox.Box.BoxID) + ", " + std::to_string(orderID) + ");";
        NDataBase::Query(insertFilledBoxQuery);
        int64_t filledBoxID = NDataBase::GetLastInsertID();

        for (const TItem& item : filledBox.Items) {
            const std::string insertItemsForFilledBoxQuery = "insert into ItemsForFilledBox(itemID, filledBoxID) values (" + std::to_string(item.ItemID) + ", " + std::to_string(filledBoxID) + ");";
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
        filledBoxes[ToInt(row["filledBoxID"])].first.Items.push_back(items[ToInt(row["itemID"])]);
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
