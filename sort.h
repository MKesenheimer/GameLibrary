/*
 *  Sort.h
 *  Created by Matthias Kesenheimer on 08.06.22.
 *  Copyright 2022. All rights reserved.
 */
#pragma once
#include <vector>
#include "Point.h"
#include <opencv2/opencv.hpp>

class sort {
public:
    static void sortLines(std::vector<cv::Vec4i>& lines);

    static void sortLines(std::vector<Line<int>>& lines);
};