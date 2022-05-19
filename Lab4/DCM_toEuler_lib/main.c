#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Matrix.h"

int main(){
    printf("Angle Extractor:\n");
    
    float originalVec[3] = {70.0, -45.0, 20};

    printf("vec: %f %f %f\n", originalVec[0], originalVec[1], originalVec[2]);

    Matrix m = MATRIX_ConstructEulerMatrix(originalVec);
    
    MATRIX_Print(m);

    float* vec = MATRIX_GetEulerAngles(m);
    printf("vec: %f %f %f\n", vec[0], vec[1], vec[2]);

    return 0;
}