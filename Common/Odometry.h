#ifndef ODO_H
#define ODO_H

char ODYMETRY_Init(void);

float ODOMETRY_GetPositionX();
float ODOMETRY_GetPositionY();
float ODOMETRY_GetDirection();
float ODOMETRY_GetRightWheelSpeed();
float ODOMETRY_GetLeftWheelSpeed();

#endif