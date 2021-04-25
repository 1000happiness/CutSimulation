#include "Constraint.h"
#include <iostream>

Constraint::Constraint(std::pair<float, float> x_interval)
{
    this->x_interval = x_interval;
}

Constraint::~Constraint() {

}



SliceConstraint::SliceConstraint(std::pair<float, float> x_interval, float radius, float slice_width): Constraint(x_interval)
{
    for (float x = x_interval.first; x <= x_interval.second; x += slice_width) {
        points.push_back(glm::vec2(x, radius));
    }
}

SliceConstraint::~SliceConstraint()
{

}

void SliceConstraint::setPoints(float x_index, float radius)
{
    float slice_width = points[1].x - points[0].x;
    int index = (x_index - x_interval.first) / slice_width;
    points[index].y = radius;
    return;
}

float SliceConstraint::calculateRadius(float x_index)
{
    float slice_width = points[1].x - points[0].x;
    int index = (x_index - x_interval.first) / slice_width;
    return points[index].y;
}

CylinderConstraint::CylinderConstraint(std::pair<float, float> x_interval, float radius): Constraint(x_interval)
{
    this->radius = radius;
    float t = 0.0f;
    for (int i = 0; i < 101; i += 1)
    {
        points.push_back(glm::vec2(t * (x_interval.second - x_interval.first) + x_interval.first, radius));
        t += 0.01f;
    }
}

CylinderConstraint::~CylinderConstraint()
{

}

float CylinderConstraint::calculateRadius(float x_index)
{
    return radius;
}

BezierConstraint::BezierConstraint(std::pair<float, float> x_interval, std::vector<glm::vec2> control_points): Constraint(x_interval)
{
    if (control_points.size() != 4) 
    {
        std::cerr << "Bezier control points size is not 4" << std::endl;
    }

    float t = 0.0f;
    float delta = 1.0f / points_num;
    for (int i = 0; i < points_num; i += 1)
    {
        float b0 = 1 * (1 - t) * (1 - t) * (1 - t);
        float b1 = 3 * (1 - t) * (1 - t) * t;
        float b2 = 3 * (1 - t) * t       * t;
        float b3 = 1 * t       * t       * t;
        glm::vec2 point(b0 * control_points[0] + b1 * control_points[1] + b2 * control_points[2] + b3 * control_points[3]);
        points.push_back(point);
        //std::cout << point.x << " " << point.y << std::endl;
        t += delta;
    }
}

BezierConstraint::~BezierConstraint()
{

}

float BezierConstraint::calculateRadius(float x_index)
{
    if (points[0].x == x_index) {
        return points[0].y;
    }
    if (points[points_num - 1].x == x_index) {
        return points[1].y;
    }

    int begin = 0;
    int end = points_num;
    int focus = (begin + end) / 2;
    
    while(end - begin > 1) 
    {
        if (points[focus].x < x_index) {
            begin = focus;
            focus = (begin + end) / 2;
        }
        else if (points[focus].x > x_index) {
            end = focus;
            focus = (begin + end) / 2;
        }
        else {
            return points[focus].y;
        }
    }
    return (x_index - points[begin].x) / (points[end].x - points[begin].x) * points[begin].y + (points[end].x - x_index) / (points[end].x - points[begin].x) * points[end].y;
}