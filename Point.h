/*
 *  Point.h
 *  Created by Matthias Kesenheimer on 01.01.21.
 *  Copyright 2022. All rights reserved.
 */
#pragma once
#include <tuple>
#include <array>

template<typename T> 
using XYPoint = std::pair<T, T>; 

// TODO: Move to extra class, or rename this header file
template<typename T>
using Line = std::array<XYPoint<T>, 2>;

template<typename T> 
struct Point {
    // coordinates
    T x, y;
    // color and alpha value (a = 0 -> transparent, a = 255 -> opaque)
    int r, g, b, a;
    // should be collision detection applied
    bool iscollidable;
};