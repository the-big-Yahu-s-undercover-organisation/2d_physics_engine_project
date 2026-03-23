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

//function to measure directional similarity between two vectors
double dot_product(const Vec2 &v1, const Vec2 &v2)
{
    return v1.x * v2.x + v1.y * v2.y; //x1*x2 + y1*y2
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
        if(length == 0.0) //division by 0
        {return Vec2{0,0};} 
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
            return std::abs(element1 - element2) < 0.000001;
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
            return element1 - element2 > 0.000000;
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
            return element2 - element1 > 0.000000;
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
    //declarations
    Shape(Dynamics state, double size); // Main constructor
    virtual ~Shape() = default;
    Shape(Shape &other);                       // Copy constructor
    Shape(Shape &&source);                     // Move constructor
    Shape &operator=(const Shape &) = default; // Copy assignment
    Shape &operator=(Shape &&) = default;      // Move assignment

    // Getters 
    Vec2 getPos() const;
    Vec2 getVelo() const;
    double getMass() const;
    double getSize() const;

    virtual bool collides(const Shape &other) const = 0; // Collision virtual method
    virtual double area() const = 0;                     // Calculates the area of the shape

private:
    Dynamics m_state; // Struct that contains data useful for movement
    double m_size{};
};

Shape::Shape(Dynamics state, double size) : m_state{state}, m_size{size} {};

Shape::Shape(Shape &other) : m_state{other.m_state}, m_size{other.m_size} {};

Shape::Shape(Shape &&source) : m_state{source.m_state}, m_size{source.m_size}
{
    Vec2 zero{0, 0};
    source.m_state.m_position = zero;
    source.m_state.m_velocity = zero;
    source.m_state.m_mass = 0.0;
    source.m_size = 0.0;
};

Vec2 Shape::getPos() const { return m_state.m_position; };

Vec2 Shape::getVelo() const { return m_state.m_velocity; };

double Shape::getMass() const { return m_state.m_mass; };

double Shape::getSize() const { return m_size; };

// Circle Class
class Circle : public Shape
{
public:
    //declarations
    Circle(Dynamics state, double size); // Main constructor
    ~Circle();
    Circle(const Circle &other) = default;            //copy constructor
    Circle(Circle &&source) = default;                //move constructor
    Circle &operator=(const Circle &other) = default; //copy assignment
    Circle &operator=(Circle &&source) = default;     //move assignement

    bool collides(const Circle &other) const;
    double area() const;

private:
    //this class doesn't need any characteristic data because the needed data already sits inside the Shape class
};

Circle::Circle(Dynamics state, double size) : Shape(state, size) {}; 

bool Circle::collides(const Circle &other) const
{
    if ((this->getPos().distance(other.getPos())) <= this->getSize() + other.getSize())
    {
        return false;
    }
    return true;
}

double Circle::area() const
{
    return std::acos(-1) * getSize() * getSize(); //area = pi * radius * radius
}


//Square class
class Square : public Shape 
{
    //declarations
    Square(Dynamics state, double size); // Main constructor
    ~Square();
    Square(Shape &other);                        // Copy constructor
    Square(Shape &&source);                      // Move constructor
    Square &operator=(const Square &) = default; // Copy assignment
    Square &operator=(Square &&) = default;      // Move assignment

    bool collides(const Circle &other) const;
    double area() const;

    private:
    //this class doesn't need any characteristic data because the needed data already sits inside the Shape class
};

Square::Square(Dynamics state, double size) : Shape(state,size) {};

double Square::area() const
{
    return std::pow(getSize()*2, 2); //area = edge^2 = (size*2)^2
}
