#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <BOARD.h>
#include <sys/attribs.h>
#include "pwm.h"

#include "Motors.h"

// left motor pins
#define LEFT_MOTOR_PWM_PIN PWM_PORTY12 // pin 5
#define LEFT_MOTOR_IN1 PORTEbits.RE1 // pin 27
#define LEFT_MOTOR_IN2 PORTEbits.RE2 // pin 28
#define LEFT_MOTOR_IN1_TRIS TRISEbits.TRISE1
#define LEFT_MOTOR_IN2_TRIS TRISEbits.TRISE2

// right motor pins
#define RIGHT_MOTOR_PWM_PIN PWM_PORTY10 // pin 6
#define RIGHT_MOTOR_IN1 PORTEbits.RE3 // pin 29
#define RIGHT_MOTOR_IN2 PORTEbits.RE4 // pin 30
#define RIGHT_MOTOR_IN1_TRIS TRISEbits.TRISE3
#define RIGHT_MOTOR_IN2_TRIS TRISEbits.TRISE4

/*
 * the encoder CN pins are as such:
 *      - leftA = CN15, RD6, pin 37
 *      - leftB = CN16, RD7, pin 36
 * 
 *      - rightA = CN17, RF4, pin 39
 *      - rightB = CN18, RF5, pin 40
 */

// current duty cycle of left and right motors
int leftMotorDC = 0;
int rightMotorDC = 0;

// current direction of left and right motors
char leftMotorDirection;
char rightMotorDirection;

// encoder counts of each motor
int leftMotorEncoderCount = 0;
int rightMotorEncoderCount = 0;

// previous encoder states for left and right motor
char LM_prevState;
char RM_prevState;

char MOTORS_Init(void){
    // init pwm pins
    PWM_Init();
    PWM_SetFrequency(PWM_2KHZ);
    PWM_AddPins(LEFT_MOTOR_PWM_PIN);
    PWM_AddPins(RIGHT_MOTOR_PWM_PIN);
    
    //init direction pins as outputs 
    LEFT_MOTOR_IN1_TRIS = 0; 
    LEFT_MOTOR_IN2_TRIS = 0; 
    RIGHT_MOTOR_IN1_TRIS = 0; 
    RIGHT_MOTOR_IN2_TRIS = 0; 
    
    MOTORS_SetDirection(LEFT_MOTOR, FORWARD);
    MOTORS_SetDirection(RIGHT_MOTOR, FORWARD);
    
    // INIT Change notify
    CNCONbits.ON = 1; // Change Notify On
    CNENbits.CNEN15 = 1; //enable leftA phase
    CNENbits.CNEN16 = 1; //enable leftB phase
    CNENbits.CNEN17 = 1; //enable rightA phase
    CNENbits.CNEN18 = 1; //enable rightB phase
    int temp1 = PORTD; 
    int temp2 = PORTF;
    IFS1bits.CNIF = 0; // clear interrupt flag
    IPC6bits.CNIP = 1; //set priority
    IPC6bits.CNIS = 3; // and sub priority
    IEC1bits.CNIE = 1; // enable change notify

    // set pins to input
    TRISDbits.TRISD6 = 1;
    TRISDbits.TRISD7 = 1;
    TRISFbits.TRISF4 = 1;
    TRISFbits.TRISF5 = 1;
    
    // initialize encoder states
    LM_prevState = (PORTDbits.RD7 << 1) | PORTDbits.RD6;
    RM_prevState = (PORTFbits.RF4 << 1) | PORTFbits.RF5;
    
    return SUCCESS;
}

char MOTORS_SetDirection(char motor, char direction){
    switch(motor){
        case(LEFT_MOTOR):
            switch(direction){
                case FORWARD:
                    leftMotorDirection = FORWARD;
                    LEFT_MOTOR_IN1 = 1;
                    LEFT_MOTOR_IN2 = 0;
                    break;
                case BACKWARD:
                    leftMotorDirection = BACKWARD;
                    LEFT_MOTOR_IN1 = 0;
                    LEFT_MOTOR_IN2 = 1;
                    break;
                
                default:
                    return ERROR;
            }
            break;
            
        case(RIGHT_MOTOR):
            switch(direction){
                case FORWARD:
                    rightMotorDirection = FORWARD;
                    RIGHT_MOTOR_IN1 = 1;
                    RIGHT_MOTOR_IN2 = 0;
                    break;
                    
                case BACKWARD:
                    rightMotorDirection = BACKWARD;
                    RIGHT_MOTOR_IN1 = 0;
                    RIGHT_MOTOR_IN2 = 1;
                    break;
                    
                default:
                    return ERROR;
            }
            break;
            
        default:
            return ERROR;
    }
    return SUCCESS;
}

char MOTORS_SetSpeed(char motor, int dc){
    if(dc > MAX_PWM){
        dc = MAX_PWM;
    }
    if(dc < -1 * MAX_PWM){
        dc = -1 * MAX_PWM;
    }
    switch(motor){
        case LEFT_MOTOR:
            leftMotorDC = abs(dc);
            if(dc < 0){
                MOTORS_SetDirection(LEFT_MOTOR, BACKWARD);
            }else{
                MOTORS_SetDirection(LEFT_MOTOR, FORWARD);
            }
            PWM_SetDutyCycle(LEFT_MOTOR_PWM_PIN, abs(dc));
            break;

        case RIGHT_MOTOR:
            rightMotorDC = abs(dc);
            if(dc < 0){
                MOTORS_SetDirection(RIGHT_MOTOR, BACKWARD);
            }else{
                MOTORS_SetDirection(RIGHT_MOTOR, FORWARD);
            }
            PWM_SetDutyCycle(RIGHT_MOTOR_PWM_PIN, abs(dc));
            break;

        default:
            return ERROR;
            break;
    }
    return SUCCESS;
}

char MOTORS_SetEncoderCount(char motor, int count){
    if(motor == LEFT_MOTOR){
        leftMotorEncoderCount = count;
        return SUCCESS;
    }
    if(motor == RIGHT_MOTOR){
        rightMotorEncoderCount = count;
        return SUCCESS;
    }
    return ERROR;
}

int MOTORS_GetEncoderCount(char motor){
    if(motor == LEFT_MOTOR){
        return leftMotorEncoderCount;
    }
    if(motor == RIGHT_MOTOR){
        return rightMotorEncoderCount;
    }
    return 0;
}

void updateLeftMotorQE(char curState){
    switch (LM_prevState) {
        case 0x00:
            switch (curState) {
                case 0x01:
                    leftMotorEncoderCount--;
                    break;

                case 0x02:
                    leftMotorEncoderCount++;
                    break;

                default:
                    //error in state transition
                    break;
            }
            break;

        case 0x01:
            switch (curState) {
                case 0x03:
                    leftMotorEncoderCount--;
                    break;
                case 0x00:
                    leftMotorEncoderCount++;
                    break;
                default:
                    //error in state transition
                    break;
            }
            break;
            
        case 0x02:
            switch (curState) {
                case 0x00:
                    leftMotorEncoderCount--;
                    break;
                case 0x03:
                    leftMotorEncoderCount++;
                    break;
                default:
                    //error in state transition
                    break;
            }
            break;
        case 0x03:
            switch (curState) {
                case 0x02:
                    leftMotorEncoderCount--;
                    break;
                case 0x01:
                    leftMotorEncoderCount++;
                    break;
                default:
                    //error in state transition
                    break;
            }
            break;
    }
    LM_prevState = curState;
}

void updateRightMotorQE(char curState){
    switch (RM_prevState) {
        case 0x00:
            switch (curState) {
                case 0x01:
                    rightMotorEncoderCount--;
                    break;

                case 0x02:
                    rightMotorEncoderCount++;
                    break;

                default:
                    //error in state transition
                    break;
            }
            break;

        case 0x01:
            switch (curState) {
                case 0x03:
                    rightMotorEncoderCount--;
                    break;
                case 0x00:
                    rightMotorEncoderCount++;
                    break;
                default:
                    //error in state transition
                    break;
            }
            break;
            
        case 0x02:
            switch (curState) {
                case 0x00:
                    rightMotorEncoderCount--;
                    break;
                case 0x03:
                    rightMotorEncoderCount++;
                    break;
                default:
                    //error in state transition
                    break;
            }
            break;
        case 0x03:
            switch (curState) {
                case 0x02:
                    rightMotorEncoderCount--;
                    break;
                case 0x01:
                    rightMotorEncoderCount++;
                    break;
                default:
                    //error in state transition
                    break;
            }
            break;
    }
    RM_prevState = curState;
}

void __ISR(_CHANGE_NOTICE_VECTOR) ChangeNotice_Handler(void) {
    static char readPort = 0;
    readPort = PORTD; // this read is required to make the interrupt work
    readPort = PORTF;
    IFS1bits.CNIF = 0;
    // returns an integer encoding current state AB
    char LM_curState = (PORTDbits.RD7 << 1) | PORTDbits.RD6;
    char RM_curState = (PORTFbits.RF4 << 1) | PORTFbits.RF5;
    
    if(LM_curState != LM_prevState){
        updateLeftMotorQE(LM_curState);
    }
    if(RM_curState != LM_prevState){
        updateRightMotorQE(RM_curState);
    }
}


//#define MOTOR_TEST
#ifdef MOTOR_TEST
#include "serial.h"
#include "timers.h"
int main(void) {
    BOARD_Init();
    SERIAL_Init();
    TIMERS_Init();
    MOTORS_Init();
    printf("\rMotors Test:\n");
    MOTORS_SetSpeed(LEFT_MOTOR, 500);
    int prevTime = 0;
    while(1){
        int curTime = TIMERS_GetMilliSeconds();
        if (abs(curTime - prevTime) > 20) {
            printf("\r%d\n", MOTORS_GetEncoderCount(LEFT_MOTOR));
            prevTime = curTime;
        }
        
    }
    BOARD_End();
}
#endif