double CalculateStepsPerDegree(double stepRatio, double stepAngle, double gearRatio); 
void MoveToPoint(double x, double y, double z, double theta3);
void AutoHome();

struct Vector
{
  public:
    long x;
    long y;
    long z;
};
struct VectorAngles
{
  public:
    double shoulder;
    double elbow;
    double wrist;
    double wristRotate;
    double rotate;
};
struct JointDelays
{
  public:
    double shoulder;
    double elbow;
    double wrist;
    double wristRotate;
    double rotate;
    long main;
};
VectorAngles FindAngles(Vector vector, float angleOfLink3);
