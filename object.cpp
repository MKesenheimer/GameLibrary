#include "object.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>

object::object(float x, float y, float vx, float vy, float hsize, float vsize, float angle, float spin, int mirrorX, int mirrorY) :
    m_x(x), m_y(y), m_vx(vx), m_vy(vy), m_hsize(hsize), m_vsize(vsize), m_phi(angle), m_oldPhi(0.0), m_npoints(0),
    m_spin(spin), m_mirrorX(mirrorX), m_mirrorY(mirrorY) {}

float object::x() const {
    return m_x;
}

float object::y() const {
    return m_y;
}

float object::xcenter() const {
    return m_x;
}

float object::ycenter() const {
    return m_y;
}

types::xypoint<float> object::getCenterXY() const {
    return {m_x, m_y};
}

types::point<float> object::getCenter() const {
    return {m_x, m_y, 255, 255, 255, 255, true};
}

float object::vx() const {
    return m_vx;
}

float object::vy() const {
    return m_vy;
}

float object::phi() const {
    return m_phi;
}

float object::spin() const {
    return m_spin;
}

float object::hsize() const {
    return m_hsize;
}

float object::vsize() const {
    return m_vsize;
}

int object::npoints() const {
    return m_npoints;
}

void object::setPos(float x, float y) {
    m_x = x;
    m_y = y;
}

void object::setv(float vx, float vy) {
    m_vx = vx;
    m_vy = vy;
}

void object::setSize(float hsize, float vsize) {
    m_hsize = hsize;
    m_vsize = vsize;
}

void object::setAngle(float angle) {
    m_phi = angle;
    // this algorithm rotates the object every time it is called
    // so we have to rotate only by the difference of the old and the
    // new angle
    float dphi = (m_phi - m_oldPhi); //angle in rad /360.0*(2*M_PI)
    for (int i = 0; i < m_npoints; ++i) {
        // rotate all m_points
        float xfs = m_points[i].x;
        float yfs = m_points[i].y;
        m_points[i].x = std::cos(dphi) * xfs - std::sin(dphi) * yfs;
        m_points[i].y = std::sin(dphi) * xfs + std::cos(dphi) * yfs;
    }
    m_oldPhi = m_phi;
}

void object::setSpin(float spin) {
    m_spin = spin;
}

void object::newPoint(float x, float y, bool iscol) {
    newPoint(x, y, 255, 255, 255, 255, iscol);
}

void object::newPoint(float x, float y, int r, int g, int b, int a, bool iscol) {
    m_points.push_back(types::point<float>());
    // move the points into the world coordinate system
    m_points[m_npoints].x = m_hsize * m_mirrorX * x * std::cos(m_phi) - m_hsize * m_mirrorY * y * std::sin(m_phi);
    m_points[m_npoints].y = m_hsize * m_mirrorX * x * std::sin(m_phi) + m_hsize * m_mirrorY * y * std::cos(m_phi);
    m_points[m_npoints].r = r;
    m_points[m_npoints].g = g;
    m_points[m_npoints].b = b;
    m_points[m_npoints].a = a;
    m_points[m_npoints].iscollidable = iscol;
    m_npoints++;
}

types::xypoint<float> object::getPointXY(int n) const {
    types::xypoint<float> point;
    if (n >= 0 && n < m_npoints) {
        point.first = m_points[n].x + m_x;
        point.second = m_points[n].y + m_y;
        return point;
    }
    std::cout << "an error occured in object.cpp: n = " << n << " is not a valid index" << std::endl;
    return point;
}

types::point<float> object::getPoint(int n) const {
    types::point<float> point;
    if (n >= 0 && n < m_npoints) {
        point = m_points[n];
        point.x = point.x + m_x;
        point.y = point.y + m_y;
        return point;
    }
    std::cout << "an error occured in object::getPoint: n = " << n << " is not a valid index" << std::endl;
    return point;
}


bool object::isCollidable(int n) const {
    if (n >= 0 && n < m_npoints)
        return m_points[n].iscollidable;
    std::cout << "an error occured in object::isCollidable: n = " << n << " is not a valid index" << std::endl;
    return true;
}

void object::modifyPoint(float x, float y, int n) {
    if (n >= 0 && n < m_npoints) {
        m_points[n].x = m_hsize * x;
        m_points[n].y = m_vsize * y;
        return;
    }
    std::cout << "an error occured in object::modifyPoint: n = " << n << " is not a valid index" << std::endl;
    return;
}

void object::updatePosition(float dt) {
    m_x = m_x + m_vx * dt;
    m_y = m_y + m_vy * dt;
    m_phi = m_phi + m_spin * dt;
    setAngle(m_phi);
}