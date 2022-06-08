/*
 *  Algorithms.h
 *  Created by Matthias Kesenheimer on 08.06.22.
 *  Copyright 2022. All rights reserved.
 */
#pragma once
#include <vector>
#include <tuple>
#include <array>
#include <algorithm>
#include "Point.h"

class Algorithms {
public:
    // Implementation of the TSP problem
    static int travelingSalesmanProblem(std::vector<std::vector<int>>& graph, int s);

    // calculate the (squared) distances of all combinations of two lines (= 4 points)
    // delta[0] = distance(p[0], q[0])
    // delta[1] = distance(p[0], q[1])
    // delta[2] = distance(p[1], q[0])
    // delta[3] = distance(p[1], q[1])
    template<typename T>
    static std::array<T, 4> lineDistanceSquare(const Line<T>& line1, const Line<T>& line2) {
        size_t k = 0;
        std::array<T, 4> delta;
        for (const XYPoint<T>& p : line1) {
            for (const XYPoint<T>& q : line2) {
                int deltaX = p.first - q.first;
                int deltaY = p.second - q.second;
                delta[k] = deltaX * deltaX + deltaY * deltaY;
                k++;
            }
        }
        return delta;
    }

    // give the minimal (squared) distance of two lines and the iterators pointing to the points with minimal distance
    template<typename T>
    static std::tuple<T, typename Line<T>::const_iterator, typename Line<T>::const_iterator> minimalLineDistance(const Line<T>& line1, const Line<T>& line2) {
        std::array<T, 4> dist = lineDistanceSquare(line1, line2);
        T* min = std::min_element(dist.begin(), dist.end());
        size_t element = std::distance(dist.begin(), min);
        switch (element) {
            case 0:
                return std::make_tuple(*min, std::begin(line1), std::begin(line2));
            case 1:
                return std::make_tuple(*min, std::begin(line1), std::begin(line2) + 1);
            case 2:
                return std::make_tuple(*min, std::begin(line1) + 1, std::begin(line2));
            case 3:
                return std::make_tuple(*min, std::begin(line1) + 1, std::begin(line2) + 1);
        }
        return std::make_tuple(*min, std::begin(line1), std::begin(line2)); 
    }
};