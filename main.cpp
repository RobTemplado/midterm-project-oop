#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include <iomanip>

using namespace std;

string toLower(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

string toUpper(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

// Function to check if a string consists only of digits
bool is_digit(const string& str) {
    for (char c : str) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

// Encapsulation
class InventoryItem {
private:
    string id;
    string name;
    int quantity;
    double price;
    string category;

public:
    InventoryItem(string itemID, string itemName, int itemQuantity, double itemPrice, string itemCategory)
        : id(toUpper(itemID)), name(itemName), quantity(itemQuantity), price(itemPrice), category(toLower(itemCategory)) {}

    string getID() const {
        return id;
    }

    string getName() const {
        return name;
    }

    int getQuantity() const {
        return quantity;
    }

    double getPrice() const {
        return price;
    }

    string getCategory() const {
        return category;
    }

    void updateQuantity(int newQuantity) {
        quantity = newQuantity;
    }

    void updatePrice(double newPrice) {
        price = newPrice;
    }

    // Display the item details
    void displayItem() const {
        cout << setw(10) << id << setw(20) << name << setw(10) << quantity << setw(10) << fixed << setprecision(2) << price << setw(20) << category << endl;
    }
};

class Storage {
protected:
    vector<InventoryItem> items = {};
    const vector<string> validCategories = {"clothing", "electronics", "entertainment"};

public:
    Storage() = default;

    bool isEmpty() const {
        return items.empty();
    }

    InventoryItem* findItemByID(const string& id) {
        string upperID = toUpper(id);
        for (auto& item : items) {
            if (item.getID() == upperID) {
                return &item;
            }
        }
        return nullptr;
    }

    virtual void addItem() = 0;
    virtual void updateItem() = 0;
    virtual void removeItem() = 0;
    virtual void displayItemsByCategory() = 0;
    virtual void displayAllItems() = 0;
    virtual void displayLowStockItems() = 0;
    virtual void searchItem() = 0;
    virtual void sortItems() = 0;
};

class Inventory : public Storage {
public:
    bool isValidCategory(const string& category) const {
        return find(validCategories.begin(), validCategories.end(), toLower(category)) != validCategories.end();
    }

    void addItem() override {
        string id, name, category;

        // Ask for a valid category
        while (true) {
            cout << "Enter Category (Clothing, Electronics, Entertainment): ";
            cin >> category;
            if (isValidCategory(category)) break;
            cout << "Category " << category << " does not exist!" << endl;
        }

        cout << "Enter Item ID: ";
        cin >> id;
        cout << "Enter Item Name: ";
        cin.ignore();
        getline(cin, name);

        // Validate quantity input
        int quantity;
        cout << "Enter Quantity: ";
        while (true) {
            string quantityStr;
            cin >> quantityStr;
            if (is_digit(quantityStr)) {
                quantity = stoi(quantityStr);
                if (quantity >= 0) break;
            }
            cout << "Invalid input. Please enter a non-negative numeric value for Quantity: ";
        }

        // Validate price input
        double price;
        cout << "Enter Price: ";
        while (true) {
            string priceStr;
            cin >> priceStr;
            if (is_digit(priceStr)) {
                price = stod(priceStr);
                if (price >= 0) break;
            }
            cout << "Invalid input. Please enter a non-negative numeric value for Price: ";
        }

        items.emplace_back(id, name, quantity, price, category);
        cout << "Item added successfully!" << endl;
    }

    void updateItem() override {
        if (isEmpty()) {
            cout << "No items." << endl;
            return;
        }
        string id;
        cout << "Enter Item ID to update: ";
        cin >> id;

        InventoryItem* item = findItemByID(id);
        if (!item) {
            cout << "Item not found!" << endl;
            return;
        }

        int choice;
        cout << "What would you like to update?\n1. Quantity\n2. Price\nEnter your choice: ";
        cin >> choice;

        if (choice == 1) {
            int newQuantity;
            cout << "Enter new quantity: ";
            while (true) {
                string quantityStr;
                cin >> quantityStr;
                if (is_digit(quantityStr)) {
                    newQuantity = stoi(quantityStr);
                    if (newQuantity >= 0) break;
                }
                cout << "Invalid input. Please enter a non-negative numeric value for Quantity: ";
            }
            cout << "Quantity of item " << item->getName() << " updated from " << item->getQuantity() << " to " << newQuantity << endl;
            item->updateQuantity(newQuantity);
        } else if (choice == 2) {
            double newPrice;
            cout << "Enter new price: ";
            while (true) {
                string priceStr;
                cin >> priceStr;
                if (is_digit(priceStr)) {
                    newPrice = stod(priceStr);
                    if (newPrice >= 0) break;
                }
                cout << "Invalid input. Please enter a non-negative numeric value for Price: ";
            }
            cout << "Price of item " << item->getName() << " updated from " << item->getPrice() << " to " << newPrice << endl;
            item->updatePrice(newPrice);
        } else {
            cout << "Invalid choice!" << endl;
        }
    }

    void removeItem() override {
        if (isEmpty()) {
            cout << "No items." << endl;
            return;
        }
        string id;
        cout << "Enter Item ID to remove: ";
        cin >> id;

        InventoryItem* item = findItemByID(id);
        if (item) {
            cout << "Item " << item->getName() << " has been removed from the inventory." << endl;
            items.erase(remove_if(items.begin(), items.end(), [&](const InventoryItem& i) { return i.getID() == item->getID(); }), items.end());
        } else {
            cout << "Item not found!" << endl;
        }
    }

    void displayItemsByCategory() override {
        string category;
        cout << "Enter Category (Clothing, Electronics, Entertainment): ";
        cin >> category;

        if (!isValidCategory(category)) {
            cout << "Category " << category << " does not exist!" << endl;
            return;
        }

        cout << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Quantity" << setw(10) << "Price" << setw(20) << "Category" << endl;
        for (const auto& item : items) {
            if (item.getCategory() == toLower(category)) { // Compare in lowercase
                item.displayItem();
            }
        }
    }

    void displayAllItems() override {
        if (isEmpty()) {
            cout << "Empty items" << endl;
            return;
        }

        cout << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Quantity" << setw(10) << "Price" << setw(20) << "Category" << endl;
        for (const auto& item : items) {
            item.displayItem();
        }
    }

    void searchItem() override {
        if (isEmpty()) {  // Check if the inventory is empty
            cout << "There is no item to search." << endl;
            return;
        }

        string id;
        cout << "Enter Item ID to search: ";
        cin >> id;

        InventoryItem* item = findItemByID(id);
        if (item) {
            cout << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Quantity" << setw(10) << "Price" << setw(20) << "Category" << endl;
            item->displayItem();
        } else {
            cout << "Item not found!" << endl;
        }
    }

    void displayLowStockItems() override {
        cout << "Low stock items:" << endl;
        cout << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Quantity" << setw(10) << "Price" << setw(20) << "Category" << endl;
        for (const auto& item : items) {
            if (item.getQuantity() < 5) {
                item.displayItem();
            }
        }
    }

    void sortItems() override {
        int order;
        cout << "Sort by price:\n1. Ascending\n2. Descending\nEnter your choice: ";
        cin >> order;

        if (order == 1) {
            sort(items.begin(), items.end(), [](const InventoryItem& a, const InventoryItem& b) {
                return a.getPrice() < b.getPrice();
            });
            cout << "Items sorted by price in ascending order." << endl;
        } else if (order == 2) {
            sort(items.begin(), items.end(), [](const InventoryItem& a, const InventoryItem& b) {
                return a.getPrice() > b.getPrice();
            });
            cout << "Items sorted by price in descending order." << endl;
        } else {
            cout << "Invalid choice! Sorting will not be performed." << endl;
        }
    }
};

// Main function to interact with the inventory
int main() {
    Inventory inventory;
    int choice;

    do {
        cout << "Inventory Management System" << endl;
        cout << "1. Add Item" << endl;
        cout << "2. Update Item" << endl;
        cout << "3. Remove Item" << endl;
        cout << "4. Display Items by Category" << endl;
        cout << "5. Display All Items" << endl;
        cout << "6. Search Item" << endl;
        cout << "7. Display Low Stock Items" << endl;
        cout << "8. Sort Items" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                inventory.addItem();
                break;
            case 2:
                inventory.updateItem();
                break;
            case 3:
                inventory.removeItem();
                break;
            case 4:
                inventory.displayItemsByCategory();
                break;
            case 5:
                inventory.displayAllItems();
                break;
            case 6:
                inventory.searchItem();
                break;
            case 7:
                inventory.displayLowStockItems();
                break;
            case 8:
                inventory.sortItems();
                break;
            case 9:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }

        cout << endl;

    } while (choice != 9);

    return 0;
}