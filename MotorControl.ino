int CheckIfProblemExists() {
  if (digitalRead(LOST_POWER_PIN) == HIGH)
    return LOST_POWER;
  return NO_PROBLEM;
}
void ShoulderHIGH()
{
   digitalWrite(SHOULDER_STEP_PIN    , HIGH);
}
void ShoulderLOW()
{
   digitalWrite(SHOULDER_STEP_PIN    , LOW);
}
void ElbowHIGH()
{
   digitalWrite(ELBOW_STEP_PIN    , HIGH);
}
void ElbowLOW()
{
   digitalWrite(ELBOW_STEP_PIN    , LOW);
}
void WristHIGH()
{
   digitalWrite(WRIST_STEP_PIN    , HIGH);
}
void WristLOW()
{
   digitalWrite(WRIST_STEP_PIN    , LOW);
}
void WristRotateHIGH()
{
   digitalWrite(WRISTROTATE_STEP_PIN    , HIGH);
}
void WristRotateLOW()
{
   digitalWrite(WRISTROTATE_STEP_PIN    , LOW);
}
void RotateHIGH()
{
   digitalWrite(ROTATE_STEP_PIN    , HIGH);
}
void RotateLOW()
{
   digitalWrite(ROTATE_STEP_PIN    , LOW);
}
void NullOperation()
{}

void SetShoulderDirection(bool _direction)
{
  if(_direction)
    digitalWrite(SHOULDER_DIR_PIN    , HIGH);
  else
    digitalWrite(SHOULDER_DIR_PIN    , LOW);
}
void SetElbowDirection(bool _direction)
{
  if(_direction)
  {
    digitalWrite(ELBOW_DIR_PIN    , LOW);
  }
  else
  {
    digitalWrite(ELBOW_DIR_PIN    , HIGH);
  }
}
void SetWristDirection(bool _direction)
{
  if(_direction)
    digitalWrite(WRIST_DIR_PIN    , LOW);
  else
    digitalWrite(WRIST_DIR_PIN    , HIGH);
}
void SetWristRotateDirection(bool _direction)
{
  if(_direction)
    digitalWrite(WRISTROTATE_DIR_PIN    , HIGH);
  else
    digitalWrite(WRISTROTATE_DIR_PIN    , LOW);
}
void SetRotateDirection(bool _direction)
{
  if(_direction)
    digitalWrite(ROTATE_DIR_PIN    , HIGH);
  else
    digitalWrite(ROTATE_DIR_PIN    , LOW);
}

void SetFunctionPointers()
{
  shoulderHigh = ShoulderHIGH;
  shoulderLow = ShoulderLOW;
  elbowHigh = ElbowHIGH;
  elbowLow = ElbowLOW;
  wristHigh = WristHIGH;
  wristLow = WristLOW;
  wristRotateHigh = WristRotateHIGH;
  wristRotateLow = WristRotateLOW;
  rotateHigh = RotateHIGH;
  rotateLow = RotateLOW;
}
void SetFunctionPointersToNull() {
  shoulderHigh = NullOperation;
  shoulderLow = NullOperation;
  elbowHigh = NullOperation;
  elbowLow = NullOperation;
  wristHigh = NullOperation;
  wristLow = NullOperation;
  wristRotateHigh = NullOperation;
  wristRotateLow = NullOperation;
  rotateHigh = NullOperation;
  rotateLow = NullOperation;
}
void SetAllDirections(bool high)
{
  if(high == true)
  {
    digitalWrite(SHOULDER_DIR_PIN    , HIGH);
    digitalWrite(ELBOW_DIR_PIN    , HIGH);
    digitalWrite(WRIST_DIR_PIN    , HIGH);
    digitalWrite(WRISTROTATE_DIR_PIN    , HIGH);
    digitalWrite(ROTATE_DIR_PIN    , HIGH);
  }
  else
  {
    digitalWrite(SHOULDER_DIR_PIN    , LOW);
    digitalWrite(ELBOW_DIR_PIN    , LOW);
    digitalWrite(WRIST_DIR_PIN    , LOW);
    digitalWrite(WRISTROTATE_DIR_PIN    , LOW);
    digitalWrite(ROTATE_DIR_PIN    , LOW);
  }
}
