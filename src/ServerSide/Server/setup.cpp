#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#include "../DataBase/database.cpp"
#include "../../Helper/helper_functions.cpp"

using namespace std;

void CreateTables() {
    string queryDropAndCreateTables = "drop table if exists Item;\n"
        "create table Item (itemID integer primary key, itemName text, weight integer, volume integer, cost integer, amount integer, image text);\n"
        "drop table if exists Box;\n"
        "create table Box (boxID integer primary key, boxName text, maxWeight integer, maxVolume integer, cost integer, available integer, image text);\n"
        "create table if not exists Users (userID integer primary key, userName text);\n"
        "create table if not exists Orders (orderID integer primary key, userID integer, orderDate text, foreign key(userID) references Users(userID));\n"
        "create table if not exists FilledBox (filledBoxID integer primary key, boxID integer, orderID integer, foreign key(boxID) references Box(boxID), foreign key(orderID) references Orders(orderID));\n"
        "create table if not exists ItemsForFilledBox (itemsForFilledBoxID integer primary key, itemID integer, filledBoxID integer, foreign key(itemID) references Items(itemID), foreign key(filledBoxID) references Orders(filledBoxID));\n";
    
    NDataBase::Query(queryDropAndCreateTables);
}

void AddElems() {
    string queryInsertItems = "insert into Item(itemName, weight, volume, cost, amount, image) values";
    string queryInsertBoxes = "insert into Box(boxName, maxWeight, maxVolume, cost, available, image) values";

    ifstream in("setup.txt");
    uint32_t curItemID = 0;
    uint32_t curBoxID = 0;
    while (true) {
        string type;
        if (!(in >> type)) {
            break;
        }
        if (type == "AddItem") {
            string itemName;
            uint64_t weight, volume, cost;
            uint32_t quantity;
            string imagePath;
            in >> itemName >> weight >> volume >> cost >> quantity >> imagePath;
            
            queryInsertItems += " ('" + itemName + "', " + to_string(weight) + ", " + to_string(volume) + ", " + to_string(cost) + ", " + to_string(quantity) + ", '" + GetImageBytes("../../../assets/images/" + imagePath) + "'),";
        } else if (type == "AddBox") {
            string boxName;
            uint64_t maxWeight, maxVolume, cost;
            string imagePath;
            in >> boxName >> maxWeight >> maxVolume >> cost >> imagePath;

            queryInsertBoxes += " ('" + boxName + "', " + to_string(maxWeight) + ", " + to_string(maxVolume) + ", " + to_string(cost) + ", 1, '" + GetImageBytes("../../../assets/images/" + imagePath) + "'),";
        }
    }

    queryInsertItems.back() = ';';
    queryInsertBoxes.back() = ';';

    NDataBase::Query(queryInsertItems);
    NDataBase::Query(queryInsertBoxes);
}

void CustomSetup() {
    while (true) {
        cout << "\nif you want to create new item print \"AddItem <itemName> <weight> <volume> <cost> <imagePath>\"\n";
        cout << "if you want to create new box print \"AddBox <boxName> <maxWeight> <maxVolume> <cost> <imagePath>\"\n";
        cout << "if you want to finish print \"quit\"" << endl;
        string type;
        cin >> type;
        if (type == "AddItem") {
            string itemName;
            uint64_t weight, volume, cost;
            uint32_t quantity;
            string imagePath;
            cin >> itemName >> weight >> volume >> cost >> imagePath;
            
            string queryInsertItem =  "insert into Item(itemName, weight, volume, cost, amount, image) values ('" + itemName + "', " + to_string(weight) + ", " + to_string(volume) + ", " + to_string(cost) + ", 0, '" + GetImageBytes(imagePath) + "');";
             NDataBase::Query(queryInsertItem);
             cout << "\nItem added successfully" << endl;
        } else if (type == "AddBox") {
            string boxName;
            uint64_t maxWeight, maxVolume, cost;
            string imagePath;
            cin >> boxName >> maxWeight >> maxVolume >> cost >> imagePath;

            string queryInsertBox = "insert into Box(boxName, maxWeight, maxVolume, cost, available, image) values ('" + boxName + "', " + to_string(maxWeight) + ", " + to_string(maxVolume) + ", " + to_string(cost) + ", 1, '" + GetImageBytes(imagePath) + "'),";
            NDataBase::Query(queryInsertBox);
            cout << "\nBox added successfully" << endl;
        } else if (type == "quit") {
            break;
        } else {
            cout << "\nunknown type. try again." << endl;
        }
    }
}

int main() {
    NDataBase::Open("db.sqlite");

    while (true) {
        cout << "\nwhat setup do you want? \"default\" or \"custom\"?" << endl;
        string response;
        cin >> response;
        if (response == "default") {
            CreateTables();
            AddElems();
            break;
        } else if (response == "custom") {
            CustomSetup();
            break;
        } else if (response == "quit") {
            break;
        } else {
            cout << "\nunknown mode. try again. if you want to quit print \"quit\"." << endl;
        }
    }
    cout << "done!" << endl;
    NDataBase::Close();
    return 0;
}
