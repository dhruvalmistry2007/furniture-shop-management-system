# Furniture Shop Management System Using C++

A console-based **Furniture Shop Management System** written in **C++**. It helps a shop owner manage furniture inventory, update stock, generate customer bills with GST, and view stock reports with low-stock alerts. All data is stored permanently in text files, so nothing is lost when the program closes.

> OOP Mini Project – C++ (single file, no external libraries)

![C++](https://img.shields.io/badge/C%2B%2B-11-blue) ![License](https://img.shields.io/badge/License-MIT-green)

---

## Features

| # | Feature | Description |
|---|---------|-------------|
| 1 | **Add Furniture** | Add a new item with ID, name, category, price and quantity |
| 2 | **Display All** | Show all items in a neat formatted table |
| 3 | **Search by ID** | Find a furniture item quickly using its ID |
| 4 | **Update Details** | Edit name, category, price, or all details together |
| 5 | **Delete Record** | Remove an item (with confirmation) |
| 6 | **Manage Stock** | Add stock, remove stock, or set an exact quantity |
| 7 | **Generate Bill** | Create a customer bill with 18% GST; stock reduces automatically after sale |
| 8 | **Stock Report** | Total items, total units, total stock value and low-stock alerts |
| 9 | **Save Data** | Manually save all records |
| 0 | **Save and Exit** | Saves everything and exits safely |

### Other highlights
- **Persistent storage** – data saved in `furniture.txt`, bills appended to `bills.txt`
- **Input validation** – invalid numbers, empty text, duplicate IDs, and the `|` character are all rejected
- **Stock status** – every item is marked `OK`, `LOW` (≤ 5 units) or `OUT`
- **Smart cart** – adding the same item twice merges into one bill line, and stock limits are respected
- **Safe exit** – if input is closed unexpectedly, data is saved before the program ends

---

## Tech Stack

- **Language:** C++ (C++11 or later)
- **Concepts used:** structures, arrays, functions, file handling (`ifstream` / `ofstream`), string handling, input validation, `iomanip` formatting
- **Libraries:** only the standard library (`iostream`, `fstream`, `iomanip`, `string`, `cstdlib`, `ctime`)


---

## Sample Output

```
+=============================================================================+
|                                  MAIN MENU                                  |
+=============================================================================+
| 1. Add Furniture Item                                                       |
| 2. Display All Furniture                                                    |
| 3. Search Furniture by ID                                                   |
| 4. Update Furniture Details                                                 |
| 5. Delete Furniture Record                                                  |
| 6. Manage Stock Quantity                                                    |
| 7. Generate Customer Bill                                                   |
| 8. Stock Report and Low-Stock Alerts                                        |
| 9. Save Data Now                                                            |
| 0. Save and Exit                                                            |
+=============================================================================+
 Enter your choice (0-9):
```


---

## Data Format

Records are stored in `furniture.txt` in this format:

```
id|name|category|price|quantity
101|Wooden Chair|Chair|1499.50|20
102|Study Table|Table|4500.00|3
```

---

## Limits and Rules

| Setting | Value |
|---------|-------|
| Maximum furniture records | 100 |
| Maximum lines in one bill | 50 |
| Furniture ID range | 1 – 9999 |
| Name length | max 20 characters |
| Category length | max 12 characters |
| Maximum stock per item | 10,000 |
| Low-stock limit | ≤ 5 units |
| GST rate | 18% |

These values can be changed from the constants at the top of `main.cpp`.

---

## Future Improvements

- Search by name or category
- Sort items by price or quantity
- Discount option on bills
- Login system for the shop owner
- Dynamic storage using `vector` instead of fixed arrays
- GUI version

---

