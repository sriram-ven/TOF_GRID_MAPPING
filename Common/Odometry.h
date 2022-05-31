#ifndef ODO_H
#define ODO_H

#define ODO_MODE1 0
#define ODO_MODE2 1

char ODOMETRY_Init(char mode);

float ODOMETRY_GetPositionX();
float ODOMETRY_GetPositionY();
float ODOMETRY_GetDirection();
float ODOMETRY_GetRightWheelSpeed();
float ODOMETRY_GetLeftWheelSpeed();
void ODEMTRY_ResetPose();

#endif