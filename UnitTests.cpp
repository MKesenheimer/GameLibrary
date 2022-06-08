#include "Algorithms.h"
#include <vector>

namespace unittests {
    void testTravelingSalesmanProblem() {
        // graph to test:
        //             (1)
        //             /|\
        //            / | \
        //           /  |  \
        //          /   |20 \
        //         /    |    \
        //      10/    (4)    \15
        //       /     / \     \
        //      /   /25     \30 \
        //     / /             \ \
        //   (2) --------------- (3)
        //              35
    
        std::vector<std::vector<int>> graph;
        graph.push_back(std::vector<int>({ 0, 10, 15, 20 }));
        graph.push_back(std::vector<int>({ 10, 0, 35, 25 }));
        graph.push_back(std::vector<int>({ 15, 35, 0, 30 }));
        graph.push_back(std::vector<int>({ 20, 25, 30, 0 }));
        int s = 0;
        std::cout << Algorithms::travelingSalesmanProblem(graph, s) << std::endl;
    }
}