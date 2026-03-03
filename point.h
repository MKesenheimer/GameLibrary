/*
 *  point.h
 *  Created by Matthias Kesenheimer on 01.01.21.
 *  Copyright 2022. All rights reserved.
 */
#pragma once
#include <tuple>
#include <array>

namespace types {
    template<typename T> 
    using xypoint = std::pair<T, T>; 
    
    // TODO: Move to extra class, or rename this header file
    template<typename T>
    using line = std::array<xypoint<T>, 2>;
    
    template<typename T> 
    struct point {
        // coordinates
        T x, y;
        // color and alpha value (a = 0 -> transparent, a = 255 -> opaque)
        int r, g, b, a;
        // should be collision detection applied
        bool iscollidable;
    };
}