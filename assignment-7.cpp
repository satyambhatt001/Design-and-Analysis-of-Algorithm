#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <list>
#include <algorithm>
#include <string>

class Graph {
public:
    int V;
    std::vector<std::unordered_set<int>> adj;
    std::unordered_map<int, int> courseSizes;

    Graph(int numCourses) : V(numCourses), adj(numCourses) {}

    void addEdge(int u, int v) {
        adj[u].insert(v);
        adj[v].insert(u);
    }

    void buildFromEnrollments(
        const std::unordered_map<int, std::vector<int>>& enrollments,
        const std::unordered_map<int, int>& courseStudentCounts) {
        
        courseSizes = courseStudentCounts;

        for (const auto& pair : enrollments) {
            const std::vector<int>& courses = pair.second;
            for (size_t i = 0; i < courses.size(); ++i) {
                for (size_t j = i + 1; j < courses.size(); ++j) {
                    if (courses[i] < V && courses[j] < V) {
                        addEdge(courses[i], courses[j]);
                    }
                }
            }
        }
    }

    int getDegree(int u) const {
        return adj[u].size();
    }
};

class Scheduler {
private:
    Graph graph;
    std::vector<int> colors;
    int numColorsUsed;

    void printSchedule(const std::string& algorithmName) const {
        std::cout << "\n--- " << algorithmName << " Scheduling Result ---\n";
        std::cout << "Total Time Slots (Colors) Used: " << numColorsUsed << "\n";
        
        std::map<int, std::vector<int>> slots;
        for (int i = 0; i < graph.V; ++i) {
            slots[colors[i]].push_back(i);
        }
        
        for(const auto& slot : slots) {
            std::cout << "  Slot " << slot.first << ": ";
            for(int course : slot.second) {
                std::cout << "C" << course << " ";
            }
            std::cout << "\n";
        }
    }

    int getFirstAvailableColor(int u) {
        std::unordered_set<int> neighbourColors;
        for (int v : graph.adj[u]) {
            if (colors[v] != -1) {
                neighbourColors.insert(colors[v]);
            }
        }

        int color = 0;
        while (neighbourColors.count(color)) {
            color++;
        }
        return color;
    }

public:
    Scheduler(const Graph& g) : graph(g), numColorsUsed(0) {}

    void solveGreedy() {
        colors.assign(graph.V, -1);
        int maxColor = 0;

        for (int u = 0; u < graph.V; ++u) {
            int c = getFirstAvailableColor(u);
            colors[u] = c;
            maxColor = std::max(maxColor, c);
        }
        
        numColorsUsed = maxColor + 1;
        printSchedule("Greedy");
    }

    void solveWelshPowell() {
        colors.assign(graph.V, -1);
        int maxColor = 0;

        std::vector<std::pair<int, int>> degrees;
        for (int i = 0; i < graph.V; ++i) {
            degrees.push_back({-graph.getDegree(i), i});
        }
        std::sort(degrees.begin(), degrees.end());

        for (const auto& pair : degrees) {
            int u = pair.second;
            int c = getFirstAvailableColor(u);
            colors[u] = c;
            maxColor = std::max(maxColor, c);
        }
        
        numColorsUsed = maxColor + 1;
        printSchedule("Welsh-Powell");
    }

    void solveDSATUR() {
        colors.assign(graph.V, -1);
        int maxColor = 0;
        int coloredCount = 0;

        std::vector<int> degrees(graph.V);
        for(int i = 0; i < graph.V; ++i) degrees[i] = graph.getDegree(i);
        
        std::vector<int> saturation(graph.V, 0);
        std::unordered_set<int> uncolored;
        for(int i = 0; i < graph.V; ++i) uncolored.insert(i);

        int startNode = 0;
        for(int i = 1; i < graph.V; ++i){
            if(degrees[i] > degrees[startNode]) startNode = i;
        }
        
        colors[startNode] = 0;
        maxColor = 0;
        coloredCount++;
        uncolored.erase(startNode);
        
        for (int v : graph.adj[startNode]) {
            saturation[v]++;
        }

        while(coloredCount < graph.V) {
            int u = -1;
            int maxSat = -1;
            int maxDeg = -1;
            
            for(int v : uncolored){
                if(saturation[v] > maxSat){
                    maxSat = saturation[v];
                    maxDeg = degrees[v];
                    u = v;
                } else if (saturation[v] == maxSat) {
                    if(degrees[v] > maxDeg){
                        maxDeg = degrees[v];
                        u = v;
                    }
                }
            }
            
            if(u == -1) u = *uncolored.begin();

            int c = getFirstAvailableColor(u);
            colors[u] = c;
            maxColor = std::max(maxColor, c);
            coloredCount++;
            uncolored.erase(u);

            for (int v : graph.adj[u]) {
                if (uncolored.count(v)) {
                    bool isNewColor = true;
                    for (int w : graph.adj[v]) {
                        if (colors[w] == c) {
                            isNewColor = false;
                            break;
                        }
                    }
                    if (isNewColor) {
                        saturation[v]++;
                    }
                }
            }
        }
        
        numColorsUsed = maxColor + 1;
        printSchedule("DSATUR");
    }

    void allocateRooms(const std::map<std::string, int>& roomCapacities) {
        std::cout << "\n--- Room Allocation (based on last schedule) ---\n";

        std::map<int, std::vector<std::pair<int, int>>> slotCourses; 
        for (int i = 0; i < graph.V; ++i) {
            int courseID = i;
            int slot = colors[i];
            int size = graph.courseSizes.count(courseID) ? graph.courseSizes.at(courseID) : 0;
            slotCourses[slot].push_back({-size, courseID});
        }

        for (auto& slotPair : slotCourses) {
            int slot = slotPair.first;
            std::vector<std::pair<int, int>>& courses = slotPair.second;
            std::sort(courses.begin(), courses.end());

            std::cout << "Slot " << slot << ":\n";

            std::vector<std::pair<int, std::string>> availableRooms;
            for(const auto& room : roomCapacities) {
                availableRooms.push_back({room.second, room.first});
            }
            std::sort(availableRooms.rbegin(), availableRooms.rend());

            int totalStudents = 0;
            int totalCapacity = 0;
            for(const auto& room : availableRooms) totalCapacity += room.first;

            for (const auto& coursePair : courses) {
                int courseSize = -coursePair.first;
                int courseID = coursePair.second;
                totalStudents += courseSize;

                bool allocated = false;
                for (size_t i = 0; i < availableRooms.size(); ++i) {
                    if (availableRooms[i].first >= courseSize) {
                        std::cout << "  - Course C" << courseID << " (" << courseSize 
                                  << " students) -> Room " << availableRooms[i].second 
                                  << " (Cap: " << availableRooms[i].first << ")\n";
                        
                        availableRooms[i].first -= courseSize;
                        
                        std::sort(availableRooms.rbegin(), availableRooms.rend());
                        allocated = true;
                        break;
                    }
                }

                if (!allocated) {
                    std::cout << "  - FAILED to allocate Course C" << courseID 
                              << " (" << courseSize << " students). No suitable room.\n";
                }
            }

            if(totalStudents > totalCapacity){
                std::cout << "  WARNING: Total students (" << totalStudents 
                          << ") exceeds total room capacity (" << totalCapacity << ") for this slot.\n";
            }
        }
    }
};

int main() {
    const int NUM_COURSES = 7;

    std::unordered_map<int, int> courseSizes = {
        {0, 100}, {1, 50}, {2, 75}, {3, 120}, {4, 30}, {5, 80}, {6, 60}
    };
    
    std::unordered_map<int, std::vector<int>> enrollments = {
        {1001, {0, 1, 2}},
        {1002, {0, 3, 4}},
        {1003, {1, 3, 5}},
        {1004, {2, 4, 6}},
        {1005, {0, 6}},
        {1006, {1, 4}},
        {1007, {3, 6}}
    };
    
    std::map<std::string, int> rooms = {
        {"Hall_A", 150},
        {"Hall_B", 100},
        {"Room_101", 50}
    };

    Graph courseGraph(NUM_COURSES);
    courseGraph.buildFromEnrollments(enrollments, courseSizes);

    Scheduler scheduler(courseGraph);

    scheduler.solveGreedy();

    scheduler.solveWelshPowell();
    
    scheduler.solveDSATUR(); 

    scheduler.allocateRooms(rooms);

    return 0;
}