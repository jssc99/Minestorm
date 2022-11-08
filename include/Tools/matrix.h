#include "vectors.h"

typedef struct Matrix2
{
    Vector2 a;
    Vector2 b;
}Matrix2;
typedef struct Matrix3
{
    Vector3 a;
    Vector3 b;
    Vector3 c;
}Matrix3;


Matrix2 rotationMatrix2(float theta);
Matrix2 homothetyMatrix2(float a);
Matrix2 identityMatrix2(void);
Vector2 productMatrix2Vector2(Matrix2 m, Vector2 v);
Vector3 productMatrix3Vector3(Matrix3 m, Vector3 v);
Vector2 cartesianVector3(Vector3 homo);
Matrix2 cartesianMatrix3(Matrix3 homo);
float detMatrix2(Matrix2 matrix);
float detMatrix3(Matrix3 matrix);
