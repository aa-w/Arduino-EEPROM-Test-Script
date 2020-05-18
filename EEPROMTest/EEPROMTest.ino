#include <EEPROM.h>

long PauseTimer = (millis() + 500);
int eeAddress = 0;
unsigned long WriteFlagLong = 1;
unsigned long TestCounter = 1;
float WriteFlagDouble = 0;

unsigned long ReadTimerStart = 0;
unsigned long ReadTimerStop = 0;

unsigned long WriteTimerStart = 0;
unsigned long WriteTimerStop = 0;

unsigned long PreviousFlag = 0;

bool TestPass = true;

byte WrittenFlag = 0;

void setup()
{
  Serial.begin(19200);
  Serial.println("Starting Test");
  Serial.println("Carrying out first write");

  EEPROM.get(8, WrittenFlag);

  

  if (WrittenFlag == 4)
  {
    Serial.println("Memory Occupied");
    
    eeAddress = 8 + sizeof(byte);
    
    
    //memory has been written to we can start the test

    EEPROM.get(eeAddress, WriteFlagLong);

    eeAddress = eeAddress + sizeof(WriteFlagLong + 16);

    EEPROM.get(eeAddress, WriteFlagDouble);
    
  }
  else
  {
    Serial.println("Memory Not Occupied");
    //memory has not been written to
    WrittenFlag = 4;

    eeAddress = 8;
    
    EEPROM.put(eeAddress, WrittenFlag);

    eeAddress = 8 + sizeof(byte);

    WriteFlagDouble = (random(0, 99) / 100.0);

    EEPROM.put(eeAddress, WriteFlagLong);

    eeAddress = eeAddress + sizeof(WriteFlagLong + 16);

    EEPROM.put(eeAddress, WriteFlagDouble);
    
    
  }

  Serial.println("First Write Completed");

}

void loop()
{
  if (millis() > PauseTimer && TestPass == true)
  {
    eeAddress = 8 + sizeof(byte);

    unsigned long ReadLongValue = 0;
    ReadTimerStart = millis();
    
    EEPROM.get(eeAddress, ReadLongValue);

    eeAddress = eeAddress + sizeof(WriteFlagLong + 16);

    float ReadFloatValue = 0;
    EEPROM.get(eeAddress, ReadFloatValue);
    ReadTimerStop = millis();

    if ((ReadLongValue == WriteFlagLong) && (ReadFloatValue == WriteFlagDouble))
    {
      //Test Passed
      TestPass = true;

      //Output Serial Every 100
      if ((ReadLongValue % 100) == 0)
      {
        Serial.print("Test ");
        Serial.print(TestCounter);
        Serial.println(" Successful");
        Serial.print("Read Time ");
        Serial.println(ReadTimerStop - ReadTimerStart);
        Serial.print("Write Time ");
        Serial.println(WriteTimerStop - WriteTimerStart);
        Serial.print(ReadLongValue);
        Serial.print(" ");
        Serial.println(ReadFloatValue);
      }

      //Set new test values
      WriteFlagLong = WriteFlagLong + 1;
      WriteFlagDouble = (random(0, 99) / 100.0);

      //Write new values to EEPROM
      eeAddress = 8 + sizeof(byte);

      WriteTimerStart = millis();
      EEPROM.put(eeAddress, WriteFlagLong);

      eeAddress = eeAddress + sizeof(WriteFlagLong + 16);

      EEPROM.put(eeAddress, WriteFlagDouble);
      WriteTimerStop = millis();
    }
    else
    {
      TestPass = false;
      Serial.print("Test ");
      Serial.print(WriteFlagLong++);
      Serial.println(" Failed");
      Serial.print(ReadLongValue);
      Serial.print(" ");
      Serial.println(ReadFloatValue);

    }

    TestCounter++;
    PauseTimer = (millis() + 100);
  }
}
