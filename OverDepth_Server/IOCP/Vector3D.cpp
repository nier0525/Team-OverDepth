#include "Vector3D.h"
#include<assert.h>
#include<math.h>
Vector3D::Vector3D()  //constructor
{
    x = 0;
    y = 0;
    z = 0;
}
Vector3D::Vector3D(float x1, float y1, float z1)     //initializing object with values.
{
    x = x1;
    y = y1;
    z = z1;
}
Vector3D::Vector3D(const Vector3D& vec)
{
    x = vec.x;
    y = vec.y;
    z = vec.z;
}

//addition

Vector3D Vector3D ::operator+(const Vector3D& vec)
{
    return Vector3D(x + vec.x, y + vec.y, z + vec.z);
}

Vector3D& Vector3D ::operator+=(const Vector3D& vec)
{
    x += vec.x;
    y += vec.y;
    z += vec.z;
    return *this;
}
//substraction//
Vector3D Vector3D ::operator-(const Vector3D& vec)
{
    return Vector3D(x - vec.x, y - vec.y, z - vec.z);
}

Vector3D& Vector3D::operator-=(const Vector3D& vec)
{
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
    return *this;
}

//scalar multiplication

Vector3D Vector3D ::operator*(float value)
{
    return Vector3D(x * value, y * value, z * value);
}



Vector3D& Vector3D::operator*=(float value)
{
    x *= value;
    y *= value;
    z *= value;
    return *this;
}

//scalar division
Vector3D Vector3D ::operator/(float value)
{
    assert(value != 0);
    return Vector3D(x / value, y / value, z / value);
}

Vector3D& Vector3D ::operator/=(float value)
{
    assert(value != 0);
    x /= value;
    y /= value;
    z /= value;
    return *this;
}


Vector3D& Vector3D::operator=(const Vector3D& vec)
{
    x = vec.x;
    y = vec.y;
    z = vec.z;
    return *this;
}

//Dot product
float Vector3D::dot_product(const Vector3D& vec)
{
    return x * vec.x + vec.y * y + vec.z * z;
}

//cross product
Vector3D Vector3D::cross_product(const Vector3D& vec)
{
    float ni = y * vec.z - z * vec.y;
    float nj = z * vec.x - x * vec.z;
    float nk = x * vec.y - y * vec.x;
    return Vector3D(ni, nj, nk);
}

float Vector3D::magnitude()
{
    return sqrt(square());
}

float Vector3D::square()
{
    return x * x + y * y + z * z;
}

Vector3D Vector3D::normalization()
{
    assert(magnitude() != 0);
    *this /= magnitude();
    return *this;
}

float Vector3D::distance(const Vector3D& vec)
{
    Vector3D dist = *this - vec;
    return dist.magnitude();
}

float Vector3D::show_X()
{
    return x;
}

float Vector3D::show_Y()
{
    return y;
}

float Vector3D::show_Z()
{
    return z;
}

