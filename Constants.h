#define MotorSpeed     500

#define ShoulderMaxAngle 180
#define ShoulderMinAngle 0
#define ElbowMaxAngle 112
#define ElbowMinAngle -112
#define WristMaxAngle 112
#define WristMinAngle -112

/*          FAIL STATES     */
#define NO_PROBLEM   0
#define LOST_POWER   1


const long length1 = 115.5;
const long length2 = 141.5;
const long length3 = 75;

const double stepRatio = 0.25f;

const double shoulderStepsPerDegree    = CalculateStepsPerDegree(stepRatio, 1.8f, 66.46f); // (6646/(double)45);
const double elbowStepsPerDegree       = CalculateStepsPerDegree(stepRatio, 1.8f, 66.46f);
const double wristStepsPerDegree       = CalculateStepsPerDegree(stepRatio, 1.8f, 66.46f);
const double wristRotateStepsPerDegree = CalculateStepsPerDegree(stepRatio, 1.8f, 66.46f);
const double rotateStepsPerDegree      = CalculateStepsPerDegree(stepRatio, 1.8f, 66.46f);

const double shoulderDegreesPerStep    = 1 / shoulderStepsPerDegree;
const double elbowDegreesPerStep       = 1 / elbowStepsPerDegree;
const double wristDegreesPerStep       = 1 / wristStepsPerDegree;
const double wristRotateDegreesPerStep = 1 / wristRotateStepsPerDegree;
const double rotateDegreesPerStep      = 1 / rotateStepsPerDegree;

const int SHOULDER_STEP_PIN = Z_STEP_PIN;
const int SHOULDER_DIR_PIN = Z_DIR_PIN;

const int ELBOW_STEP_PIN = Q_STEP_PIN;
const int ELBOW_DIR_PIN = Q_DIR_PIN;

const int WRIST_STEP_PIN = Y_STEP_PIN;
const int WRIST_DIR_PIN = Y_DIR_PIN;

const int WRISTROTATE_STEP_PIN = X_STEP_PIN;
const int WRISTROTATE_DIR_PIN = X_DIR_PIN;

const int ROTATE_STEP_PIN = E_STEP_PIN;
const int ROTATE_DIR_PIN = E_DIR_PIN;
