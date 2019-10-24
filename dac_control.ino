#include <Wire.h>

#define AK4118A_REG_CLK_POWER_DN_CTL 0x00
#define AK4118A_REG_FORMAT_DE_EM_CTL 0x01
#define AK4118A_REG_IN_OUT_CTL0      0x02
#define AK4118A_REG_IN_OUT_CTL1      0x03
#define AK4118A_REG_INT0_MASK        0x04
#define AK4118A_REG_INT1_MASK        0x05
#define AK4118A_REG_RECEIVER_STATUS0 0x06
#define AK4118A_REG_RECEIVER_STATUS1 0x07
#define AK4118A_REG_RX_CH_STAT_BYTE0 0x08
#define AK4118A_REG_RX_CH_STAT_BYTE1 0x09
#define AK4118A_REG_RX_CH_STAT_BYTE2 0x0A
#define AK4118A_REG_RX_CH_STAT_BYTE3 0x0B
#define AK4118A_REG_RX_CH_STAT_BYTE4 0x0C
#define AK4118A_REG_RX_CH_STAT_BYTE5 0x27
#define AK4118A_REG_TX_CH_STAT_BYTE0 0x0D
#define AK4118A_REG_TX_CH_STAT_BYTE1 0x0E
#define AK4118A_REG_TX_CH_STAT_BYTE2 0x0F
#define AK4118A_REG_TX_CH_STAT_BYTE3 0x10
#define AK4118A_REG_TX_CH_STAT_BYTE4 0x11
#define AK4118A_REG_TX_CH_STAT_BYTE5 0x28
#define AK4118A_REG_BURST_PREAMBLE_PC_BYTE0 0x12
#define AK4118A_REG_BURST_PREAMBLE_PC_BYTE1 0x13
#define AK4118A_REG_BURST_PREAMBLE_PD_BYTE0 0x14
#define AK4118A_REG_BURST_PREAMBLE_PD_BYTE1 0x15
#define AK4118A_REG_STC_DAT_DETECT          0x26
#define AK4118A_REG_RX_CHANNEL_STATUS_BYTE5 0x27
#define AK4118A_REG_TX_CHANNEL_STATUS_BYTE5 0x28


#define AK4118A_QINT                (0x01<<7)
#define AK4118A_AUTO                (0x01<<6)
#define AK4118A_CINT                (0x01<<5)
#define AK4118A_UNLCK               (0x01<<4)
#define AK4118A_DTSCD               (0x01<<3)
#define AK4118A_PEM                 (0x01<<2)
#define AK4118A_AUDION              (0x01<<1)
#define AK4118A_PAR                 (0x01<<0)

#define AK4118A_STATUS1_FS(x)               (((x)&0x0F)<<4)
#define AK4118A_STATUS1_QCRC                (0x01<<1)
#define AK4118A_STATUS1_CCRC                (0x01<<0)

#define AK4118A_CLK_PWR_CS12        (0x01<<7)
#define AK4118A_CLK_PWR_BCU         (0x01<<6)
#define AK4118A_CLK_PWR_CM(x)       (((x)&0x03)<<4)
#define AK4118A_CLK_PWR_OCKS(x)     (((x)&0x03)<<2)
#define AK4118A_CLK_PWR_PWN         (0x01<<1)
#define AK4118A_CLK_PWR_RSTN        (0x01<<0)

#define AK4118A_FORMAT_MONO         (0x01<<7)
#define AK4118A_FORMAT_DIF(x)       (((x)&0x07)<<4)
#define AK4118A_FORMAT_DEAU         (0x01<<3)
#define AK4118A_FORMAT_DEM(x)       (((x)&0x03)<<1)
#define AK4118A_FORMAT_DFS          (0x01<<0)

#define AK4118A_CONTROL0_TX1E       (0x01<<7)
#define AK4118A_CONTROL0_OPS1(x)    (((x)&0x07)<<4)
#define AK4118A_CONTROL0_TX0E       (0x01<<3)
#define AK4118A_CONTROL0_OPS0(x)    (((x)&0x07)<<0)

#define AK4118A_CONTROL1_EFH(x)     (((x)&0x03)<<6)
#define AK4118A_CONTROL1_UDIT       (0x01<<5)
#define AK4118A_CONTROL1_TLR        (0x01<<4)
#define AK4118A_CONTROL1_DIT        (0x01<<3)
#define AK4118A_CONTROL1_IPS(x)     (((x)&0x07)<<0)

#define AK4118A_RECONIZE_26H        0x0
#define AK4118A_RECONIZE_27H        0x0
#define AK4118A_RECONIZE_28H        0x10

#define AK4118A_DETECT_CH7          (0x01<<7)
#define AK4118A_DETECT_CH6          (0x01<<6)
#define AK4118A_DETECT_CH5          (0x01<<5)
#define AK4118A_DETECT_CH4          (0x01<<4)
#define AK4118A_DETECT_CH3          (0x01<<3)
#define AK4118A_DETECT_CH2          (0x01<<2)
#define AK4118A_DETECT_CH1          (0x01<<1)
#define AK4118A_DETECT_CH0          (0x01<<0)

static byte input = 0;

/* These Registers values come from the one send by original SCREEN + MCU connected to the DAC and visualize with sigrock and saleae logic analyzer*/
const byte registers_init[35][3] = {
    // AK4118A
    {0x13, 0x00, 0x0B},
    {0x13, 0x01, 0x50},
    {0x13, 0x02, 0x00},
    {0x13, 0x03, 0x05},
    {0x13, 0x25, 0x77},
    {0x10, 0x00, 0x17},
    {0x11, 0x00, 0x17},
    {0x10, 0x01, 0x22},
    {0x11, 0x01, 0x22},
    {0x10, 0x02, 0x0A},
    {0x11, 0x02, 0x08},
    {0x10, 0x03, 0xFF},
    {0x10, 0x04, 0xFF},
    {0x11, 0x03, 0xFF},
    {0x11, 0x04, 0xFF},
    {0x10, 0x05, 0x00},
    {0x11, 0x05, 0x00},
    {0x10, 0x06, 0x00},
    {0x11, 0x06, 0x00},
    {0x10, 0x07, 0x09},
    {0x11, 0x07, 0x09},
    {0x10, 0x09, 0x04},
    {0x11, 0x09, 0x04},
    {0x10, 0x0B, 0xC0},
    {0x11, 0x0B, 0xC0},
    {0x10, 0x01, 0x22},
    {0x10, 0x02, 0x08},
    {0x10, 0x05, 0x00},
    {0x11, 0x01, 0x22},
    {0x11, 0x02, 0x0A},
    {0x11, 0x05, 0x00},
    {0x10, 0x06, 0x00},
    {0x10, 0x09, 0x04},
    {0x11, 0x06, 0x00},
    {0x11, 0x09, 0x04},
};
byte readRegister(int i2c_addr, int addr)
{
    byte reg;
    Wire.beginTransmission(i2c_addr); // select device with "beginTransmission()"
    Wire.write(addr); // select starting register with "write()"
    Wire.endTransmission(); // end write operation, as we just wanted to select the starting register
    Wire.requestFrom(i2c_addr, 1); // select number of bytes to get from the device (2 bytes in this case)
    reg = Wire.read();

    return reg;
}

void writeRegister(int i2c_addr, byte addr, byte value)
{
    byte reg;
    Wire.beginTransmission(i2c_addr); // select device with "beginTransmission()"
    Wire.write(addr); // select starting register with "write()"
    Wire.write(value);
    Wire.endTransmission(); // end write operation, as we just wanted to select the starting register

}

void reset(unsigned char i2c_addr)
{
    char reg;

    reg = readRegister(i2c_addr, AK4118A_REG_CLK_POWER_DN_CTL);
    reg &=~AK4118A_CLK_PWR_RSTN;
    writeRegister(i2c_addr, AK4118A_REG_CLK_POWER_DN_CTL, reg);
    delay(100);
    reg |=AK4118A_CLK_PWR_RSTN;
    writeRegister(i2c_addr, AK4118A_REG_CLK_POWER_DN_CTL, reg);
}

byte inputSelect(byte i2c_addr, byte channel)
{
    char reg;

    reg = readRegister(i2c_addr, AK4118A_REG_IN_OUT_CTL1);
    reg &= ~AK4118A_CONTROL1_IPS(0x07);
    reg |= AK4118A_CONTROL1_IPS(channel);
    writeRegister(i2c_addr, AK4118A_REG_IN_OUT_CTL1, reg);

    reg = readRegister(i2c_addr, AK4118A_REG_IN_OUT_CTL1);
    if (AK4118A_CONTROL1_IPS(channel)==(reg&0x07)) return 1; else return 0;
}

unsigned int readStatus(unsigned char i2c_addr)
{
    unsigned int ret_value;

    ret_value = readRegister(i2c_addr, AK4118A_REG_RECEIVER_STATUS0);
    ret_value <<= 8;
    ret_value |= readRegister(i2c_addr, AK4118A_REG_RECEIVER_STATUS1);
    return ret_value;
}

// ----------------------------- Function that changes a single bit in a register --------------------------------------------------------------------
void changeBit(int devaddr, byte regaddr, int data, boolean setting)
{
    byte r = readRegister(devaddr, regaddr);
    if (setting == 1)
    {
        bitSet(r, data);
    } else
        if (setting == 0)
        {
            bitClear(r, data);
        }
    writeRegister(devaddr, regaddr, r);
}

void setup()
{
    Wire.begin();

    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);

    pinMode(6, OUTPUT);
    // D6 Low => Input = ak4118a
    // D6 High => Input = i2s
    digitalWrite(6, HIGH);

    pinMode(11, INPUT_PULLUP);
    pinMode(12, INPUT_PULLUP);
    pinMode(13, INPUT_PULLUP);

    Serial.begin(9600);

    //while (!Serial);             // Leonardo: wait for serial monitor

    Serial.println("Reset ak4118a");
    reset(0x13);
    byte reg;

    for (int i = 0; i < 35; i++)
    {
        writeRegister(registers_init[i][0],
                      registers_init[i][1],
                      registers_init[i][2]);
    }

    // D6 High
    // 5 => Squeezebox spdif
    // 6 => TV coax
    inputSelect(0x13, 5);

    reg = readRegister(0x13, AK4118A_REG_IN_OUT_CTL1)&0x07;
    Serial.print("Selected input : 0x");
    Serial.println(reg,HEX);

    selectInput(2);


}
void printSelectedInput()
{
    byte reg;
    reg = readRegister(0x13, AK4118A_REG_IN_OUT_CTL1)&0x07;
    Serial.print("Selected input : 0x");
    Serial.println(reg,HEX);
}

char getCommand()
{
    char c = '\0';
    if (Serial.available())
    {
        c = Serial.read();
    }
    return c;
}

void selectInput(byte in)
{
    switch (in)
    {
    case 0:
        inputSelect(0x13, 5);
        digitalWrite(6, LOW);
        Serial.println("Select input SPDIF (Touch)");
        printSelectedInput();
        input = 0;
        break;
    case 1:
        inputSelect(0x13, 6);
        digitalWrite(6, LOW);
        Serial.println("Select input COAX (TV)");
        printSelectedInput();
        input = 1;
        break;
    case 2:
        digitalWrite(6, HIGH);
        inputSelect(0x13, 5);
        Serial.println("Select input I2S (Squeezelite)");
        printSelectedInput();
        input = 2;
        break;
    default:
        break;
    }


}

void loop()
{
    byte reg;
    byte val;
    char command = getCommand();
    switch (command)
    {
    case '0':
        selectInput(0);
        break;
    case '1':
        selectInput(1);
        break;

    case '2':
        selectInput(2);
        break;

    default:
        break;
    }


    val = digitalRead(11);
    Serial.print("D11 : ");
    Serial.println(val);

    if (val == 0)
    {
        input++;
        if (input >= 3)
            input = 0;
        selectInput(input);
    }

    reg = readRegister(0x13, 0x25);
    Serial.print("Detect : ");
    Serial.println(val, BIN);



    delay(250);           // wait 5 seconds for next scan
}
