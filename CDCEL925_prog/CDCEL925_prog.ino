#include <Wire.h>

#define CDCEL925_ADDR 0x64  // 7-bit address only

struct RegisterSetting {
  uint8_t reg;
  uint8_t value;
};

RegisterSetting cdcel925_config1[] = {
  // 19.2MHz in, Y1 57.6, Y4 104.825
  //{0x00, 0x01},
  //{0x01, 0x08},
  { 0x02, 0xB4 },
  { 0x03, 0x03 },
  { 0x04, 0x02 },
  { 0x05, 0x50 },
  //{0x06, 0x60},
  //{0x07, 0x00},
  //{0x08, 0x00},
  //{0x09, 0x00},
  //{0x0A, 0x00},
  //{0x0B, 0x00},
  //{0x0C, 0x00},
  //{0x0D, 0x00},
  //{0x0E, 0x00},
  //{0x0F, 0x00},
  { 0x10, 0x00 },
  { 0x11, 0x00 },
  { 0x12, 0x00 },
  { 0x13, 0x00 },
  { 0x14, 0xAD },
  { 0x15, 0x02 },
  { 0x16, 0x00 },
  { 0x17, 0x00 },
  { 0x18, 0xFF },
  { 0x19, 0xF0 },
  { 0x1A, 0x02 },
  { 0x1B, 0x46 },
  { 0x1C, 0xFF },
  { 0x1D, 0xF0 },
  { 0x1E, 0x02 },
  { 0x1F, 0x44 },
  { 0x20, 0x00 },
  { 0x21, 0x00 },
  { 0x22, 0x00 },
  { 0x23, 0x00 },
  { 0x24, 0x6D },
  { 0x25, 0x02 },
  { 0x26, 0x02 },
  { 0x27, 0x02 },
  { 0x28, 0x98 },
  { 0x29, 0x35 },
  { 0x2A, 0xDA },
  { 0x2B, 0xA7 },
  { 0x2C, 0x98 },
  { 0x2D, 0x35 },
  { 0x2E, 0xDA },
  { 0x2F, 0xA7 }
};

RegisterSetting cdcel925_config2[] = {
  // 19.2MHz in, Y1 104.825, Y4 220.075
  //{0x00, 0x01},
  //{0x01, 0x08},
  { 0x02, 0xB4 },
  { 0x03, 0x01 },
  { 0x04, 0x02 },
  { 0x05, 0x50 },
  //{0x06, 0x60},
  //{0x07, 0x00},
  //{0x08, 0x00},
  //{0x09, 0x00},
  //{0x0A, 0x00},
  //{0x0B, 0x00},
  //{0x0C, 0x00},
  //{0x0D, 0x00},
  //{0x0E, 0x00},
  //{0x0F, 0x00},
  { 0x10, 0x00 },
  { 0x11, 0x00 },
  { 0x12, 0x00 },
  { 0x13, 0x00 },
  { 0x14, 0x6D },
  { 0x15, 0x02 },
  { 0x16, 0x00 },
  { 0x17, 0x00 },
  { 0x18, 0x95 },
  { 0x19, 0x3B },
  { 0x1A, 0x9A },
  { 0x1B, 0xA8 },
  { 0x1C, 0x95 },
  { 0x1D, 0x3B },
  { 0x1E, 0x9A },
  { 0x1F, 0xA8 },
  { 0x20, 0x00 },
  { 0x21, 0x00 },
  { 0x22, 0x00 },
  { 0x23, 0x02 },
  { 0x24, 0x6D },
  { 0x25, 0x02 },
  { 0x26, 0x01 },
  { 0x27, 0x00 },
  { 0x28, 0xE6 },
  { 0x29, 0xA9 },
  { 0x2A, 0x42 },
  { 0x2B, 0xC7 },
  { 0x2C, 0xE6 },
  { 0x2D, 0xA9 },
  { 0x2E, 0x42 },
  { 0x2F, 0xC7 }
};

const uint8_t cdcel925_config_size1 = sizeof(cdcel925_config1) / sizeof(cdcel925_config1[0]);
const uint8_t cdcel925_config_size2 = sizeof(cdcel925_config2) / sizeof(cdcel925_config2[0]);
// -------------------------------------------------

void setup() {
  Wire.begin();
  Wire.setClock(10000);
  Serial.begin(115200);
  delay(1000);

  showHelp();
  readRegister(0x00);
  readRegister(0x01);
}

void loop() {

  if (Serial.available() > 0) {
    switch (Serial.read()) {
      case '1':
        Serial.println("1");
        programDevice(1);
        showHelp();
        break;
      case '2':
        Serial.println("2");
        programDevice(2);
        showHelp();
        break;
      case 'e':
        Serial.println("e");
        writeEEPROM();
        showHelp();
        break;
      case 'm':
        Serial.println("m");
        setManualFrequencies();
        showHelp();
        break;
      case 'd':
        Serial.println("d");
        powerDown();
        break;
      case 'u':
        Serial.println("u");
        powerUp();
        break;
    }
  }
}

// -------------------------------------------------

void writeRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(CDCEL925_ADDR);
  reg = reg | 0x80; // bit wise or to tell the device we are programing a register, not a block.
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();  // repeated start
  delay(5);
}

// -------------------------------------------------

uint8_t readRegister(uint8_t reg) {
  Wire.beginTransmission(CDCEL925_ADDR);
  reg = reg | 0x80; // bit wise or to tell the device we are programing a register, not a block.
  Wire.write(reg);
  Wire.endTransmission(false);  // repeated start

  Wire.requestFrom(CDCEL925_ADDR, (uint8_t)1);

  if (Wire.available()) {
    int x = Wire.read();
    Serial.println(x, HEX);
    return x;
  }

  return 0xFF; // return FF if no data
}

// -------------------------------------------------

void programDevice(int config_num) { //program depending on config that wants to be loaded. 
  if (config_num == 1) {
    for (uint8_t i = 0; i < cdcel925_config_size1; i++) {
      uint8_t commandByte = cdcel925_config1[i].reg;
      writeRegister(commandByte, cdcel925_config1[i].value);

      Serial.print("Reg 0x");
      Serial.println(cdcel925_config1[i].reg, HEX);
    }

    Serial.println("config 1 written.");
  }
  if (config_num == 2) {
    for (uint8_t i = 0; i < cdcel925_config_size2; i++) {
      uint8_t commandByte = cdcel925_config1[i].reg;
      writeRegister(commandByte, cdcel925_config1[i].value);

      Serial.print("Reg 0x");
      Serial.println(cdcel925_config1[i].reg, HEX);
    }

    Serial.println("config 2 written.");
  }
  delay(10);
}

void writeEEPROM() {
  // write the RAM to EEPROM so that device loads up current config
  Serial.println("EEPROM Write");
  Wire.beginTransmission(CDCEL925_ADDR);
  Wire.write(0x86);  // byte write to offset 6
  Wire.write(0x01);  // EEWRITE = 1
  uint8_t retval = Wire.endTransmission(true);
  if (retval == 0) {
    Serial.println("Write OK.");

  } else {
    Serial.println("Write Error.");
  }
}

void showHelp() {

  Serial.println("Send 1 to write config 1 Y1 57.6MHz Y4 104.825MHz");
  Serial.println("Send 2 to write config 2 Y1 104.825MHz Y4 220.075MHz");
  Serial.println("Send e to write config to EEPROM");
  Serial.println("Send u to write to power up");
  Serial.println("Send d to write to power down");
  Serial.println("Send m to write to the manual test config");
}

void powerDown() {
  writeRegister(0x01, 0x18);
  Serial.println("Powered down");
}

void powerUp() {
  writeRegister(0x01, 0x08);
  Serial.println("Powered Up");
}

void setManualFrequencies() {
  Serial.println("Applying Manual Calibration...");

  // 2. Output Mux & Enable (Reg 0x02 - 0x05)
  // These tell the chip to connect the PLLs to the Pins
  writeRegister(0x02, 0xB4);  // Y0, Y1 enabled, Y1 source = PLL1
  writeRegister(0x03, 0x01);  // Y2, Y3 source = PLL2 (standard config)
  writeRegister(0x04, 0x02);  // Y4 source = PLL2
  writeRegister(0x05, 0x50);  // Clock Switch settings

  // 3. PLL1 (Y1: 104.875 MHz)
  writeRegister(0x14, 0x6D);  // P1 = 1
  writeRegister(0x15, 0x02);
  writeRegister(0x16, 0x00);  // VCO1 Range = Low (for ~105MHz)
  writeRegister(0x17, 0x00);  
  writeRegister(0x18, 0x95);  // N1[7:0]
  writeRegister(0x19, 0x3B);  // N1[11:8] | M1[8]
  writeRegister(0x1A, 0x9A);  // M1[7:0]
  writeRegister(0x1B, 0xA8);
  writeRegister(0x1C, 0x95);
  writeRegister(0x1D, 0x3B);
  writeRegister(0x1E, 0x9A);
  writeRegister(0x1F, 0xA8);

  // 4. PLL2 (Y4: 220.025 MHz)
  writeRegister(0x24, 0x6D);  // P2 = 1
  writeRegister(0x25, 0x02);
  writeRegister(0x26, 0x01);  // VCO2 Range = High (Required for >125MHz)
  writeRegister(0x27, 0x00);
  writeRegister(0x28, 0xE6);  // N2[7:0]
  writeRegister(0x29, 0xA9);  // N2[11:8] | M2[8]
  writeRegister(0x2A, 0x42);  // M2[7:0]
  writeRegister(0x2B, 0xC7);
  writeRegister(0x2C, 0xE6);
  writeRegister(0x2D, 0xA9);
  writeRegister(0x2E, 0x42);
  writeRegister(0x2F, 0xC7);

  Serial.println("Manual Calibration Complete.");
}