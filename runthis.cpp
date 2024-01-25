#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <utility>
// t 317080 1049866
struct Graph {
    std::set<int> nodes;
    std::map<int, std::set<int>> edges;
    std::map<int, int> labels; // Additional data structure to store node labels

    void addNode(int node, int label) {
        nodes.insert(node);
        labels[node] = label; // Store the label of the node
    }

    void addEdge(int from, int to) {
        edges[from].insert(to);
        edges[to].insert(from); // Assuming the graph is undirected
    }

    void loadGraphFromFile(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;

        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }

        while (getline(file, line)) {
            std::istringstream iss(line);
            char type;
            int id1, id2, label;
            
            iss >> type;
            if (type == 'v') {
                iss >> id1 >> label;
                addNode(id1, label);
            } else if (type == 'e') {
                iss >> id1 >> id2 >> label;
                addEdge(id1, id2);
            }
        }

        file.close();
    }

    void compressGraph() {
        std::map<std::set<int>, int> neighborMap;
        /*
        两个相同的节点，并且如果两个节点（在这个例子中是两个C节点）
        具有相同的邻接节点，它们可以被合并为一个节点，因为在子图匹配过程中，它们是不可区分的。
        图（a）中节点C之间的边在图（b）中被移除了，因为合并了这两个节点。
        此外，图（a）中的节点C与节点E之间的两条边在图（b）中被压缩成了一条边。根据上图的算法去压缩图。
        */
        // Identify nodes with identical sets of neighbors and mark edges for removal
        for (int node1 : nodes) {
            std::set<int> neighbors1 = edges[node1];
            neighbors1.erase(node1); // Remove self-loop if present

            if (!neighbors1.empty() && neighborMap.find(neighbors1) == neighborMap.end()) {
                // This is the first node with this set of neighbors
                neighborMap[neighbors1] = node1;
            } else if (!neighbors1.empty()) {
                // Found another node with the same set of neighbors
                int node2 = neighborMap[neighbors1];
                // Remove edges between node1 and its neighbors if node2 also has them
                for (int neighbor : neighbors1) {
                    if (edges[neighbor].count(node2) && labels[node1] == labels[node2]) {
                        edges[node1].erase(neighbor);
                        edges[neighbor].erase(node1);
                    }
                }
            }
        }
    }

    void relabelNodes() {
        std::map<int, int> newLabels;
        int newLabel = 1;

        // Assign new continuous labels starting from 1 to each node
        for (int node : nodes) {
            newLabels[node] = newLabel++;
        }

        // Create a new set of edges with updated node labels
        std::map<int, std::set<int>> newEdges;
        for (auto& edgeSet : edges) {
            int updatedFrom = newLabels[edgeSet.first];
            std::set<int> updatedNeighbors;
            for (int neighbor : edgeSet.second) {
                updatedNeighbors.insert(newLabels[neighbor]);
            }
            newEdges[updatedFrom] = updatedNeighbors;
        }

        // Update the graph with new labels and edges
        edges = newEdges;
        nodes.clear();
        for (int i = 1; i < newLabel; ++i) {
            nodes.insert(i);
        }

    // If you store labels for each node and they are not just node identifiers,
        // you should update them here as well.
        // Assuming labels are just identifiers and don't carry additional information
        labels.clear();
        for (int i = 1; i < newLabel; ++i) {
            labels[i] = 0; // Assign a default label if needed
        }
    }

    void printCompressedGraph(std::ostream& out) {
        for (int node : nodes) {
            out << "v " << node << " " << labels[node] << std::endl;
        }
        for (const auto& edgeSet : edges) {
            for (int neighbor : edgeSet.second) {
                if (edgeSet.first < neighbor) { // To avoid printing duplicate edges
                    out << "e " << edgeSet.first << " " << neighbor << " 0" << std::endl;
                }
            }
        }
    }
    void removeIsolatedNodes() {
        std::set<int> nodesToRemove;

        // Identify nodes with degree 0
        for (int node : nodes) {
            if (edges[node].empty()) {
                nodesToRemove.insert(node);
            }
        }

        // Remove the identified nodes
        for (int node : nodesToRemove) {
            nodes.erase(node);
            labels.erase(node); // Also remove the label if necessary
        }
    }
};

int main() {
    Graph g;

    // Load the graph data from the file
    g.loadGraphFromFile("./datasets/dblp.graph");

    // Compress the graph
    g.compressGraph();

    // Remove isolated nodes
    // g.removeIsolatedNodes();

    // Relabel nodes to maintain continuous numbering from 1
    // g.relabelNodes();

    // Save the compressed graph to a file
    std::ofstream outFile("./datasets/compressed_dblp.graph");
    if (!outFile.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        return -1;
    }

    // Print the compressed graph to the file
    g.printCompressedGraph(outFile);

    outFile.close();
    std::cout << "Compression complete. Output saved to 'compressed_youtube.graph'" << std::endl;

    return 0;
}

