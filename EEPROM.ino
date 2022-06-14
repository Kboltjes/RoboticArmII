#include <Wire.h>

#define ADDR_Ax 0b100 //A2, A1, A0
#define ADDR (0b1010 << 3) + ADDR_Ax

void writeToEEPROM(byte data_addr, byte data) {
  Wire.beginTransmission(ADDR);
  Wire.write(data_addr);
  Wire.write(data);
  Wire.flush();
  Wire.endTransmission(true);
  delay(4);
}
void writeToEEPROM(byte data_addr, byte data[], int dataLength) {
  Wire.beginTransmission(ADDR);
  Wire.write(data_addr);
  for (int x = 0; x < dataLength; x++) {
    Wire.write(data[x]);
  }
  Wire.flush();
  Wire.endTransmission(true);
  delay(4);
}

byte readFromEEPROM(byte data_addr){
  byte data = 0xFF;
  Wire.beginTransmission(ADDR);
  Wire.write(data_addr);
  Wire.endTransmission(false);
  Wire.requestFrom(ADDR, 1); //retrieve 1 returned byte
  delay(1);
  if(Wire.available()){
    data = Wire.read();
  }
  return data;
}
byte *readFromEEPROM(byte data_addr, int numBytes){
  if (numBytes == 0) 
    return NULL;
  byte *data = malloc(numBytes);
  Wire.beginTransmission(ADDR);
  Wire.write(data_addr);
  Wire.endTransmission(false);
  Wire.requestFrom(ADDR, numBytes); //retrieve 1 returned byte
  delay(1);
  for (int x = 0; x < numBytes; x++) {
    if(Wire.available()){
      data[x] = Wire.read();
    }
  }
  return data;
}

void ScanI2CDevices()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknow error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }   
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
}
