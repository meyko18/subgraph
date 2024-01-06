#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include "graph.h" // 假设 Graph 类定义在这个文件中

// 计算聚类系数
double calculateClusteringCoefficient(graph& graph, int vertexId);

// 在过滤器中使用聚类系数
bool isCandidateWithClusteringCoefficient(graph& dataGraph, graph& queryGraph, int vData, int vQuery, double threshold);

#endif // GRAPH_UTILS_H
