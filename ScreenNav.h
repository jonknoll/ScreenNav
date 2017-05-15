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

@description


*******************************************************************************/
#ifndef SCREENNAV_H
#define SCREENNAV_H


#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * public definitions
 ******************************************************************************/

enum
{
   EV_NO_EVENT = 0,
   EV_ENTER = 1,
   EV_EXIT = 2,
   
   // must be last in this list
   SCREEN_NAV_FIRST_CUSTOM_EVENT
};
   
   

/*******************************************************************************
 * public typedefs
 ******************************************************************************/
typedef void (*screenHandlerFunc_t)(int event, int param);
#if 0
typedef void (*navExitCallbackFunc_t)(void);
#endif

/*******************************************************************************
 * public variables
 ******************************************************************************/


/*******************************************************************************
 * public functions
 ******************************************************************************/

class ScreenNav
{
public:
   ScreenNav();
   void start(screenHandlerFunc_t firstScreen);
   void sendEvent(int event, int param);
   void changeScreen(screenHandlerFunc_t nextScreen);
#if 0
   void delayEvent(uint32_t ms, int event, int param);
   void enter();
   void exit();
   void setNavExitCallback(navExitCallbackFunc_t callbackFunc);
#endif
   
private:
   screenHandlerFunc_t currentScreen;
   screenHandlerFunc_t prevScreen;
#if 0
   navExitCallbackFunc_t exitCallback_p;
   void nullMenuHandler(int event, int param);
#endif
};



#ifdef __cplusplus
}
#endif

#endif
