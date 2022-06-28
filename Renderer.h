/*
 *  Renderer.h
 *  Created by Matthias Kesenheimer on 31.12.20.
 *  Copyright 2020. All rights reserved.
 */
#pragma once
#include <SDL.h>
#include "Object.h"
#include "Point.h"

//#define LUMAX_OUTPUT
#ifdef LUMAX_OUTPUT
extern "C" {
#include "lumax/lumax.h"
}

struct ColorCorrectionParameters {
    float ar = 0.00385236, br = -0.256863, cr = 70;
    float ag = 0.00415225, bg = -0.388235, cg = 84;
    float ab = 0.0032526, bb = -0.205882, cb = 96;
};

struct LumaxParameters {
    // maximum of possible points for the laser output
    int maxPositions = 65536;
    int mirrorFactX = 1; // 1: not mirrored, -1: mirrored
    int mirrorFactY = 1; // 1: not mirrored, -1: mirrored
    float scalingX = 1;
    float scalingY = 1;
    int swapXY = 0;
};

// the Lumax renderer
struct LumaxRenderer {
    std::vector<Point<float>> points;
    LumaxParameters parameters;
    ColorCorrectionParameters colorCorr;
};
#endif

class Renderer {
public:
    // Draw an Object to a SDL_Renderer
    static void drawObject(const Object& object, SDL_Renderer *ren);

    static void setDimensions(int width, int height);

    // Screen dimensions of the SDL screen
    static int screen_width;
    static int screen_height;

    // transform point from [x1, x2] to points in [y1, y2]
    static float transform(float x, float x1, float x2, float y1, float y2);

#ifdef LUMAX_OUTPUT
    // Draw an Object to the Lumax Renderer
    static void drawObject(const Object& object, LumaxRenderer& ren);

    // send a vector of Points to the Lumax Renderers
    static void drawPoints(std::vector<Point<float>>& points, LumaxRenderer& ren);

    // send the points in the buffer to the Lumax device
    static int sendPointsToLumax(void *lumaxHandle, LumaxRenderer& ren, int scanSpeed);

private:
    // function to apply the color polynom to the points
    static void applyColorCorrection(const LumaxRenderer& ren, std::vector<Point<float>>& points);

    // add a point to the Lumax renderer
    static void addPoint(LumaxRenderer& ren, float x, float y, int r, int g, int b, float xScaling, float yScaling);

    static int colorPolynom(int value, float a, float b, float c);
#endif
};