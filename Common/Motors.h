#ifndef MOTORS_H
#define MOTORS_H

#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1

#define FORWARD 0
#define BACKWARD 1

char MOTORS_Init(void);

char MOTORS_SetSpeed(char motor, int dc);
char MOTORS_SetDirection(char motor, char direction);
char MOTORS_SetEncoderCount(char motor, int count);
int MOTORS_GetEncoderCount(char motor);

#endif