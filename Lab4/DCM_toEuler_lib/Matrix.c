#include <stdio.h>
#include <stdlib.h>
#include "xc.h"
#include <math.h>

#include "matrix.h"

Matrix MATRIX_Init(int rows, int columns){
    if((rows < 0) || (columns < 0)){
        return NULL;
    }
    Matrix m = (Matrix)malloc(sizeof(MatrixObj));
    m->arr = (float*)malloc(sizeof(float)*rows*columns);
    m->rows = rows;
    m->columns = columns;
    for(int i = 0; i < m->rows; i++){
        for(int j = 0; j < m->columns; j++){
            m->arr[rows*i + j] = 0.0;
        }
    }
    return m;
}

void MATRIX_Free(Matrix m){
    free(m->arr);
    free(m);
    return;
}

float MATRIX_GetValue(Matrix m, int row, int col){
    if(m == NULL){
        printf("\r\rMATRIX_GetValue Error: Input matrix is NULL\n");
        return 0;
    }
    if((row < 0) || (row >= m->rows)){
        printf("\rMATRIX_GetValue ERROR: row out of bounds\n");
        return 0;
    }
    if((col < 0) || (col >= m->columns)){
        printf("\rMATRIX_GetValue ERROR: column out of bounds\n");
        return 0;
    }
    return m->arr[m->columns*row + col];
}

int MATRIX_SetValue(Matrix m, int row, int col, float value){
    if(m == NULL){
        printf("\r\rMATRIX_SetValue Error: Input matrix is NULL\n");
        return 0;
    }
    if((row < 0) || (row >= m->rows)){
        printf("\rMATRIX_SetValue ERROR: row out of bounds\n");
        return 1;
    }
    if((col < 0) || (col >= m->columns)){
        printf("\rMATRIX_SetValue ERROR: column out of bounds\n");
        return 1;
    }
    m->arr[m->columns*row + col] = value;
    return 0;
}

Matrix MATRIX_GetColumnVector(Matrix m, int col){
    if(m == NULL){
        printf("\rMATRIX_GetColumnVector Error: Input matrix is NULL\n");
        return NULL;
    }
    Matrix columnVec = MATRIX_Init(m->rows, 1);
    for(int i = 0; i < m->rows; i++){
        float value = MATRIX_GetValue(m, i, col);
        MATRIX_SetValue(columnVec, i, 0, value);
    }
    return columnVec;
}

Matrix MATRIX_GetRowVector(Matrix m, int row){
    if(m == NULL){
        printf("\r\rMATRIX_GetRowVector Error: Input matrix is NULL\n");
        return NULL;
    }
    Matrix columnVec = MATRIX_Init(1, m->columns);
    for(int i = 0; i < m->columns; i++){
        float value = MATRIX_GetValue(m, row, i);
        MATRIX_SetValue(columnVec, 0, i, value);
    }
    return columnVec;
}

Matrix MATRIX_Multiply(Matrix m, Matrix n){
    if(m == NULL || n == NULL){
        printf("\rMATRIX_Multiply ERROR: NULL input\n");
        return NULL;
    }
    if(m->columns != n->rows){
        printf("\rMATRIX_Multiply Error: m columns != n rows\n");
        return NULL;
    }
    
    Matrix result = MATRIX_Init(m->rows, n->columns);
    for(int i = 0; i < result->rows; i++){
        for(int j = 0; j < result->columns; j++){
            float value = 0.0;
            for(int k = 0; k < n->rows; k++){
                float mik = MATRIX_GetValue(m, i, k);
                float nkj = MATRIX_GetValue(n, k, j);
                value += (mik * nkj);
            }
            MATRIX_SetValue(result, i, j, value);
        }
    }
    return result;
}

Matrix MATRIX_Subtract(Matrix m, Matrix n) {
    if(m == NULL || n == NULL){
        printf("\rMATRIX_Subtract ERROR: NULL input\n");
        return NULL;
    }
    if((m->rows != n->rows) || (m->columns != n->columns)){
        printf("\rMATRIX_Subtract ERROR: matrices must be the same size\n");
        return NULL;
    }
    Matrix result = MATRIX_Init(m->rows, m->columns);
    uint8_t i, j;
    float val = 0;
    for (i = 0; i < m->rows; i++) {
        for (j = 0; j < m->columns; j++) {
            val = MATRIX_GetValue(m, i, j) - MATRIX_GetValue(n, i, j);
            MATRIX_SetValue(result, i, j, val);
        }
    }
    return result;
}

Matrix MATRIX_MultiplyScalar(Matrix m, float s){
    if(m == NULL){
        printf("\rMATRIX_MultiplyScalar Error: Input matrix is NULL\n");
        return NULL;
    }
    uint8_t i, j;
    for (i = 0; i < m->rows; i++) {
        for (j = 0; j < m->columns; j++) {
            MATRIX_SetValue(m, i, j, s*MATRIX_GetValue(m, i, j));
        }
    }
    return m;
}

Matrix MATRIX_Transpose(Matrix m){
    if(m == NULL){
        printf("\rMATRIX_Transpose Error: Input matrix is NULL");
        return NULL;
    }
    Matrix result = MATRIX_Init(m->columns, m->rows);
    for(uint8_t i = 0; i < m->rows; i++){
        for(uint8_t j = 0; j < m->columns; j++){
            MATRIX_SetValue(result, j, i, MATRIX_GetValue(m, i, j));
        }
    }
    return result;
}

Matrix MATRIX_Exponential(Matrix m){
    if(m == NULL){
        printf("\rMATRIX_Transpose Error: Input matrix is NULL");
        return NULL;
    }
    Matrix result = MATRIX_Init(m->rows, m->columns);
    for(uint8_t i = 0; i < m->rows; i++){
        for(uint8_t j = 0; j < m->columns; j++){
            float value = MATRIX_GetValue(m, i, j);
            MATRIX_SetValue(result, i, j, exp(value));
        }
    }
    return result;
}

void MATRIX_Print(Matrix m){
    if(m == NULL){
        printf("\rMATRIX_Print Error: Input matrix is NULL\n");
        return;
    }
    printf("\r");
    for(int i = 0; i < m->rows; i++){
        for(int j = 0; j < m->columns; j++){
            printf("%f ", MATRIX_GetValue(m, i, j));
        }
        printf("\n\r");
    }
    printf("\n\r");
}

Matrix MATRIX_ConstructEulerMatrix(float* angles){
    float yaw = angles[0] * M_PI / 180;
    float pitch = angles[1] * M_PI / 180;
    float roll = angles[2] * M_PI / 180;

    Matrix m = MATRIX_Init(3, 3);

    MATRIX_SetValue(m, 0, 0, cosf(pitch) * cosf(yaw));
    MATRIX_SetValue(m, 0, 1, cosf(pitch) * sinf(yaw));
    MATRIX_SetValue(m, 0, 2, -1 * sinf(pitch));
    MATRIX_SetValue(m, 1, 0, (sinf(roll)*sinf(pitch)*cosf(yaw)) - (cosf(roll)*sinf(yaw)));
    MATRIX_SetValue(m, 1, 1, (sinf(roll)*sinf(pitch)*sinf(yaw)) + (cosf(roll)*cosf(yaw)));
    MATRIX_SetValue(m, 1, 2, (sinf(roll)*cosf(pitch)));
    MATRIX_SetValue(m, 2, 0, (cosf(roll)*sinf(pitch)*cosf(yaw)) + (sinf(roll)*sinf(yaw)));
    MATRIX_SetValue(m, 2, 1, (cosf(roll)*sinf(pitch)*sinf(yaw)) - (sinf(roll)*cosf(yaw)));
    MATRIX_SetValue(m, 2, 2, (cosf(roll)*cosf(pitch)));

    return m;
}

Matrix MATRIX_ConstructCPMatrix(float* angles) {
    float p = (angles[0]); //* M_PI) / 180; //rotation rate abt x-axis
    float q = (angles[1]); //* M_PI) / 180; //rotation rate abt y-axis
    float r = (angles[2]); //* M_PI) / 180; //rotation rate abt z-axis

    Matrix m = MATRIX_Init(3, 3);

    MATRIX_SetValue(m, 0, 0, 0);
    MATRIX_SetValue(m, 0, 1, -r);
    MATRIX_SetValue(m, 0, 2, q);
    MATRIX_SetValue(m, 1, 0, r);
    MATRIX_SetValue(m, 1, 1, 0);
    MATRIX_SetValue(m, 1, 2, -p);
    MATRIX_SetValue(m, 2, 0, -q);
    MATRIX_SetValue(m, 2, 1, p);
    MATRIX_SetValue(m, 2, 2, 0);

    return m;
}

float* MATRIX_GetEulerAngles(Matrix m){
    if(m->rows != 3 || m->columns != 3){
        return NULL;
    }
    float* vec = (float*)malloc(sizeof(float) * 3);

    // extraction
    float pitch = asinf(-1.0 * MATRIX_GetValue(m, 0, 1));
    float yaw = asinf(MATRIX_GetValue(m, 0, 1) / (cosf(pitch)));
    float roll = asinf(MATRIX_GetValue(m, 1, 2) / cosf(pitch));

    vec[0] = yaw;
    vec[1] = pitch;
    vec[2] = roll;

    // conversion from degrees to radians
    for(int i = 0; i < 3; i++){
        vec[i] = 180.0 * vec[i] / M_PI;
    }

    return vec;
}