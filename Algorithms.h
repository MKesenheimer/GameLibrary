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
                T deltaX = p.first - q.first;
                T deltaY = p.second - q.second;
                delta[k] = deltaX * deltaX + deltaY * deltaY;
                k++;
            }
        }
        return delta;
    }

    // gives the minimal (squared) distance of two lines and the iterators pointing to the points with minimal distance
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

    // gives the point to the point of a line that has minimal distance to another point
    template<typename T>
    static typename Line<T>::const_iterator minimalPLinePoint(const XYPoint<T>& point, const Line<T>& line) {
        const std::array<T, 2>& delta = minimalPLineDistance2(point, line);
        if (delta[0] <= delta[1])
            return std::begin(line);
        else
            return (std::begin(line) + 1);
    }

    // gives the minimal (squared) distance of a line and a point
    template<typename T>
    static T minimalPLineDistance(const XYPoint<T>& point, const Line<T>& line) {
        const std::array<T, 2>& delta = minimalPLineDistance2(point, line);
        return std::min(delta[0], delta[1]);
    }

    template<typename T>
    static std::array<T, 2> minimalPLineDistance2(const XYPoint<T>& point, const Line<T>& line) {
        size_t k = 0;
        std::array<T, 2> delta;
        for (const XYPoint<T>& q : line) {
            T deltaX = point.first - q.first;
            T deltaY = point.second - q.second;
            delta[k] = deltaX * deltaX + deltaY * deltaY;
            k++;
        }
        return delta;
    }
};