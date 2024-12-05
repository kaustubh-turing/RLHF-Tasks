#include <iostream>
#include <vector>
#include <algorithm>

class Graph {
private:
    int V;  // Number of vertices
    std::vector<std::vector<int>> adjList;

public:
    Graph(int vertices) : V(vertices), adjList(vertices) {}

    void addEdge(int u, int v) {
        adjList[u].push_back(v);
        adjList[v].push_back(u);  // Undirected graph
    }

    std::vector<int> colorGraph() {
        std::vector<int> colors(V, -1);
        colors[0] = 0;  // First vertex gets first color

        for (int vertex = 1; vertex < V; vertex++) {
            std::vector<bool> used_colors(V, false);

            // Mark colors of adjacent vertices
            for (int adj : adjList[vertex]) {
                if (colors[adj] != -1) {
                    used_colors[colors[adj]] = true;
                }
            }

            // Find first unused color
            int color = 0;
            while (color < V && used_colors[color]) {
                color++;
            }

            colors[vertex] = color;
        }

        return colors;
    }
};

int main() {
    // Test case
    Graph g(5);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);
    g.addEdge(3, 4);

    auto colors = g.colorGraph();

    std::cout << "Resource Allocation:\n";
    for (int i = 0; i < 5; i++) {
        std::cout << "Resource " << i << " -> Time Slot " << colors[i] + 1 << "\n";
    }

    int max_color = *std::max_element(colors.begin(), colors.end());
    std::cout << "\nTotal time slots needed: " << max_color + 1 << "\n";

    return 0;
} 