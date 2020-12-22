#pragma once

#include <string>
#include <vector>

#include "filled_box.cpp"

struct TOrder {
    uint64_t OrderID;
    uint64_t UserID;
    std::string UserName;
    std::string OrderDate;
    std::vector<TFilledBox> FilledBoxes;

    TOrder() = default;

    TOrder(const uint64_t orderID, const uint64_t userID, const std::string& userName, const std::string& orderDate, const std::vector<TFilledBox>& filledBoxes = {})
        : OrderID(orderID)
        , UserID(userID)
        , UserName(userName)
        , OrderDate(orderDate)
        , FilledBoxes(filledBoxes) {}
};
