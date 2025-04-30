#include "vex.h"
#include "robot-config.h"

motor_group adm = motor_group(L1, R1, L2, R2, L3, R3, L4, R4);
motor_group intake = motor_group(roller, conveyor);
motor_group wallStake = motor_group(wallStake1, wallStake2);


float autoRingColor, autoConveyorPosition;

//autoIsRed = True means we are sorting OUT blue, meaning we are on the red alliance.
bool autoIsRed = false;

/*void autoColorSort() {
  while (1) {
    wait(20, msec);
    
    // colorDetect.setLight(ledState::on);
    // colorDetect.setLightPower(100, percent);
  

  //print distance away on brain screen
  // Brain.Screen.setCursor(1, 30);
  // Brain.Screen.print(ringDist.objectDistance(inches));

  if ((ringDist.objectDistance(inches) < 2)) {

    //print ringColor on controller screen (removed bc printing to controller screen takes 200ms)
    autoRingColor = colorDetect.hue();
    // Controller1.Screen.setCursor(3, 14);
    // Controller1.Screen.print(autoRingColor);

    autoConveyorPosition = conveyor.position(degrees);
    
    if(autoIsRed && (autoRingColor > 360 || 20 > autoRingColor)) {
      //wait(.09, sec);
      conveyor.spin(forward, 12, volt);
      waitUntil(conveyor.position(degrees) > autoConveyorPosition + 100);
      conveyor.spin(reverse, 12 ,volt);
      wait(.2, sec);
      conveyor.spin(forward, 12, volt);
      //Controller1.rumble(".");
    }
    else if (!autoIsRed && (autoRingColor > 180 && 260 > autoRingColor)) {
      //wait(.09, sec);
      conveyor.spin(forward, 12, volt);
      waitUntil(conveyor.position(degrees) > autoConveyorPosition + 162);
      conveyor.spin(reverse, 12 ,volt);
      wait(.2, sec);
      conveyor.spin(forward, 12, volt);
      //Controller1.rumble(".");
    }
  }
  }
}*/

void drive_for_time(int timeMs, directionType direction, int maxVoltage) 
{
  adm.spin(direction, maxVoltage, volt);
  wait(timeMs, msec);
  adm.stop();
}

void drive_for_time(int timeMs, directionType direction) 
{
  drive_for_time(timeMs, direction, 7);
}

void drive_for_time(int timeMs) 
{
  drive_for_time(timeMs, fwd, 7);
}

void default_constants(){
  chassis.set_drive_constants(10, 1.5, 0, 10, 0);
  chassis.set_heading_constants(6, .4, 0, 1, 0);
  chassis.set_turn_constants(12, .4, 0.01, 3.1, 15);
  //chassis.set_swing_constants(12, .35, .002, 2, 15);
  chassis.set_swing_constants(12, .3, .001, 2, 15);
  chassis.set_drive_exit_conditions(1.5, 100, 3000);
  chassis.set_turn_exit_conditions(2, 100, 1000);
  chassis.set_swing_exit_conditions(2, 300, 1400);

  

  //Calibrate inertial sensor
  inertialSensor.calibrate();
  wait(2000, msec);
  Controller1.rumble("--");

  wallStake.setVelocity(100, percent);

  /*chassis.set_drive_constants(10, 1.5, 0, 10, 0);
  chassis.set_heading_constants(6, .4, 0, 1, 0);
  chassis.set_turn_constants(12, .4, .03, 3, 15);
  chassis.set_swing_constants(12, .3, .001, 2, 15);
  chassis.set_drive_exit_conditions(1.5, 300, 1000);
  chassis.set_turn_exit_conditions(1, 300, 3000);
  chassis.set_swing_exit_conditions(1, 300, 3000);*/
}

void odom_constants(){
  default_constants();
  chassis.drive_max_voltage = 8;
  chassis.drive_settle_error = 3;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RedLeft(){
  //Red left auton
  //score alliance stake
  chassis.set_heading(90);
  chassis.turn_to_angle(127.5);
  chassis.drive_distance(5.5);
  wallStake.spin(fwd, 12, volt);
  wait(1.5, sec);
  chassis.drive_distance(-15.25);
  wallStake.spin(reverse, 12, volt);
  //grab mogo
  chassis.turn_to_angle(180);
  chassis.drive_distance(-22); // 17.5 bare minimum to reach goal
  wallStake.stop();
  mogoMech.set(true);
  //grab stack of 2 rings
  chassis.turn_to_angle(-90);
  intake.spin(fwd, 12, volt);
  chassis.drive_distance(17);
  chassis.turn_to_angle(180);
  //grab ring in corner
  chassis.drive_distance(19);
  chassis.turn_to_angle(-135);
  // chassis.drive_distance(24);
  drive_for_time(900, fwd, 8);
  wait(200, msec);
  chassis.drive_distance(-15);
  chassis.turn_to_angle(90);
  chassis.drive_distance(60);
}

void RedRight(){
  //Red right auton
  wait(1, sec);
  chassis.set_heading(-90);
  chassis.turn_to_angle(-127.5);
  chassis.drive_distance(5.5);
  wallStake.spin(fwd, 12, volt);
  wait(.5, sec);
  chassis.drive_distance(-15.25);
  wallStake.spin(reverse, 12, volt);
  chassis.turn_to_angle(-180);
  chassis.drive_distance(-22); // 17.5 bare minimum to reach goal
  wallStake.stop();
  mogoMech.set(true);
  chassis.turn_to_angle(90);
  intake.spin(fwd, 12, volt);
  chassis.drive_distance(17);
  chassis.turn_to_angle(-180);
  chassis.drive_distance(19);
  chassis.turn_to_angle(135);
  // chassis.drive_distance(24);
  drive_for_time(900, fwd, 8);
  wait(200, msec);
  chassis.drive_distance(-15);
  chassis.turn_to_angle(-90);
  chassis.drive_distance(60);
  chassis.drive_distance(-40);
}

void BlueLeft(){
  //Blue Left auton
  wait(1, sec);
  chassis.set_heading(90);
  chassis.turn_to_angle(127.5);
  chassis.drive_distance(5.5);
  wallStake.spin(fwd, 12, volt);
  wait(.5, sec);
  chassis.drive_distance(-15.25);
  wallStake.spin(reverse, 12, volt);
  chassis.turn_to_angle(180);
  chassis.drive_distance(-22); // 17.5 bare minimum to reach goal
  wallStake.stop();
  mogoMech.set(true);
  chassis.turn_to_angle(-90);
  intake.spin(fwd, 12, volt);
  chassis.drive_distance(17);
  chassis.turn_to_angle(180);
  chassis.drive_distance(19);
  chassis.turn_to_angle(-135);
  // chassis.drive_distance(24);
  drive_for_time(900, fwd, 8);
  wait(200, msec);
  chassis.drive_distance(-15);
  chassis.turn_to_angle(90);
  chassis.drive_distance(60);
  chassis.drive_distance(-40);
}

void BlueRight(){
  //Blue right auton
  chassis.set_heading(-90);
  chassis.turn_to_angle(-127.5);
  chassis.drive_distance(5.5);
  wallStake.spin(fwd, 12, volt);
  wait(.5, sec);
  chassis.drive_distance(-15.25);
  wallStake.spin(reverse, 12, volt);
  chassis.turn_to_angle(-180);
  chassis.drive_distance(-22); // 17.5 bare minimum to reach goal
  wallStake.stop();
  mogoMech.set(true);
  chassis.turn_to_angle(90);
  intake.spin(fwd, 12, volt);
  chassis.drive_distance(17);
  chassis.turn_to_angle(-180);
  chassis.drive_distance(19);
  chassis.turn_to_angle(135);
  // chassis.drive_distance(24);
  drive_for_time(900, fwd, 8);
  wait(200, msec);
  chassis.drive_distance(-15);
  chassis.turn_to_angle(-90);
  chassis.drive_distance(60);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void redAuto(){
  //HS AWP RED
  
  autoIsRed = false;
  // thread autoColorSortThread = thread(autoColorSort);

  //rush
  roller.spin(forward, 12, volt);
  doinker.set(true);
  chassis.drive_distance(36, 35);
  chassis.drive_distance(3);
  doinker.set(false);
  wait(.1, seconds);
  chassis.drive_distance(-30);
  doinker.set(true);
  chassis.drive_distance(3);
  chassis.left_swing_to_angle(-20);
  doinker.set(false);
  wait(.1, seconds);
  chassis.left_swing_to_angle(0);
  //zero against wall
  drive_for_time(1000, reverse);
  chassis.set_heading(0);
  chassis.turn_to_angle(0);
  doinker.set(false);
  wait(.1, seconds);
  //grab mogo
  chassis.drive_distance(18);
  chassis.turn_to_angle(-145);
  chassis.set_drive_constants(4, 1.5, 0, 10, 0);
  chassis.drive_distance(-32);
  mogoMech.set(true);
  wait(.2, seconds);
  chassis.drive_distance(6);
  intake.spin(forward, 12, volt);
  //pick up both 2-ring stackes
  chassis.turn_to_angle(-90);
  chassis.set_drive_constants(4, 1.5, 0, 10, 0);
  chassis.drive_distance(40);
  chassis.drive_distance(-11);
  chassis.turn_to_angle(180);
  chassis.drive_distance(35);
  chassis.drive_distance(-23);
  //unjam conveyor if it's jammed
  intake.spin(reverse, 12, volt);
  wait(.3, seconds);
  intake.spin(forward, 12, volt);
  //grab bottom ring from neg corner
  chassis.turn_to_angle(-140);
  drive_for_time(1200, forward);
  chassis.drive_distance(-2);
  wait(.6, seconds);
  chassis.drive_distance(-10);
  //zero against side wall
  chassis.turn_to_angle(-90);
  drive_for_time(750, fwd, 5);
  wait(.1, sec);
  chassis.set_heading(-90);
  chassis.turn_to_angle(-90);
  chassis.drive_distance(-20);
  //grab ring to put on alliance stake with fish mech
  chassis.turn_to_angle(98);
  chassis.drive_distance(40.5);
  autoConveyorPosition = conveyor.position(degrees);
  waitUntil((colorDetect.isNearObject() && ringDist.objectDistance(inches) < 2.5) || (conveyor.position(degrees) == autoConveyorPosition + 3000));
  intake.stop();
  //put ring on alliance stake
  chassis.turn_to_angle(180);
  chassis.drive_distance(-4);
  wallStake.spin(fwd, 12, volt);
  wait(.7, seconds);
  wallStake.spin(reverse, 12, volt);
  wait(.8, sec);
  chassis.set_drive_constants(10, 1.5, 0, 10, 0);
  //touch the ladder
  chassis.turn_to_angle(0);
  roller.spin(forward, 12, volt);
  drive_for_time(2000, fwd, 12);
  

}

void blueAuto(){
  //HS AWP BLUE
  
  autoIsRed = true;
  // thread autoColorSortThread = thread(autoColorSort);


  //prep for rush
  roller.spin(forward, 12, volt);
  doinker.set(true);
  chassis.drive_distance(35, -60);
  chassis.drive_distance(3);
  //Rush
  //wait(.1, seconds);
  doinker.set(false);
  //wait(.1, seconds);
  chassis.drive_distance(-37, -35);
  //chassis.left_swing_to_angle(30);
  chassis.turn_to_angle(-2);
  //chassis.turn_to_angle(5);
  doinker.set(true);
  //chassis.drive_distance(3);
  chassis.turn_to_angle(-10);
  chassis.turn_to_angle(0);
  //zero against wall
  drive_for_time(1500, reverse);
  doinker.set(false);
  chassis.set_heading(0);
  chassis.turn_to_angle(0);
  wait(.1, seconds);
  //grab mogo
  chassis.drive_distance(30);
  chassis.turn_to_angle(180);
  chassis.set_drive_constants(4, 1.5, 0, 10, 0);
  chassis.drive_distance(-18);
  mogoMech.set(true);
  wait(.1, seconds);
  chassis.drive_distance(9);
  mogoMech.set(false);
  //wait(.2, seconds);
  chassis.drive_distance(-4);
  mogoMech.set(true);
  wait(.2, seconds);
  intake.spin(forward, 12, volt);
  //pick up both 2-ring stackes
  chassis.turn_to_angle(90);
  chassis.set_drive_constants(4, 1.5, 0, 10, 0);
  chassis.drive_distance(23);
  chassis.drive_distance(-9);
  chassis.turn_to_angle(180);
  chassis.drive_distance(35);
  chassis.drive_distance(-5);
  //unjam conveyor if it's jammed
  // intake.spin(reverse, 12, volt);
  // wait(.3, seconds);
  // intake.spin(forward, 12, volt);
  //grab bottom ring from neg corner
  chassis.turn_to_angle(130);
  drive_for_time(750, forward);
  chassis.drive_distance(-2);
  wait(.6, seconds);
  chassis.drive_distance(-10);
  //zero against side wall
  chassis.turn_to_angle(90);
  drive_for_time(750, fwd, 5);
  wait(.1, sec);
  chassis.set_heading(90);
  chassis.turn_to_angle(90);
  chassis.drive_distance(-20);
  //grab ring to put on alliance stake with fish mech
  chassis.turn_to_angle(-98);
  chassis.drive_distance(42);
  autoConveyorPosition = conveyor.position(degrees);
  waitUntil((colorDetect.isNearObject() && ringDist.objectDistance(inches) < 2.5) || (conveyor.position(degrees) == autoConveyorPosition + 6000));
  intake.stop();
  //put ring on alliance stake
  chassis.turn_to_angle(180);
  chassis.drive_distance(-4.5);
  wallStake.spin(fwd, 12, volt);
  wait(.8, seconds);
  wallStake.spin(reverse, 12, volt);
  wait(.9, sec);
  chassis.set_drive_constants(10, 1.5, 0, 10, 0);
  //touch the ladder
  chassis.turn_to_angle(0);
  roller.spin(forward, 12, volt);
  drive_for_time(2000, fwd, 4);

  
}

void AWP_Plus_One(){
  //Just RED Allince stake
  //wait(20, seconds);
  chassis.drive_distance(10);
  chassis.turn_to_angle(90);
  roller.spin(fwd, 12, volt);
  chassis.drive_distance(24);
  chassis.turn_to_angle(0);
  drive_for_time(1000, reverse);
  chassis.drive_distance(1.8);
  intake.spin(fwd, 12, volt);
  wait(2, sec);
  conveyor.spin(reverse, 12, volt);
  drive_for_time(2000);
  intake.stop();


}

void Prog_Skills(){
  //Prog Skills

  //grab 1 ring
  chassis.set_drive_constants(4, 1.5, 0, 10, 0);
  roller.spin(forward, 12, volt);
  chassis.drive_distance(40);
  //grab mogo
  chassis.turn_to_angle(-90);
  chassis.drive_distance(-25);
  mogoMech.set(true);
  wait(.3, seconds);
  intake.spin(forward, 12, volt);
  //grab 3 extra rings
  chassis.drive_distance(6);
  chassis.turn_to_angle(0);
  chassis.drive_distance(24);
  chassis.turn_to_angle(90);
  chassis.drive_distance(14);
  chassis.drive_distance(-5);
  chassis.turn_to_angle(-180);
  wait(1, seconds);
  // conveyor.stop();
  chassis.drive_distance(50);
  // conveyor.spin(forward);
  // waitUntil(ringDist.objectDistance(inches) < 2);
  // drive_for_time(500, fwd, 4);
  // wallStake.spin(fwd, 12, volt);
  // wait(1, seconds);
  // wallStake.spin(reverse, 12, volt);
  // wait(1, sec);
  // chassis.drive_distance(-10);
  //grab ring in corner
  chassis.turn_to_angle(135);
  drive_for_time(1000, forward);
  wait(1.5, seconds);
  conveyor.spin(reverse, 12, volt);
  wait(.5, seconds);
  conveyor.spin(forward, 12, volt);
  //put goal in corner
  chassis.drive_distance(-10);
  chassis.turn_to_angle(-45);
  drive_for_time(1000, reverse);
  conveyor.spin(reverse, 12, volt);
  mogoMech.set(false);
  //get robot out or corner
  chassis.drive_distance(15);
  wait(5, sec);

  // push goal into corner
  intake.spin(fwd, 12, volt);
  chassis.set_drive_exit_conditions(1.5, 100, 4000);
  chassis.set_drive_constants(6, 1.5, 0, 10, 0);
  chassis.drive_distance(80);
  intake.stop();
  chassis.turn_to_angle(130);
  chassis.drive_distance(-40);
  mogoMech.set(true);
  wait(.2, sec);
  intake.spin(fwd, 12 ,volt);
  chassis.turn_to_angle(-45);
  chassis.drive_distance(12);
  wait(1, sec);
  chassis.turn_to_angle(135);
  intake.stop();
  chassis.drive_distance(-40);
  mogoMech.set(false);
  chassis.set_drive_exit_conditions(1.5, 100, 1000);
  chassis.drive_distance(19);
  chassis.drive_distance(-20);
  chassis.drive_distance(20);


}

void odom_test(){
  //RED Alliance HS auton Neg corner

  autoIsRed = true;

  chassis.set_drive_constants(4.5, 1.5, 0, 10, 0);
  chassis.set_heading(90);
  chassis.turn_to_angle(127.5);
  chassis.drive_distance(5.5);
  wallStake.spin(fwd, 12, volt);
  wait(.5, sec);
  chassis.drive_distance(-15.25);
  wallStake.spin(reverse, 12, volt);
  chassis.turn_to_angle(180);
  chassis.drive_distance(-22); // 17.5 bare minimum to reach goal
  wallStake.stop();
  mogoMech.set(true);
  chassis.turn_to_angle(-90);
  intake.spin(fwd, 12, volt);
  chassis.drive_distance(17);
  chassis.turn_to_angle(180);
  chassis.drive_distance(19);
  chassis.turn_to_angle(-135);
  chassis.drive_distance(24);
  wait(200, msec);
  chassis.drive_distance(-15);
  chassis.turn_to_angle(90);
  chassis.drive_distance(60);

}

void holonomic_odom_test(){
  //Empty auton slot

  

}

void worlds_auton(){
  //Worlds Auton

  //AWP + score some form the middle and cross the bar to start on the far side

  

}

void safe_worlds_auton(){
  //Safe worlds auton
  //Maybe use for safe auton where we shave a few seconds off


}



