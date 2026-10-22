#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int MAX_ITEMS = 100;
const int MAX_STOCK = 10000;
const int LOW_STOCK_LIMIT = 5;
const double GST_RATE = 18.0;
const int TABLE_WIDTH = 79;
const int BILL_WIDTH = 65;
const string DATA_FILE = "furniture.txt";
const string BILL_FILE = "bills.txt";

class UI {
public:
    static string makeLine(char ch, int width) {
        return "+" + string(width - 2, ch) + "+";
    }

    static string centerText(const string &text, int width) {
        int space = width - 2 - (int)text.length();
        if (space < 0) space = 0;
        int left = space / 2;
        int right = space - left;
        return "|" + string(left, ' ') + text + string(right, ' ') + "|";
    }

    static void printHeading(const string &title) {
        cout << "\n" << makeLine('=', TABLE_WIDTH) << "\n";
        cout << centerText(title, TABLE_WIDTH) << "\n";
        cout << makeLine('=', TABLE_WIDTH) << "\n";
    }

    static void printMenuLine(int number, const string &text) {
        cout << "|   " << number << ".  " << left << setw(70) << text << "|\n";
    }

    static void pauseScreen() {
        string dummy;
        cout << "\n  Press Enter to continue...";
        getline(cin, dummy);
    }

    static string readLine(const string &prompt) {
        string line;
        cout << prompt;
        if (!getline(cin, line)) {
            exit(0);
        }
        return line;
    }

    static bool isNumber(const string &s, bool allowDot) {
        if (s.empty()) return false;
        int dots = 0;
        for (size_t i = 0; i < s.length(); i++) {
            if (s[i] == '.' && allowDot) {
                dots++;
                if (dots > 1) return false;
            } else if (s[i] < '0' || s[i] > '9') {
                return false;
            }
        }
        return s != ".";
    }

    static int readInt(const string &prompt, int low, int high) {
        while (true) {
            string line = readLine(prompt);
            if (!isNumber(line, false) || line.length() > 9) {
                cout << "  [!] Please enter a valid whole number.\n";
                continue;
            }
            int value = atoi(line.c_str());
            if (value < low || value > high) {
                cout << "  [!] Value must be between " << low << " and " << high << ".\n";
                continue;
            }
            return value;
        }
    }

    static double readPrice(const string &prompt) {
        while (true) {
            string line = readLine(prompt);
            if (!isNumber(line, true) || line.length() > 10) {
                cout << "  [!] Please enter a valid price (example: 1500 or 1499.50).\n";
                continue;
            }
            double value = atof(line.c_str());
            if (value <= 0 || value > 9999999.99) {
                cout << "  [!] Price must be more than 0 and less than 10000000.\n";
                continue;
            }
            return value;
        }
    }

    static string readText(const string &prompt, int maxLength) {
        while (true) {
            string text = readLine(prompt);
            if (text.find_first_not_of(' ') == string::npos) {
                cout << "  [!] This field cannot be empty.\n";
            } else if (text.find('|') != string::npos) {
                cout << "  [!] The '|' character is not allowed.\n";
            } else if ((int)text.length() > maxLength) {
                cout << "  [!] Maximum " << maxLength << " characters allowed.\n";
            } else {
                return text;
            }
        }
    }

    static bool readYesNo(const string &prompt) {
        while (true) {
            string answer = readLine(prompt);
            if (answer == "y" || answer == "Y") return true;
            if (answer == "n" || answer == "N") return false;
            cout << "  [!] Please type y or n.\n";
        }
    }

    static void printTableHeader() {
        cout << makeLine('-', TABLE_WIDTH) << "\n";
        cout << "| " << left << setw(5) << "ID"
             << " | " << setw(20) << "Name"
             << " | " << setw(12) << "Category"
             << " | " << right << setw(10) << "Price (Rs)"
             << " | " << setw(5) << "Qty"
             << " | " << left << setw(8) << "Status" << " |\n";
        cout << makeLine('-', TABLE_WIDTH) << "\n";
    }
};

class Item {
protected:
    int id;
    string name;
    double price;

public:
    Item(int id = 0, const string &name = "", double price = 0.0)
        : id(id), name(name), price(price) {}

    virtual ~Item() {}

    int getId() const { return id; }
    string getName() const { return name; }
    double getPrice() const { return price; }

    void setId(int newId) { id = newId; }
    void setName(const string &newName) { name = newName; }
    void setPrice(double newPrice) { price = newPrice; }

    bool operator==(int targetId) const {
        return id == targetId;
    }

    bool operator==(const Item &other) const {
        return id == other.id;
    }

    virtual void display(ostream &out) const = 0;
};

class Furniture : public Item {
private:
    string category;
    int quantity;

public:
    Furniture(int id = 0, const string &name = "", const string &category = "", double price = 0.0, int quantity = 0)
        : Item(id, name, price), category(category), quantity(quantity) {}

    string getCategory() const { return category; }
    int getQuantity() const { return quantity; }

    void setCategory(const string &newCategory) { category = newCategory; }
    void setQuantity(int newQty) { quantity = newQty; }

    string getStatus() const {
        if (quantity == 0) return "OUT";
        if (quantity <= LOW_STOCK_LIMIT) return "LOW";
        return "OK";
    }

    Furniture& operator+=(int qty) {
        this->quantity += qty;
        return *this;
    }

    Furniture& operator-=(int qty) {
        this->quantity -= qty;
        return *this;
    }

    void display(ostream &out) const override {
        out << "| " << left << setw(5) << id
            << " | " << setw(20) << name
            << " | " << setw(12) << category
            << " | " << right << setw(10) << fixed << setprecision(2) << price
            << " | " << setw(5) << quantity
            << " | " << left << setw(8) << getStatus() << " |\n";
    }

    friend ostream& operator<<(ostream &out, const Furniture &f) {
        f.display(out);
        return out;
    }

    friend istream& operator>>(istream &in, Furniture &f) {
        string idStr, priceStr, qtyStr;
        if (getline(in, idStr, '|')) {
            getline(in, f.name, '|');
            getline(in, f.category, '|');
            getline(in, priceStr, '|');
            getline(in, qtyStr);
            f.id = atoi(idStr.c_str());
            f.price = atof(priceStr.c_str());
            f.quantity = atoi(qtyStr.c_str());
        }
        return in;
    }

    void writeToStream(ostream &out) const {
        out << id << "|" << name << "|" << category << "|"
            << fixed << setprecision(2) << price << "|" << quantity << "\n";
    }
};

class CartItem {
private:
    Furniture item;
    int quantity;

public:
    CartItem(const Furniture &f, int q) : item(f), quantity(q) {}

    const Furniture& getItem() const { return item; }
    int getQuantity() const { return quantity; }
    void addQuantity(int q) { quantity += q; }

    double getLineTotal() const {
        return item.getPrice() * quantity;
    }
};

class Bill {
private:
    string customerName;
    string timestamp;
    vector<CartItem> cartItems;

public:
    Bill(const string &customer = "") : customerName(customer) {
        time_t now = time(0);
        string when = ctime(&now);
        if (!when.empty() && when[when.length() - 1] == '\n') {
            when.erase(when.length() - 1);
        }
        timestamp = when;
    }

    void addItem(const Furniture &f, int qty) {
        for (size_t i = 0; i < cartItems.size(); i++) {
            if (cartItems[i].getItem().getId() == f.getId()) {
                cartItems[i].addQuantity(qty);
                return;
            }
        }
        cartItems.push_back(CartItem(f, qty));
    }

    size_t getCartCount() const { return cartItems.size(); }
    bool isEmpty() const { return cartItems.empty(); }

    int getQuantityForId(int id) const {
        for (size_t i = 0; i < cartItems.size(); i++) {
            if (cartItems[i].getItem().getId() == id) {
                return cartItems[i].getQuantity();
            }
        }
        return 0;
    }

    const vector<CartItem>& getCartItems() const { return cartItems; }

    double calculateSubtotal() const {
        double subtotal = 0.0;
        for (size_t i = 0; i < cartItems.size(); i++) {
            subtotal += cartItems[i].getLineTotal();
        }
        return subtotal;
    }

    double calculateGST() const {
        return calculateSubtotal() * GST_RATE / 100.0;
    }

    double calculateGrandTotal() const {
        return calculateSubtotal() + calculateGST();
    }

    friend ostream& operator<<(ostream &out, const Bill &bill) {
        out << fixed << setprecision(2);
        out << UI::makeLine('=', BILL_WIDTH) << "\n";
        out << UI::centerText("FURNITURE SHOP INVOICE", BILL_WIDTH) << "\n";
        out << UI::makeLine('=', BILL_WIDTH) << "\n";
        out << "| Date & Time: " << left << setw(BILL_WIDTH - 17) << bill.timestamp << " |\n";
        out << "| Customer   : " << left << setw(BILL_WIDTH - 17) << bill.customerName << " |\n";
        out << UI::makeLine('-', BILL_WIDTH) << "\n";
        out << "| " << left << setw(3) << "No."
            << " | " << setw(20) << "Item Name"
            << " | " << right << setw(5) << "Qty"
            << " | " << setw(10) << "Price(Rs)"
            << " | " << setw(11) << "Total(Rs)" << " |\n";
        out << UI::makeLine('-', BILL_WIDTH) << "\n";

        for (size_t i = 0; i < bill.cartItems.size(); i++) {
            const CartItem &ci = bill.cartItems[i];
            out << "| " << left << setw(3) << (i + 1)
                << " | " << setw(20) << ci.getItem().getName()
                << " | " << right << setw(5) << ci.getQuantity()
                << " | " << setw(10) << ci.getItem().getPrice()
                << " | " << setw(11) << ci.getLineTotal() << " |\n";
        }

        double subtotal = bill.calculateSubtotal();
        double gst = bill.calculateGST();
        double grandTotal = bill.calculateGrandTotal();

        ostringstream gstLabel;
        gstLabel << "GST (" << (int)GST_RATE << "%) (Rs.)";

        out << UI::makeLine('-', BILL_WIDTH) << "\n";
        out << "| " << left << setw(BILL_WIDTH - 18) << "Subtotal (Rs.)"
            << right << setw(14) << subtotal << " |\n";
        out << "| " << left << setw(BILL_WIDTH - 18) << gstLabel.str()
            << right << setw(14) << gst << " |\n";
        out << UI::makeLine('-', BILL_WIDTH) << "\n";
        out << "| " << left << setw(BILL_WIDTH - 18) << "GRAND TOTAL (Rs.)"
            << right << setw(14) << grandTotal << " |\n";
        out << UI::makeLine('=', BILL_WIDTH) << "\n";
        out << UI::centerText("Thank you! Visit again.", BILL_WIDTH) << "\n";
        out << UI::makeLine('=', BILL_WIDTH) << "\n";

        return out;
    }
};

class FurnitureShop {
private:
    vector<Furniture> inventory;
    string dataFilePath;
    string billFilePath;

public:
    FurnitureShop(const string &dataFile = DATA_FILE, const string &billFile = BILL_FILE)
        : dataFilePath(dataFile), billFilePath(billFile) {
        loadFromFile();
    }

    ~FurnitureShop() {
        saveToFile();
    }

    size_t size() const { return inventory.size(); }

    Furniture& operator[](size_t index) {
        return inventory[index];
    }

    const Furniture& operator[](size_t index) const {
        return inventory[index];
    }

    int findIndexById(int id) const {
        for (size_t i = 0; i < inventory.size(); i++) {
            if (inventory[i] == id) {
                return (int)i;
            }
        }
        return -1;
    }

    void saveToFile() const {
        ofstream file(dataFilePath.c_str());
        if (!file) {
            cout << "  [!] Error: could not open file for saving.\n";
            return;
        }
        for (size_t i = 0; i < inventory.size(); i++) {
            inventory[i].writeToStream(file);
        }
        file.close();
    }

    void loadFromFile() {
        ifstream file(dataFilePath.c_str());
        if (!file) return;

        inventory.clear();
        Furniture f;
        while (file >> f) {
            if (inventory.size() < MAX_ITEMS) {
                inventory.push_back(f);
            }
        }
        file.close();
    }

    void displayAll() const {
        UI::printTableHeader();
        for (size_t i = 0; i < inventory.size(); i++) {
            cout << inventory[i];
        }
        cout << UI::makeLine('-', TABLE_WIDTH) << "\n";
    }

    void addFurniture() {
        UI::printHeading("ADD NEW FURNITURE ITEM");

        if (inventory.size() >= MAX_ITEMS) {
            cout << "  [!] Shop is full. Cannot add more than " << MAX_ITEMS << " items.\n";
            UI::pauseScreen();
            return;
        }

        int id = UI::readInt("  Enter Furniture ID (1-9999)   : ", 1, 9999);
        if (findIndexById(id) != -1) {
            cout << "  [!] ID " << id << " already exists. Duplicate IDs are not allowed.\n";
            UI::pauseScreen();
            return;
        }

        string name = UI::readText("  Enter Name (max 20 letters)   : ", 20);
        string category = UI::readText("  Enter Category (max 12 letters): ", 12);
        double price = UI::readPrice("  Enter Price (Rs)              : ");
        int quantity = UI::readInt("  Enter Quantity in stock       : ", 0, MAX_STOCK);

        inventory.push_back(Furniture(id, name, category, price, quantity));
        saveToFile();

        cout << "\n  [OK] Furniture added and saved successfully!\n";
        UI::pauseScreen();
    }

    void displayFurniture() const {
        UI::printHeading("ALL FURNITURE ITEMS");
        if (inventory.empty()) {
            cout << "  No furniture records found. Add some first.\n";
        } else {
            displayAll();
            cout << "  Total records: " << inventory.size() << "\n";
        }
        UI::pauseScreen();
    }

    void searchFurniture() const {
        UI::printHeading("SEARCH FURNITURE BY ID");
        if (inventory.empty()) {
            cout << "  No furniture records found.\n";
            UI::pauseScreen();
            return;
        }

        int id = UI::readInt("  Enter Furniture ID to search: ", 1, 9999);
        int index = findIndexById(id);

        if (index == -1) {
            cout << "  [!] No furniture found with ID " << id << ".\n";
        } else {
            cout << "\n  Record found:\n";
            UI::printTableHeader();
            cout << inventory[index];
            cout << UI::makeLine('-', TABLE_WIDTH) << "\n";
        }
        UI::pauseScreen();
    }

    void updateFurniture() {
        UI::printHeading("UPDATE FURNITURE DETAILS");
        if (inventory.empty()) {
            cout << "  No furniture records found.\n";
            UI::pauseScreen();
            return;
        }

        int id = UI::readInt("  Enter Furniture ID to update: ", 1, 9999);
        int index = findIndexById(id);

        if (index == -1) {
            cout << "  [!] No furniture found with ID " << id << ".\n";
            UI::pauseScreen();
            return;
        }

        cout << "\n  Current details:\n";
        UI::printTableHeader();
        cout << inventory[index];
        cout << UI::makeLine('-', TABLE_WIDTH) << "\n";

        cout << "\n  What do you want to update?\n";
        cout << "    1. Name\n    2. Category\n    3. Price\n    4. All details\n";
        int choice = UI::readInt("  Enter choice (1-4): ", 1, 4);

        switch (choice) {
            case 1:
                inventory[index].setName(UI::readText("  New Name     : ", 20));
                break;
            case 2:
                inventory[index].setCategory(UI::readText("  New Category : ", 12));
                break;
            case 3:
                inventory[index].setPrice(UI::readPrice("  New Price    : "));
                break;
            case 4:
                inventory[index].setName(UI::readText("  New Name     : ", 20));
                inventory[index].setCategory(UI::readText("  New Category : ", 12));
                inventory[index].setPrice(UI::readPrice("  New Price    : "));
                inventory[index].setQuantity(UI::readInt("  New Quantity : ", 0, MAX_STOCK));
                break;
        }

        saveToFile();
        cout << "\n  [OK] Record updated and saved successfully!\n";
        UI::pauseScreen();
    }

    void deleteFurniture() {
        UI::printHeading("DELETE FURNITURE RECORD");
        if (inventory.empty()) {
            cout << "  No furniture records found.\n";
            UI::pauseScreen();
            return;
        }

        int id = UI::readInt("  Enter Furniture ID to delete: ", 1, 9999);
        int index = findIndexById(id);

        if (index == -1) {
            cout << "  [!] No furniture found with ID " << id << ".\n";
            UI::pauseScreen();
            return;
        }

        UI::printTableHeader();
        cout << inventory[index];
        cout << UI::makeLine('-', TABLE_WIDTH) << "\n";

        if (UI::readYesNo("\n  Are you sure you want to delete this record? (y/n): ")) {
            inventory.erase(inventory.begin() + index);
            saveToFile();
            cout << "  [OK] Record deleted and file updated.\n";
        } else {
            cout << "  Delete cancelled.\n";
        }
        UI::pauseScreen();
    }

    void manageStock() {
        UI::printHeading("MANAGE STOCK QUANTITY");
        if (inventory.empty()) {
            cout << "  No furniture records found.\n";
            UI::pauseScreen();
            return;
        }

        int id = UI::readInt("  Enter Furniture ID: ", 1, 9999);
        int index = findIndexById(id);

        if (index == -1) {
            cout << "  [!] No furniture found with ID " << id << ".\n";
            UI::pauseScreen();
            return;
        }

        cout << "\n  Current stock:\n";
        UI::printTableHeader();
        cout << inventory[index];
        cout << UI::makeLine('-', TABLE_WIDTH) << "\n";

        cout << "\n    1. Add stock (new delivery)\n";
        cout << "    2. Remove stock (damaged / returned)\n";
        cout << "    3. Set exact quantity\n";
        int choice = UI::readInt("  Enter choice (1-3): ", 1, 3);

        switch (choice) {
            case 1: {
                int amount = UI::readInt("  Quantity to add: ", 1, MAX_STOCK);
                if (inventory[index].getQuantity() + amount > MAX_STOCK) {
                    cout << "  [!] Stock cannot go above " << MAX_STOCK << ". No change made.\n";
                    UI::pauseScreen();
                    return;
                }
                inventory[index] += amount;
                break;
            }
            case 2: {
                if (inventory[index].getQuantity() == 0) {
                    cout << "  [!] Stock is already 0. Nothing to remove.\n";
                    UI::pauseScreen();
                    return;
                }
                int amount = UI::readInt("  Quantity to remove: ", 1, inventory[index].getQuantity());
                inventory[index] -= amount;
                break;
            }
            case 3: {
                int amount = UI::readInt("  New quantity: ", 0, MAX_STOCK);
                inventory[index].setQuantity(amount);
                break;
            }
        }

        saveToFile();
        cout << "\n  [OK] Stock updated. New quantity of " << inventory[index].getName()
             << " = " << inventory[index].getQuantity() << "\n";

        if (inventory[index].getQuantity() <= LOW_STOCK_LIMIT) {
            cout << "  [ALERT] This item is now " << inventory[index].getStatus()
                 << " on stock. Please reorder soon!\n";
        }
        UI::pauseScreen();
    }

    void generateBill() {
        UI::printHeading("GENERATE CUSTOMER BILL");
        if (inventory.empty()) {
            cout << "  No furniture available to sell.\n";
            UI::pauseScreen();
            return;
        }

        string customer = UI::readText("  Enter customer name (max 30 letters): ", 30);
        Bill bill(customer);

        cout << "\n  Available furniture:\n";
        displayAll();

        while (true) {
            int id = UI::readInt("\n  Enter Furniture ID to buy (0 = finish): ", 0, 9999);
            if (id == 0) break;

            int index = findIndexById(id);
            if (index == -1) {
                cout << "  [!] No furniture found with ID " << id << ".\n";
                continue;
            }

            int inCart = bill.getQuantityForId(id);
            int available = inventory[index].getQuantity() - inCart;

            if (available <= 0) {
                cout << "  [!] " << inventory[index].getName()
                     << " is out of stock (or all units already added).\n";
                continue;
            }

            cout << "  Selected: " << inventory[index].getName()
                 << " | Price: Rs. " << inventory[index].getPrice()
                 << " | Available: " << available << "\n";

            int qty = UI::readInt("  Enter quantity: ", 1, available);
            bill.addItem(inventory[index], qty);
            cout << "  [+] Added to bill.\n";
        }

        if (bill.isEmpty()) {
            cout << "\n  No items selected. Bill cancelled.\n";
            UI::pauseScreen();
            return;
        }

        cout << "\n" << bill;

        if (UI::readYesNo("\n  Confirm sale and update stock? (y/n): ")) {
            const vector<CartItem> &cartItems = bill.getCartItems();
            for (size_t i = 0; i < cartItems.size(); i++) {
                int itemIndex = findIndexById(cartItems[i].getItem().getId());
                if (itemIndex != -1) {
                    inventory[itemIndex] -= cartItems[i].getQuantity();
                }
            }
            saveToFile();

            ofstream billFile(billFilePath.c_str(), ios::app);
            if (billFile) {
                billFile << bill << "\n";
                billFile.close();
            }

            cout << "  [OK] Sale completed. Stock updated, data and bill saved.\n";
        } else {
            cout << "  Sale cancelled. Stock was not changed.\n";
        }
        UI::pauseScreen();
    }

    void showStockReport() const {
        UI::printHeading("STOCK REPORT AND LOW-STOCK ALERTS");
        if (inventory.empty()) {
            cout << "  No furniture records found.\n";
            UI::pauseScreen();
            return;
        }

        int totalUnits = 0;
        int lowCount = 0;
        double totalValue = 0.0;

        for (size_t i = 0; i < inventory.size(); i++) {
            totalUnits += inventory[i].getQuantity();
            totalValue += inventory[i].getPrice() * inventory[i].getQuantity();
            if (inventory[i].getQuantity() <= LOW_STOCK_LIMIT) {
                lowCount++;
            }
        }

        displayAll();
        cout << "  Total different items : " << inventory.size() << "\n";
        cout << "  Total units in stock  : " << totalUnits << "\n";
        cout << "  Total stock value     : Rs. " << fixed << setprecision(2) << totalValue << "\n";
        cout << "\n" << UI::makeLine('*', TABLE_WIDTH) << "\n";

        if (lowCount == 0) {
            cout << UI::centerText("All items have enough stock.", TABLE_WIDTH) << "\n";
        } else {
            ostringstream alertTitle;
            alertTitle << "LOW STOCK ALERTS (quantity <= " << LOW_STOCK_LIMIT << ")";
            cout << UI::centerText(alertTitle.str(), TABLE_WIDTH) << "\n";
            cout << UI::makeLine('*', TABLE_WIDTH) << "\n";

            for (size_t i = 0; i < inventory.size(); i++) {
                if (inventory[i].getQuantity() == 0) {
                    cout << "  [OUT OF STOCK] ID " << inventory[i].getId()
                         << " - " << inventory[i].getName() << "\n";
                } else if (inventory[i].getQuantity() <= LOW_STOCK_LIMIT) {
                    cout << "  [LOW STOCK]    ID " << inventory[i].getId()
                         << " - " << inventory[i].getName()
                         << " (only " << inventory[i].getQuantity() << " left)\n";
                }
            }
        }

        cout << UI::makeLine('*', TABLE_WIDTH) << "\n";
        UI::pauseScreen();
    }

    void showWelcome() const {
        cout << "\n" << UI::makeLine('=', TABLE_WIDTH) << "\n";
        cout << UI::centerText("", TABLE_WIDTH) << "\n";
        cout << UI::centerText("FURNITURE SHOP MANAGEMENT SYSTEM", TABLE_WIDTH) << "\n";
        cout << UI::centerText("Object Oriented Programming - C++", TABLE_WIDTH) << "\n";
        cout << UI::centerText("", TABLE_WIDTH) << "\n";
        cout << UI::makeLine('=', TABLE_WIDTH) << "\n";
        cout << "  " << inventory.size() << " record(s) loaded from " << dataFilePath << "\n";
    }

    void showMenu() const {
        UI::printHeading("MAIN MENU");
        UI::printMenuLine(1, "Add Furniture Item");
        UI::printMenuLine(2, "Display All Furniture");
        UI::printMenuLine(3, "Search Furniture by ID");
        UI::printMenuLine(4, "Update Furniture Details");
        UI::printMenuLine(5, "Delete Furniture Record");
        UI::printMenuLine(6, "Manage Stock Quantity");
        UI::printMenuLine(7, "Generate Customer Bill");
        UI::printMenuLine(8, "Stock Report and Low-Stock Alerts");
        UI::printMenuLine(9, "Save Data Now");
        UI::printMenuLine(0, "Save and Exit");
        cout << UI::makeLine('=', TABLE_WIDTH) << "\n";
    }

    void run() {
        cout << fixed << setprecision(2);
        showWelcome();

        int choice = -1;
        do {
            showMenu();
            choice = UI::readInt("  Enter your choice (0-9): ", 0, 9);

            switch (choice) {
                case 1: addFurniture();     break;
                case 2: displayFurniture(); break;
                case 3: searchFurniture();  break;
                case 4: updateFurniture();  break;
                case 5: deleteFurniture();  break;
                case 6: manageStock();      break;
                case 7: generateBill();     break;
                case 8: showStockReport();  break;
                case 9:
                    saveToFile();
                    cout << "\n  [OK] All records saved to " << dataFilePath << "\n";
                    UI::pauseScreen();
                    break;
                case 0:
                    saveToFile();
                    cout << "\n" << UI::makeLine('=', TABLE_WIDTH) << "\n";
                    cout << UI::centerText("Data saved safely. Thank you! Goodbye.", TABLE_WIDTH) << "\n";
                    cout << UI::makeLine('=', TABLE_WIDTH) << "\n";
                    break;
            }
        } while (choice != 0);
    }
};

int main() {
    FurnitureShop shop;
    shop.run();
    return 0;
}
