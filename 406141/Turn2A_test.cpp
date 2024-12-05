#include <iostream>
#include <vector>
#include <algorithm>

class Graph {
private:
    struct Edge {
        int to;
        int weight;
    };
    
    int V;
    std::vector<std::vector<Edge>> adjList;

public:
    Graph(int vertices) : V(vertices), adjList(vertices) {}

    void addEdge(int u, int v, int weight) {
        if (weight < 1 || weight > 10) {
            throw std::invalid_argument("Weight must be between 1 and 10");
        }
        adjList[u].push_back({v, weight});
        adjList[v].push_back({u, weight});
    }

    std::vector<int> colorGraph() {
        std::vector<int> colors(V, -1);
        colors[0] = 0;

        for (int vertex = 1; vertex < V; vertex++) {
            std::vector<bool> used_colors(V, false);
            
            // Sort adjacent vertices by weight
            std::vector<std::pair<int, int>> weighted_adj;
            for (const Edge& e : adjList[vertex]) {
                weighted_adj.push_back({e.weight, e.to});
            }
            std::sort(weighted_adj.rbegin(), weighted_adj.rend());

            // Mark colors of adjacent vertices
            for (const auto& [weight, adj] : weighted_adj) {
                if (colors[adj] != -1 && weight > 5) {
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
    Graph g(5);
    
    // Add edges with weights (1-10)
    g.addEdge(0, 1, 10);  // Critical conflict
    g.addEdge(0, 2, 7);   // High priority
    g.addEdge(1, 2, 3);   // Low priority
    g.addEdge(1, 3, 8);   // High priority
    g.addEdge(2, 3, 2);   // Low priority
    g.addEdge(3, 4, 5);   // Medium priority

    auto colors = g.colorGraph();

    std::cout << "Resource Allocation:\n";
    for (int i = 0; i < 5; i++) {
        std::cout << "Resource " << i << " -> Time Slot " << colors[i] + 1 << "\n";
    }

    int max_color = *std::max_element(colors.begin(), colors.end());
    std::cout << "\nTotal time slots needed: " << max_color + 1 << "\n";

    return 0;
} 