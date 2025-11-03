#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

class Item {
public:
    string name;
    double weight;
    double value;
    bool divisible;
    int priority;

    Item(string n, double w, double v, bool d, int p)
        : name(n), weight(w), value(v), divisible(d), priority(p) {}

    double valuePerWeight() const {
        return value / weight;
    }

    void display() const {
        cout << left << setw(20) << name
             << setw(10) << weight
             << setw(10) << value
             << setw(12) << priority
             << setw(15) << fixed << setprecision(2) << valuePerWeight()
             << setw(15) << (divisible ? "Divisible" : "Indivisible")
             << endl;
    }
};

class KnapsackSolver {
private:
    vector<Item> items;
    double capacity;
    double totalWeightCarried = 0.0;
    double totalValue = 0.0;

    static bool compare(const Item& a, const Item& b) {
        if (a.priority == b.priority)
            return a.valuePerWeight() > b.valuePerWeight();
        return a.priority < b.priority;  // lower number = higher priority
    }

public:
    KnapsackSolver(const vector<Item>& itemList, double cap)
        : items(itemList), capacity(cap) {}

    double solve() {
        sort(items.begin(), items.end(), compare);

        cout << "\nSorted Items (by Priority, then Value/Weight):\n";
        cout << left << setw(20) << "Item"
             << setw(10) << "Weight"
             << setw(10) << "Value"
             << setw(12) << "Priority"
             << setw(15) << "Value/Weight"
             << setw(15) << "Type" << "\n";

        for (const auto& item : items) {
            item.display();
        }

        cout << "\nItems selected for transport:\n";

        for (const auto& item : items) {
            if (capacity <= 0)
                break;

            if (item.divisible) {
                double takenWeight = min(item.weight, capacity);
                double takenValue = item.valuePerWeight() * takenWeight;
                totalValue += takenValue;
                totalWeightCarried += takenWeight;
                capacity -= takenWeight;

                cout << " - " << item.name << ": " << takenWeight << " kg, Utility = " << takenValue
                     << ", Priority = " << item.priority << ", Type = Divisible\n";
            } else {
                if (item.weight <= capacity) {
                    totalValue += item.value;
                    totalWeightCarried += item.weight;
                    capacity -= item.weight;

                    cout << " - " << item.name << ": " << item.weight << " kg, Utility = " << item.value
                         << ", Priority = " << item.priority << ", Type = Indivisible\n";
                }
            }
        }

        return totalValue;
    }

    void finalReport() const {
        cout << "\n===== Final Report =====\n";
        cout << "Total weight carried: " << fixed << setprecision(2) << totalWeightCarried << " kg\n";
        cout << "Total utility value carried: " << fixed << setprecision(2) << totalValue << " units\n";
    }
};

int main() {
    vector<Item> items = {
        Item("Medical Kits", 10, 100, false, 1),
        Item("Food Packets", 20, 60, true, 3),
        Item("Drinking Water", 30, 90, true, 2),
        Item("Blankets", 15, 45, false, 3),
        Item("Infant Formula", 5, 50, false, 1)
    };

    double capacity;
    cout << "Enter maximum weight capacity of the boat (in kg): ";
    cin >> capacity;

    KnapsackSolver solver(items, capacity);
    solver.solve();
    solver.finalReport();

    return 0;
}
