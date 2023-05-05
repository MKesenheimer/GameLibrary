#include "collision.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

bool collision::checkCollision(const object& o1, const object& o2) {
    // if objects consists of no points, check their dimensions
    if (o1.npoints() == 0 && o2.npoints() == 0) {
        float dim1 = dim(o1);
        float dim2 = dim(o2);
        if(dim1 + dim2 >= dist(o1, o2)) {
          return true;
        } else {
          return false;
        }
    // else, do a more sophisticated collision control
    } else {
        bool collided = false;
        // first check: if the sum of the dimensions of the objects is less
        // then their distance from center to center -> enable advanced collision detection
        const float dim1 = dim(o1);
        const float dim2 = dim(o2);
        if(dim1 + dim2 >= dist(o1, o2)) {
            // advanced collision control
            if(dim1 <= dim2) {
                // start of line between two points
                types::point<float> start = o2.getCenter();
                for (int i = 0; i < o2.npoints(); ++i) {
                    // end of line
                    types::point<float> end = o2.getPoint(i);
                    // number of points on line
                    const int nsteps = (int)(10 * dist(start, end) / dim1);
                    for (int j = 0; j < nsteps; ++j) {
                        //build new points that lay on the line
                        float x = start.x + j * (end.x - start.x) / nsteps;
                        float y = start.y + j * (end.y - start.y) / nsteps;
                        types::xypoint<float> check = {x, y};
                        if(dist(o1, check) <= dim1) collided = true;
                    }
                    start = end;
                }
            } else {
                // start of line between two points
                types::point<float> start = o1.getCenter();
                for (int i = 0; i < o1.npoints(); ++i) {
                    // end of line
                    types::point<float> end = o1.getPoint(i);
                    // number of points on line
                    const int nsteps = (int)(10 * dist(start, end) / dim2);
                    for (int j = 0; j < nsteps; ++j) {
                        // build new points that lay on the line
                        float x = start.x + j * (end.x - start.x) / nsteps;
                        float y = start.y + j * (end.y - start.y) / nsteps;
                        types::xypoint<float> check = {x, y};
                        if(dist(o2, check) <= dim2) collided = true;
                    }
                    start = end;
                }
            }
        }
        return collided;
    }
}

float collision::dist(const object& o1, const object& o2) {
    const types::xypoint<float>& cen1 = o1.getCenterXY();
    const types::xypoint<float>& cen2 = o2.getCenterXY();
    // calculate the distance of the two objects
    const float dist = sqrt(pow(cen1.first - cen2.first, 2) + pow(cen1.second - cen2.second, 2));
    return dist;
}

float collision::dist(const object& o, const types::xypoint<float>& point) {
    const types::xypoint<float>& cen = o.getCenterXY();
    const float dist = sqrt(pow(cen.first - point.first, 2) + pow(cen.second - point.second, 2));
    return dist;
}

float collision::dist(const object& o, const types::point<float>& point) {
    const types::xypoint<float>& cen = o.getCenterXY();
    const float dist = sqrt(pow(cen.first - point.x, 2) + pow(cen.second - point.y, 2));
    return dist;
}

float collision::dist(const types::point<float>& point0, const types::point<float>& point1) {
    const float dist = sqrt(pow(point0.x - point1.x, 2) + pow(point0.y - point1.y, 2));
    return dist;
}

float collision::dim(const object& o) {
    if (o.npoints() == 0) {
        return o.hsize() / 2;
    } else {
        types::xypoint<float> r0 = o.getCenterXY();
        float rAbs = 0;
        for(int i = 0; i < o.npoints(); ++i) {
            if(o.isCollidable(i)) {
                types::point<float> r = o.getPoint(i);
                float x = r.x - r0.first;
                float y = r.y - r0.second;
                if(rAbs <= sqrt(pow(x, 2) + pow(y, 2)))
                    rAbs = sqrt(pow(x, 2) + pow(y, 2));
            }
        }
        return rAbs;
    }
}