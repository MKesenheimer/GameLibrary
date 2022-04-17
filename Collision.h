/*
 *  Collision.h
 *  Created by Matthias Kesenheimer on 11.01.12.
 *  Copyright 2012. All rights reserved.
 */
#pragma once
#include <tuple>
#include "Object.h"
#include "Point.h"

class Collision {
public:
    //this function takes two lists of points and checks if the
    //the objects formed by the points has collided
    //Note: the points should form an convex object at best
    static bool checkCollision(const Object& o1, const Object& o2);

private:
    //calculates the center of the object formed by the points
    // returns the x- and y- coordinate in a vector
    static Point center(const Object& o);
    
    //returns the length of the largest distance from center
    //this assumed to be the dimension of the object
    static float dim(const Object& o);
    
    //calculate the distance of two objects
    static float dist(const Object& o1, const Object& o2);
    static float dist(const Object& o, const IndexedPoint& point);
    static float dist(const Object& o, const Point& point);
    static float dist(const IndexedPoint& point0, const IndexedPoint& point1);
};