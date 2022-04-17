/*
 *  LaserPoint.h
 *  Created by Matthias Kesenheimer on 01.01.21.
 *  Copyright 2022. All rights reserved.
 */
#pragma once
#include <tuple>

typedef std::pair<float, float> XYPoint; 

struct Point {
  float x, y;
  int r, g, b, a;
  bool iscollidable;
};