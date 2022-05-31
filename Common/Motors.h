#ifndef MOTORS_H
#define MOTORS_H

#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1

#define FORWARD 1
#define BACKWARD -1

#define MAX_MOTOR_SPEED 1000
#define ENCODER_TICKS_PER_REVOLUTION 2160

char MOTORS_Init();

char MOTORS_SetSpeed(char motor, int dc);
char MOTORS_SetDirection(char motor, char direction);
int MOTORS_GetDirection(char motor); // -1: counter-clockwise, 1: clockwise

// position tracking mode functions
char MOTORS_SetEncoderCount(char motor, int count);
int MOTORS_GetEncoderCount(char motor);

// speed tracking mode functions
float MOTORS_GetMotorSpeed(char motor);

#endif
