//Written by Dan Riches - 2022


#define WE 2
#define OE 3

#define RA0 22
#define RA18 40

#define RD0 44
#define RD7 51

#define debugOutput 1

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
byte ReadID();                                        //Returns 0xB5=SST39SF010A, 0xB6 = SST39SF020A, 0xB7 = SST39SF040A
void EnterIDMode();                                   //Enter ID Mode, must be called before trying to read the ID of the device
void ExitIDMode();                                    //Exit ID Mode, must be called before any other operation
void ListCommands();                                  //Lists commands that are available

const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;
char messageFromPC[32] = {0};
long addressFromPC = 0;
long endAddressFromPC = 0;
int byteFromPC = 0;
boolean commandValid = 0;

long writeAddressCounter = 0;
long bytesWrittenCounter = 0;
boolean StreamMode = 0;           //Set to 1 when the serial data isn't formatted and just a stream of 2048 bytes
long bytesToWrite = 0;

void recvWithStartEndMarkers() 
{
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
    byte bc[256];      //Our byte buffer which needs to be bigger, say 256 bytes
    //We need to check StreamMode == 1 and then ignore the start and end markers
 // if (Serial.available() > 0) {
    while (Serial.available() > 0 && newData == false) {
        if(StreamMode == 0)
          rc = Serial.read();
        else
          Serial.readBytes(bc, bytesToWrite);

        if(StreamMode == 1)
        {
            //Write each byte in the bc array using  bytesToWrite as the max count
            for(int c = 0; c < bytesToWrite; c++)
            {
              programData(bc[c], writeAddressCounter);
              writeAddressCounter++;
            }
            Serial.println("Flash Written Successfully.");
            StreamMode = 0;
            
        } else
        {
          if (recvInProgress == true) 
          {
              if (rc != endMarker) 
              {
                  receivedChars[ndx] = rc;
                  ndx++;
                  if (ndx >= numChars) 
                  {
                      ndx = numChars - 1;
                  }
              }
              else 
              {
                  receivedChars[ndx] = '\0'; // terminate the string
                  recvInProgress = false;
                  ndx = 0;
                  newData = true;
              }
          }
          else if (rc == startMarker) 
          {
              recvInProgress = true;
          }
        }
    }
}

void printdump(long address, byte value)
{
  //Print address at start of each line of 16 values
  boolean startLine = false;
  if((address % 16) == 0)
  {
    startLine = true;
    Serial.println("");
    if(address < 16)
      Serial.print("0x000");
  
    if(address > 15 && address < 256)
      Serial.print("0x00");
  
    if(address > 255 && address < 4096)
      Serial.print("0x0");
  
    if(address > 4095)
      Serial.print("0x");
  
    Serial.print(address, HEX);
  }

  Serial.print("  ");
  if(value < 16)
    Serial.print("0x0");
  else
    Serial.print("0x");

  Serial.print(value, HEX);
}

void showNewData() 
{
    if (newData == true) 
    {
      commandValid = 0;       //Set to 1 if command is valid so the drop out catches invalid commands
      char * strtokIndx;

      //Obtain the command character (W R E D I)
      strtokIndx = strtok(receivedChars,",");
      strcpy(messageFromPC, strtokIndx);
      if(debugOutput != 0)
      {
        Serial.print("Message is: ");
        Serial.println(messageFromPC);
      }

      //Parse the first parameter which is the address
      strtokIndx = strtok(NULL, ",");
      addressFromPC = atol(strtokIndx);
      if(debugOutput != 0)
      {
        Serial.print("Address is: ");
        Serial.println(addressFromPC);
      }

      //parse the second paramter which is the byte value, or the end address in the case of a read (R) or dump (D)
      strtokIndx = strtok(NULL, ",");
      if(messageFromPC[0] == 'R' || messageFromPC[0] == 'D' || messageFromPC[0] == 'Y')
        endAddressFromPC = atol(strtokIndx);
      else
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
        commandValid = 1;
      }

      //Erase entire chip
      if(messageFromPC[0] == 'E')
      {
        eraseChip();
        commandValid = 1;
      }

      //Read Chip ID
      if(messageFromPC[0] == 'I')
      {
        Serial.print("Reading Chip ID... ");
        EnterIDMode();
        byte id = ReadID();
        Serial.println(id, HEX);
        ExitIDMode();

        if(id != 0xB5 && id != 0xB6 && id != 0xB7)
        {
          Serial.println("Device is not made by SST, proceed with caution!");
        }

        if(id == 0xB5)
        {
          Serial.println("Device ID: SST39SF010A");
          Serial.println("End Address in decimal is: 131071 out of total 131072 bytes");
        }
          
        if(id == 0xB6)
        {
          Serial.println("Device ID: SST39SF020A");
          Serial.println("End Address in decimal is: 262143 out of total 262144 bytes");
        }

        if(id == 0xB7)
        {
          Serial.println("Device ID: SST39SF040A");
          Serial.println("End Address in decimal is: 524287 out of total 524288 bytes");
        }
        commandValid = 1;
      }

      //Read data from chip
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
        commandValid = 1;
      }

      //Dumps a block exactly specified in the command <D,from,to> in a format resembling a hex editor
      if(messageFromPC[0] == 'D')
      {
        Serial.println("Dumping Chip Contents");
        Serial.print("Start Address: ");
        Serial.print(addressFromPC, HEX);
        Serial.print(" to End Address: ");
        Serial.println(endAddressFromPC, HEX);
        int j = 0;
        //Read blocks of 16 bytes
        for(long i = addressFromPC; i < endAddressFromPC + 1; i++)
        {
          j = readData(i);
          printdump(i, j);
        }
        commandValid = 1;
        Serial.println("");
        Serial.println("Flash Dump Successful!");
      }

      //Set the write address counter
      if(messageFromPC[0] == 'X')
      {
        //Reset Counters
        writeAddressCounter = addressFromPC;
        bytesWrittenCounter = 0;
        Serial.print("SET Write Counters, start from ");
        Serial.print(addressFromPC, HEX);
        Serial.println(" - Done.");
        commandValid = 1;
      }

      //Start the flash stream off (16 byte chunks)
      if(messageFromPC[0] == 'Y')
      {
        Serial.println("Start of stream:");
        StreamMode = 1;
        commandValid = 1;
      }

      //Set the bytes to write counter
      if(messageFromPC[0] == 'Z')
      {
        bytesToWrite = addressFromPC;
        writeAddressCounter = 0;
        commandValid = 1;
      }

      //Leave this part at the end, ie add new commands above this line
      if(commandValid == 0)
      {
        Serial.println("Invalid Command Received!");
        Serial.println("Valid commands are as follows:");
        ListCommands();
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
  Serial.println("Commands available are as follows:");
  Serial.println("<W,a,b>   Write to address a with byte value b.");
  Serial.println("<R,a,0>   Read a 256 byte block from chip starting at address a. Dump maybe more useful");
  Serial.println("<E,0,0>   Erase entire chip, both parameters are ignored but must be supplied.");
  Serial.println("<D,s,e>   Dump Chips memory from s address to e address in decimal only.");
  Serial.println("<I,0,0>   Read Chips Device ID and report it's type, only SST39SF010 / 020 / 040 are supported.");
}

void ListCommands()
{
  Serial.println("<W,a,b>   Write to address a with byte value b.");
  Serial.println("<R,a,0>   Read a 256 byte block from chip starting at address a. Dump maybe more useful");
  Serial.println("<E,0,0>   Erase entire chip, both parameters are ignored but must be supplied.");
  Serial.println("<D,s,e>   Dump Chips memory from s address to e address in decimal only.");
  Serial.println("<I,0,0>   Read Chips Device ID and report it's type, only SST39SF010 / 020 / 040 are supported.");
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

void EnterIDMode()
{
  setDigitalOut();

  writeByte(0xAA, 0x5555);
  writeByte(0x55, 0x2AAA);
  writeByte(0x90, 0x5555);
}

void ExitIDMode()
{
  setDigitalOut();

  writeByte(0xAA, 0x5555);
  writeByte(0x55, 0x2AAA);
  writeByte(0xF0, 0x5555);
}

byte ReadID()
{
  byte manufacturer;
  byte deviceID;

  manufacturer = readData(0x00);
  //if(manufacturer != 0xBF)
  //  return 0;
  Serial.print("Manufacturer ID: ");
  Serial.println(manufacturer);
  deviceID = readData(0x01);

  return deviceID;
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
