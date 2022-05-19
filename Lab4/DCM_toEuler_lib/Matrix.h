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

Matrix MATRIX_ConstructEulerMatrix(float* angles);
float* MATRIX_GetEulerAngles(Matrix m);