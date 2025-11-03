#include <iostream>
#include <vector>
#include <algorithm>

struct Item {
    int id;
    int weight;
    int value;
};

void printSelectedItems(int N, int W, const std::vector<Item>& items, const std::vector<std::vector<int>>& dp) {
    std::cout << "\nItems to include in the truck:" << std::endl;
    int totalValue = dp[N][W];
    int remainingWeight = W;
    std::vector<int> selectedItemIds;

    for (int i = N; i > 0 && totalValue > 0; i--) {
        if (totalValue != dp[i - 1][remainingWeight]) {
            selectedItemIds.push_back(items[i - 1].id);
            totalValue -= items[i - 1].value;
            remainingWeight -= items[i - 1].weight;
        }
    }
    
    if (selectedItemIds.empty()) {
        std::cout << "No items could be selected within the weight limit." << std::endl;
    } else {
        std::cout << "Item IDs: ";
        for (int i = selectedItemIds.size() - 1; i >= 0; --i) {
            std::cout << selectedItemIds[i] << (i == 0 ? "" : ", ");
        }
        std::cout << std::endl;
    }
}

int solveKnapsack(int W, const std::vector<Item>& items, int N) {
    std::vector<std::vector<int>> dp(N + 1, std::vector<int>(W + 1, 0));

    for (int i = 1; i <= N; i++) {
        for (int w = 1; w <= W; w++) {
            int currentWeight = items[i - 1].weight;
            int currentValue = items[i - 1].value;

            if (currentWeight <= w) {
                dp[i][w] = std::max(currentValue + dp[i - 1][w - currentWeight], dp[i - 1][w]);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    printSelectedItems(N, W, items, dp);

    return dp[N][W];
}

int main() {
    int N, W;

    std::cout << "--- Relief Truck Logistics Optimizer ---" << std::endl;
    std::cout << "Enter the number of different item types: ";
    std::cin >> N;
    std::cout << "Enter the truck's maximum capacity (kg): ";
    std::cin >> W;

    std::vector<Item> items(N);
    std::cout << "\nEnter the weight and utility value for each item:" << std::endl;

    for (int i = 0; i < N; i++) {
        items[i].id = i + 1;
        std::cout << "Item " << items[i].id << " - Weight (kg): ";
        std::cin >> items[i].weight;
        std::cout << "Item " << items[i].id << " - Utility Value: ";
        std::cin >> items[i].value;
    }

    int maxUtility = solveKnapsack(W, items, N);

    std::cout << "\n----------------------------------------" << std::endl;
    std::cout << "Maximum total utility value that can be carried: " << maxUtility << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    return 0;
}
