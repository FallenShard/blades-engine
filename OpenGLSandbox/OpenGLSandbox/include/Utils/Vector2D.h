#ifndef VECTOR_2D_H
#define VECTOR_2D_H

// A template class that represents a simple 2d Euclidean vector
template <typename T>
class Vector2D
{
public:
    // Default constructor, initializes coordinates to zero
    Vector2D();
    // Creates a vector with given coordinates
    Vector2D(T x, T y);
    // Copy constructor
    Vector2D(const Vector2D<T>& vector);

    // Destructor which currently does nothing
    ~Vector2D();

    // Common overloaded operators for vectors
    Vector2D<T>& operator=(const Vector2D<T>& vector);
    const Vector2D<T> operator*(T scalar);
    const Vector2D<T> operator+(const Vector2D<T>& vector);
    const Vector2D<T> operator-(const Vector2D<T>& vector);

    bool operator==(const Vector2D<T>& vector);
    bool operator!=(const Vector2D<T>& vector);
    
    float dotProduct(const Vector2D<T>& vector);

    // Getter and setter methods
    T getX() const;
    T getY() const;
    void setX(T x);
    void setY(T y);

private:
    T m_x;
    T m_y;
};

// Common typedefs
typedef Vector2D<unsigned int> Vector2Du;
typedef Vector2D<int>          Vector2Di;
typedef Vector2D<float>        Vector2Df;
typedef Vector2D<double>       Vector2Dd;

#include "Vector2D.inl"

#endif