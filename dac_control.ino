
#include <Wire.h>

#define CT7302_ADDRESS 0x10
#define NB_VAL 32

#define BUTTON_PIN 2 // 2 or 3 for ISR on nano


typedef struct Input_Sel_Config {
  byte type;
  byte cmd;
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
  INPUT_SEL_SENTINEL,
}Input_Sel;

static Input_Sel_Config input_sel[] = {
  {INPUT_SEL_I2S_IN_0, 5, "INPUT_SEL_I2S_IN_0", "La manufacture du son"},
  {INPUT_SEL_SPDIF_IN_0, 0, "INPUT_SEL_SPDIF_IN_0", "TV"},
  {INPUT_SEL_SPDIF_IN_1, 1, "INPUT_SEL_SPDIF_IN_1", "IN1"},
  /*{INPUT_SEL_SPDIF_IN_2, 2, "INPUT_SEL_SPDIF_IN_2", "IN2"},
  {INPUT_SEL_SPDIF_IN_3, 3;"INPUT_SEL_SPDIF_IN_3", "IN3"},
  {INPUT_SEL_SPDIF_IN_4, 4, "INPUT_SEL_SPDIF_IN_4", "IN4"},
  {INPUT_SEL_I2S_IN_1, 6, "INPUT_SEL_I2S_IN_1", "I2S1"},
  {INPUT_SEL_I2S_IN_2, 7, "INPUT_SEL_I2S_IN_2", "I2S2"},*/
};

typedef struct {
  byte address;
  byte value;
} CT7302_I2C_Config;


/* Table copied from what dev board of CT7302 did on i2c at startup*/
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


int input_src = INPUT_SEL_SPDIF_IN_0;
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
  

  
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
  Serial.println("DAC Control v0.1");

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
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}


int val = 0;

void loop() {
  
   val = digitalRead(BUTTON_PIN);

   if (!val)
   {
    
     input_src++;
     if (input_src >= 3)
  	input_src = 0;
     WriteReg(0x04, 0x30 + input_sel[input_src].cmd);
     Serial.print(input_sel[input_src].pretty_name);
     Serial.print("\t");
     Serial.println(input_sel[input_src].name);
     delay(300);

   }



}
