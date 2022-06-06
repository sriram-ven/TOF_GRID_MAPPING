#ifndef ODO_H
#define ODO_H

char ODOMETRY_Init();

float ODOMETRY_GetPositionX();
float ODOMETRY_GetPositionY();
float ODOMETRY_GetDirection();
float ODOMETRY_GetRightWheelSpeed();
float ODOMETRY_GetLeftWheelSpeed();
void ODEMTRY_ResetPose();

#endif