/*
 *  Point.h
 *  Created by Matthias Kesenheimer on 01.01.21.
 *  Copyright 2022. All rights reserved.
 */
#pragma once
#include <tuple>

typedef std::pair<float, float> XYPoint; 

struct Point {
    // coordinates
    float x, y;
    // color and alpha value (a = 0 -> transparent, a = 255 -> opaque)
    int r, g, b, a;
    // should be collision detection applied
    bool iscollidable;
};