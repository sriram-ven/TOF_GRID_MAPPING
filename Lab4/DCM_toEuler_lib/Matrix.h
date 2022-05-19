#include <stdlib.h>
#include <stdio.h>

typedef struct MatrixObj{
    float* arr;
    int rowSize;
    int colSize;
}MatrixObj;

typedef MatrixObj* Matrix;

Matrix MATRIX_Init(int rowSize, int colSize);
void MATRIX_Free(Matrix m);
float MATRIX_GetValue(Matrix m, int row, int col);
int MATRIX_SetValue(Matrix m, int row, int col, float value);
void MATRIX_Print(Matrix m);

Matrix MATRIX_Multiply(Matrix m1, Matrix m2);
Matrix MATRIX_Subtract(Matrix m1, Matrix m2);
Matrix MATRIX_MultiplyScalar(Matrix m1, float s);

Matrix MATRIX_ConstructEulerMatrix(float* angles);
Matrix MATRIX_ConstructCPMatrix(float* angles);
float* MATRIX_GetEulerAngles(Matrix m);