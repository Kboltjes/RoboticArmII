void AutoHome() {
  if (g_doStopArm != NO_PROBLEM)
    return;
  MoveToPoint(0, 0, (length1 + length2 + length3), 90);
}
/*
   This function moves the arm to an absolute point in space.
*/

void MoveToPoint(double x, double y, double z, double theta3)
{
  if (g_doStopArm != NO_PROBLEM)
    return;
  digitalWrite(DID_WRITE_STATE_LED_PIN, LOW);
  if (x == 0 && y == 0 && z == (length1 + length2 + length3) && theta3 == 90) {
    MoveNewDegrees(90, 0, 0, 0, 0);
  }
  Vector vec;
  vec.x = x;
  vec.z = y;
  vec.y = z;
  currentPoint.x = x;
  currentPoint.y = y;
  currentPoint.z = z;

  VectorAngles angles = FindAngles(vec, theta3);

  MoveNewDegrees(angles.shoulder, angles.elbow, (180 - angles.wrist), currentAngles.wristRotate, angles.rotate);
}

double CalculateStepsPerDegree(double stepRatio, double stepAngle, double gearRatio) {
  double shaftDegreesPerRevolution = gearRatio * 360;
  double shaftDegreePerStep = stepAngle * stepRatio;
  double stepsPerRevolution = shaftDegreesPerRevolution / shaftDegreePerStep;
  double stepsPerDegree = stepsPerRevolution / 360;
  return stepsPerDegree;
}

VectorAngles FindAngles(Vector vector, double angleOfLink3)
{
  double distanceOut = sqrt((vector.x * vector.x) + (vector.z * vector.z));
  //vector.z -= length3*cos(radians(angleOfLink3));
  distanceOut -= length3*cos(radians(angleOfLink3));
  vector.y -= length3*sin(radians(angleOfLink3));
   
  //double r   = (vector.z * vector.z) + (vector.y * vector.y);
  double r   = (distanceOut * distanceOut) + ((double)vector.y * vector.y);
  long length_squared  = (length1 * length1) + (length2 * length2);
  double term2 = (r - length_squared)/((double)2*length1*length2);
  double term1 = -1 * sqrt(1 - (term2 * term2));
  
  double th2   = atan2(term1, term2);
  
  double k    = length1 + length2*cos(th2);
  double k1    = length2*sin(th2);
  double gamma = atan2(k1,k);
  
  //float th1   = atan2(vector.y, vector.z) - gamma;
  double th1   = atan2(vector.y, distanceOut) - gamma;
  
  double shoulderDegrees = abs(degrees(th1));
  double elbowDegrees = -1*degrees(th2);

  double a = 180 - shoulderDegrees;
  double angleAbout180Degrees = 180 - elbowDegrees - a;
  double wristDegrees = 180 - angleAbout180Degrees + angleOfLink3;

  VectorAngles output;
  output.shoulder = shoulderDegrees;
  output.elbow = elbowDegrees;
  output.wrist = wristDegrees;
  output.wristRotate = 0;
  output.rotate = 90 - degrees(atan((double)vector.z / (double)vector.x));
  return output;
}
