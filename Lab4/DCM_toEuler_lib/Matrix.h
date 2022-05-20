#include <stdlib.h>
#include <stdio.h>
#include "xc.h"

typedef struct MatrixObj{
    float* arr;
    int rows;
    int columns;
}MatrixObj;

typedef MatrixObj* Matrix;

Matrix MATRIX_Init(int rowSize, int colSize);
Matrix MATRIX_Copy(Matrix m);
void MATRIX_Free(Matrix m);
void MATRIX_Print(Matrix m);

float MATRIX_GetValue(Matrix m, int row, int col);
int MATRIX_SetValue(Matrix m, int row, int col, float value);
Matrix MATRIX_GetRowVector(Matrix m, int row);
Matrix MATRIX_GetColumnVector(Matrix m, int col);
Matrix MATRIX_ConstructIdentity(int size);

Matrix MATRIX_Multiply(Matrix m, Matrix n);
Matrix MATRIX_Subtract(Matrix m, Matrix n);
Matrix MATRIX_Add(Matrix m, Matrix n);
Matrix MATRIX_MultiplyScalar(Matrix m, float s);
Matrix MATRIX_Transpose(Matrix m);
Matrix MATRIX_Exponential(Matrix pqr, float p, float q, float r, float deltaT);

Matrix MATRIX_ConstructEulerMatrix(float* angles);
float* MATRIX_GetEulerAngles(Matrix m);
Matrix MATRIX_ConstructCPMatrix(float* angles);