#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Matrix.h"

Matrix MATRIX_Init(int rowSize, int colSize) {
    if (rowSize < 0 || colSize < 0) {
        return NULL;
    }
    Matrix m = (Matrix) malloc(sizeof (MatrixObj));
    m->arr = (float*) malloc(sizeof (float)*rowSize * colSize);
    m->rowSize = rowSize;
    m->colSize = colSize;
    for (int i = 0; i < m->rowSize; i++) {
        for (int j = 0; j < m->colSize; j++) {
            m->arr[rowSize * i + j] = 0.0;
        }
    }
    return m;
}

void MATRIX_Free(Matrix m) {
    free(m->arr);
    free(m);
    return;
}

float MATRIX_GetValue(Matrix m, int row, int col) {
    return m->arr[m->rowSize * row + col];
}

int MATRIX_SetValue(Matrix m, int row, int col, float value) {
    if (row < 0 || row >= m->rowSize) {
        return 1;
    }
    if (col < 0 || col >= m->colSize) {
        return 1;
    }
    m->arr[m->rowSize * row + col] = value;
    return 0;
}

void MATRIX_Print(Matrix m) {
    for (int i = 0; i < m->rowSize; i++) {
        for (int j = 0; j < m->colSize; j++) {
            printf("%f ", m->arr[m->rowSize * i + j]);
        }
        printf("\n");
    }
    printf("\n");
}

Matrix MATRIX_Multiply(Matrix m1, Matrix m2) {
    Matrix m = MATRIX_Init(m1->rowSize, m2->colSize);
    static uint8_t i, j, k;
    static float val;

    for (i = 0; i < m1->rowSize; i++) {
        for (j = 0; j < m2->colSize; j++) {
            val = 0;
            for (k = 0; k < m1->colSize; k++) {
                val += MATRIX_GetValue(m1, i, k) * MATRIX_GetValue(m2, k, j);
                MATRIX_SetValue(m, i, j, val);
            }
        }
    }
    return m;
}

Matrix MATRIX_ConstructEulerMatrix(float* angles) {
    float yaw = angles[0] * M_PI / 180;
    float pitch = angles[1] * M_PI / 180;
    float roll = angles[2] * M_PI / 180;

    Matrix m = MATRIX_Init(3, 3);

    MATRIX_SetValue(m, 0, 0, cosf(pitch) * cosf(yaw));
    MATRIX_SetValue(m, 0, 1, cosf(pitch) * sinf(yaw));
    MATRIX_SetValue(m, 0, 2, -1 * sinf(pitch));
    MATRIX_SetValue(m, 1, 0, (sinf(roll) * sinf(pitch) * cosf(yaw)) - (cosf(roll) * sinf(yaw)));
    MATRIX_SetValue(m, 1, 1, (sinf(roll) * sinf(pitch) * sinf(yaw)) + (cosf(roll) * cosf(yaw)));
    MATRIX_SetValue(m, 1, 2, (sinf(roll) * cosf(pitch)));
    MATRIX_SetValue(m, 2, 0, (cosf(roll) * sinf(pitch) * cosf(yaw)) + (sinf(roll) * sinf(yaw)));
    MATRIX_SetValue(m, 2, 1, (cosf(roll) * sinf(pitch) * sinf(yaw)) - (sinf(roll) * cosf(yaw)));
    MATRIX_SetValue(m, 2, 2, (cosf(roll) * cosf(pitch)));

    return m;
}

Matrix MATRIX_ConstructCPMatrix(float* angles) {
    float p = (angles[0]); //* M_PI) / 180; //rotation rate abt x-axis
    float q = (angles[1]); //* M_PI) / 180; //rotation rate abt y-axis
    float r = (angles[2]); //* M_PI) / 180; //rotation rate abt z-axis

    Matrix m = MATRIX_Init(3, 3);

    //initially zero matrix
    MATRIX_SetValue(m, 0, 1, -r);
    MATRIX_SetValue(m, 0, 2, q);
    MATRIX_SetValue(m, 1, 0, r);
    MATRIX_SetValue(m, 1, 2, -p);
    MATRIX_SetValue(m, 2, 0, -q);
    MATRIX_SetValue(m, 2, 1, p);

    return m;
}

float* MATRIX_GetEulerAngles(Matrix m) {
    if (m->colSize != 3 || m->rowSize != 3) {
        return NULL;
    }
    float* vec = (float*) malloc(sizeof (float) * 3);

    // extraction
    float pitch = asinf(-1.0 * MATRIX_GetValue(m, 0, 2));
    float yaw = asinf(MATRIX_GetValue(m, 0, 1) / (cosf(pitch)));
    float roll = asinf(MATRIX_GetValue(m, 1, 2) / cosf(pitch));

    vec[0] = yaw;
    vec[1] = pitch;
    vec[2] = roll;

    // conversion from degrees to radians
    for (int i = 0; i < 3; i++) {
        vec[i] = 180.0 * vec[i] / M_PI;
    }

    return vec;
}