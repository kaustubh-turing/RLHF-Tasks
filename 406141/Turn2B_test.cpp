#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

class Graph {
private:
    struct Edge {
        int to;
        int weight;
    };
    
    int V;
    std::vector<std::vector<Edge>> adjList;
    std::vector<std::string> resourceNames;

public:
    Graph(int vertices) : V(vertices), adjList(vertices) {
        for (int i = 0; i < vertices; i++) {
            resourceNames.push_back("Resource " + std::to_string(i));
        }
    }

    void addEdge(int u, int v, int weight) {
        if (weight < 1 || weight > 10) {
            throw std::invalid_argument("Weight must be between 1 and 10");
        }
        adjList[u].push_back({v, weight});
        adjList[v].push_back({u, weight});
    }

    void setResourceName(int vertex, const std::string& name) {
        if (vertex >= 0 && vertex < V) {
            resourceNames[vertex] = name;
        }
    }

    std::vector<int> colorGraph() {
        std::vector<int> colors(V, -1);
        std::vector<std::pair<int, int>> vertices;
        
        // Sort vertices by total conflict weight
        for (int i = 0; i < V; i++) {
            int total_weight = 0;
            for (const Edge& e : adjList[i]) {
                total_weight += e.weight;
            }
            vertices.push_back({total_weight, i});
        }
        std::sort(vertices.rbegin(), vertices.rend());

        // Color vertices in order of total weight
        for (const auto& [_, vertex] : vertices) {
            std::vector<bool> used_colors(V, false);
            
            for (const Edge& e : adjList[vertex]) {
                if (colors[e.to] != -1) {
                    if (e.weight >= 8) {
                        used_colors[colors[e.to]] = true;  // Must avoid
                    } else if (e.weight >= 4) {
                        used_colors[colors[e.to]] = true;  // Try to avoid
                    }
                }
            }

            int color = 0;
            while (color < V && used_colors[color]) {
                color++;
            }
            colors[vertex] = color;
        }

        return colors;
    }

    void printSchedule(const std::vector<int>& colors) {
        std::cout << "\nResource Allocation Schedule:\n";
        std::cout << "----------------------------\n";
        
        int maxColor = *std::max_element(colors.begin(), colors.end());
        
        for (int slot = 0; slot <= maxColor; slot++) {
            std::cout << "Time Slot " << slot + 1 << ":\n";
            for (int i = 0; i < V; i++) {
                if (colors[i] == slot) {
                    std::cout << "  " << resourceNames[i];
                    // Show conflicts
                    for (const Edge& e : adjList[i]) {
                        if (colors[e.to] == slot) {
                            std::cout << " (Warning: Conflict with " << resourceNames[e.to] 
                                    << ", Priority: " << e.weight << ")";
                        }
                    }
                    std::cout << "\n";
                }
            }
            std::cout << "\n";
        }
    }
};

int main() {
    Graph g(6);
    
    // Add weighted conflicts
    g.addEdge(0, 1, 10);  // Critical
    g.addEdge(0, 2, 7);   // High
    g.addEdge(1, 2, 9);   // High
    g.addEdge(1, 3, 4);   // Medium
    g.addEdge(2, 4, 3);   // Low
    g.addEdge(3, 4, 2);   // Low
    g.addEdge(4, 5, 6);   // Medium

    g.setResourceName(0, "CPU Core 1");
    g.setResourceName(1, "CPU Core 2");
    g.setResourceName(2, "Memory Bank 1");
    g.setResourceName(3, "Memory Bank 2");
    g.setResourceName(4, "Network Interface");
    g.setResourceName(5, "Storage Device");

    auto colors = g.colorGraph();

    g.printSchedule(colors);

    int max_color = *std::max_element(colors.begin(), colors.end());
    std::cout << "\nTotal time slots needed: " << max_color + 1 << "\n";

    return 0;
} 