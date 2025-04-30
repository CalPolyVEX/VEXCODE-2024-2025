#include "vex.h"
#include "robot-config.h"

// What I changed since the last version of code:

// lb code + one button mogo mech

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration ****SMALL BOT****:
// [Name]               [Type]        [Port(s)]
// Controller1          controller
// L1                   motor         6
// L2                   motor         7
// L3                   motor         8
// R1                   motor         1
// R2                   motor         2
// R3                   motor         3
// roller               motor         5
// conveyor             motor         4
// wallStake            motor         9
// doinker              digital_out   B
// ringChuck            digital_out   C
// mogoMech             digital_out   D
// colorDetect          optical       21
// ---- END VEXCODE CONFIGURED DEVICES ----

using namespace vex;
competition Competition;

motor_group leftDriveMotors = motor_group(L1, L2, L3);
motor_group rightDriveMotors = motor_group(R1, R2, R3);
motor_group wallStakeMain = motor_group(wallStake1, wallStake2);


float ringColor;

bool wallStakeFeedFwdDis, isRed, ringSortDisable = true, ringDetectOverride, antijamDisable = true;

thread wsThread;

#define LOADING 1
#define PRESCORING 2
#define SCORING 3

#ifdef GOLD
#define VERT PORT6
#define HORI PORT8
#define INERTIAL PORT21
#endif

#ifdef GREEN
#define VERT PORT5
#define HORI PORT6
#define INERTIAL PORT9
#endif


Drive chassis(

  //Pick your drive setup from the list below:
  //ZERO_TRACKER_NO_ODOM
  //ZERO_TRACKER_ODOM
  //TANK_ONE_FORWARD_ENCODER
  //TANK_ONE_FORWARD_ROTATION
  //TANK_ONE_SIDEWAYS_ENCODER
  //TANK_ONE_SIDEWAYS_ROTATION
  //TANK_TWO_ENCODER
  //TANK_TWO_ROTATION
  //HOLONOMIC_TWO_ENCODER
  //HOLONOMIC_TWO_ROTATION
  //
  //Write it here:
  TANK_TWO_ROTATION,
  
  //Add the names of your Drive motors into the motor groups below, separated by commas, i.e. motor_group(Motor1,Motor2,Motor3).
  //You will input whatever motor names you chose when you configured your robot using the sidebar configurer, they don't have to be "Motor1" and "Motor2".
  
  //Left Motors:
  leftDriveMotors,
  
  //Right Motors:
  rightDriveMotors,
  
  //Specify the PORT NUMBER of your inertial sensor, in PORT format (i.e. "PORT1", not simply "1"):
  INERTIAL,
  
  //Input your wheel diameter. (4" omnis are actually closer to 4.125"):
  2.75,
  
  //External ratio, must be in decimal, in the format of input teeth/output teeth.
  //If your motor has an 84-tooth gear and your wheel has a 60-tooth gear, this value will be 1.4.
  //If the motor drives the wheel directly, this value is 1:
  (float) (0.94736842),
  
  //Gyro scale, this is what your gyro reads when you spin the robot 360 degrees.
  //For most cases 360 will do fine here, but this scale factor can be very helpful when precision is necessary.
  360,
  
  /*---------------------------------------------------------------------------*/
  /*                                  PAUSE!                                   */
  /*                                                                           */
  /*  The rest of the drive constructor is for robots using POSITION TRACKING. */
  /*  If you are not using position tracking, leave the rest of the values as  */
  /*  they are.                                                                */
  /*---------------------------------------------------------------------------*/
  
  //If you are using ZERO_TRACKER_ODOM, you ONLY need to adjust the FORWARD TRACKER CENTER DISTANCE.
  
  //FOR HOLONOMIC DRIVES ONLY: Input your drive motors by position. This is only necessary for holonomic drives, otherwise this section can be left alone.
  //LF:      //RF:    
  PORT16,     PORT16,
  
  //LB:      //RB: 
  PORT16,     PORT16,
  
  //If you are using position tracking, this is the Forward Tracker port (the tracker which runs parallel to the direction of the chassis).
  //If this is a rotation sensor, enter it in "PORT1" format, inputting the port below.
  //If this is an encoder, enter the port as an integer. Triport A will be a "1", Triport B will be a "2", etc.
  VERT,
  
  //Input the Forward Tracker diameter (reverse it to make the direction switch):
  -2.00,
  
  //Input Forward Tracker center distance (a positive distance corresponds to a tracker on the right side of the robot, negative is left.)
  //For a zero tracker tank drive with odom, put the positive distance from the center of the robot to the right side of the drive.
  //This distance is in inches:
  0.0,
  
  //Input the Sideways Tracker Port, following the same steps as the Forward Tracker Port:
  HORI,
  
  //Sideways tracker diameter (reverse to make the direction switch):
  2.0,
  
  //Sideways tracker center distance (positive distance is behind the center of the robot, negative is in front):
  -0.75
  
  );

motor_group intakeMain = motor_group(roller, conveyor);
motor_group admMain = motor_group(L1, R1, L2, R2, L3, R3, L4, R4);
// motor wallStake = motor(PORT1, ratio18_1, false);

int current_auton_selection = 0;
int conveyorPosition, wallStakeState = 0;
bool auto_started = false;

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  default_constants();

  chassis.set_drive_constants(5.5, 1.5, 0, 10, 0);


  while (auto_started == false) {
    Brain.Screen.clearScreen();
    switch (current_auton_selection) {
    case 0:
      Brain.Screen.printAt(50, 50, "Red Left");
      break;
    case 1:
      Brain.Screen.printAt(50, 50, "Red Right");
      break;
    case 2:
      Brain.Screen.printAt(50, 50, "Blue Left");
      break;
    case 3:
      Brain.Screen.printAt(50, 50, "Blue Right");
      break;
    case 4:
      Brain.Screen.printAt(50, 50, "TEST DO NOT USE - Empty Slot");
      break;
    case 5:
      Brain.Screen.printAt(50, 50, "DO NOT USE - Empty Slot");
      break;
    case 6:
      Brain.Screen.printAt(50, 50, "Blue Auto v2");
      break;
    case 7:
      Brain.Screen.printAt(50, 50, "Red Auto v2");
      break;
    }
    if (Brain.Screen.pressing()) {
      while (Brain.Screen.pressing()) {
        wait(10, msec);
      }
      current_auton_selection++;
    } else if (current_auton_selection == 8) {
      current_auton_selection = 0;
    }
    task::sleep(10);
  }
}

void autonomous(void) {
  auto_started = true;
  switch (current_auton_selection) {
  case 0:
    isRed = true;
    ringSortDisable = false;
    wallStakeFeedFwdDis = true;
    RedLeft();
    break;
  case 1:
    isRed = true;
    ringSortDisable = false;
    wallStakeFeedFwdDis = true;
    RedRight();
    break;
  case 2:
    isRed = false;
    wallStakeFeedFwdDis = true;
    ringSortDisable = false;
    BlueLeft();
    break;
  case 3:
    isRed = false;
    wallStakeFeedFwdDis = true;
    ringSortDisable = false;
    BlueRight();
    break;
  case 4:
    odom_test();
    break;
  case 5:
    safe_worlds_auton();
    break;
  case 6:
    worlds_auton();
    break;
  }
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/



int whenStarted1() {
  // // Zero wallStake
  // wallStake.setStopping(hold);
  // wallStakeFeedFwdDis = false;
  // wallStake.setVelocity(80, rpm);
  // while (!(wallStake.current(amp) > 1.4)) {
  //   wallStake.spin(reverse);
  //   wait(10, msec);
  // }
  // wallStake.setPosition(0, degrees);
  // wallStake.stop();
  // Set drive motor stopping to coast
  admMain.setStopping(coast);

  colorDetect.setLight(ledState::on);
  colorDetect.setLightPower(100, percent);
  wallStakeMain.setVelocity(100, percent);

  return 0;
}


//**RING SORTING**
void colorSort() {
   
  //  ANTI-JAM ANTIJAM ANTI JAM
  //  while(!antijamDisable){
  //     wait(10, msec);

  //     if((conveyor.velocity(rpm) == 0) && (conveyor.current(amp) > 2.1)) {
  //     conveyor.spin(reverse, 12, volt);
  //     wait(.2, seconds);
  //     conveyor.spin(forward, 9, volt);
  //     wait(.2, seconds);
  //   }
  while(1){
    wait(10, msec);
  
  //print distance away on brain screen
  Brain.Screen.setCursor(1, 30);
  Brain.Screen.print(ringDist.objectDistance(inches));

  if (ringDist.objectDistance(inches) > 2.5 || ringSortDisable) continue;

    //print ringColor on controller screen (removed bc printing to controller screen takes 200ms)
    ringColor = colorDetect.hue();
    // Controller1.Screen.setCursor(3, 14);
    // Controller1.Screen.print(ringColor);

    conveyorPosition = conveyor.position(degrees);
    
    if(!isRed && (ringColor > 360 || 20 > ringColor)) {
      //wait(.09, sec);
      //printf("Launching red\n");
      conveyor.spin(forward, 12, volt);
      waitUntil(conveyor.position(degrees) > conveyorPosition + 700);
      //wait(10 * conveyor.current(amp), msec);
      conveyor.spin(reverse, 12 ,volt);
      wait(.2, sec);
      conveyor.spin(forward, 12, volt);
      Controller1.rumble("..");
      wait(200, msec);
    }
    else if (isRed && (ringColor > 180 && 260 > ringColor)) {
      //wait(.09, sec);
      //printf("Launching blue\n");
      conveyor.spin(forward, 12, volt);
      waitUntil(conveyor.position(degrees) > conveyorPosition + 700);
      //wait(10 * conveyor.current(amp), msec);
      conveyor.spin(reverse, 12 ,volt);
      wait(.2, sec);
      conveyor.spin(forward, 12, volt);
      Controller1.rumble(".");
      wait(200, msec);
    }

    // Controller1.Screen.setCursor(3, 14);
    // Controller1.Screen.print(ringColor);
    
  }
}


void wallStakeAutoHold() {
  colorDetect.setLight(ledState::on);
  colorDetect.setLightPower(100, percent);
  wallStakeMain.setVelocity(100, percent);
  wallStakeMain.setStopping(hold);
  wallStakeFeedFwdDis = false;
  // Set drive motor stopping to coast
  admMain.setStopping(coast);
  
  while (1) {
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print(wallStakeMain.position(degrees));
    Brain.Screen.print("   ");
    if(Controller1.ButtonDown.pressing()){
      wallStakeMain.setPosition(0, degrees);
    }

    // if (!wallStakeFeedFwdDis && wallStake.position(degrees) < 100) {
    //   wallStake.spin(reverse, 1, volt);
    // }

    wait(20, msec);
  }
}

void usercontrol(void) {
  ringSortDisable = true;
  antijamDisable = true;
  admMain.setStopping(coast);
}

void updateDrivetrainVelocity() {
  int leftVelocity =
      (Controller1.Axis3.position() + (Controller1.Axis1.position() / 1)) * 120;
  int rightVelocity =
      (Controller1.Axis3.position() - (Controller1.Axis1.position() / 1)) * 120;

  if (abs(Controller1.Axis3.position()) < 5 && abs(Controller1.Axis1.position()) < 5)
  {
    leftVelocity = 0;
    rightVelocity = 0;
  }

  motor_group(L1, L2, L3, L4)
      .spin(vex::forward, leftVelocity, vex::voltageUnits::mV);
  motor_group(R1, R2, R3, R4)
      .spin(vex::forward, rightVelocity, vex::voltageUnits::mV);
}

// intake control
void onevent_Controller1ButtonL1_pressed_0() {
  //intakeMain.spin(forward, 9, volt);
  conveyor.spin(fwd, 12, volt);
  roller.spin(fwd, 12, volt);
}

void onevent_Controller1ButtonL2_pressed_0() {
  intakeMain.spin(reverse, 12, volt);
}

void onevent_Controller1ButtonL2_released_0() { intakeMain.stop(); }

void toggleDoinker() { doinker.set(!doinker.value()); }

void onR2Pressed()
//add later**** Make controller rumble once when clamped, rumble twice when released******
{ 
  // mogoMech.set(!mogoMech.value());
  bool newState = !mogoMech.value();
  mogoMech.set(newState);
}

void onDownPressed()
{
  bool newTipperState = !tipper.value();
  tipper.set(newTipperState);
}

void wsSpinToPosition(int position, double kP, double kD, double tolerance)
{  
  // double kP = 200;
  // double kD = 1000;

  rotationWallStake.resetPosition();

  double startPos = rotationWallStake.position(deg);
  double target = startPos + position;

  double error = target - rotationWallStake.position(deg);
  double lastError = 0;

  while(fabs(error) > tolerance)
  {
    error = target - rotationWallStake.position(deg);
    wallStakeMain.spin(fwd, (error * kP) + ((error - lastError) * kD), vex::voltageUnits::mV);
    lastError = error;
    wait(10, msec);
  }

  wallStakeMain.stop(hold);
}

int wsState = 0;

void onR1Pressed() {
  wsState = ++wsState & 3;

  wsThread.interrupt();

  
  if (wsState == LOADING)
  {
    wsThread = thread([](){
      wsSpinToPosition(13, 300, 0, 1);
      waitUntil(conveyor.current(amp) > 2.1 && conveyor.velocity(rpm) < 2);
      conveyor.spin(fwd, 2, volt);
      wallStakeMain.stop(coast);
    });
  
    roller.spin(fwd, 12, volt);
    conveyor.spin(fwd, 12, volt);
  } else if (wsState == PRESCORING)
  {
    wsThread = thread([](){
      conveyor.spin(reverse, 4, volt);
      wsSpinToPosition(68, 250, 0, 3);
      conveyor.stop();
      if (Controller1.ButtonY.pressing())
      {
        wsSpinToPosition(59, 200, 0, 2);
        wallStakeMain.spin(reverse, 12, volt);
        wait(500, msec);
        wallStakeMain.stop(coast);
        wsState = 0;
      }
      
    });
  } else if (wsState == SCORING)
  {
    wsThread = thread([](){
      wsState = 0;
      wsSpinToPosition(60, 200, 0, 5);
      wallStakeMain.spin(reverse, 12, volt);
      wait(400, msec);
      wallStakeMain.stop(coast);
    });
    
  }
  
  antijamDisable = false;
}

void onRightPressed() {
  ringSortDisable = false;
  isRed = false;
  Controller1.Screen.setCursor(3, 1);
  Controller1.Screen.print("chuck red ");
}

void onLeftPressed() {

  ringSortDisable = false;
  isRed = true;
  Controller1.Screen.setCursor(3, 1);
  Controller1.Screen.print("chuck blue");
}

void onAPressed() {
  ringDetectOverride = false;
  Controller1.rumble("-");
  while (!(((ringDist.objectDistance(inches) < 2.5)) || ringDetectOverride)) {
    roller.spin(forward, 12, volt);
    conveyor.spin(forward, 7, volt);
    wait(10, msec);
  }

  
  if (!ringDetectOverride) {
    intakeMain.stop();
    conveyor.stop();
  }
  roller.spin(fwd, 8, volt);
}

void enableRingDetectOverride() { ringDetectOverride = true; }

void onAxis2Changed() {
  int position = Controller1.Axis2.position();
  if (abs(position) < 50) {
    wallStakeFeedFwdDis = false;
    wallStake1.stop(brake);
    wallStake2.stop(hold);
    return;
  }
  wsThread.interrupt();
  wsState = 0;
  conveyor.stop();
  wallStakeFeedFwdDis = true;
  wallStakeMain.spin(fwd, position * 0.12, volt);
}

//void autonomousRoutine() {
  // Code here
//}

int main() {
  leftDriveMotors.setStopping(vex::coast);
  rightDriveMotors.setStopping(vex::coast);
  thread colorSortThread = thread(colorSort);
  thread wsAutoHold = thread(wallStakeAutoHold);

  rotationWallStake.setPosition(0, degrees);

  colorDetect.integrationTime(5);

  Controller1.ButtonL1.pressed(onevent_Controller1ButtonL1_pressed_0);
  Controller1.ButtonL2.pressed(onevent_Controller1ButtonL2_pressed_0);
  Controller1.ButtonL2.released(onevent_Controller1ButtonL2_released_0);
  Controller1.Axis1.changed(updateDrivetrainVelocity);
  Controller1.Axis3.changed(updateDrivetrainVelocity);
  Controller1.Axis2.changed(onAxis2Changed);
  Controller1.ButtonB.pressed(toggleDoinker);
  Controller1.ButtonL1.pressed(enableRingDetectOverride);
  Controller1.ButtonL2.pressed(enableRingDetectOverride);
  Controller1.ButtonLeft.pressed(onLeftPressed);
  Controller1.ButtonRight.pressed(onRightPressed);
  Controller1.ButtonR1.pressed(onR1Pressed);
  Controller1.ButtonR2.pressed(onR2Pressed);
  Controller1.ButtonDown.pressed(onDownPressed);
  Controller1.ButtonA.pressed(onAPressed);

  Competition.drivercontrol(usercontrol);
  Competition.autonomous(autonomous);
  pre_auton();
}