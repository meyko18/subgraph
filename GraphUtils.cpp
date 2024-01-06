#include "GraphUtils.h"

// 计算聚类系数
double calculateClusteringCoefficient(graph& graph, int vertexId) {
    
    std::vector<int> neighbors = graph.getNeighbors(vertexId);
    int neighborCount = neighbors.size();
    if (neighborCount < 2) {
        // 如果邻居数量少于2，则聚类系数为0
        return 0.0;
    }

    int edgesBetweenNeighbors = 0;
    std::cout << "neighbors.size(): " << neighbors.size() << std::endl;
    for (size_t i = 0; i < neighbors.size(); ++i) {
        for (size_t j = i + 1; j < neighbors.size(); ++j) {
            if (graph.hasEdge(neighbors[i], neighbors[j])) {
                ++edgesBetweenNeighbors;
            }
        }
    }

    double possibleEdges = neighborCount * (neighborCount - 1) / 2.0;

    
    return (possibleEdges == 0) ? 0 : (2.0 * edgesBetweenNeighbors) / possibleEdges;
}


// 在您的过滤器中使用聚类系数
bool isCandidateWithClusteringCoefficient(graph& dataGraph, graph& queryGraph, int vData, int vQuery, double threshold) {
    
    double ccData = calculateClusteringCoefficient(dataGraph, vData);
    double ccQuery = calculateClusteringCoefficient(queryGraph, vQuery);


    return std::abs(ccData - ccQuery) == threshold;
}