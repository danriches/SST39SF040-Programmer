//Written poorly by mikemint64
//visit at mint64.home.blog

#define WE 2
#define OE 3

#define RA0 22
#define RA18 40

#define RD0 44
#define RD7 51

#define debugOutput 0

void setCtrlPins();                                   //Setup control signals
void setAddrPinsOut();                                //Setup address signals
void setDigitalOut();                                 //Set D0-D7 as outputs
void setDigitalIn();                                  //Set D0-D7 as inputs
void setAddress(unsigned long addr);                  //Set Address across A0-A18
void setByte(byte out);                               //Set data across D0-D7
byte readByte();                                      //Read byte across D0-D7
void writeByte(byte data, unsigned long address);     //Write a byte of data at a specific address
void programData(byte data, unsigned long address);   //Executes the program command sequence
byte readData(unsigned long address);                 //Read data as specific address
void eraseChip();                                     //Executes the erase chip command sequece
void readSerialCommand(byte in);                      //Decodes incomming serial commands 

const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;
char messageFromPC[32] = {0};
unsigned int addressFromPC = 0;
int byteFromPC = 0;

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
 
 // if (Serial.available() > 0) {
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void showNewData() {
    if (newData == true) 
    {
      char * strtokIndx;

      strtokIndx = strtok(receivedChars,",");
      strcpy(messageFromPC, strtokIndx);
      if(debugOutput != 0)
      {
        Serial.print("Message is: ");
        Serial.println(messageFromPC);
      }
      
      strtokIndx = strtok(NULL, ",");
      addressFromPC = atoi(strtokIndx);
      if(debugOutput != 0)
      {
        Serial.print("Address is: ");
        Serial.println(addressFromPC);
      }
      
      strtokIndx = strtok(NULL, ",");
      byteFromPC = atoi(strtokIndx);
      if(debugOutput != 0)
      {
        Serial.print("Byte is: ");
        Serial.println(byteFromPC);
      }

      newData = false;

      //Now Process the command if it's valid
      if(messageFromPC[0] == 'W')
      {
        Serial.println("Writing byte...");
        programData(byteFromPC, addressFromPC);
        int k = 0;
        k = readData(addressFromPC);
        if( k == byteFromPC)
        {
          Serial.print("Successfully wrote ");
          if(byteFromPC < 16)
          {
            Serial.print("0x0");
            Serial.println(byteFromPC, HEX);
          }
          else
          {
            Serial.print("0x");
            Serial.println(byteFromPC, HEX);
          }
        } else
        {
          Serial.print("Failed! Read value: ");
          if(k < 16)
          {
            Serial.print("0x0");
            Serial.println(k, HEX);
          }
          else
          {
            Serial.print("0x");
            Serial.println(k, HEX);
          }
        }
      }

      if(messageFromPC[0] == 'E')
      {
        eraseChip();
      }

      if(messageFromPC[0] == 'R')
      {
        Serial.println("Reading Chip");
        int j = 0;
        for(int i = 0; i < 256; i++)
        {
          Serial.print("Address: 0x");
          if(addressFromPC + i < 16)
            Serial.print("0");
          Serial.print(addressFromPC + i, HEX);
          Serial.print(" ");
          j = readData(addressFromPC + i);
          if(j < 16)
            Serial.print("0x0");
          else
            Serial.print("0x");
          Serial.println(j,HEX);    //reads byteFromPC amount of bytes from addressFromPC as the address, max 255 bytes at a time
        }
      }
    }
}

void setup() 
{
  setDigitalIn();
  setCtrlPins();
  setAddrPinsOut();

  Serial.begin(115200);
  delay(2);
  Serial.println("Arduino Flash Programmer Started");
}

void loop()
{
  recvWithStartEndMarkers();
  showNewData();
}

void eraseChip()
{
  setDigitalOut();

  writeByte(0xAA, 0x5555);
  writeByte(0x55, 0x2AAA);
  writeByte(0x80, 0x5555);
  writeByte(0xAA, 0x5555);
  writeByte(0x55, 0x2AAA);
  writeByte(0x10, 0x5555);

  delay(100);
  setDigitalIn();
  Serial.println("Erase entire chip successful!");
}

byte readData(unsigned long address)
{
  byte temp_read;
  setDigitalIn();
  
  digitalWrite(WE, HIGH);
  digitalWrite(OE, HIGH);
  
  setAddress(address);
  
  digitalWrite(OE, LOW);
  delayMicroseconds(1);
  
  temp_read = readByte();
  
  digitalWrite(OE, HIGH);

  return temp_read;
}

void writeByte(byte data, unsigned long address)
{
  digitalWrite(OE, HIGH);
  digitalWrite(WE,HIGH);
  
  setAddress(address);
  setByte(data);
  
  digitalWrite(WE, LOW);
  delayMicroseconds(1);
  digitalWrite(WE, HIGH); 
}

void programData(byte data, unsigned long address)
{
  setDigitalOut();
  
  writeByte(0xAA, 0x5555);
  writeByte(0x55, 0x2AAA);
  writeByte(0xA0, 0x5555);
  writeByte(data, address);
  
  delayMicroseconds(30);
}

byte readByte()
{
  byte temp_in = 0;
  for(int i = 0; i < 8; i++)
  {
    if (digitalRead( RD0 + i))
    { 
      bitSet(temp_in, i);
    }
  }
  return temp_in;
}

void setByte(byte out)
{
  for(int i = 0; i < 8; i++)
    digitalWrite( RD0 + i, bitRead(out, i) );
}

void setAddress(unsigned long addr)
{
  for(int i = 0; i < 19; i++)
    digitalWrite( RA0 + i , bitRead(addr, i) );
}

void setAddrPinsOut()
{
  for(int i = RA0; i <= RA18; i++)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
}

void setDigitalOut()
{
  for(int i = RD0; i <= RD7; i++)
    pinMode(i, OUTPUT);
}

void setDigitalIn()
{
  for(int i = RD0; i <= RD7; i++)
    pinMode(i, INPUT);
}

void setCtrlPins()
{
  pinMode(WE, OUTPUT);
  pinMode(OE, OUTPUT);

  digitalWrite(WE, HIGH);
  digitalWrite(OE, HIGH);
}
