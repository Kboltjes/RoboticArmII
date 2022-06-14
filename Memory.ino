uint32_t MergeIntoFraction(uint32_t current, uint8_t newByte);
double binaryToDecimal(String binary, int len);
int ReadBits(uint32_t value, int begin, int end);
double ParseDouble(uint8_t signBit, uint8_t exponentField, uint32_t fractionField);

VectorAngles ReadDegrees() {
  VectorAngles va;
  va.shoulder = ReadShoulderDegrees();
  va.elbow = ReadElbowDegrees();
  va.wrist = ReadWristDegrees();
  va.wristRotate = ReadWristRotateDegrees();
  va.rotate = ReadRotateDegrees();
  return va;
}
void WriteDegrees(VectorAngles angles) {
  WriteRotateDegrees(angles.rotate);
  WriteShoulderDegrees(angles.shoulder);
  WriteElbowDegrees(angles.elbow);
  WriteWristDegrees(angles.wrist);
  WriteWristRotateDegrees(angles.wristRotate);
}

void WriteDegrees(double rotateDeg, double shoulderDeg, double elbowDeg, double wristDeg, double wristRotateDeg) {
  WriteRotateDegrees(rotateDeg);
  WriteShoulderDegrees(shoulderDeg);
  WriteElbowDegrees(elbowDeg);
  WriteWristDegrees(wristDeg);
  WriteWristRotateDegrees(wristRotateDeg);
}

void PrintStoredDegrees() {
  VectorAngles va = ReadDegrees();
  Serial.println("Stored Degrees:");
  Serial.print("          Shoulder:      ");
  Serial.println(va.shoulder, 6);
  Serial.print("          Elbow:         ");
  Serial.println(va.elbow, 6);
  Serial.print("          Wrist:         ");
  Serial.println(va.wrist, 6);
  Serial.print("          WristRotation: ");
  Serial.println(va.wristRotate, 6);
  Serial.print("          Rotation:      ");
  Serial.println(va.rotate, 6);
}

void WriteRotateDegrees(double deg) {
  WriteDouble(deg, 0);
}
void WriteShoulderDegrees(double deg) {
  WriteDouble(deg, 4);
}
void WriteElbowDegrees(double deg) {
  WriteDouble(deg, 8);
}
void WriteWristDegrees(double deg) {
  WriteDouble(deg, 12);
}
void WriteWristRotateDegrees(double deg) {
  WriteDouble(deg, 16);
}

double ReadRotateDegrees() {
  return ReadDouble(0);
}
double ReadShoulderDegrees() {
  return ReadDouble(4);
}
double ReadElbowDegrees() {
  return ReadDouble(8);
}
double ReadWristDegrees() {
  return ReadDouble(12);
}
double ReadWristRotateDegrees() {
  return ReadDouble(16);
}

union DoubleBytes {
  double mainValue;
  unsigned char bytes[4];
};

void WriteDouble(double valToWrite, uint8_t address) {
  DoubleBytes doubleBytes;
  doubleBytes.mainValue = valToWrite;
  byte bigEndianBytes[] = { doubleBytes.bytes[3], doubleBytes.bytes[2], doubleBytes.bytes[1], doubleBytes.bytes[0] };
  writeToEEPROM(address, bigEndianBytes, 4);
  /*
  for (int x = 3; x >= 0; x--) {
    uint16_t addr = address + (3 - x);
    writeToEEPROM(addr, doubleBytes.bytes[x]);
    Serial.print("VALUE[");
    Serial.print(doubleBytes.bytes[x]);
    Serial.print(", ");
    Serial.print((int)addr);
    Serial.println("]");
  }
  Serial.println(sizeof(double));
  */
}

int ReadBits(uint32_t value, int begin, int end)
{
  unsigned long mask = (1 << (end - begin)) - 1;
  return (value >> begin) & mask;
}
double ParseDouble(uint8_t signBit, uint8_t exponentField, uint32_t fractionField) {
#define BIAS (127)
  double result = 0;

  int decimalPlace = exponentField - BIAS;
  double biasedExponent = pow(10, exponentField - BIAS);

  double fraction = 1;
  int rounding = -1;
  String string = "1.";
  for (int x = 22; x >= 0; x--) {
    int bit_read = ReadBits(fractionField, x, x + 1);
    if (bit_read == 1){
      rounding = x;
      string += "1";
    }
    else
      string += "0";
  }
  int currentDecimal = string.indexOf('.');
  string.remove(currentDecimal, 1);
  if (decimalPlace < -1) {
    String zeros = "";
    for (int x = 0; x < abs(decimalPlace) - 1; x++) {
      zeros += "0";
    }
    string = "0." + zeros + string;
  }
  else if (decimalPlace == -1) {
    string = "0." + string;
  }
  else {
    for (int x = 0; x < decimalPlace; x++) {
      string += "0";
    }
    String firstHalf = string.substring(0, decimalPlace + 1);
    String secondHalf = string.substring(decimalPlace + 1);
    string = firstHalf + "." + secondHalf;
  }
  double answer = binaryToDecimal(string, 23 - rounding);
  
  if (signBit != 0)
    answer = abs(answer) * -1;
  else
    answer = abs(answer);
  return answer;
}

double ReadDouble(uint8_t address) {
#define KEEP_HIGHEST_BIT (128)
#define EXCLUDE_HIGHEST_BIT (127)
#define KEEP_LOWEST_BIT (1)

#define EXCLUDE_LOWEST_BIT (254)

  //uint8_t firstByte = 64;//64;
  //uint8_t secondByte = 64;//73;
  //uint8_t thirdByte = 0;//15;
  //uint8_t fourthByte = 0;//219;
  
  uint8_t *bytes = readFromEEPROM(address, 4);
  //uint8_t secondByte = readFromEEPROM(address + 1);
  //uint8_t thirdByte = readFromEEPROM(address + 2);
  //uint8_t fourthByte = readFromEEPROM(address + 3);

  uint8_t signBit = bytes[0] & KEEP_HIGHEST_BIT;

  uint8_t exponentField = bytes[0] & EXCLUDE_HIGHEST_BIT;
  exponentField = (exponentField << 1) & EXCLUDE_LOWEST_BIT;
  exponentField = exponentField | ((bytes[1] >> 7) & KEEP_LOWEST_BIT);

  uint32_t fractionField = bytes[1] & EXCLUDE_HIGHEST_BIT;
  fractionField = MergeIntoFraction(fractionField, bytes[2]);
  fractionField = MergeIntoFraction(fractionField, bytes[3]);

  free(bytes);
  return ParseDouble(signBit, exponentField, fractionField);
}

uint32_t MergeIntoFraction(uint32_t current, uint8_t newByte) {
#define KEEP_HIGHEST_23_BITS (8388607)
  current = (current << 8) & KEEP_HIGHEST_23_BITS;
  current = current | newByte;
  return current;
}

double binaryToDecimal(String binary, int rounding)
{
  size_t point = binary.indexOf('.');

  if (point < 0 || point > binary.length())
    point = binary.length();

  double intDecimal = 0, fracDecimal = 0, twos = 1;

  for (int i = point - 1; i >= 0; --i)
  {
    intDecimal += (binary[i] - '0') * twos;
    twos *= 2;
  }
  twos = 2;
  for (int i = point + 1; i < binary.length(); ++i)
  {
    fracDecimal += (binary[i] - '0') / twos;
    twos *= 2.0;
  }
  return intDecimal + fracDecimal;
}
