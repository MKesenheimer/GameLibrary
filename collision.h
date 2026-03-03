/*
 *  Collision.h
 *  Created by Matthias Kesenheimer on 11.01.12.
 *  Copyright 2012. All rights reserved.
 */
#pragma once
#include <tuple>
#include "object.h"
#include "point.h"

class collision {
public:
    //this function takes two lists of points and checks if the
    //the objects formed by the points has collided
    //Note: the points should form an convex object at best
    static bool checkCollision(const object& o1, const object& o2);

private:    
    //returns the length of the largest distance from center
    //this assumed to be the dimension of the object
    static float dim(const object& o);
    
    //calculate the distance of two objects
    static float dist(const object& o1, const object& o2);
    static float dist(const object& o, const types::point<float>& point);
    static float dist(const object& o, const types::xypoint<float>& point);
    static float dist(const types::point<float>& point0, const types::point<float>& point1);
};