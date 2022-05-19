#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Matrix.h"

#define DCM_INTEGRATION
//#define MATRIX_TEST

#ifdef  MATRIX_TEST
int main(){
    printf("Angle Extractor:\n");
    
    float originalVec[3] = {70.0, -45.0, 20};
    static float vec2[3] = {23.0, -15.0, 90};

    printf("vec: %f %f %f\n", originalVec[0], originalVec[1], originalVec[2]);

    Matrix m = MATRIX_ConstructEulerMatrix(originalVec);
    
    MATRIX_Print(m);

    float* vec = MATRIX_GetEulerAngles(m);
    printf("vec: %f %f %f\n", vec[0], vec[1], vec[2]);
    
    m = MATRIX_ConstructCPMatrix(originalVec);
    MATRIX_Print(m);
    
    printf("test\n");
    
    Matrix m1= MATRIX_ConstructCPMatrix(originalVec);
    printf("es\n");
    Matrix m2 = MATRIX_ConstructCPMatrix(originalVec);
    printf("test\n");
    m = MATRIX_Multiply(m1, m2);
    
    MATRIX_Print(m);

    return 0;
}
#endif

#ifdef DCM_INTEGRATION
int main(){
    static uint8_t rot = 0;
    static float angleRates[3] = {10, 5, 10};
    
    Matrix mw = MATRIX_ConstructCPMatrix(angleRates);
    Matrix m = MATRIX_ConstructEulerMatrix(angleRates);
    Matrix mres = MATRIX_Init(3, 3);

    
    while(1){
    }
}
#endif