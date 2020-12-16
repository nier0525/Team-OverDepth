#pragma once
class Vector3D
{
public:
    float x, y, z;
    Vector3D();

    Vector3D(float x1, float y1, float z1 = 0);
    Vector3D(const Vector3D& vec);    //copy constructor
    Vector3D operator+(const Vector3D& vec);    //addition
    Vector3D& operator+=(const Vector3D& vec);  ////assigning new result to the vector
    Vector3D operator-(const Vector3D& vec);    //substraction
    Vector3D& operator-=(const Vector3D& vec);  //assigning new result to the vector
    Vector3D operator*(float value);    //multiplication
    Vector3D& operator*=(float value);  //assigning new result to the vector.
    Vector3D operator/(float value);    //division
    Vector3D& operator/=(float value);  //assigning new result to the vector
    Vector3D& operator=(const Vector3D& vec);
    float dot_product(const Vector3D& vec); //scalar dot_product
    Vector3D cross_product(const Vector3D& vec);    //cross_product

    float magnitude();  //magnitude of the vector
    Vector3D normalization();   //nor,malized vector
    float square(); //gives square of the vector

    float distance(const Vector3D& vec);    //gives distance between two vectors
    float show_X(); //return x
    float show_Y(); //return y
    float show_Z(); //return z
};

/*
forward vector:
x = 2 * (x*z + w*y)
y = 2 * (y*z - w*x)
z = 1 - 2 * (x*x + y*y)

up vector
x = 2 * (x*y - w*z)
y = 1 - 2 * (x*x + z*z)
z = 2 * (y*z + w*x)

left vector
x = 1 - 2 * (y*y + z*z)
y = 2 * (x*y + w*z)
z = 2 * (x*z - w*y)
*/
