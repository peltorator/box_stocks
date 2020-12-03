#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "database.cpp"

using namespace std;

string GetImage(string filename) {
    ifstream file(filename, ios::binary);
    string s((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    string ans = "";
    for (size_t i = 0; i < s.size(); i++) {
        for (int j = 7; j >= 0; j--) {
            ans.push_back(((s[i] >> j) & 1) + '0');
        }
    }
    return ans;
}

void CreateTables(TDataBase dataBase) {
    string queryDropAndCreateTables = "drop table if exists Item;\n"
        "create table if not exists Item (itemID integer primary key, itemName text, weight integer, volume integer, amount integer, image text);"
        "drop table if exists Box;"
        "create table if not exists Box (boxID integer primary key, boxName text, maxWeight integer, maxVolume integer, cost integer, available integer, image text);";
    
    dataBase.Query(queryDropAndCreateTables);
}

void AddElems(TDataBase dataBase) {
    string queryInsertItems = "insert into Item(itemName, weight, volume, amount, image) values";
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
            uint64_t weight, volume;
            uint32_t quantity;
            string imagePath;
            in >> itemName >> weight >> volume >> quantity >> imagePath;
            
            queryInsertItems += " ('" /*+ to_string(curItemID++) + ", '" */+ itemName + "', " + to_string(weight) + ", " + to_string(volume) + ", " + to_string(quantity) + ", '" + GetImage("../images/" + imagePath) + "'),";
        } else if (type == "AddBox") {
            string boxName;
            uint64_t maxWeight, maxVolume, cost;
            string imagePath;
            in >> boxName >> maxWeight >> maxVolume >> cost >> imagePath;

            queryInsertBoxes += " ('" /*+ to_string(curBoxID++) + ", '" */+ boxName + "', " + to_string(maxWeight) + ", " + to_string(maxVolume) + ", " + to_string(cost) + ", 1, '" + GetImage("../images/" + imagePath) + "'),";
        }
    }

    queryInsertItems.back() = ';';
    queryInsertBoxes.back() = ';';

    dataBase.Query(queryInsertItems);
    dataBase.Query(queryInsertBoxes);
}

int main() {
    TDataBase dataBase("db.sqlite");

    CreateTables(dataBase);
    AddElems(dataBase);
    dataBase.Close();
    return 0;
}
