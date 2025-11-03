#include <iostream>
#include <vector> 
#include <limits> 
using namespace std; 
O(V) 
Large multistage logistics graphs 
const int INF = numeric_limits<int>::max(); 
void findShortestPath(int numNodes, const vector<vector<int>>& adjMatrix) { 
vector<int> minCost(numNodes, INF); 
vector<int> route(numNodes, -1); 
minCost[numNodes - 1] = 0; 
for (int i = numNodes - 2; i >= 0; --i) { 
for (int j = i + 1; j < numNodes; ++j) { 
if (adjMatrix[i][j] != INF && minCost[j] != INF) { 
int currentCost = adjMatrix[i][j] + minCost[j]; 
if (currentCost < minCost[i]) { 
minCost[i] = currentCost; 
route[i] = j; 
} 
} 
} 
} 
cout << "\n========== Route Optimization Report ==========" << endl; 
if (minCost[0] == INF) { 
cout << "No valid route exists from the source to the destination." << 
endl; 
} else { 
cout << "Minimum Cost: " << minCost[0] << endl; 
cout << "Optimal Path: "; 
int currentNode = 0; 
while (currentNode != -1) { 
cout << currentNode; 
if (route[currentNode] != -1) { 
cout << " -> "; 
} 
currentNode = route[currentNode]; 
} 
cout << endl; 
} 
cout << "============================================" << endl; 
} 
int main() { 
int nodeCount, edgeCount; 
cout << "Enter the number of nodes (warehouses/hubs): "; 
cin >> nodeCount; 
cout << "Enter the number of routes (edges): "; 
cin >> edgeCount; 
vector<vector<int>> costGraph(nodeCount, vector<int>(nodeCount, INF)); 
cout << "\nEnter " << edgeCount << " routes (format: source destination 
cost):" << endl; 
for (int i = 0; i < edgeCount; ++i) { 
int source, dest, cost; 
cin >> source >> dest >> cost; 
if (source >= 0 && source < nodeCount && dest >= 0 && dest < nodeCount) 
{ 
costGraph[source][dest] = cost; 
} else { 
cout << "Warning: Invalid node number entered. Skipping this 
route." << endl; 
} 
} 
findShortestPath(nodeCount, costGraph); 
return 0; 
} 
