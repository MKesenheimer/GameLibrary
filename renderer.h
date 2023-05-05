/*
 *  renderer.h
 *  Created by Matthias Kesenheimer on 31.12.20.
 *  Copyright 2020. All rights reserved.
 */
#pragma once
#include <SDL.h>
#include "object.h"
#include "point.h"

//#define LUMAX_OUTPUT
#ifdef LUMAX_OUTPUT
extern "C" {
#include "lumax/lumax.h"
}
#endif

class renderer {
public:
    // Draw an Object to a SDL_Renderer
    static void drawObject(const object& object, SDL_Renderer *ren);

    static void setDimensions(int width, int height);

    // Screen dimensions of the SDL screen
    static int screen_width;
    static int screen_height;

    // transform point from [x1, x2] to points in [y1, y2]
    static float transform(float x, float x1, float x2, float y1, float y2);

#ifdef LUMAX_OUTPUT
    struct colorCorrectionParameters {
        float ar, br, cr;
        float ag, bg, cg;
        float ab, bb, cb;
    };
    
    struct lumaxParameters {
        // maximum of possible points for the laser output
        int maxPositions = 65536;
        int mirrorFactX = 1; // 1: not mirrored, -1: mirrored
        int mirrorFactY = 1; // 1: not mirrored, -1: mirrored
        float scalingX = 1;
        float scalingY = 1;
        int swapXY = 0;
        colorCorrectionParameters colorCorr = {0, 1, 0, 0, 1, 0, 0, 1, 0};
    };

    // the Lumax renderer
    struct lumaxRenderer {
        std::vector<types::point<float>> points;
        lumaxParameters parameters;
    };

    // Draw an Object to the Lumax Renderer
    static void drawObject(const object& object, lumaxRenderer& ren);

    // send a vector of Points to the Lumax Renderers
    static void drawPoints(std::vector<types::point<float>>& points, lumaxRenderer& ren);

    // send the points in the buffer to the Lumax device
    static int sendPointsToLumax(void *lumaxHandle, lumaxRenderer& ren, int scanSpeed);

private:
    // function to apply the color polynom to the points
    static void applyColorCorrection(const lumaxRenderer& ren, std::vector<types::point<float>>& points);

    // add a point to the Lumax renderer
    static void addPoint(lumaxRenderer& ren, float x, float y, int r, int g, int b, float xScaling, float yScaling);

    static int colorPolynom(int value, float a, float b, float c);
#endif
};