
#include <Wire.h>

#define CT7302_ADDRESS 0x10
#define NB_VAL 32

typedef struct Input_Sel_Config {
  byte type;
  char *name;
  char *pretty_name;
};

typedef enum
{
  INPUT_SEL_SPDIF_IN_0 = 0,
  INPUT_SEL_SPDIF_IN_1,
  INPUT_SEL_SPDIF_IN_2,
  INPUT_SEL_SPDIF_IN_3,
  INPUT_SEL_SPDIF_IN_4,
  INPUT_SEL_I2S_IN_0,
  INPUT_SEL_I2S_IN_1,
  INPUT_SEL_I2S_IN_2,
}Input_Sel;

static Input_Sel_Config input_sel[] = {
  {INPUT_SEL_SPDIF_IN_0, "INPUT_SEL_SPDIF_IN_0", "TV"},
  {INPUT_SEL_SPDIF_IN_1, "INPUT_SEL_SPDIF_IN_1", "IN1"},
  {INPUT_SEL_SPDIF_IN_2, "INPUT_SEL_SPDIF_IN_2", "IN2"},
  {INPUT_SEL_SPDIF_IN_3, "INPUT_SEL_SPDIF_IN_3", "IN3"},
  {INPUT_SEL_SPDIF_IN_4, "INPUT_SEL_SPDIF_IN_4", "IN4"},
  {INPUT_SEL_I2S_IN_0, "INPUT_SEL_I2S_IN_0", "La manufacture du son"},
  {INPUT_SEL_I2S_IN_1, "INPUT_SEL_I2S_IN_1", "I2S1"},
  {INPUT_SEL_I2S_IN_2, "INPUT_SEL_I2S_IN_2", "I2S2"},
};

typedef struct {
  byte address;
  byte value;
} CT7302_I2C_Config;

CT7302_I2C_Config ct7302_config[22] = {
  {0x06, 0x48},
  {0x10, 0xD0},
  {0x11, 0x00},
  {0x12, 0x08},
  {0x13, 0x00},
  {0x14, 0x40},
  {0x1C, 0x03},
  {0x30, 0x23},
  {0x31, 0x19},
  {0x32, 0x1E},
  {0x39, 0xF3},
  {0x3B, 0xFF},
  {0x40, 0x02},
  {0x45, 0x00},
  {0x4E, 0x47},
  {0x4F, 0xA4},
  {0x4C, 0x00},
  {0x4D, 0x37},
  {0x59, 0x2D},
  {0x61, 0x08},
  {0x62, 0x01},
  {0x04, 0x30},

};

static unsigned int linear_val[9] = {0, 10, 56, 138, 217, 337, 674, 900, 1000};

void WriteReg(byte add, byte val)
{
   Wire.beginTransmission(CT7302_ADDRESS);
   Wire.write(add);
   Wire.write(val);
   Wire.endTransmission();
  
  Wire.beginTransmission(CT7302_ADDRESS);
  Wire.write(0x00);
  Wire.endTransmission();

}

void setup() {
  int i = 0;
  byte val[NB_VAL] = {0};
  for (int i = 0; i < NB_VAL; i++)
  {
    val[i] = 0x55;
  }
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
  Serial.println("Wait for data : ");

  WriteReg(0x04, 0x30 + INPUT_SEL_SPDIF_IN_0);
  WriteReg(0x06, 0x48);

  Wire.beginTransmission(CT7302_ADDRESS);
  Wire.write(0x0);
  //Wire.write(0x21);
  Wire.requestFrom(CT7302_ADDRESS, NB_VAL);
  for (int i = 0; i < NB_VAL; i++)
  {
    //while(Wire.available());
    val[i] = Wire.read();
  }
  Wire.endTransmission();
  
  for (int i = 0; i < NB_VAL; i++)
  {
   Serial.print("0x");
   Serial.print(val[i], HEX);
   Serial.print(" ");
  }
  Serial.print("\n");

  for (i = 0; i < 22; i++)
  {
      WriteReg(ct7302_config[i].address, ct7302_config[i].value);
  }
  
  WriteReg(0x04, 0x30);

}


int analog_val = 0;
int input_src= INPUT_SEL_SPDIF_IN_0;

void loop() {
  int tmp = input_src;
  // put your main code here, to run repeatedly:
  analog_val = analogRead(A0);
  //Serial.println(analog_val);
  for (int i = 0; i < 8; i++)
  {
    if (analog_val >= linear_val[i] && analog_val <= linear_val[i+1])
    {
      tmp = i;
      break;
    }
  }
 
  if (tmp != input_src)
  {
      WriteReg(0x04, 0x30 + tmp);
      Serial.print(input_sel[tmp].pretty_name);
      Serial.print("\t");
      Serial.println(input_sel[tmp].name);
      input_src = tmp;
  }

  delay(100);

}
