// main source-code file
#include <cmath>
#include <algorithm>
#include <vector>

enum Orientation
{
    horizontal,
    vectical
};

enum Operator
{
    eq, // equal
    ne, // not equal
    gt, // greater than
    ge, // greater than or equal
    lt, // less than
    le  // less than or equal
};

enum Figure
{
    None,
    Circle,
    Square,
    Triangle,
    Rectangle,
    Pentagon,
};

template <typename T>
bool cmp(T element1, T element2, Operator opp)
{
    switch (opp)
    {
    case Operator::eq:
        if ((double)element1 && (double)element2)
        {
            return std::abs(element1 - element2) = < 0.000001;
        }
        return element1 == element2;

    case Operator::ne:
        if ((double)element1 && (double)element2)
        {
            return std::abs(element1 - element2) > 0.000001;
        }
        return element1 != element2;

    case Operator::gt:
        if ((double)element1 && (double)element2)
        {
            return element1 - element2 > 0.000001;
        }
        return element1 > element2;

    case Operator::ge:
        if ((double)element1 && (double)element2)
        {
            return element1 - element2 >= -0.000001;
        }
        return element1 >= element2;

    case Operator::lt:
        if ((double)element1 && (double)element2)
        {
            return element2 - element1 > 0.000001;
        }
        return element1 < element2;

    case Operator::le:
        if ((double)element1 && (double)element2)
        {
            return element2 - element1 >= -0.000001;
        }
        return element1 <= element2;
    }
}

// function to measure directional similarity between two vectors
double dot_product(const Vec2 &v1, const Vec2 &v2)
{
    return v1.x * v2.x + v1.y * v2.y; // x1*x2 + y1*y2
}

double normalize_angle(double angle)
{
    double TWO_PI = 2.0 * std::acos(-1.0);
    while (angle < 0)
        angle += TWO_PI;
    while (angle >= TWO_PI)
        angle -= TWO_PI;
    return angle;
}

struct Vec2
{
    double y{};
    double x{};

    // calculate distance to point source (0.0 ,0.0)
    double distance_from_source() // root(x^2 + y^2)
    {
        return std::sqrt(x * x + y * y);
    }

    // calculate the magnitude
    double magnitude()
    {
        return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
    }

    // Calculate distance between two Vec2 (positions)
    double distance(const Vec2 other)
    {
        return (*this - other).distance_from_source();
    }

    // returns a vector with the same direction as the original, but with length 1.
    Vec2 normalize()
    {
        double length = distance_from_source();
        if (length == 0.0) // division by 0
        {
            return Vec2{0, 0};
        }
        return (*this / length);
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

    // return the angle of the vector in radians between interval [0; 2pi}
    double angle()
    {
        if (x == 0 && y == 0)
        {
            return NULL;
        } // undefined -> no direction, so no angle either.
        double PI = std::acos(-1);
        double angle = std::atan2(y, x);
        if (angle < 0) // if angle is negative -> add 2pi
        {
            angle += 2 * PI;
        }
        return angle;
    }

    Vec2 Vproject(Vec2 &dest) const
    {
        return dest * (dot_product(this, dest) / dest.magnitude());
    }

    double oneDproject(const Vec2 &dest) const
    {
        return dot_product(this, dest);
    }

    // Operator overloading
    Vec2 operator+(const Vec2 &v) const { return {y + v.y, x + v.x}; };
    Vec2 operator-(const Vec2 &v) const { return {y - v.y, x - v.x}; };
    Vec2 operator*(double s) const { return {y * s, x * s}; };
    Vec2 operator/(double s) const { return {y / s, x / s}; };
    Vec2 &operator+=(const Vec2 &v)
    {
        x += v.x;
        y += v.y;
        return *this;
    };
    Vec2 &operator-=(const Vec2 &v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    };
    Vec2 &operator*=(double s)
    {
        x *= s;
        y *= s;
        return *this;
    };
};

bool cmp_distance(Vec2 vector1, Vec2 vector2, Operator opp) { return cmp(vector1.distance_from_source(), vector2.distance_from_source(), opp); }

bool cmp_angle(Vec2 vector1, Vec2 vector2, Operator opp) { return cmp(vector1.angle(), vector2.angle(), opp); }

bool cmp_value(Vec2 vector1, Vec2 vector2, Operator opp, Orientation orientation) { return cmp(vector1.value(orientation), vector2.value(orientation), opp); }

struct Dynamics
{
    // Data needed for dynamics
    double m_mass{};
    Figure figure{};

    // Linear movement
    Vec2 m_position{};
    Vec2 m_velocity{};
    Vec2 m_acceleration{};
    Vec2 m_force{};

    // Rotating
    double m_angle{};
    double m_inertia{};
    double m_angvelocity{};
    double m_torque{};
};

// Superclass Shape
class Shape
{
public:
    // declarations
    Shape(Dynamics state) : m_state{state} {};       // Main constructor for all shapes
    virtual ~Shape() = default;                      // destructor
    Shape(Shape &other) : m_state{other.m_state} {}; // copy constructor
    Shape(Shape &&source) : m_state{source.m_state}  // move constructor
    {
        Vec2 zero{0, 0};
        source.m_state.m_position = zero;
        source.m_state.m_velocity = zero;
        source.m_state.m_mass = 0.0;
    }
    Shape &operator=(const Shape &) = default; // copy assignment
    Shape &operator=(Shape &&) = default;      // move assignment

    // Getters
    double getMass() const { return m_state.m_mass; }
    Figure getFigure() const { return m_state.figure; }
    Vec2 getPos() const { return m_state.m_position; }
    Vec2 getVel() const { return m_state.m_velocity; }
    Vec2 getAcc() const { return m_state.m_acceleration; }
    Vec2 getForce() const { return m_state.m_force; }
    double getAngle() const { return m_state.m_angle; }
    double getInertia() const { return m_state.m_inertia; }
    double getAngVel() const { return m_state.m_angvelocity; }
    double getTorque() const { return m_state.m_torque; }
    virtual double getSize() const {};

    virtual double get_distance_MiddleToSide(double angle) const = 0;
    virtual std::vector<Vec2> getVertices() const = 0;
    std::vector<Vec2> getEdgeVectors() const
    {
        if (getFigure() == Figure::Circle)
        {
            return {}; // A circle doesnt have edge vectors
        }
        std::vector<Vec2> edges{};
        std::vector<Vec2> vertices{getVertices()};

        int amount{vertices.size()};
        for (int i = 0; i < amount; i++)
        {
            Vec2 first{vertices[i]};
            Vec2 next{vertices[(i + 1) % amount]}; // We take the next (if the last, we take the first)
            Vec2 edge = next - first;
            edges.push_back(edge);
        }
        // No check of coordinates needed, because the vertices are stored counter clockwise.
        return edges;
    }
    std::vector<Vec2> getNormals() const
    {
        std::vector<Vec2> normals{};
        std::vector<Vec2> to_be_normalized{getEdgeVectors()};

        auto getnormal = [](Vec2 vector) -> Vec2
        {
            Vec2 normal{};
            normal.x = -vector.y;
            normal.y = vector.x;
            // let vector be (x, y). Then the normal is either (-y, x) or (y, -x)
            return normal;
        };

        for (const auto &v : to_be_normalized)
        {
            normals.push_back(getnormal(v).normalize());
        }

        return normals;
    }
    bool collides(const Shape &other) const
    {
        // If both are circles. The logic is simple
        if (this->getFigure() == Figure::Circle && other.getFigure() == Figure::Circle)
        {
            double r1 = this->getSize();
            double r2 = other.getSize();
            Vec2 pos1 = this->getPos();
            Vec2 pos2 = other.getPos();
            if (pos1.distance(pos2) <= r1 + r2)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        std::vector<Vec2> normals{getNormals()};
        std::vector<Vec2> normals2{other.getNormals()};
        normals.insert(normals.end(), normals2.begin(), normals2.end()); // Make 1 big list of normal vectors.
        // If either one is a circle we have to change the logic a bit.
        const std::vector<Vec2> vertices{getVertices()};
        const std::vector<Vec2> vertices2{other.getVertices()};
        if ((this->getFigure() == Figure::Circle && other.getFigure() != Figure::Circle) || (this->getFigure() != Figure::Circle && other.getFigure() == Figure::Circle))
        {
            Vec2 closestVertex{};
            Vec2 pos1 = this->getPos();
            Vec2 pos2 = other.getPos();
            auto findClosest = [](const std::vector<Vec2> &vertices, Vec2 pos) -> Vec2
            {
                Vec2 closest{};
                double dclosest{};
                if (vertices.size())
                {
                    closest = vertices[0];
                    dclosest = closest.distance(pos);
                }
                for (const Vec2 &v : vertices)
                {
                    double dcurrent = pos.distance(v);
                    if (dcurrent < dclosest)
                    {
                        closest = v;
                        dclosest = dcurrent;
                    }
                }
                return closest;
            };
            Vec2 extraVector{};
            if (this->getFigure() != Figure::Circle)
            {
                closestVertex = findClosest(vertices, pos2);
                extraVector = pos2 - closestVertex;
            }
            else
            {
                closestVertex = findClosest(vertices2, pos1);
                extraVector = pos1 - closestVertex;
            }
            normals.push_back(extraVector.normalize());
        }
        for (const Vec2 &v : normals)
        {
            double minA, maxA, minB, maxB;

            project_and_find(*this, vertices, v, minA, maxA);
            project_and_find(other, vertices2, v, minB, maxB);

            if (maxA < minB || maxB < minA)
            {
                return false;
            }
        }
        return true;
    }
    void project_and_find(const Shape &shape, const std::vector<Vec2> &vertices, const Vec2 &v, double &min, double &max) const
    {
        if (shape.getFigure() == Figure::Circle)
        {
            // With circles the logic is a bit different
            Vec2 pos = shape.getPos();
            min = pos.oneDproject(v) - shape.getSize();
            max = pos.oneDproject(v) + shape.getSize();

            return;
        }
        bool maxFirst = true;
        bool minFirst = true;

        for (const Vec2 &vertex : vertices)
        {
            // We find the minimum and maximum projections as our "edges" of our shadows.
            double projection = vertex.oneDproject(v);
            if (projection > max || maxFirst)
            {
                max = projection;
                maxFirst = false;
            }
            if (projection < min || minFirst)
            {
                min = projection;
                minFirst = false;
            }
        }
    }

private:
    Dynamics m_state; // Struct that contains data useful for the shapes' movement
};

// function to use between shape-objects
/*
bool collides(const Shape &shape_A, const Shape &shape_B) // Collision checker for all shapes
{
    Vec2 AB = shape_B.getPos() - shape_A.getPos();
    double AB_len = AB.distance_from_source();
    if (AB_len == 0.0)
    {
        return true;
    }
    double AB_angle = AB.angle();
    double PI = std::acos(-1);
    double BA_angle;
    if (AB_angle <= PI)
    {
        BA_angle = AB_angle + PI;
    }
    else
    {
        BA_angle = AB_angle - PI;
    }
    double len_A = shape_A.get_distance_MiddleToSide(normalize_angle(AB_angle - shape_A.getAngle()));
    double len_B = shape_B.get_distance_MiddleToSide(normalize_angle(BA_angle - shape_B.getAngle()));
    if (len_A + len_B < AB_len)
    {
        return false;
    }
    return true;
}
*/

// Circle Class
class Circle : public Shape
{
public:
    // declarations
    Circle(Dynamics state) : Shape(state), size{size} {}; // constructor
    ~Circle() = default;                                  // destructor
    Circle(Circle &other) = default;                      // copy constructor
    Circle(Circle &&source) = default;                    // move constructor
    Circle &operator=(const Circle &other) = default;     // copy assignment
    Circle &operator=(Circle &&source) = default;         // move assignement

    // Shape-specific functions
    Figure getFigure() { return Figure::Circle; }
    double getSize() { return size; };
    double get_distance_MiddleToSide(double angle) const override { return size; } // for a Circle it's easy, the distance stays the same for each angle
    double getArea() const { return std::acos(-1) * size * size; }                 // area = pi * radius * radius
    std::vector<Vec2> getVertices() const override
    {
        std::vector<Vec2> vertices{};
        vertices.push_back(getPos());
        return vertices;
    }

private:
    double size{};
};

// Square class
class Square : public Shape
{
public:
    // declarations
    Square(Dynamics state) : Shape(state), size{size} {}; // constructor
    ~Square() = default;                                  // destructor
    Square(Square &other) = default;                      // copy constructor
    Square(Square &&source) = default;                    // move constructor
    Square &operator=(const Square &) = default;          // copy assignment
    Square &operator=(Square &&) = default;               // move assignment

    // Shape-specific functions
    Figure getFigure() { return Figure::Square; }
    double getSize() { return size; }
    double get_distance_MiddleToSide(double angle) const override { return size / std::max(std::abs(std::sin(angle)), std::abs(std::cos(angle))); }
    double getArea() { return std::pow(size * 2, 2); } // area = edge^2 = (size*2)^2
    std::vector<Vec2> getVertices() const override
    {
        std::vector<Vec2> vertices{};
        Vec2 position = getPos();

        double cos = std::cos(getAngle());
        double sin = std::sin(getAngle());
        // lambda function to account for the rotation of the shape
        auto rotate_and_translate = [position, cos, sin](double this_x, double this_y) -> Vec2
        {
            Vec2 vertex;

            vertex.x = position.x + (this_x * cos - this_y * sin); // x' = xposition + (x * cos(angle) - y*sin(angle))
            vertex.y = position.y + (this_x * sin + this_y * cos); // y' = yposition + (x * sin(angle) + y * cos(angle))

            return vertex;
        };

        vertices.push_back(rotate_and_translate(-size, -size)); // left bottom
        vertices.push_back(rotate_and_translate(-size, size));  // left top
        vertices.push_back(rotate_and_translate(size, size));   // right top
        vertices.push_back(rotate_and_translate(size, -size));  // right bottom

        return vertices;
    }

private:
    double size{};
};

// Triangle class
class Triangle : public Shape
{
public:
    // declarations
    Triangle(Dynamics state, double size) : Shape(state), size{size} {}; // constructor
    ~Triangle() = default;                                               // destructor
    Triangle(Triangle &other) = default;                                 // copy constructor
    Triangle(Triangle &&source) = default;                               // move constructor
    Triangle &operator=(const Triangle &) = default;                     // copy assignment
    Triangle &operator=(Triangle &&) = default;                          // move assignment

    // Shape-specific functions
    Figure getFigure() { return Figure::Triangle; }
    double getSize() { return size; }
    double get_distance_MiddleToSide(double angle) const override
    {
        const double period = 2.0 * std::acos(-1.0) / 3.0;
        return size / std::cos(std::remainder(angle, period));
    }
    double getArea() const { return 3.0 * std::sqrt(3.0) / 4.0 * (size * size); } // area = edge^2 = ((size*2)^2)/2
    std::vector<Vec2> getVertices() const override
    {
        std::vector<Vec2> vertices;
        Vec2 position = getPos();

        double cos = std::cos(getAngle());
        double sin = std::sin(getAngle());
        // lambda function to account for the rotation of the shape
        auto rotate_and_translate = [position, cos, sin](double this_x, double this_y) -> Vec2
        {
            Vec2 vertex{};

            vertex.x = position.x + (this_x * cos - this_y * sin); // x' = xposition + (x * cos(angle) - y*sin(angle))
            vertex.y = position.y + (this_x * sin + this_y * cos); // y' = yposition + (x * sin(angle) + y * cos(angle))

            return vertex;
        };

        double root3 = std::sqrt(3);

        vertices.push_back(rotate_and_translate(size, 0));                          // right
        vertices.push_back(rotate_and_translate(-size / 2.0, root3 * size / 2.0));  // top
        vertices.push_back(rotate_and_translate(-size / 2.0, root3 * -size / 2.0)); // bottom

        return vertices;
    }

private:
    double size{};
};

// Pentagon class
class Pentagon : public Shape
{
    // declarations
    Pentagon(Dynamics state, Vec2 dimensions) : Shape(state), size{size} {}; // constructor
    ~Pentagon() = default;                                                   // destructor
    Pentagon(Pentagon &other) = default;                                     // copy constructor
    Pentagon(Pentagon &&source) = default;                                   // move constructor
    Pentagon &operator=(const Pentagon &) = default;                         // copy assignment
    Pentagon &operator=(Pentagon &&) = default;                              // move assignment

    // shape-specific functions
    Figure getFigure() { return Figure::Pentagon; }
    double getSize() { return size; }
    double get_distance_MiddleToSide(double angle) const override
    {
        // we still have to implement this
    }
    double getArea() const
    {
        // we still have to implement this
    }

private:
    double size{};
};

// Rectangle class
class Rectangle : public Shape
{
public:
    // declarations
    Rectangle(Dynamics state, Vec2 dimensions) : Shape(state), dimensions{dimensions} {}; // constructor
    ~Rectangle() = default;                                                               // destructor
    Rectangle(Rectangle &other) = default;                                                // copy constructor
    Rectangle(Rectangle &&source) = default;                                              // move constructor
    Rectangle &operator=(const Rectangle &) = default;                                    // copy assignment
    Rectangle &operator=(Rectangle &&) = default;                                         // move assignment

    // Shape-specific functions
    Figure getFigure() { return Figure::Rectangle; }
    Vec2 getDimensions() { return dimensions; }
    double get_distance_MiddleToSide(double angle) const override
    {
        // we still have to implement this
    }
    double getArea() const { return dimensions.y * dimensions.x; } // area = base * height

    std::vector<Vec2> getVertices() const override
    {
        std::vector<Vec2> vertices{};
        Vec2 position = getPos();

        double cos = std::cos(getAngle());
        double sin = std::sin(getAngle());

        // lambda function to account for the rotation of the shape
        auto rotate_and_translate = [position, cos, sin](double this_x, double this_y) -> Vec2
        {
            Vec2 vertex{};

            vertex.x = position.x + (this_x * cos - this_y * sin); // x' = xposition + (x * cos(angle) - y*sin(angle))
            vertex.y = position.y + (this_x * sin + this_y * cos); // y' = yposition + (x * sin(angle) + y * cos(angle))

            return vertex;
        };
        vertices.push_back(rotate_and_translate(-(dimensions.x / 2.0), -(dimensions.y / 2.0))); // left bottom
        vertices.push_back(rotate_and_translate(-(dimensions.x / 2.0), (dimensions.y / 2.0)));  // left top
        vertices.push_back(rotate_and_translate((dimensions.x / 2.0), (dimensions.y / 2.0)));   // right top
        vertices.push_back(rotate_and_translate((dimensions.x / 2.0), -(dimensions.y / 2.0)));  // right bottom

        return vertices;
    }

private:
    Vec2 dimensions{};
};
