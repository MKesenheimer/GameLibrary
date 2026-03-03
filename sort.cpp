#include "sort.h"
#include <algorithm>
#include "algorithms.h"

void sort::sortLines(std::vector<cv::Vec4i>& lines) {
    if (lines.size() <= 1)
        return;
    
    for (size_t k = 0; k < lines.size() - 1; ++k) {
        types::line<int>* line0 = reinterpret_cast<types::line<int>*>(lines[k].val);
        //const types::xypoint<int>& point0 = (*line0)[0];
        const types::xypoint<int>& point1 = (*line0)[1];

        // find closest line
        std::vector<cv::Vec4i>::iterator closest = std::min_element(lines.begin() + k + 1, lines.end(),
            [point1](cv::Vec4i& a, cv::Vec4i& b) {
                types::line<int>* lineA = reinterpret_cast<types::line<int>*>(a.val);
                types::line<int>* lineB = reinterpret_cast<types::line<int>*>(b.val);
                return algorithms::minimalPLineDistance<int>(point1, *lineA) < algorithms::minimalPLineDistance<int>(point1, *lineB);
            }
        );

        // determine closest point of closest line
        types::line<int>* nextLine = reinterpret_cast<types::line<int>*>(closest->val);
        //std::cout << (*nextLine)[0].first << std::endl;
        types::line<int>::const_iterator nextPoint = algorithms::minimalPLinePoint<int>(point1, *nextLine);

        // if the points of the line are not in the correct order, swap them
        if (nextPoint != nextLine->begin())
            std::iter_swap(nextLine->begin(), nextLine->begin() + 1);
        
        // move the element we found to the current position k + 1
        size_t i = std::distance(lines.begin(), closest);
        std::iter_swap(lines.begin() + k + 1, lines.begin() + i);
    }
}

void sort::sortLines(std::vector<types::line<int>>& lines) {
    if (lines.size() <= 1)
        return;
    
    for (size_t k = 0; k < lines.size() - 1; ++k) {
        types::line<int>& line0 = lines[k];
        const types::xypoint<int>& point1 = line0[1];

        // find closest line
        std::vector<types::line<int>>::iterator closest = std::min_element(lines.begin() + k + 1, lines.end(),
            [point1](types::line<int>& lineA, types::line<int>& lineB) {
                return algorithms::minimalPLineDistance<int>(point1, lineA) < algorithms::minimalPLineDistance<int>(point1, lineB);
            }
        );

        // determine closest point of closest line
        types::line<int>& nextLine = *closest;
        types::line<int>::const_iterator nextPoint = algorithms::minimalPLinePoint<int>(point1, nextLine);

        // if the points of the line are not in the correct order, swap them
        if (nextPoint != nextLine.begin())
            std::iter_swap(nextLine.begin(), nextLine.begin() + 1);
        
        // move the element we found to the current position k + 1
        size_t i = std::distance(lines.begin(), closest);
        std::iter_swap(lines.begin() + k + 1, lines.begin() + i);
    }
}