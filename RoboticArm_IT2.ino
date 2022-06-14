#include <Stepper.h>
#include <Wire.h>

#include "Kinematics.h"
#include "Pinouts.h"
#include "Constants.h"
#include "MotorControl.h"
#include "Globals.h"

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(100000);

  pinMode(FAN_PIN , OUTPUT);
  pinMode(HEATER_0_PIN , OUTPUT);
  pinMode(HEATER_1_PIN , OUTPUT);
  pinMode(LED_PIN  , OUTPUT);

  pinMode(X_STEP_PIN  , OUTPUT);
  pinMode(X_DIR_PIN    , OUTPUT);
  pinMode(X_ENABLE_PIN    , OUTPUT);

  pinMode(Y_STEP_PIN  , OUTPUT);
  pinMode(Y_DIR_PIN    , OUTPUT);
  pinMode(Y_ENABLE_PIN    , OUTPUT);

  pinMode(Z_STEP_PIN  , OUTPUT);
  pinMode(Z_DIR_PIN    , OUTPUT);
  pinMode(Z_ENABLE_PIN    , OUTPUT);


  pinMode(Q_STEP_PIN  , OUTPUT);
  pinMode(Q_DIR_PIN    , OUTPUT);
  pinMode(Q_ENABLE_PIN    , OUTPUT);

  pinMode(E_STEP_PIN  , OUTPUT);
  pinMode(E_DIR_PIN    , OUTPUT);
  pinMode(E_ENABLE_PIN    , OUTPUT);

  pinMode(START_BTN_PIN  , INPUT);
  pinMode(ZERO_BTN_PIN  , INPUT);
  pinMode(LOST_POWER_PIN  , INPUT);

  digitalWrite(X_ENABLE_PIN    , LOW);
  digitalWrite(Y_ENABLE_PIN    , LOW);
  digitalWrite(Z_ENABLE_PIN    , LOW);
  digitalWrite(E_ENABLE_PIN    , LOW);
  digitalWrite(Q_ENABLE_PIN    , LOW);

  pinMode (DID_WRITE_STATE_LED_PIN, OUTPUT);
  digitalWrite(DID_WRITE_STATE_LED_PIN, LOW);

  delay(500);

  currentAngles = ReadDegrees();
  WriteDegrees(currentAngles);
  digitalWrite(DID_WRITE_STATE_LED_PIN, HIGH);
}

/*
   This function takes in 5 generic types and returns the max of the five.
*/

template<typename T>
T FindMax(T x, T y, T z, T q, T w) {
  T maxSteps = x;
  if (y > maxSteps)
    maxSteps = y;
  if (z > maxSteps)
    maxSteps = z;
  if (q > maxSteps)
    maxSteps = q;
  if (w > maxSteps)
    maxSteps = w;
  return maxSteps;
}

/*
   This function takes in the target degrees for a joint and a function pointer that changes the direction
   of that joint. If the target degrees is negative, it changes the direction of the motor.
*/

int SetJointDirection(double targetDegrees, void (*ChangeDirection)(bool)) {
  if (targetDegrees < 0) {
    ChangeDirection(false);
    return -1;
  }
  else
    ChangeDirection(true);
  return 1;
}
/*
   This function takes in the number of steps that each joint will take and fills out and allocates the JointDelays struct pointer.
*/

JointDelays *SetJointDelays(long shoulderSteps, long elbowSteps, long wristSteps, long wristRotateSteps, long rotateSteps) {
  JointDelays *jd = malloc(sizeof(JointDelays));
  jd->main = FindMax(shoulderSteps, elbowSteps, wristSteps, wristRotateSteps, rotateSteps);
  
  if (shoulderSteps != 0)
    jd->shoulder =     (jd->main / shoulderSteps);
  else 
    jd->shoulder = 0;
  
  if (elbowSteps != 0)
    jd->elbow =        (jd->main / elbowSteps);
  else 
    jd->elbow = 0;
  
  if (wristSteps != 0)
    jd->wrist =        (jd->main / wristSteps);
  else 
    jd->wrist = 0;
  
  if (wristRotateSteps != 0)
    jd->wristRotate =  (jd->main / wristRotateSteps);
  else 
    jd->wristRotate = 0;

  if (rotateSteps != 0)
    jd->rotate =       (jd->main / rotateSteps);
  else 
    jd->rotate = 0;
  return jd;
}

/*
   This function takes in absolute degrees and moves the arm to those absolute degrees based on the currentDegrees struct.
*/

void MoveNewDegrees(double shoulderDegrees, double elbowDegrees, double wristDegrees, double wristRotateDegrees, double rotateDegrees)
{
  if (g_doStopArm != NO_PROBLEM)
    return;
    
  if (isnan(rotateDegrees)) 
    rotateDegrees = 0;
  if (isnan(wristRotateDegrees)) 
    wristRotateDegrees = 0;
  
  digitalWrite(DID_WRITE_STATE_LED_PIN, LOW);
  // Check that the angles are possible
  bool didFail = false;
  if (wristDegrees < WristMinAngle || wristDegrees > WristMaxAngle || isnan(wristDegrees) == true)
  {
    didFail = true;
    Print("Impossible Wrist Angle: %f\n", wristDegrees);
  }
  if (elbowDegrees < ElbowMinAngle || elbowDegrees > ElbowMaxAngle || isnan(wristDegrees) == true) {
    didFail = true;
    Print("Impossible Elbow Angle: %f\n", elbowDegrees);
  }
  if (shoulderDegrees > ShoulderMaxAngle || shoulderDegrees < ShoulderMinAngle || isnan(wristDegrees) == true) {
    didFail = true;
    Print("Impossible Shoulder Angle: %f\n", shoulderDegrees);
  }
  if (didFail == true) {
    digitalWrite(DID_WRITE_STATE_LED_PIN, HIGH);
    return;
  }

  // find target degrees compared to current
  double targetShoulder =  shoulderDegrees - currentAngles.shoulder;
  double targetElbow =  elbowDegrees - currentAngles.elbow;
  double targetWrist =  wristDegrees - currentAngles.wrist;
  double targetWristRotate =  wristRotateDegrees - currentAngles.wristRotate;
  double targetRotate = rotateDegrees - currentAngles.rotate;

  // set directions if target is negative
  int shoulderIsPos = SetJointDirection(targetShoulder, SetShoulderDirection);
  int elbowIsPos = SetJointDirection(targetElbow, SetElbowDirection);
  int wristIsPos = SetJointDirection(targetWrist, SetWristDirection);
  int wristRotIsPos = SetJointDirection(targetWristRotate, SetWristRotateDirection);
  int rotIsPos = SetJointDirection(targetRotate, SetRotateDirection);

  targetShoulder = abs(targetShoulder);
  targetElbow = abs(targetElbow);
  targetWrist = abs(targetWrist);
  targetWristRotate = abs(targetWristRotate);
  targetRotate = abs(targetRotate);

  long shoulderSteps =     (long)(shoulderStepsPerDegree * targetShoulder);
  long elbowSteps =        (long)(elbowStepsPerDegree * targetElbow);
  long wristSteps =        (long)(wristStepsPerDegree * targetWrist);
  long wristRotateSteps =  (long)(wristRotateStepsPerDegree * targetWristRotate);
  long rotateSteps =       (long)(rotateStepsPerDegree * targetRotate);

  JointDelays *delays = SetJointDelays(shoulderSteps, elbowSteps, wristSteps, wristRotateSteps, rotateSteps);

  /*
  Serial.println(shoulderDegrees);
  Serial.println(currentAngles.shoulder);
  Serial.println(targetShoulder);


  Serial.print("Delays:");
  Serial.print("  Main: ");
  Serial.println(delays->main);
  Serial.print("    S: ");
  Serial.println(delays->shoulder, 6);
  Serial.print("    E: ");
  Serial.println(delays->elbow, 6);
  Serial.print("    W: ");
  Serial.println(delays->wrist, 6);
  Serial.print("    WR: ");
  Serial.println(delays->wristRotate, 6);
  Serial.print("    R: ");
  Serial.println(delays->rotate, 6);

  Serial.print("Steps:");
  Serial.print("    S: ");
  Serial.println(shoulderSteps);
  Serial.print("    E: ");
  Serial.println(elbowSteps);
  Serial.print("    W: ");
  Serial.println(wristSteps);
  Serial.print("    WR: ");
  Serial.println(wristRotateSteps);
  Serial.print("    R: ");
  Serial.println(rotateSteps);
  */

  SetFunctionPointers();

  //Print("Target Delays[M: %ld, S: %ld,", delays->main, delays->shoulder);
  //Print("E: %ld, W: %ld, WR: %ld, R: %ld]\n", delays->elbow, delays->wrist, delays->wristRotate, delays->rotate);

  long currentShoulderSteps = 0;
  long currentElbowSteps = 0;
  long currentWristSteps = 0;
  long currentWristRotateSteps = 0;
  long currentRotateSteps = 0;
  bool didAct = false;
  for (int x = 0; x < delays->main; x++)
  {
    g_doStopArm = CheckIfProblemExists();
    if (g_doStopArm != NO_PROBLEM)
      break;
 
    else {
      if ((delays->shoulder != 0) & ((x % (long)delays->shoulder) == 0) & (currentShoulderSteps < shoulderSteps))
      {
        shoulderHigh();
        currentAngles.shoulder += shoulderIsPos * shoulderDegreesPerStep;
        currentShoulderSteps++;
        didAct = true;
      }
      if ((delays->elbow != 0) & ((x % (long)delays->elbow) == 0) & (currentElbowSteps < elbowSteps))
      {
        elbowHigh();
        currentAngles.elbow += elbowIsPos * elbowDegreesPerStep;
        currentElbowSteps++;
        didAct = true;
      }
      if ((delays->wrist != 0) & ((x % (long)delays->wrist) == 0) & (currentWristSteps < wristSteps))
      {
        wristHigh();
        currentAngles.wrist += wristIsPos * wristDegreesPerStep;
        currentWristSteps++;
        didAct = true;
      }
      if ((delays->wristRotate != 0) & ((x % (long)delays->wristRotate) == 0) & (currentWristRotateSteps < wristRotateSteps))
      {
        wristRotateHigh();
        currentAngles.wristRotate += wristRotIsPos * wristRotateDegreesPerStep;
        currentWristRotateSteps++;
        didAct = true;
      }
      if ((delays->rotate != 0) & ((x % (long)delays->rotate) == 0) & (currentRotateSteps < rotateSteps))
      {
        rotateHigh();
        currentAngles.rotate += rotIsPos * rotateDegreesPerStep;
        currentRotateSteps++;
        didAct = true;
      }

      if (didAct)
        delayMicroseconds(MotorSpeed);

      if ((delays->shoulder != 0) & ((x % (long)delays->shoulder) == 0))
        shoulderLow();
      if ((delays->elbow != 0) & ((x % (long)delays->elbow) == 0))
        elbowLow();
      if ((delays->wrist != 0) & ((x % (long)delays->wrist) == 0))
        wristLow();
      if ((delays->wristRotate != 0) & ((x % (long)delays->wristRotate) == 0))
        wristRotateLow();
      if ((delays->rotate != 0) & ((x % (long)delays->rotate) == 0))
        rotateLow();

      didAct = false;
    }
  }
  WriteDegrees(currentAngles);
  digitalWrite(DID_WRITE_STATE_LED_PIN, HIGH);
  free(delays);
}


bool runOnce = true;
void loop ()
{
  int zeroBtnVal = digitalRead(ZERO_BTN_PIN);
  if (zeroBtnVal == HIGH) {
    digitalWrite(DID_WRITE_STATE_LED_PIN, LOW);
    currentAngles = { 90, 0, 0, 0, 0};
    WriteDegrees(currentAngles);
    digitalWrite(DID_WRITE_STATE_LED_PIN, HIGH);
  }
  
  int btnVal = digitalRead(START_BTN_PIN);
  if (btnVal == HIGH) {
    if (runOnce == true) {
      //PrintStoredDegrees();
      AutoHome();
        
      //for (int x = 118; x < 155; x++)
      //  MoveToPoint(0, x, 75, theta3);


      MoveToPoint(0, 130, 75, theta3);
      AutoHome();
      
      //MoveToPoint(0, 62, 70, theta3);
      //MoveToPoint(0, 154, 90, theta3);
      //MoveToPoint(0, 100, 80, theta3);
      //MoveToPoint(0, 62, 70, theta3);

      //MoveToPoint(0, 90, 54, theta3);
      //MoveToPoint(0, 90, 39, theta3);
      
      //MoveToPoint(0, 0, 331, 90);
      //AutoHome();
      //MoveToPoint(0, 90, 39, theta3);
      runOnce = false;
    }
  }
}
