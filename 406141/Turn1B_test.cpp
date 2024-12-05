#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

class Graph {
private:
    int V;
    std::vector<std::vector<int>> adjList;
    std::vector<std::string> resourceNames;

public:
    Graph(int vertices) : V(vertices), adjList(vertices) {
        for (int i = 0; i < vertices; i++) {
            resourceNames.push_back("Resource " + std::to_string(i));
        }
    }

    void addEdge(int u, int v) {
        if (u >= 0 && u < V && v >= 0 && v < V) {
            adjList[u].push_back(v);
            adjList[v].push_back(u);
        }
    }

    void setResourceName(int vertex, const std::string& name) {
        if (vertex >= 0 && vertex < V) {
            resourceNames[vertex] = name;
        }
    }

    std::vector<int> colorGraph() {
        std::vector<int> colors(V, -1);
        std::vector<bool> available(V, true);

        // Color first vertex
        colors[0] = 0;

        // Color remaining vertices
        for (int vertex = 1; vertex < V; vertex++) {
            // Mark colors of adjacent vertices as unavailable
            for (int adj : adjList[vertex]) {
                if (colors[adj] != -1) {
                    available[colors[adj]] = false;
                }
            }

            // Find first available color
            int cr;
            for (cr = 0; cr < V; cr++) {
                if (available[cr]) break;
            }

            colors[vertex] = cr;

            // Reset available colors
            for (int adj : adjList[vertex]) {
                if (colors[adj] != -1) {
                    available[colors[adj]] = true;
                }
            }
        }

        return colors;
    }

    void printSchedule(const std::vector<int>& colors) {
        std::cout << "\nResource Allocation Schedule:\n";
        std::cout << "----------------------------\n";
        
        int maxColor = *std::max_element(colors.begin(), colors.end());
        
        // Print by time slot
        for (int slot = 0; slot <= maxColor; slot++) {
            std::cout << "Time Slot " << slot + 1 << ":\n";
            for (int i = 0; i < V; i++) {
                if (colors[i] == slot) {
                    std::cout << "  " << resourceNames[i] << "\n";
                }
            }
            std::cout << "\n";
        }
        
        std::cout << "Total time slots required: " << maxColor + 1 << "\n";
    }
};

int main() {
    Graph g(6);
    
    // Add resource conflicts
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 5);

    // Set custom resource names
    g.setResourceName(0, "CPU Core 1");
    g.setResourceName(1, "CPU Core 2");
    g.setResourceName(2, "Memory Bank 1");
    g.setResourceName(3, "Memory Bank 2");
    g.setResourceName(4, "Network Interface");
    g.setResourceName(5, "Storage Unit");

    // Color the graph
    auto colors = g.colorGraph();

    // Print schedule
    g.printSchedule(colors);

    return 0;
} 