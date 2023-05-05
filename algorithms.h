/*
 *  algorithms.h
 *  Created by Matthias Kesenheimer on 08.06.22.
 *  Copyright 2022. All rights reserved.
 */
#pragma once
#include <vector>
#include <tuple>
#include <array>
#include <algorithm>
#include <cmath>
#include <sstream>
#include "point.h"
#include <iostream>

class algorithms {
public:
    // Implementation of the TSP problem
    static int travelingSalesmanProblem(std::vector<std::vector<int>>& graph, int s);

    // calculate the (squared) distances of all combinations of two lines (= 4 points)
    // delta[0] = distance(p[0], q[0])
    // delta[1] = distance(p[0], q[1])
    // delta[2] = distance(p[1], q[0])
    // delta[3] = distance(p[1], q[1])
    template<typename T>
    static std::array<T, 4> lineDistanceSquare(const types::line<T>& line1, const types::line<T>& line2) {
        size_t k = 0;
        std::array<T, 4> delta;
        for (const types::xypoint<T>& p : line1) {
            for (const types::xypoint<T>& q : line2) {
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
    static std::tuple<T, typename types::line<T>::const_iterator, typename types::line<T>::const_iterator> minimallineDistance(const types::line<T>& line1, const types::line<T>& line2) {
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
    static typename types::line<T>::const_iterator minimalPLinePoint(const types::xypoint<T>& point, const types::line<T>& line) {
        const std::array<T, 2>& delta = minimalPLineDistance2(point, line);
        if (delta[0] <= delta[1])
            return std::begin(line);
        else
            return (std::begin(line) + 1);
    }

    // gives the minimal (squared) distance of a line and a point
    template<typename T>
    static T minimalPLineDistance(const types::xypoint<T>& point, const types::line<T>& line) {
        const std::array<T, 2>& delta = minimalPLineDistance2(point, line);
        return std::min(delta[0], delta[1]);
    }

    template<typename T>
    static std::array<T, 2> minimalPLineDistance2(const types::xypoint<T>& point, const types::line<T>& line) {
        size_t k = 0;
        std::array<T, 2> delta;
        for (const types::xypoint<T>& q : line) {
            T deltaX = point.first - q.first;
            T deltaY = point.second - q.second;
            delta[k] = deltaX * deltaX + deltaY * deltaY;
            k++;
        }
        return delta;
    }

    // constrain a number between two boundaries
    template<typename T> 
    inline static T constrain(T value, T min, T max) {
        return std::min(std::max(value, min), max);
    }

    // convert string to arbitrary data type and constrain if needed between lower and upper
    template<typename T>
    static inline T strTo(const std::string& str, T lower = 0, T upper = 0) {
        T tmp = 0;
        try {
            std::istringstream(str) >> tmp;
            if (lower != 0 || upper != 0) {
                if (tmp < lower || tmp > upper) {
                    std::cout << "Warning: Number not in range: " << tmp << ", (" << lower << ", " << upper << ")" << std::endl;
                    return constrain<T>(tmp, lower, upper);
                }
            }
        } catch (const std::exception& e) {
            std::cout << "Warning: " << e.what() << ": No valid input: " << tmp << std::endl;
        }
        return tmp;
    }

    // Convert arbitrary type to string
    template<typename T>
    static inline std::string typeToStr(T a) {
        std::stringstream ss;
        ss << a;
        return ss.str();
    }
};