// main source-code file
#include <cmath>

typedef enum Orientation
{
    horizontal,
    vectical
} Orientation;

typedef enum Operator
{
    eq, // equal
    ne, // not equal
    gt, // greater than
    ge, // greater than or equal
    lt, // less than
    le, // less than or equal
} Operator;

typedef struct Vec2
{
    double y;
    double x;

    // calculate distance to point (0.0 ,0.0)
    double distance() // root(x^2 + y^2)
    {
        return std::sqrt((std::pow(x, 2) + std::pow(y, 2)));
    }

    // return value of x or y based on the orientation that we want
    double value(Orientation orientation)
    {
        if (orientation == Orientation::horizontal)
        {
            return x;
        }
        else // if orientation is vertical
        {
            return y;
        }
    }

    // return the angle of the vector in radians between interval [0; 2pi]
    double angle()
    {
        double PI = std::acos(-1);
        double angle = std::atan2(x, y);
        if (angle < 0) // if angle is negative -> add 2pi
        {
            angle += 2 * PI;
        }
        return angle;
    }
} Vec2;

template <typename T>
bool cmp(T element1, T element2, Operator opp)
{
    switch (opp)
    {
    case Operator::eq:
        return element1 == element2;

    case Operator::ne:
        return element1 != element2;

    case Operator::gt:
        return element1 > element2;

    case Operator::ge:
        return element1 >= element2;

    case Operator::lt:
        return element1 < element2;

    case Operator::le:
        return element1 <= element2;
    }
}

bool cmp_distance(Vec2 vector1, Vec2 vector2, Operator opp)
{
    double d1 = vector1.distance();
    double d2 = vector2.distance();
    return cmp(d1, d2, opp);
}

bool cmp_angle(Vec2 vector1, Vec2 vector2, Operator opp)
{
    double a1 = vector1.angle();
    double a2 = vector2.angle();
    return cmp(a1, a2, opp);
}

bool cmp_value(Vec2 vector1, Vec2 vector2, Operator opp, Orientation orientation)
{
    double v1 = vector1.value(orientation);
    double v2 = vector2.value(orientation);
    return cmp(v1, v2, opp);
}