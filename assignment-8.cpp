#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

using namespace std;

const int INF = numeric_limits<int>::max();

struct Node {
    vector<int> path;
    vector<vector<int>> reducedMatrix;
    int cost;
    int lowerBound;
    int city;
    int level;
};

struct CompareNode {
    bool operator()(const Node& a, const Node& b) const {
        return a.lowerBound > b.lowerBound;
    }
};

int rowReduction(vector<vector<int>>& matrix, int N) {
    int reductionCost = 0;
    for (int i = 0; i < N; ++i) {
        int minVal = INF;
        for (int j = 0; j < N; ++j) {
            if (matrix[i][j] < minVal) {
                minVal = matrix[i][j];
            }
        }

        if (minVal != 0 && minVal != INF) {
            reductionCost += minVal;
            for (int j = 0; j < N; ++j) {
                if (matrix[i][j] != INF) {
                    matrix[i][j] -= minVal;
                }
            }
        }
    }
    return reductionCost;
}

int colReduction(vector<vector<int>>& matrix, int N) {
    int reductionCost = 0;
    for (int j = 0; j < N; ++j) {
        int minVal = INF;
        for (int i = 0; i < N; ++i) {
            if (matrix[i][j] < minVal) {
                minVal = matrix[i][j];
            }
        }

        if (minVal != 0 && minVal != INF) {
            reductionCost += minVal;
            for (int i = 0; i < N; ++i) {
                if (matrix[i][j] != INF) {
                    matrix[i][j] -= minVal;
                }
            }
        }
    }
    return reductionCost;
}

bool isVisited(const vector<int>& path, int city) {
    for (int i : path) {
        if (i == city) {
            return true;
        }
    }
    return false;
}

void printPath(const vector<int>& path) {
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i];
        if (i < path.size() - 1) {
            cout << " -> ";
        }
    }
    cout << "\n";
}

void solveTSP(const vector<vector<int>>& costMatrix, int N) {
    priority_queue<Node, vector<Node>, CompareNode> pq;

    int minTourCost = INF;
    vector<int> finalPath;

    Node root;
    root.path.push_back(0);
    root.reducedMatrix = costMatrix;
    root.cost = 0;
    root.city = 0;
    root.level = 0;

    int rowRed = rowReduction(root.reducedMatrix, N);
    int colRed = colReduction(root.reducedMatrix, N);
    root.lowerBound = rowRed + colRed;

    pq.push(root);

    while (!pq.empty()) {
        Node curr = pq.top();
        pq.pop();

        if (curr.lowerBound > minTourCost) {
            continue;
        }

        if (curr.level == N - 1) {
            int costToStart = costMatrix[curr.city][0];
            if (costToStart == INF) {
                continue;
            }

            int currentTourCost = curr.cost + costToStart;

            if (currentTourCost < minTourCost) {
                minTourCost = currentTourCost;
                finalPath = curr.path;
                finalPath.push_back(0);
            }
            continue;
        }

        for (int next_city = 0; next_city < N; ++next_city) {
            if (!isVisited(curr.path, next_city)) {
                
                int edgeCostReduced = curr.reducedMatrix[curr.city][next_city];
                if (edgeCostReduced == INF) {
                    continue;
                }

                Node child;
                child.path = curr.path;
                child.path.push_back(next_city);
                child.reducedMatrix = curr.reducedMatrix;
                child.cost = curr.cost + costMatrix[curr.city][next_city];
                child.city = next_city;
                child.level = curr.level + 1;

                for (int j = 0; j < N; ++j) {
                    child.reducedMatrix[curr.city][j] = INF;
                }
                for (int i = 0; i < N; ++i) {
                    child.reducedMatrix[i][next_city] = INF;
                }

                child.reducedMatrix[next_city][0] = INF;

                int childRowRed = rowReduction(child.reducedMatrix, N);
                int childColRed = colReduction(child.reducedMatrix, N);

                child.lowerBound = curr.lowerBound + edgeCostReduced + childRowRed + childColRed;

                if (child.lowerBound < minTourCost) {
                    pq.push(child);
                }
            }
        }
    }

    cout << "Optimal Tour Cost: " << minTourCost << "\n";
    cout << "Optimal Path: ";
    printPath(finalPath);
}


int main() {

    const int N = 5;
    vector<vector<int>> costMatrix = {
        { INF, 20,  30,  10,  11  },
        { 15,  INF, 16,  4,   2   },
        { 3,   5,   INF, 2,   4   },
        { 19,  6,   18,  INF, 3   },
        { 16,  4,   7,   16,  INF }
    };


    solveTSP(costMatrix, N);

    return 0;
}