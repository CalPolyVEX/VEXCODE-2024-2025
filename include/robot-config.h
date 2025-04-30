#pragma once

using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor roller;
extern motor conveyor;
extern motor wallStake1;
extern motor wallStake2;
extern motor L1;
extern motor L2;
extern motor L3;
extern motor L4;
extern motor R1;
extern motor R2;
extern motor R3;
extern motor R4;
extern inertial inertialSensor;
extern optical colorDetect;
extern distance ringDist;
extern digital_out mogoMech;
extern digital_out doinker;
extern digital_out tipper;
extern digital_out ringChuck;
extern digital_in ringSwitch;
extern rotation rotationWallStake;
// extern motor_group leftDriveMotors;
// extern motor_group rightDriveMotors;
// extern motor_group allDriveMotors;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );