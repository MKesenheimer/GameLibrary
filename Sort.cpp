#include "Sort.h"
#include <algorithm>
#include "Algorithms.h"

void Sort::sortLines(std::vector<cv::Vec4i>& lines) {
    if (lines.size() <= 1)
        return;
    
    for (size_t k = 0; k < lines.size() - 1; ++k) {
        Line<int>* line0 = reinterpret_cast<Line<int>*>(lines[k].val);
        //const XYPoint<int>& point0 = (*line0)[0];
        const XYPoint<int>& point1 = (*line0)[1];

        // find closest line
        std::vector<cv::Vec4i>::iterator closest = std::min_element(lines.begin() + k + 1, lines.end(),
            [point1](cv::Vec4i& a, cv::Vec4i& b) {
                Line<int>* lineA = reinterpret_cast<Line<int>*>(a.val);
                Line<int>* lineB = reinterpret_cast<Line<int>*>(b.val);
                return Algorithms::minimalPLineDistance<int>(point1, *lineA) < Algorithms::minimalPLineDistance<int>(point1, *lineB);
            }
        );

        // determine closest point of closest line
        Line<int>* nextLine = reinterpret_cast<Line<int>*>(closest->val);
        //std::cout << (*nextLine)[0].first << std::endl;
        Line<int>::const_iterator nextPoint = Algorithms::minimalPLinePoint<int>(point1, *nextLine);

        // if the points of the line are not in the correct order, swap them
        if (nextPoint != nextLine->begin())
            std::iter_swap(nextLine->begin(), nextLine->begin() + 1);
        
        // move the element we found to the current position k + 1
        size_t i = std::distance(lines.begin(), closest);
        std::iter_swap(lines.begin() + k + 1, lines.begin() + i);
    }
}

void Sort::sortLines(std::vector<Line<int>>& lines) {
    if (lines.size() <= 1)
        return;
    
    for (size_t k = 0; k < lines.size() - 1; ++k) {
        Line<int>& line0 = lines[k];
        const XYPoint<int>& point1 = line0[1];

        // find closest line
        std::vector<Line<int>>::iterator closest = std::min_element(lines.begin() + k + 1, lines.end(),
            [point1](Line<int>& lineA, Line<int>& lineB) {
                return Algorithms::minimalPLineDistance<int>(point1, lineA) < Algorithms::minimalPLineDistance<int>(point1, lineB);
            }
        );

        // determine closest point of closest line
        Line<int>& nextLine = *closest;
        Line<int>::const_iterator nextPoint = Algorithms::minimalPLinePoint<int>(point1, nextLine);

        // if the points of the line are not in the correct order, swap them
        if (nextPoint != nextLine.begin())
            std::iter_swap(nextLine.begin(), nextLine.begin() + 1);
        
        // move the element we found to the current position k + 1
        size_t i = std::distance(lines.begin(), closest);
        std::iter_swap(lines.begin() + k + 1, lines.begin() + i);
    }
}