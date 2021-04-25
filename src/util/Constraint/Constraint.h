#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <vector>

#include <glm/vec2.hpp>

class Constraint
{
public:
    std::vector<glm::vec2> points;
    std::pair<float, float> x_interval;
    Constraint(std::pair<float, float> x_interval);
    ~Constraint();
    virtual float calculateRadius(float x_index) = 0;
};

class SliceConstraint : public Constraint
{
public:
    SliceConstraint(std::pair<float, float> x_interval, float radius, float slice_width);
    ~SliceConstraint();
    float calculateRadius(float x_index);
    void setPoints(float x_index, float radius);
};

class CylinderConstraint: public Constraint
{
public:
    float radius;
    CylinderConstraint(std::pair<float, float> x_interval, float radius);
    ~CylinderConstraint();
    float calculateRadius(float x_index);
};

class BezierConstraint : public Constraint
{
public:
    BezierConstraint(std::pair<float, float> x_interval, std::vector<glm::vec2> control_points);
    ~BezierConstraint();
    float calculateRadius(float x_index);
private:
    const static int points_num = 1000;
};

#endif // !CONSTRAINT_H
