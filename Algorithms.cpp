#include "Algorithms.h"
#include <algorithm>

// Implementation of traveling Salesman Problem
int Algorithms::travelingSalesmanProblem(std::vector<std::vector<int>>& graph, int s) {
    // store all vertex apart from source vertex
    std::vector<int> vertex;
    vertex.reserve(graph.size());
    for (int i = 0; i < graph.size(); i++)
        if (i != s)
            vertex.push_back(i);
 
    // store minimum weight Hamiltonian Cycle.
    int min_path = INT_MAX;
    do {
        // store current Path weight(cost)
        int current_pathweight = 0;
 
        // compute current path weight
        int k = s;
        for (int i = 0; i < vertex.size(); i++) {
            current_pathweight += graph[k][vertex[i]];
            k = vertex[i];
        }
        current_pathweight += graph[k][s];
 
        // update minimum
        min_path = std::min(min_path, current_pathweight);
 
    } while (
        std::next_permutation(vertex.begin(), vertex.end()));
 
    return min_path;
}