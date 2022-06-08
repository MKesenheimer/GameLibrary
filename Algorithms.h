/*
 *  Algorithms.h
 *  Created by Matthias Kesenheimer on 08.06.22.
 *  Copyright 2022. All rights reserved.
 */
#pragma once
#include <vector>

class Algorithms {
public:
    // Implementation of the TSP problem
    static int travelingSalesmanProblem(std::vector<std::vector<int>>& graph, int s);
};