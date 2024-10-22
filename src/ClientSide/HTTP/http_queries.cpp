#pragma once

#include <vector>
#include <iostream>
#include <sstream>
#include <utility>

#include "../../../libs/easylogging/easylogging++.cc"

#include "../../Model/filled_box.cpp"
#include "../../Model/box.cpp"
#include "../../Model/item.cpp"
#include "../../Model/order.cpp"
#include "../../../libs/cpp-httplib/httplib.h"
#include "../../Helper/helper_functions.cpp"

namespace NHttp {

    httplib::Client cli("http://localhost:8080");

    std::string DecodeImage(const std::string &s) {
        std::string bytes;
        const size_t BYTE_SZ = 8;
        bytes.reserve(s.size() / BYTE_SZ);
        for (size_t i = 0; i < s.size(); i += BYTE_SZ) {
            char c = 0;
            for (size_t j = i; j < i + BYTE_SZ; j++) {
                c = (c << 1) | (s[j] - '0');
            }
            bytes.push_back(c);
        }
        return bytes;
    }

    std::vector<std::pair<TBox, uint32_t>> GetBoxes() {
        auto res = cli.Get("/get_boxes");
        if (res == nullptr) {
            LOG(ERROR) << "Can't load boxes from the server";
            return {};
        }
        std::stringstream body(res->body);
        std::vector<std::pair<TBox, uint32_t>> boxes;
        int boxesSize;
        body >> boxesSize;
        boxes.reserve(boxesSize);
        for (int i = 0; i < boxesSize; i++) {
            int64_t id, weight, volume, cost, amount;
            std::string name, image;
            body >> id >> name >> weight >> volume >> cost >> image >> amount;
            boxes.emplace_back(TBox(id, name, weight, volume, cost, DecodeImage(image)), amount);
        }
        return boxes;
    }

    std::vector<TBox> GetAvailableBoxes() {
        std::vector<std::pair<TBox, uint32_t>> allBoxes = GetBoxes();
        std::vector<TBox> availableBoxes;
        for (const auto& [box, amount] : allBoxes) {
            if (amount > 0) {
                availableBoxes.emplace_back(box);
            }
        }
        return availableBoxes;
    }

    std::vector<std::pair<TItem, uint32_t>> GetItems() {
        auto res = cli.Get("/get_items");
        if (res == nullptr) {
            LOG(ERROR) << "Can't load items from the server";
            return {};
        }
        std::stringstream body(res->body);
        std::vector<std::pair<TItem, uint32_t>> items;
        int itemsSize;
        body >> itemsSize;
        items.reserve(itemsSize);
        for (int i = 0; i < itemsSize; i++) {
            int64_t id, weight, volume, cost, amount;
            std::string name, image;
            body >> id >> name >> weight >> volume >> cost >> image >> amount;
            items.emplace_back(TItem(id, name, weight, volume, cost, DecodeImage(image)), amount);
        }
        return items;
    }

    void AddItem(const uint64_t itemID) {
        auto res = cli.Get(("/add_item/" + std::to_string(itemID)).c_str());
        if (res == nullptr) {
            LOG(ERROR) << "Can't add an item to the server";
            return;
        }
    }

    void DeleteItem(const uint64_t itemID) {
        auto res = cli.Get(("/delete_item/" + std::to_string(itemID)).c_str());
        if (res == nullptr) {
            LOG(ERROR) << "Can't delete an item from the server";
            return;
        }
    }

    bool OrderIsEmpty() {
        auto res = cli.Get("/order_is_empty");
        if (res == nullptr) {
            LOG(ERROR) << "Can't load 'order_is_empty' info from the server";
            return true;
        }
        std::stringstream body(res->body);
        int ans;
        body >> ans;
        return static_cast<bool>(ans);
    }

    std::vector<TFilledBox> ShopBuy() {
        auto res = cli.Get("/buy");
        if (res == nullptr) {
            LOG(ERROR) << "Can't buy from the server";
            return {};
        }
        std::stringstream body(res->body);
        std::vector<TFilledBox> order;
        int orderSize;
        body >> orderSize;
        order.reserve(orderSize);
        for (int i = 0; i < orderSize; i++) {
            int64_t boxID;
            body >> boxID;
            int itemsSize;
            body >> itemsSize;
            std::vector<uint64_t> itemIDs;
            itemIDs.reserve(itemsSize);
            for (int j = 0; j < itemsSize; j++) {
                uint64_t itemID;
                body >> itemID;
                itemIDs.emplace_back(itemID);
            }
            order.emplace_back(boxID, itemIDs);
        }
        return order;
    }

    void UpdateItems(const std::vector<std::pair<uint64_t, int32_t>>& items) {
        for (const auto& [itemID, amount] : items) {
            if (amount != 0) {
                auto res = cli.Get(("/update_item/" + std::to_string(itemID) + "/" + std::to_string(amount)).c_str());
                if (res == nullptr) {
                    LOG(ERROR) << "Can't update an item on the server";
                }
            }
        }
    }

    void UpdateBoxes(const std::vector<std::pair<uint64_t, int32_t>>& boxes) {
        for (const auto& [boxID, amount] : boxes) {
            if (amount != 0) {
                auto res = cli.Get(("/update_box/" + std::to_string(boxID) + "/" + std::to_string(amount)).c_str());
                if (res == nullptr) {
                    LOG(ERROR) << "Can't update boxes on the server";
                    return;
                }
            }
        }
    }

    std::vector<TOrder> GetOrders() {
        auto res = cli.Get("/get_orders");
        if (res == nullptr) {
            LOG(ERROR) << "Can't get orders from the server";
            return {};
        }
        std::stringstream body(res->body);
        std::vector<TOrder> orders;
        int ordersSize;
        body >> ordersSize;
        orders.reserve(ordersSize);
        for (int i = 0; i < ordersSize; i++) {
            int64_t orderID, userID;
            std::string userName, orderDate;
            body >> orderID >> userID >> userName >> orderDate;
            std::vector<TFilledBox> filledBoxes;
            int filledBoxesSize;
            body >> filledBoxesSize;
            for (int j = 0; j < filledBoxesSize; j++) {
                uint64_t boxID;
                body >> boxID;
                int itemsSize;
                body >> itemsSize;
                std::vector<uint64_t> itemIDs;
                itemIDs.reserve(itemsSize);
                for (int k = 0; k < itemsSize; k++) {
                    uint64_t itemID;
                    body >> itemID;
                    itemIDs.emplace_back(itemID);
                }
                filledBoxes.emplace_back(boxID, itemIDs);
            }
            orders.emplace_back(orderID, userID, userName, orderDate, filledBoxes);
        }
        return orders;
    }
}
