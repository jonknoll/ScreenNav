/***************************************************************************//**
MIT License

Copyright (c) 2017 Jon Knoll

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

@description See header file for a description
*******************************************************************************/
#include "Arduino.h"
#include "ScreenNav.h"


/*******************************************************************************
 * local definitions
 ******************************************************************************/
//#define SERIAL_DEBUG



/*******************************************************************************
 * local typedefs
 ******************************************************************************/


/*******************************************************************************
 *  public variables
 ******************************************************************************/


/*******************************************************************************
 *  local variables
 ******************************************************************************/


/*******************************************************************************
 *  public functions
 ******************************************************************************/


ScreenNav::ScreenNav()
{
#if 0
   exitCallback_p = 0;
#endif
}

void ScreenNav::start(screenHandlerFunc_t firstScreen)
{
   currentScreen = firstScreen;
   prevScreen = firstScreen;
   sendEvent(EV_ENTER, 0);
}

void ScreenNav::sendEvent(int event, int param)
{
#ifdef SERIAL_DEBUG
   Serial.print(F("Sending event="));
   Serial.print(event);
   Serial.print(F(", param=0x"));
   Serial.println((int)param, 16);
#endif
   currentScreen(event, param);
}

void ScreenNav::changeScreen(screenHandlerFunc_t nextScreen_p)
{
#ifdef SERIAL_DEBUG
   Serial.println(F("Send Exit Event"));
#endif
   currentScreen(EV_EXIT, (int)nextScreen_p);
   
   prevScreen = currentScreen;
   currentScreen = nextScreen_p;

#ifdef SERIAL_DEBUG
   Serial.println(F("Send Enter Event"));
#endif
   currentScreen(EV_ENTER, (int)prevScreen);
}


#if 0
void ScreenNav::delayEvent(uint32_t ms, int event, int param)
{
   delay(ms);
   currentScreen(event, param);
}

void ScreenNav::enter()
{
   currentScreen(EV_ENTER, 0);
}

void ScreenNav::exit()
{
   currentScreen(EV_EXIT, (int)currentScreen);
   currentScreen = ScreenNav::nullMenuHandler;
   if(ScreenNav::exitCallback_p != 0)
   {
      navExitCallbackFunc_t();
   }
}

void ScreenNav::setNavExitCallback(navExitCallbackFunc_t callbackFunc)
{
   exitCallback_p = callbackFunc;
}

void ScreenNav::nullMenuHandler(int event, int param)
{
}

#endif

/*******************************************************************************
 * local functions
 ******************************************************************************/
