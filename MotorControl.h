#include "Pinouts.h"

typedef void (*fcnPtr)();
fcnPtr shoulderHigh;
fcnPtr shoulderLow;
fcnPtr elbowHigh;
fcnPtr elbowLow;
fcnPtr wristHigh;
fcnPtr wristLow;
fcnPtr wristRotateHigh;
fcnPtr wristRotateLow;
fcnPtr rotateHigh;
fcnPtr rotateLow;

void SetFunctionPointers();
void SetFunctionPointersToNull();

void SetAllDirections(bool isHigh);

void SetRotateDirection(bool _direction);
void SetWristRotateDirection(bool _direction);
void SetWristDirection(bool _direction);
void SetElbowDirection(bool _direction);
void SetShoulderDirection(bool _direction);

void ShoulderHIGH();
void ShoulderLOW();
void ElbowHIGH();
void ElbowLOW();
void WristHIGH();
void WristLOW();
void WristRotateHIGH();
void WristRotateLOW();
void RotateHIGH();
void RotateLOW();
void NullOperation();
