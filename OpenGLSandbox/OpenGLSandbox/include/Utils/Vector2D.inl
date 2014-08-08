template <typename T>
Vector2D<T>::Vector2D()
    :   m_x(0)
    ,   m_y(0)
{
}

template <typename T>
Vector2D<T>::Vector2D(T x, T y)
    :   m_x(x)
    ,   m_y(y)
{
}

template <typename T>
Vector2D<T>::Vector2D(const Vector2D<T>& vector)
    : m_x(vector.m_x)
    , m_y(vector.m_y)
{
}

template <typename T>
Vector2D<T>::~Vector2D()
{
}

template <typename T>
const Vector2D<T> Vector2D<T>::operator*(T scalar)
{
    return Vector2D<T>(m_x * scalar, m_y * scalar);
}

template <typename T>
const Vector2D<T> Vector2D<T>::operator+(const Vector2D<T>& vector)
{
    return Vector2D<T>(m_x + vector.m_x, m_y + vector.m_y);
}

template <typename T>
const Vector2D<T> Vector2D<T>::operator-(const Vector2D<T>& vector)
{
    return Vector2D<T>(m_x - vector.m_x, m_y + vector.m_y);
}

template <typename T>
Vector2D<T>& Vector2D<T>::operator=(const Vector2D<T>& vector)
{
    // Check for self-assignment
    if (this == &vector)
        return *this;

    m_x = vector.m_x;
    m_y = vector.m_y;

    return *this;
}

template <typename T>
float Vector2D<T>::dotProduct(const Vector2D<T>& vector)
{
    return Vector2D<T>(m_x * vector.m_x, m_y * vector.m_y);
}

template <typename T>
bool Vector2D<T>::operator==(const Vector2D<T>& vector)
{
    if (m_x == vector.m_x && m_y == vector.m_y)
        return true;
    else
        return false;
}

template <typename T>
bool Vector2D<T>::operator!=(const Vector2D<T>& vector)
{
    return !(*this == vector);
}

template <typename T>
T Vector2D<T>::getX() const
{
    return m_x;
}

template <typename T>
T Vector2D<T>::getY() const
{
    return m_y;
}

template <typename T>
void Vector2D<T>::setX(T x)
{
    m_x = x;
}

template <typename T>
void Vector2D<T>::setY(T y)
{
    m_y = y;
}