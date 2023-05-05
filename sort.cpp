#include "sort.h"
#include <algorithm>
#include "Algorithms.h"

void sort::sortLines(std::vector<cv::Vec4i>& lines) {
    if (lines.size() <= 1)
        return;
    
    for (size_t k = 0; k < lines.size() - 1; ++k) {
        line<int>* line0 = reinterpret_cast<Line<int>*>(lines[k].val);
        //const XYPoint<int>& point0 = (*line0)[0];
        const xypoint<int>& point1 = (*line0)[1];

        // find closest line
        std::vector<cv::Vec4i>::iterator closest = std::min_element(lines.begin() + k + 1, lines.end(),
            [point1](cv::Vec4i& a, cv::Vec4i& b) {
                line<int>* lineA = reinterpret_cast<line<int>*>(a.val);
                line<int>* lineB = reinterpret_cast<line<int>*>(b.val);
                return algorithms::minimalPLineDistance<int>(point1, *lineA) < algorithms::minimalPLineDistance<int>(point1, *lineB);
            }
        );

        // determine closest point of closest line
        line<int>* nextLine = reinterpret_cast<line<int>*>(closest->val);
        //std::cout << (*nextLine)[0].first << std::endl;
        line<int>::const_iterator nextPoint = algorithms::minimalPLinePoint<int>(point1, *nextLine);

        // if the points of the line are not in the correct order, swap them
        if (nextPoint != nextLine->begin())
            std::iter_swap(nextLine->begin(), nextLine->begin() + 1);
        
        // move the element we found to the current position k + 1
        size_t i = std::distance(lines.begin(), closest);
        std::iter_swap(lines.begin() + k + 1, lines.begin() + i);
    }
}

void sort::sortLines(std::vector<line<int>>& lines) {
    if (lines.size() <= 1)
        return;
    
    for (size_t k = 0; k < lines.size() - 1; ++k) {
        line<int>& line0 = lines[k];
        const xypoint<int>& point1 = line0[1];

        // find closest line
        std::vector<line<int>>::iterator closest = std::min_element(lines.begin() + k + 1, lines.end(),
            [point1](line<int>& lineA, line<int>& lineB) {
                return Algorithms::minimalPLineDistance<int>(point1, lineA) < Algorithms::minimalPLineDistance<int>(point1, lineB);
            }
        );

        // determine closest point of closest line
        line<int>& nextLine = *closest;
        line<int>::const_iterator nextPoint = algorithms::minimalPLinePoint<int>(point1, nextLine);

        // if the points of the line are not in the correct order, swap them
        if (nextPoint != nextLine.begin())
            std::iter_swap(nextLine.begin(), nextLine.begin() + 1);
        
        // move the element we found to the current position k + 1
        size_t i = std::distance(lines.begin(), closest);
        std::iter_swap(lines.begin() + k + 1, lines.begin() + i);
    }
}