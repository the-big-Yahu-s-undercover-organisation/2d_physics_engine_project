// main source-code file
#include <cmath>

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

// function to measure directional similarity between two vectors
double dot_product(const Vec2 &v1, const Vec2 &v2)
{
    return v1.x * v2.x + v1.y * v2.y; // x1*x2 + y1*y2
}

struct Vec2
{
    double y{};
    double x{};

    // calculate distance to point source (0.0 ,0.0)
    double distance_from_source() // root(x^2 + y^2)
    {
        return std::sqrt(dot_product(this, Vec2{0, 0}));
    }

    // Calculate distance between two Vec2 (positions)
    double distance(const Vec2 other)
    {
        return std::sqrt(dot_product(this, other));
    }

    // returns a vector with the same direction as the original, but with length 1.
    Vec2 normalize(const Vec2 &v)
    {
        double length = distance_from_source();
        if (length == 0.0) // division by 0
        {
            return Vec2{0, 0};
        }
        return v / length;
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
        if(x == 0 && y == 0) {return NULL;} //undefined -> no direction, so no angle either.
        double PI = std::acos(-1);
        double angle = std::atan2(y, x);
        if (angle < 0) // if angle is negative -> add 2pi
        {
            angle += 2 * PI;
        }
        return angle;
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

struct Dynamics
{
    // Data needed for dynamics
    double m_mass{};

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

template <typename T>
bool cmp(T element1, T element2, Operator opp)
{
    switch (opp)
    {
    case Operator::eq:
        if ((double)element1 && (double)element2)
        {
            return std::abs(element1 - element2) =< 0.000001;
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

bool cmp_distance(Vec2 vector1, Vec2 vector2, Operator opp)
{
    return cmp(vector1.distance_from_source(), vector2.distance_from_source(), opp);
}

bool cmp_angle(Vec2 vector1, Vec2 vector2, Operator opp)
{
    return cmp(vector1.angle(), vector2.angle(), opp);
}

bool cmp_value(Vec2 vector1, Vec2 vector2, Operator opp, Orientation orientation)
{
    return cmp(vector1.value(orientation), vector2.value(orientation), opp);
}

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
    Vec2 getPos() const { return m_state.m_position; }
    Vec2 getVelo() const { return m_state.m_velocity; }
    double getMass() const { return m_state.m_mass; }

    bool collides(const Shape &other) // Collision method for all shapes
    {
        //first we have to get the vector between the 2 shapes
        //vector from A to B -> b-a
        //vector from B to a -> a-b

        //then we take the length of that vector using the distance method

        //then we take the direction (angle) of the vector between the 2 shapes

        //now using that angle we are going to determine the length from the middle of our shape 
        //to its side

        //now we take the sum of the 2 length's of each shapes distance from middle to side

        //now we compare that length with the length of the vector between the 2 shapes

        //if lt -> no collision, if ge -> collision
    }

private:
    Dynamics m_state; // Struct that contains data useful for the shapes' movement
};

// Circle Class
class Circle : public Shape
{
public:
    // declarations
    Circle(Dynamics state) : Shape(state) {};         // constructor
    ~Circle();                                        // destructor
    Circle(Circle &other) = default;                  // copy constructor
    Circle(Circle &&source) = default;                // move constructor
    Circle &operator=(const Circle &other) = default; // copy assignment
    Circle &operator=(Circle &&source) = default;     // move assignement

    // Shape-specific functions
    double getSize() { return size; };
    bool collides(Circle &other)
    {
        if ((this->getPos().distance(other.getPos())) <= size + other.size)
        {
            return false;
        }
        return true;
    }
    double getArea() const { return std::acos(-1) * size * size; } // area = pi * radius * radius

private:
    double size{};
};

// Square class
class Square : public Shape
{
    // declarations
    Square(Dynamics state) : Shape(state) {};    // constructor
    ~Square();                                   // destructor
    Square(Shape &other);                        // copy constructor
    Square(Shape &&source);                      // move constructor
    Square &operator=(const Square &) = default; // copy assignment
    Square &operator=(Square &&) = default;      // move assignment

    // Shape-specific functions
    double getSize() { return size; }
    bool collides(Square &other) {
        // we still have to implement this function
    };
    double getArea() { return std::pow(size * 2, 2); } // area = edge^2 = (size*2)^2

private:
    double size{};
};

// Triangle class
class Triangle : public Shape
{
    // declarations
    Triangle(Dynamics state, double size) : Shape(state) {}; // constructor
    ~Triangle();                                             // destructor
    Triangle(Shape &other);                                  // copy constructor
    Triangle(Shape &&source);                                // move constructor
    Triangle &operator=(const Triangle &) = default;         // copy assignment
    Triangle &operator=(Triangle &&) = default;              // move assignment

    // Shape-specific functions
    double getSize() { return size; }
    bool collides(const Triangle &other) const
    {
        // we still have to implement this function
    }
    double getArea() const { return std::pow(size * 2, 2) / 2; } // area = edge^2 = ((size*2)^2)/2

private:
    double size{};
};

// Rectangle class
class Rectangle : public Shape
{
    // declarations
    Rectangle(Dynamics state, Vec2 dimensions) : Shape(state) {}; // constructor
    ~Rectangle();                                                 // destructor
    Rectangle(Shape &other);                                      // copy constructor
    Rectangle(Shape &&source);                                    // move constructor
    Rectangle &operator=(const Rectangle &) = default;            // copy assignment
    Rectangle &operator=(Rectangle &&) = default;                 // move assignment

    // Shape-specific functions
    Vec2 getDimensions() { return dimensions; }
    bool collides(const Rectangle &other) const
    {
        // we still have to implement this
    }
    double getArea() const { return dimensions.y * dimensions.x; } // area = base * height

private:
    Vec2 dimensions{};
};
