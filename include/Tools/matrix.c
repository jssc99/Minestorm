#include "matrix.h"
#include <math.h>

// Create an rotation matrix of angle 'theta' centered on origin
Matrix2 rotationMatrix2(float theta)
{
    Matrix2 rotation = {{cosf(theta),
                         -sinf(theta)},
                        {sinf(theta),
                         cosf(theta)}};
    return rotation;
}

// Create an homothety matrix of coeff 'a' centered on origin
Matrix2 homothetyMatrix2(float a)
{
    Matrix2 homothety = {{a, 0}, {0, a}};
    return homothety;
}

// The identity matrix 2x2
Matrix2 identityMatrix2(void)
{
    return homothetyMatrix2(1);
}

// Transform a vector2 by a matrix2
Vector2 productMatrix2Vector2(Matrix2 m, Vector2 v)
{
    Vector2 newVector = {m.a.x * v.x + m.a.y * v.y,
                         m.b.x * v.x + m.b.y * v.y};
    return newVector;
}

// Transform a vector3 by a matrix3
Vector3 productMatrix3Vector3(Matrix3 m, Vector3 v)
{
    Vector3 newVector = {m.a.x * v.x + m.a.y * v.y + m.a.z * v.z,  // x
                         m.b.x * v.x + m.b.y * v.y + m.b.z * v.z,  // y
                         m.b.x * v.x + m.b.y * v.y + m.b.z * v.z}; // z
    return newVector;
}

// Return cartesian values for an homogeneous vector
Vector2 cartesianVector3(Vector3 homo)
{
    Vector2 cart = {homo.x / homo.z, homo.y / homo.z};
    return cart;
}

// Return cartesian values for an homogeneous matrix
Matrix2 cartesianMatrix3(Matrix3 homo)
{
    Matrix2 cart = {cartesianVector3(homo.a), cartesianVector3(homo.b)};
    return cart;
}

// Get the determinant of a matrix 2x2
float detMatrix2(Matrix2 matrix)
{
    return (float)(matrix.a.x * matrix.b.y - matrix.b.x * matrix.a.y);
}

// Get the determinant of a matrix 3x3
float detMatrix3(Matrix3 matrix)
{
    float det = matrix.a.x * (matrix.b.y * matrix.c.z - matrix.c.y * matrix.b.z) - matrix.b.x * (matrix.a.y * matrix.c.z - matrix.c.y * matrix.a.z) + matrix.c.x * (matrix.a.y * matrix.b.z - matrix.b.y * matrix.a.z);
    return det;
}