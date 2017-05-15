#include "Arduino.h"
#include <avr/pgmspace.h>
#include "ScreenNav.h"


//////////////////////////
// Defines and typedefs
//////////////////////////

#define SUPPRESS_WARNING(x) (void)(x)


enum
{
   EV_BUTTON1 = SCREEN_NAV_FIRST_CUSTOM_EVENT,
   EV_BUTTON2,
   EV_BUTTON3,
   EV_BUTTON4,
   EV_1SEC,
   EV_KEYPRESS
};


typedef struct
{
   const char *menuName;
   screenHandlerFunc_t menuItem;
} menuItem_t;


//////////////////////////
// LOCAL VARIABLES
//////////////////////////
const float FREQ_CAL_LIMIT = 1.0;
float freqCalValue = 0.0;


// BACKLIGHT MENU STUFF
typedef struct
{
   const char *str;
   int sec;
} backlightTimes_t;

int backlightTO;
const char alwaysOffStr[] PROGMEM = "ALWAYS OFF";
const char time1sec[] PROGMEM = "1 SEC";
const char time10sec[] PROGMEM = "10 SEC";
const char time30sec[] PROGMEM = "30 SEC";
const char time1min[] PROGMEM = "1 MIN";
const char time2min[] PROGMEM = "2 MIN";
const char time5min[] PROGMEM = "5 MIN";
const char time30min[] PROGMEM = "30 MIN";
const char timeAlwaysOn[] PROGMEM = "ALWAYS ON";

int backlightTimeoutIndex = 0;
PROGMEM const backlightTimes_t backlightTimes[] = {
      { alwaysOffStr,   0 },
      { time1sec,       1 },
      { time10sec,      10 },
      { time30sec,      30 },
      { time1min,       60 },
      { time2min,       2 * 60 },
      { time5min,       5 * 60 },
      { time30min,      30 * 60 },
      { timeAlwaysOn,   -1 }
};
#define NUM_BACKLIGHT_TIMES (sizeof(backlightTimes)/sizeof(backlightTimes_t))

// 1 second tick
uint32_t nextMsec = 0;


// Top Menu Screens
void topWelcomeScreen(int event, int param);
void topRunningTimeScreen(int event, int param);
void topKeypressScreen(int event, int param);

// Setup Screen Table
const PROGMEM char freqCalibrationStr[] = "Freq Calibration";
const PROGMEM char backlightStr[] = "Backlight";
const PROGMEM char resetStr[] = "Factory Reset";

PROGMEM const menuItem_t menuLevel1[] = 
      {
            { freqCalibrationStr, setupFreqCalScreen },
            { backlightStr, setupBacklightScreen },
            { resetStr, setupResetScreen },
      };
#define MENULEVEL1_NUM_ITEMS (sizeof(menuLevel1)/sizeof(menuLevel1[0]))

//////////////////////////
// Function declarations
//////////////////////////
void setupMenuScreen(int event, int param);
void setupFreqCalScreen(int event, int param);
void setupBacklightScreen(int event, int param);
void setupResetScreen(int event, int param);



// Declare a screen navigator
ScreenNav Nav;


//////////////////////////
// Functions
//////////////////////////

//The setup function is called once at startup of the sketch
void setup()
{
   Serial.begin(9600);
   Serial.println(F("START!"));
   Nav.start(topWelcomeScreen);
}

// The loop function is called in an endless loop
void loop()
{
   int serialRead;
   uint32_t msec;
   
   // Send 1 second tick event
   msec = millis();
   if(msec > nextMsec)
   {
      nextMsec = msec + 1000;
      Nav.sendEvent(EV_1SEC, 0);
   }
   
   // Send Keyboard event
   serialRead = Serial.read();
   if(serialRead != -1)
   {
      switch(serialRead)
      {
         case '1':
            Nav.sendEvent(EV_BUTTON1, 0);
            break;
         case '2':
            Nav.sendEvent(EV_BUTTON2, 0);
            break;
         case '3':
            Nav.sendEvent(EV_BUTTON3, 0);
            break;
         case '4':
            Nav.sendEvent(EV_BUTTON4, 0);
            break;
         default:
            Nav.sendEvent(EV_KEYPRESS, serialRead);
            break;
      }
   }
}

/////////////////////////////
// TOP MENU SCREENS
/////////////////////////////

void topWelcomeScreen(int event, int param)
{
   SUPPRESS_WARNING(param);
   
   switch(event)
   {
      case EV_ENTER:
         Serial.println(F("Welcome To Example Menu System!"));
         Serial.println(F("Button1 = Setup Menu\nButton2 = Prev Screen"));
         Serial.println(F("Button3 = Next Screen\nButton4 = Enter"));
         break;
      case EV_BUTTON1:
         Nav.changeScreen(setupMenuScreen);
         break;
      case EV_BUTTON2:
         Nav.changeScreen(topKeypressScreen);
         break;
      case EV_BUTTON3:
         Nav.changeScreen(topRunningTimeScreen);
         break;
      case EV_BUTTON4:
         char buffer[30];
         Serial.println(F("Settings:"));
         Serial.print(F("Frequency Calibration = "));
         Serial.print(freqCalValue);
         Serial.println(F(" Hz"));
         Serial.print(F("Backlight Timeout = "));
         strcpy_P(buffer, (char *)pgm_read_word(&(backlightTimes[backlightTimeoutIndex].str)));
         Serial.println(buffer);
         break;
   }
}

void topRunningTimeScreen(int event, int param)
{
   SUPPRESS_WARNING(param);
   
   switch(event)
   {
      case EV_ENTER:
         Serial.println(F("Current Running Time:"));
         break;
      case EV_BUTTON1:
         Nav.changeScreen(setupMenuScreen);
         break;
      case EV_BUTTON2:
         Nav.changeScreen(topWelcomeScreen);
         break;
      case EV_BUTTON3:
         Nav.changeScreen(topKeypressScreen);
         break;
      case EV_1SEC:
         Serial.print(F("The running time is "));
         Serial.println(millis());
         break;
   }
}

void topKeypressScreen(int event, int param)
{
   switch(event)
   {
      case EV_ENTER:
         Serial.println(F("Press a key:"));
         break;
      case EV_BUTTON1:
         Nav.changeScreen(setupMenuScreen);
         break;
      case EV_BUTTON2:
         Nav.changeScreen(topRunningTimeScreen);
         break;
      case EV_BUTTON3:
         Nav.changeScreen(topWelcomeScreen);
         break;
      case EV_KEYPRESS:
         Serial.print(F("Key pressed was: "));
         Serial.println(param);
         break;
   }
}


/////////////////////////////
// SETUP SCREENS
/////////////////////////////

void setupMenuScreen(int event, int param)
{
   static screenHandlerFunc_t prevScreen;
   static bool setupMode = false;
   static uint8_t index = 0;
   char buffer[30];
   screenHandlerFunc_t nextScreen;
   
   switch(event)
   {
      case EV_ENTER:
         if(setupMode == false)
         {
            // entering setup from higher menu
            Serial.println(F("Welcome to Setup menu!"));
            prevScreen = (screenHandlerFunc_t)param;
            setupMode = true;
         }
         Serial.print(F("Set: "));
         strcpy_P(buffer, (char *)pgm_read_word(&(menuLevel1[index].menuName)));
         Serial.println(buffer);
         break;
      case EV_BUTTON1:
         // leaving setup
         setupMode = false;
         Nav.changeScreen(prevScreen);
         break;
      case EV_BUTTON2:
         if(index > 0)
         {
            index--;
            
            Serial.print(F("Set: "));
            strcpy_P(buffer, (char *)pgm_read_word(&(menuLevel1[index].menuName)));
            Serial.println(buffer);
         }
         break;
      case EV_BUTTON3:
         if(index < MENULEVEL1_NUM_ITEMS - 1)
         {
            index++;
            
            Serial.print(F("Set: "));
            strcpy_P(buffer, (char *)pgm_read_word(&(menuLevel1[index].menuName)));
            Serial.println(buffer);
         }
         break;
      case EV_BUTTON4:
         Serial.print(F("Enter menu: "));
         strcpy_P(buffer, (char *)pgm_read_word(&(menuLevel1[index].menuName)));
         Serial.println(buffer);
         nextScreen = (screenHandlerFunc_t)pgm_read_word(&(menuLevel1[index].menuItem));
         Nav.changeScreen(nextScreen);
         break;
   }
}

void setupFreqCalScreen(int event, int param)
{
   SUPPRESS_WARNING(param);
   static float tempFreqCalValue;
   const float increment = 0.01;
   
   switch(event)
   {
      case EV_ENTER:
         tempFreqCalValue = freqCalValue;
         Serial.println(F("Freq Calibration"));
         Serial.print(F("Current Value = "));
         Serial.println(tempFreqCalValue);
         break;
      case EV_BUTTON1:
         Nav.changeScreen(setupMenuScreen);
         break;
      case EV_BUTTON2:
         if(tempFreqCalValue > -FREQ_CAL_LIMIT)
         {
            tempFreqCalValue -= increment;
            Serial.print(F("Current Value = "));
            Serial.println(tempFreqCalValue);
         }
         break;
      case EV_BUTTON3:
         if(tempFreqCalValue < FREQ_CAL_LIMIT)
         {
            tempFreqCalValue += increment;
            Serial.print(F("Current Value = "));
            Serial.println(tempFreqCalValue);
         }
         break;
      case EV_BUTTON4:
         freqCalValue = tempFreqCalValue;
         Serial.println(F("Saved!"));
         Nav.changeScreen(setupMenuScreen);
         break;
   }
}

void setupBacklightScreen(int event, int param)
{
   SUPPRESS_WARNING(param);
   static int tempTimeoutIndex;
   char buffer[30];
   
   switch(event)
   {
      case EV_ENTER:
         tempTimeoutIndex = backlightTimeoutIndex;
         Serial.println(F("Set Backlight Time:"));
         Serial.println(F("Backlight Timeout = "));
         strcpy_P(buffer, (char *)pgm_read_word(&(backlightTimes[tempTimeoutIndex].str)));
         Serial.println(buffer);
         break;
      case EV_EXIT:
         break;
      case EV_BUTTON1:
         Nav.changeScreen(setupMenuScreen);
         break;
      case EV_BUTTON2:
         if(tempTimeoutIndex > 0)
         {
            tempTimeoutIndex--;
         }
         Serial.println(F("Backlight Timeout = "));
         strcpy_P(buffer, (char *)pgm_read_word(&(backlightTimes[tempTimeoutIndex].str)));
         Serial.println(buffer);
         break;
      case EV_BUTTON3:
         if(tempTimeoutIndex < int(NUM_BACKLIGHT_TIMES - 1))
         {
            tempTimeoutIndex++;
         }
         Serial.println(F("Backlight Timeout = "));
         strcpy_P(buffer, (char *)pgm_read_word(&(backlightTimes[tempTimeoutIndex].str)));
         Serial.println(buffer);
         break;
      case EV_BUTTON4:
         backlightTimeoutIndex = tempTimeoutIndex;
         Serial.println(F("Saved!"));
         Nav.changeScreen(setupMenuScreen);
         break;
   }
}

void setupResetScreen(int event, int param)
{
   SUPPRESS_WARNING(param);
   static uint8_t resetStep = 0;
   
   switch(event)
   {
      case EV_ENTER:
         Serial.println(F("Factory Reset"));
         break;
      case EV_EXIT:
         resetStep = 0;
         break;
      case EV_BUTTON1:
         Nav.changeScreen(setupMenuScreen);
         break;
      case EV_BUTTON3:
         if(resetStep == 1)
         {
            Serial.print(F("Factory Reset: RESETTING!"));
            resetStep = 2;
         }
         break;
      case EV_BUTTON4:
         Serial.println(F("Factory Reset: ARE YOU SURE?? Press button 3 to confirm."));
         resetStep = 1;
         break;
      case EV_1SEC:
         if(resetStep == 2)
         {
            Serial.println(F("DONE!"));
            Nav.changeScreen(topWelcomeScreen);
         }
         break;
   }
}
