#include "shop.h"
#include "shop.cpp"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

const string ADMIN = "ADMIN";
const string USER = "USER";
const string PASSWORD = "isaf27";

string ChooseMode() {
    while (true) {
        cout << "Выберите режим работы (" << ADMIN << " или " << USER << ")" << endl;
        string mode;
        while (true) {
            getline(cin, mode);
            if (mode == ADMIN || mode == USER) {
                break;
            }
            cout << "Некорректный режим работы. Выберите режим работы из предложенных вариантов: " << ADMIN << " или " << USER << endl;
        }
        if (mode == ADMIN) {
            cout << "Для входа в режим администратора необходимо ввести пароль:" << endl;
            string userPassword;
            size_t iter = 0;
            while (true) {
                iter++;
                getline(cin, userPassword);
                if (PASSWORD == userPassword) {
                    break;
                }
                if (iter == 1) {
                    cout << "Неверный пароль. Попробуйте еще раз" << endl;
                } else {
                    cout << "Пароль снова неверный. Если вы не знаете пароль, попробуйте режим пользователя.\n";
                    cout << "Продолжить вводить пароль (continue) или выбрать режим работы заново (restart)?" << endl;
                    string decision;
                    while (true) {
                        getline(cin, decision);
                        if (decision == "continue" || decision == "restart") {
                            break;
                        }
                        cout << "Неверная опция. Продолжить вводить пароль (continue) или выбрать режим работы заново (restart)?" << endl;
                    }
                    if (decision == "continue") {
                        continue;
                    } else {
                        break;
                    }
                }
            }
            if (userPassword == PASSWORD) {
                return ADMIN;
            } else {
                continue;
            }
        } else {
            cout << "Вы выбрали режим работы " << USER << endl;
            return USER;
        }
    }
}

void HelpGetItems() {
    cout << "Введите список товаров на складе в формате\n";
    cout << "\"AddItem <ItemID> <ItemWeight> <ItemVolume> <Quantity>\"\n";
    cout << "В случае необходимости, вы можете вызвать помощь при помощи команды \"Help\"\n";
    cout << "Для завершения ввода товаров воспользуйтесь командой \"Quit\"" << endl;
}

vector<pair<TItem, uint32_t>> GetItems() {
    HelpGetItems();
    vector<pair<TItem, uint32_t>> ans;
    while (true) {
        string type;
        cin >> type;
        if (type == "AddItem") {
            uint64_t itemID, weight, volume;
            uint32_t quantity;
            cin >> itemID >> weight >> volume >> quantity;
            ans.push_back({TItem(itemID, weight, volume), quantity});
        } else if (type == "Help") {
            HelpGetItems();
        } else if (type == "Quit") {
            break;
        } else {
            cout << "Неизвестная команда\n\n";
            HelpGetItems();
        }
    }
    return ans;
}

void HelpGetBoxes() {
    cout << "Введите список коробок на складе в формате\n";
    cout << "\"AddBox <BoxID> <MaxWeight> <MaxVolume> <Cost>\"\n";
    cout << "В случае необходимости, вы можете вызвать помощь при помощи команды \"Help\"\n";
    cout << "Для завершения ввода коробок воспользуйтесь командой \"Quit\"" << endl;
}


vector<TBox> GetBoxes() {
    HelpGetBoxes();
    vector<TBox> ans;
    while (true) {
        string type;
        cin >> type;
        if (type == "AddBox") {
            uint64_t boxID, maxWeight, maxVolume, cost;
            cin >> boxID >> maxWeight >> maxVolume >> cost;
            ans.push_back(TBox(boxID, maxWeight, maxVolume, cost));
        } else if (type == "Help") {
            HelpGetBoxes();
        } else if (type == "Quit") {
            break;
        } else {
            cout << "Неизвестная команда\n\n";
            HelpGetBoxes();
        }
    }
    return ans;
}

const string SETTINGS_PATH = "settings.txt";

void FillSettings(const vector<pair<TItem, uint32_t>>& items, const vector<TBox>& boxes) {
    ofstream out(SETTINGS_PATH);
    for (const auto& [item, amount] : items) {
        out << "AddItem\t" << item.GetItemID() << '\t' << item.GetWeight() << '\t' << item.GetVolume() << '\t' << amount << '\n';
    }
    for (const TBox& box : boxes) {
        out << "AddBox\t" << box.GetBoxID() << '\t' << box.GetMaxWeight() << '\t' << box.GetMaxVolume() << '\t' << box.GetCost() << '\n';
    }
    cout << "\n\nНастройки сохранены. Хорошего дня :)" << endl;
}

pair<vector<pair<TItem, uint32_t>>, vector<TBox>> GetSettings() {
    vector<pair<TItem, uint32_t>> items;
    vector<TBox> boxes;
    ifstream in(SETTINGS_PATH);
    while (true) {
        string type;
        if (!(in >> type)) {
            break;
        }
        if (type == "AddItem") {
            uint64_t itemID, weight, volume;
            uint32_t quantity;
            in >> itemID >> weight >> volume >> quantity;
            items.push_back({TItem(itemID, weight, volume), quantity});
        } else if (type == "AddBox") {
            uint64_t boxID, maxWeight, maxVolume, cost;
            in >> boxID >> maxWeight >> maxVolume >> cost;
            boxes.push_back(TBox(boxID, maxWeight, maxVolume, cost));
        }
    }
    return {items, boxes};
}

void HelpSelectItems(vector<pair<TItem, uint32_t>>& remainingItems) {
    cout << "Выберите товары, которые вы хотите купить из списка ниже:\n";
    cout << "ItemID\tItemWeight\tItemVolume\tQuantityRemaining\n";
    for (const auto [item, amount] : remainingItems) {
        cout << item.GetItemID() << "\t\t" << item.GetWeight() << "\t\t" << item.GetVolume() << "\t\t" << amount << '\n';
    }
    cout << "Чтобы добавить товар в корзину, введите команду \"AddItem <ItemID>\"\n";
    cout << "Чтобы удалить товар из корзины, введите команду \"DeleteItem <ItemID>\"\n";
    cout << "В случае необходимости, вы можете вызвать помощь при помощи команды \"Help\"\n";
    cout << "Для завершения ввода товаров воспользуйтесь командой \"Quit\"" << endl;

}

void SelectItems(TShop& shop, vector<pair<TItem, uint32_t>>& items) {
    vector<pair<TItem, uint32_t>> remainingItems = items;
    HelpSelectItems(remainingItems);
    while (true) {
        string type;
        cin >> type;
        if (type == "AddItem") {
            uint64_t itemID;
            cin >> itemID;
            size_t index = 0;
            while (index < items.size() && items[index].first.GetItemID() != itemID) {
                index++;
            }
            if (index == items.size()) {
                cout << "Товара с таким ID не существует\n\n";
                HelpSelectItems(remainingItems);
                continue;
            }
            if (remainingItems[index].second == 0) {
                cout << "Этот товар закончился :(\n\n";
                HelpSelectItems(remainingItems);
                continue;
            }
            shop.AddItem(itemID);
            remainingItems[index].second--;
            cout << "Товар успешно добавлен в корзину" << endl;
        } else if (type == "DeleteItem") {
            uint64_t itemID;
            cin >> itemID;
            size_t index = 0;
            while (index < items.size() && items[index].first.GetItemID() != itemID) {
                index++;
            }
            if (index == items.size()) {
                cout << "Товара с таким ID не существует\n\n";
                HelpSelectItems(remainingItems);
                continue;
            }
            if (remainingItems[index].second == items[index].second) {
                cout << "Этого товара нет в вашей корзине :(\n\n";
                HelpSelectItems(remainingItems);
                continue;
            }
            shop.DeleteItem(itemID);
            remainingItems[index].second++;
            cout << "Товар успешно удален из корзины" << endl;
        } else if (type == "Help") {
            HelpSelectItems(remainingItems);
        } else if (type == "Quit") {
            break;
        } else {
            cout << "Неизвестная команда\n\n";
            HelpSelectItems(remainingItems);
        }
    }
}

void PrintBoxes(const vector<TFilledBox>& filledBoxes) {
    cout << "\n\n\n\n\nВы успешно завершили покупку. Ваш заказ будет доставлен в течение 3 дней. По коробкам он будет разбит следующим образом:\n\n";
    size_t index = 0;
    for (const TFilledBox& filledBox : filledBoxes) {
        index++;
        cout << "Коробка " << index << " стоит " << filledBox.GetBox().GetCost() << " тугриков. ";
        cout << "В ней будут находиться товары со следующими ID: ";
        for (const TItem& item : filledBox.GetItems()) {
            cout << item.GetItemID() << ' ';
        }
        cout << endl;
    }
    cout << "\n\nСпасибо за использование нашего интернет-магазина. Хорошего дня :)" << endl;
}

int main()
{
    if (ChooseMode() == "ADMIN") {
        vector<pair<TItem, uint32_t>> items = GetItems();
        vector<TBox> boxes = GetBoxes();
        FillSettings(items, boxes);
    } else {
        auto [items, boxes] = GetSettings();
        TShop shop(items, boxes);
        SelectItems(shop, items);
        vector<TFilledBox> filledBoxes = shop.Buy();
        PrintBoxes(filledBoxes);
    }
    return 0;
}
