#include "Algorithms.h"
#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

// TODO: use google unittests
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

        // result should be 80;
        std::cout << Algorithms::travelingSalesmanProblem(graph, s) << std::endl;
    }

    // uncomment if distanceSquare4i was moved to Algorithms.
    void testLineDistanceSquare() {
        Line<int> p = Line<int>({XYPoint<int>({1, 2}), XYPoint<int>({3, 4})});
        Line<int> q = Line<int>({XYPoint<int>({5, 6}), XYPoint<int>({7, 8})});
        std::array<int, 4> d = Algorithms::lineDistanceSquare<int>(p, q);

        //  result should be 32, 72, 8, 32;
        std::cout << d[0] << ", " << d[1] << ", " << d[2] << ", " << d[3] << std::endl;
    }

    void testHoughLinesReinterpretCast() {
        cv::Vec4i l1 = {1, 2, 3, 4};
        cv::Vec4i l2 = {5, 6, 7, 8};
        std::cout << "l1 = " << l1[0] << ", " << l1[1] << ", " << l1[2] << ", " << l1[3] << std::endl;
        std::cout << "l2 = " << l2[0] << ", " << l2[1] << ", " << l2[2] << ", " << l2[3] << std::endl;

        Line<int>* line1 = reinterpret_cast<Line<int>*>(l1.val);
        Line<int>* line2 = reinterpret_cast<Line<int>*>(l2.val);

        std::array<int, 4> d = Algorithms::lineDistanceSquare((*line1), (*line2));
        std::cout << "d  = " << d[0] << ", " << d[1] << ", " << d[2] << ", " << d[3] << std::endl;
    }

    void testMinimalLineDistance() {
        Line<int> p = Line<int>({XYPoint<int>({1, 2}), XYPoint<int>({3, 4})});
        Line<int> q = Line<int>({XYPoint<int>({5, 6}), XYPoint<int>({7, 8})});
        auto test = Algorithms::minimalLineDistance<int>(p, q);

        // result should be "minimal distance = 8, P1 = (3, 4), P2 = (5, 6)""
        std::cout << "minimal distance = " << std::get<0>(test) << ", P1 = (" << std::get<1>(test)->first << ", " << std::get<1>(test)->second << ")" << ", P2 = (" << std::get<2>(test)->first << ", " << std::get<2>(test)->second << ")" << std::endl;
    }
}