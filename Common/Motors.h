#ifndef MOTORS_H
#define MOTORS_H

typedef enum{
    POSITION_TRACKING, SPEED_TRACKING,
}MotorMode;

#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1

#define FORWARD 0
#define BACKWARD 1

#define MAX_MOTOR_SPEED 1000
#define ENCODER_TICKS_PER_REVOLUTION 2160

char MOTORS_Init(MotorMode mode);

char MOTORS_SetSpeed(char motor, int dc);
char MOTORS_SetDirection(char motor, char direction);

// position tracking mode functions
char MOTORS_SetEncoderCount(char motor, int count);
int MOTORS_GetEncoderCount(char motor);

// speed tracking mode functions
float MOTORS_GetMotorSpeed(char motor);

#endif
