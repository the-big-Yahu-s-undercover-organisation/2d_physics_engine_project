// main source-code file
#include <cmath>
#include <algorithm>

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
 
    //data of the struct 
    double y{};
    double x{};

    //methods of the struct
    Vec2(double x_value, double y_value ) //constructor
    {
        x = x_value;
        y = y_value;
    }
    ~Vec2() = default; //destructor
    Vec2(Vec2 &other) = default; //copy
    Vec2(Vec2 &&source) = default; //move
    Vec2 &operator=(const Vec2 &other) = default;     // copy assignment
    Vec2 &operator=(Vec2 &&source) = default;         // move assignement

    // Operator overloading

    //Vec2 _ Vec2 operations
    Vec2 &operator+=(const Vec2 &v)
    {
        x += v.x;
        y += v.y;
        return *this;
    };
    Vec2 operator+(Vec2 &v) { return *this += v; }
    Vec2 &operator-=(const Vec2 &v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    };
    Vec2 operator-( const Vec2 &v) { return *this -= v; }
    Vec2 &operator*=(const Vec2 &v)
    {
        x *= v.x;
        y *= v.y;
        return *this;
    };
    Vec2 operator*( const Vec2 &v) { return *this *= v; }
    Vec2 &operator/=(const Vec2 &v)
    {
        x /= v.x;
        y /= v.y;
        return *this;
    };
    Vec2 operator-( const Vec2 &v) { return *this /= v; }

    //Vec2 _ double operations
    Vec2 &operator+=(const double &s)
    {
        x += s;
        y += s;
        return *this;
    };
    Vec2 operator+(double &s) { return *this += s; }
    Vec2 &operator-=(const double &s)
    {
        x -= s;
        y -= s;
        return *this;
    };
    Vec2 operator-(double &s) { return *this -= s; }
    Vec2 &operator*=(const double &s)
    {
        x *= s;
        y *= s;
        return *this;
    };
    Vec2 operator*(double &s) { return *this *= s; }
    Vec2 &operator/=(const double &s)
    {
        x /= s;
        y /= s;
        return *this;
    };
    Vec2 operator/(double &s) { return *this /= s; }

    //extra operators
    Vec2 squared() {return *this * *this;}
    Vec2 cubed() {return this->squared() * *this;}

    //comparator
    bool operator==(Vec2 &other) {return this->y == other.y && this->x == other.x;}


    //normal methods
    double get_x() {return x;}
    double get_y() {return y;}
    double get_value() {return x + y;}

    // calculate distance to point source (0.0 ,0.0)
    double distance_from_source() // root(x^2 + y^2)
    {
        return std::sqrt((this->squared()).get_value());
    }

    // Calculate distance between two Vec2 (positions)
    double distance(const Vec2 other)
    {
        return (*this - other).distance_from_source();
    }

    // function to measure directional similarity between two vectors
    double dot_product( Vec2 &v) {return (*this * v).get_value(); } // x1*x2 + y1*y2

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

};

bool cmp_distance(Vec2 vector1, Vec2 vector2, Operator opp) { return cmp(vector1.distance_from_source(), vector2.distance_from_source(), opp); }

bool cmp_angle(Vec2 vector1, Vec2 vector2, Operator opp) { return cmp(vector1.angle(), vector2.angle(), opp); }

bool cmp_value(Vec2 vector1, Vec2 vector2, Operator opp, Orientation orientation) { return cmp(vector1.value(orientation), vector2.value(orientation), opp); }

struct Dynamics
{
    ~Dynamics() = default; //destructor
    Dynamics(Dynamics &other) = default; //copy
    Dynamics(Dynamics &&other) = default; //move
    
    // Data needed for dynamics
    double m_mass{0};
    Figure figure{Figure::None};

    // Linear movement
    Vec2 m_position{0,0};
    Vec2 m_velocity{0,0};
    Vec2 m_acceleration{0,0};
    Vec2 m_force{0,0};

    // Rotating
    double m_angle{0};
    double m_inertia{0};
    double m_angvelocity{0};
    double m_torque{0};
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
    Vec2 getPos() const { return Vec2{m_state.m_position.y, m_state.m_position.x}; }
    Vec2 getVel() const { return Vec2{m_state.m_velocity.y, m_state.m_velocity.x}; }
    Vec2 getAcc() const { return Vec2{m_state.m_acceleration.y, m_state.m_acceleration.x}; }
    Vec2 getForce() const { return Vec2{m_state.m_force.y, m_state.m_force.x}; }
    double getAngle() const { return m_state.m_angle; }
    double getInertia() const { return m_state.m_inertia; }
    double getAngVel() const { return m_state.m_angvelocity; }
    double getTorque() const { return m_state.m_torque; }

    virtual double get_distance_MiddleToSide(double angle) const = 0;

private:
    Dynamics m_state; // Struct that contains data useful for the shapes' movement
};

// function to use between shape-objects
bool collides(const Shape &shape_A, const Shape &shape_B) // Collision checker for all shapes
{
    Vec2 AB{shape_B.getPos() - shape_A.getPos()}; //we create the vector between the 2 middle-points of the shapes
    double AB_len = AB.distance_from_source(); //now we take the length of that vector
    if (AB_len == 0.0) {return true;}
    double AB_angle {AB.angle()};
    double PI {std::acos(-1)};
    double BA_angle;
    if (AB_angle <= PI) {BA_angle = AB_angle + PI;}
    else {BA_angle = AB_angle - PI;}
    if (shape_A.get_distance_MiddleToSide(normalize_angle(AB_angle - shape_A.getAngle())) + shape_B.get_distance_MiddleToSide(normalize_angle(BA_angle - shape_B.getAngle())) < AB_len) {return false;}
    return true;
}

// Circle Class
class Circle : public Shape
{
public:
    // declarations
    Circle(Dynamics state) : Shape(state), size{size} {}; // constructor
    ~Circle();                                            // destructor
    Circle(Circle &other) = default;                      // copy constructor
    Circle(Circle &&source) = default;                    // move constructor
    Circle &operator=(const Circle &other) = default;     // copy assignment
    Circle &operator=(Circle &&source) = default;         // move assignement

    // Shape-specific functions
    Figure getFigure() { return Figure::Circle; }
    double getSize() { return size; };
    double get_distance_MiddleToSide(double angle) const override { return size; } // for a Circle it's easy, the distance stays the same for each angle
    double getArea() const { return std::acos(-1) * size * size; }                 // area = pi * radius * radius

private:
    double size{};
};

// Square class
class Square : public Shape
{
public:
    // declarations
    Square(Dynamics state) : Shape(state), size{size} {}; // constructor
    ~Square();                                            // destructor
    Square(Shape &other);                                 // copy constructor
    Square(Shape &&source);                               // move constructor
    Square &operator=(const Square &) = default;          // copy assignment
    Square &operator=(Square &&) = default;               // move assignment

    // Shape-specific functions
    Figure getFigure() { return Figure::Square; }
    double getSize() { return size; }
    double get_distance_MiddleToSide(double angle) const override { return size / std::max(std::abs(std::sin(angle)), std::abs(std::cos(angle))); }
    double getArea() { return std::pow(size * 2, 2); } // area = edge^2 = (size*2)^2

private:
    double size{};
};

// Triangle class
class Triangle : public Shape
{
public:
    // declarations
    Triangle(Dynamics state, double size) : Shape(state), size{size} {}; // constructor
    ~Triangle();                                                         // destructor
    Triangle(Shape &other);                                              // copy constructor
    Triangle(Shape &&source);                                            // move constructor
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
    double getArea() const { return std::pow(size * 2, 2) / 2; } // area = edge^2 = ((size*2)^2)/2

private:
    double size{};
};

// Pentagon class
class Pentagon : public Shape
{
    // declarations
    Pentagon(Dynamics state, Vec2 dimensions) : Shape(state), size{size} {}; // constructor
    ~Pentagon();                                                             // destructor
    Pentagon(Shape &other);                                                  // copy constructor
    Pentagon(Shape &&source);                                                // move constructor
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
    ~Rectangle();                                                                         // destructor
    Rectangle(Shape &other);                                                              // copy constructor
    Rectangle(Shape &&source);                                                            // move constructor
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

private:
    Vec2 dimensions{0,0};
};
