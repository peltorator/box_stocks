//#include "shop.h"
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
        cout << "\nВыберите режим работы (" << ADMIN << " или " << USER << ")" << endl;
        string mode;
        while (true) {
            getline(cin, mode);
            if (mode == ADMIN || mode == USER) {
                break;
            }
            cout << "\nНекорректный режим работы. Выберите режим работы из предложенных вариантов: " << ADMIN << " или " << USER << endl;
        }
        if (mode == ADMIN) {
            cout << "\nДля входа в режим администратора необходимо ввести пароль:" << endl;
            string userPassword;
            size_t iter = 0;
            while (true) {
                iter++;
                getline(cin, userPassword);
                if (PASSWORD == userPassword) {
                    break;
                }
                if (iter <= 2) {
                    cout << "\nНеверный пароль. Попробуйте еще раз" << endl;
                } else {
                    cout << "\nПароль снова неверный. Если вы не знаете пароль, попробуйте режим пользователя.";
                    cout << "\nПродолжить вводить пароль (continue) или выбрать режим работы заново (restart)?" << endl;
                    string decision;
                    while (true) {
                        getline(cin, decision);
                        if (decision == "continue" || decision == "restart") {
                            break;
                        }
                        cout << "\nНеверная опция. Продолжить вводить пароль (continue) или выбрать режим работы заново (restart)?" << endl;
                    }
                    if (decision == "continue") {
                        cout << "\nВведите пароль:" << endl;
                        continue;
                    } else {
                        cout << "\nЗавершение режима администратора." << endl;
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
            cout << "\nВы выбрали режим работы " << USER << endl;
            return USER;
        }
    }
}

void HelpGetItems() {
    cout << "\nВведите список товаров на складе в формате";
    cout << "\n\"AddItem <ItemName> <ItemWeight> <ItemVolume> <Quantity>\".";
    cout << "\nВ случае необходимости, вы можете вызвать помощь при помощи команды \"Help\".";
    cout << "\nДля завершения ввода товаров воспользуйтесь командой \"Quit\"." << endl;
}

vector<pair<TItem, uint32_t>> GetItems() {
    HelpGetItems();
    vector<pair<TItem, uint32_t>> ans;
    while (true) {
        string type;
        cin >> type;
        if (type == "AddItem") {
            string itemName;
            uint64_t weight, volume;
            uint32_t quantity;
            cin >> itemName >> weight >> volume >> quantity;
            ans.push_back({TItem(itemName, weight, volume), quantity});
            cout << "\nТовар успешно добавлен на склад." << endl;
        } else if (type == "Help") {
            HelpGetItems();
        } else if (type == "Quit") {
            cout << "\nВвод товаров завершен." << endl;
            break;
        } else {
            cout << "\nНеизвестная команда\n";
            HelpGetItems();
        }
    }
    return ans;
}

void HelpGetBoxes() {
    cout << "\nВведите список коробок на складе в формате";
    cout << "\n\"AddBox <BoxName> <MaxWeight> <MaxVolume> <Cost>\".";
    cout << "\nВ случае необходимости, вы можете вызвать помощь при помощи команды \"Help\".";
    cout << "\nДля завершения ввода коробок воспользуйтесь командой \"Quit\"." << endl;
}


vector<TBox> GetBoxes() {
    HelpGetBoxes();
    vector<TBox> ans;
    while (true) {
        string type;
        cin >> type;
        if (type == "AddBox") {
            string boxName;
            uint64_t maxWeight, maxVolume, cost;
            cin >> boxName >> maxWeight >> maxVolume >> cost;
            ans.push_back(TBox(boxName, maxWeight, maxVolume, cost));
            cout << "\nКоробка успешно добавлена на склад." << endl;
        } else if (type == "Help") {
            HelpGetBoxes();
        } else if (type == "Quit") {
            cout << "\nВвод коробок завершен." << endl;
            break;
        } else {
            cout << "\nНеизвестная команда\n";
            HelpGetBoxes();
        }
    }
    return ans;
}

const string SETTINGS_PATH = "settings.txt";

void FillSettings(const vector<pair<TItem, uint32_t>>& items, const vector<TBox>& boxes) {
    ofstream out(SETTINGS_PATH);
    for (const auto& [item, amount] : items) {
        out << "AddItem\t" << item.GetItemName() << '\t' << item.GetWeight() << '\t' << item.GetVolume() << '\t' << amount << '\n';
    }
    for (const TBox& box : boxes) {
        out << "AddBox\t" << box.GetBoxName() << '\t' << box.GetMaxWeight() << '\t' << box.GetMaxVolume() << '\t' << box.GetCost() << '\n';
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
            string itemName;
            uint64_t weight, volume;
            uint32_t quantity;
            in >> itemName >> weight >> volume >> quantity;
            items.push_back({TItem(itemName, weight, volume), quantity});
        } else if (type == "AddBox") {
            string boxName;
            uint64_t maxWeight, maxVolume, cost;
            in >> boxName >> maxWeight >> maxVolume >> cost;
            boxes.push_back(TBox(boxName, maxWeight, maxVolume, cost));
        }
    }
    return {items, boxes};
}

void HelpSelectItems(vector<pair<TItem, uint32_t>>& remainingItems) {
    cout << "\nВыберите товары, которые вы хотите купить из списка ниже:\n";
    cout << "\nItemID\tItemName\tItemWeight\tItemVolume\tQuantityRemaining\n";
    for (const auto [item, amount] : remainingItems) {
        cout << '\n' << item.GetItemID() << "\t\t" << item.GetItemName() << "\t\t" << item.GetWeight() << "\t\t" << item.GetVolume() << "\t\t" << amount;
    }
    cout << "\n\nЧтобы добавить товар в корзину, введите команду \"AddItem <ItemID>\".";
    cout << "\nЧтобы удалить товар из корзины, введите команду \"DeleteItem <ItemID>\".";
    cout << "\nДля просмотра текущей корзины можно воспользоваться командой \"PrintOrder\".";
    cout << "\nВ случае необходимости, вы можете вызвать помощь при помощи команды \"Help\".";
    cout << "\nДля завершения ввода товаров воспользуйтесь командой \"Quit\"." << endl;

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
                cout << "\nТовара с таким ID не существует\n";
                HelpSelectItems(remainingItems);
                continue;
            }
            if (remainingItems[index].second == 0) {
                cout << "\nЭтот товар уже закончился :(\n";
                HelpSelectItems(remainingItems);
                continue;
            }
            shop.AddItem(itemID);
            remainingItems[index].second--;
            cout << "\nТовар успешно добавлен в корзину" << endl;
        } else if (type == "DeleteItem") {
            uint64_t itemID;
            cin >> itemID;
            size_t index = 0;
            while (index < items.size() && items[index].first.GetItemID() != itemID) {
                index++;
            }
            if (index == items.size()) {
                cout << "\nТовара с таким ID не существует\n";
                HelpSelectItems(remainingItems);
                continue;
            }
            if (remainingItems[index].second == items[index].second) {
                cout << "\nЭтого товара нет в вашей корзине :(\n";
                HelpSelectItems(remainingItems);
                continue;
            }
            shop.DeleteItem(itemID);
            remainingItems[index].second++;
            cout << "\nТовар успешно удален из корзины" << endl;
        } else if (type == "PrintOrder") {
            cout << "\nВаша корзина:";
            cout << "\nItemID\tItemName\tQuantity\n";
            bool orderIsEmpty = true;
            for (size_t index = 0; index < items.size(); index++) {
                if (items[index].second > remainingItems[index].second) {
                    orderIsEmpty = false;
                    cout << '\n' << items[index].first.GetItemID() << "\t\t" << items[index].first.GetItemName() << "\t\t" << items[index].second - remainingItems[index].second;
                }
            }
            if (orderIsEmpty) {
                cout << "\n________________________";
                cout << "\nВаша корзина пуста!";
            }
            cout << endl;
        } else if (type == "Help") {
            HelpSelectItems(remainingItems);
        } else if (type == "Quit") {
            cout << "\nВвод товаров завершен." << endl;
            break;
        } else {
            cout << "\nНеизвестная команда\n";
            HelpSelectItems(remainingItems);
        }
    }
}

void PrintBoxes(const vector<TFilledBox>& filledBoxes) {
    if (filledBoxes.size() == 0) {
        cout << "\n____________________";
        cout << "\nВаш заказ невозможно упаковать по коробкам, потому что один из товаров, который вы заказали, не влезает ни в одну коробку! Мы сожалеем.";
    } else {
        cout << "\n\n\n\n\nВы успешно завершили покупку. Ваш заказ будет доставлен в течение 3 дней. По коробкам он будет разбит следующим образом:\n\n";
        size_t index = 0;
        uint64_t costSum = 0;
        for (const TFilledBox& filledBox : filledBoxes) {
            index++;
            cout << "\nКоробка " << index << " (" << filledBox.GetBox().GetBoxName() << ") стоит " << filledBox.GetBox().GetCost() << " тугриков.";
            costSum += filledBox.GetBox().GetCost();
            cout << "\nВ ней будут находиться следующие товары: ";
            cout << "\nItemID\t\tItemName\n";
            for (const TItem& item : filledBox.GetItems()) {
                cout << item.GetItemID() << "\t\t" << item.GetItemName() << '\n';
            }
            cout << endl;
        }
        cout << "Суммарная стоимость коробок " << costSum << " тугриков.";
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
        if (shop.OrderIsEmpty()) {
            cout << "Вы ничего не купили. Надеемся, что в следующий раз вам что-то приглянется!" << endl;
        } else {
            vector<TFilledBox> filledBoxes = shop.Buy();
            PrintBoxes(filledBoxes);
        }
    }
    return 0;
}
