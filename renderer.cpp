#include "renderer.h"
#include <SDL2_gfxPrimitives.h>
#include <iostream>
#include "algorithms.h"

int renderer::screen_width = 900;
int renderer::screen_height = 800;

void renderer::drawObject(const object& object, SDL_Renderer *ren) {
    if (object.npoints() == 0) {
        // if the object consists only of one point, draw a filled circle
        types::point point = object.getCenter();
        filledEllipseRGBA(ren, (int)object.x(), (int)object.y(), (int)(object.hsize() / 2), (int)(object.vsize() / 2), point.r, point.g, point.b, point.a);
    } else {
        types::point point = object.getPoint(0);
        int xp_old = (int)(point.x);
        int yp_old = (int)(point.y);
        for (int i = 1; i < object.npoints(); ++i) {
            point = object.getPoint(i);
            lineRGBA(ren, (int)point.x, (int)point.y, xp_old, yp_old, point.r, point.g, point.b, point.a);
            xp_old = (int)point.x;
            yp_old = (int)point.y;
        }
    }
}

void renderer::setDimensions(int width, int height) {
  screen_width = width;
  screen_height = height;
}

float renderer::transform(float x, float x1, float x2, float y1, float y2) {
    if (x2 != x1)
        return (((y2 - y1) * x + x2 * y1 - x1 * y2) / (x2 - x1));
    return 0;
}

#ifdef LUMAX_OUTPUT
void renderer::drawObject(const object& object, lumaxRenderer& ren) {
    // TODO: if objects consists only of one point (see above)
    if (object.xcenter() >= 0 && object.xcenter() <= screen_width &&
        object.ycenter() >= 0 && object.ycenter() <= screen_height) {
        types::point point = object.getPoint(0);
        float xp_old = point.x;
        float yp_old = point.y;
        addPoint(ren, xp_old, yp_old, 0, 0, 0, ren.parameters.scalingX, ren.parameters.scalingY); // start with a dark point
        for (int i = 0; i < object.npoints(); ++i) {
            point = object.getPoint(i);
            // Lumax has a 2^8 = 256 * 256 = 65536 color range
            addPoint(ren, point.x, point.y, point.r * 256, point.g * 256, point.b * 256, ren.parameters.scalingX, ren.parameters.scalingY);
            xp_old = point.x;
            yp_old = point.y;
        }
        addPoint(ren, xp_old, yp_old, 0, 0, 0, ren.parameters.scalingX, ren.parameters.scalingY); // end with a dark point
    }
}

void renderer::drawPoints(std::vector<types::point<float>>& points, lumaxRenderer& ren) {
    applyColorCorrection(ren, points);
    float xp_old = screen_width / 2;
    float yp_old = screen_height / 2;
    addPoint(ren, xp_old, yp_old, 0, 0, 0, ren.parameters.scalingX, ren.parameters.scalingY); // start with a dark point
    for (const types::point<float>& point : points) {
        // Lumax has a 2^8 = 256 * 256 = 65536 color range
        addPoint(ren, point.x, point.y, point.r * 256, point.g * 256, point.b * 256, ren.parameters.scalingX, ren.parameters.scalingY);
        xp_old = point.x;
        yp_old = point.y;
    }
    addPoint(ren, xp_old, yp_old, 0, 0, 0, ren.parameters.scalingX, ren.parameters.scalingY); // end with a dark point
}

int renderer::sendPointsToLumax(void *lumaxHandle, lumaxRenderer& ren, int scanSpeed) {
    if (!lumaxHandle) return -1;
    //lumax_verbosity |= DBWAITFORBUFFER;
    size_t numOfPoints = ren.points.size();
    TLumax_Point points[numOfPoints];
    for (int i = 0; i < numOfPoints; ++i) {
        points[i].Ch1 = static_cast<int>(ren.points[i].x);
        points[i].Ch2 = static_cast<int>(ren.points[i].y);
        points[i].Ch3 = ren.points[i].r;
        points[i].Ch4 = ren.points[i].g;
        points[i].Ch5 = ren.points[i].b;
        points[i].Ch8 = 0;
        points[i].Ch6 = 0;
        points[i].Ch7 = 0;
    }
    int result = Lumax_SendFrame(lumaxHandle, points, numOfPoints, scanSpeed, 0, NULL);
    int TimeToWait, BufferChanged;
    result = Lumax_WaitForBuffer(lumaxHandle, 17, &TimeToWait, &BufferChanged);
    //std::cout << "TimeToWait = " << TimeToWait << std::endl;
    ren.points.clear();
    return 0;
}

void renderer::applyColorCorrection(const lumaxRenderer& ren, std::vector<types::point<float>>& points) {
    for (auto& p : points) {
        // do color correction only if at least one laser is on
        // (if all lasers are off -> blank move, does not need correction)
        if (p.r != 0 || p.g != 0 || p.b != 0) {
            //std::cout << "red in = " << p.r;
            p.r = colorPolynom(p.r, ren.parameters.colorCorr.ar, ren.parameters.colorCorr.br, ren.parameters.colorCorr.cr);
            //std::cout << ", red out = " << p.r << std::endl;
            p.g = colorPolynom(p.g, ren.parameters.colorCorr.ag, ren.parameters.colorCorr.bg, ren.parameters.colorCorr.cg);
            p.b = colorPolynom(p.b, ren.parameters.colorCorr.ab, ren.parameters.colorCorr.bb, ren.parameters.colorCorr.cb);
        }
    }
}

void renderer::addPoint(lumaxRenderer& ren, float x, float y, int r, int g, int b, float xScaling, float yScaling) {
    const float mid = ren.parameters.maxPositions / 2;
    if (ren.parameters.swapXY == 1) {
      int temp = y;
      y = x;
      x = temp;
    }
    float xl = transform(x, 0, screen_width, mid - xScaling * ren.parameters.mirrorFactX * ren.parameters.maxPositions / 2, mid + xScaling * ren.parameters.mirrorFactX * ren.parameters.maxPositions / 2);
    float yl = transform(y, screen_height, 0, mid - yScaling * ren.parameters.mirrorFactY * ren.parameters.maxPositions / 2, mid + yScaling * ren.parameters.mirrorFactY * ren.parameters.maxPositions / 2);
    //if (xl >= mid - xScaling * ren.parameters.maxPositions / 2 && xl <= mid + xScaling * ren.parameters.maxPositions / 2 &&
    //    yl >= mid - yScaling * ren.parameters.maxPositions / 2 && yl <= mid + yScaling * ren.parameters.maxPositions / 2)
    ren.points.push_back({xl, yl, r, g, b, 1, false});
}

int renderer::colorPolynom(int value, float a, float b, float c) {
    return algorithms::constrain<int>(a * pow(value, 2.0) + b * value + c, 0, 255);
}
#endif