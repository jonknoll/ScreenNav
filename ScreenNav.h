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
Will add later. For now, look at the example code. It gives a pretty good
set of examples of how to write screens using this class.

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
   
   // Set your first event enumeration to this value
   SCREEN_NAV_FIRST_CUSTOM_EVENT
};
   

// Make all of your screen functions of this type
typedef void (*screenHandlerFunc_t)(int event, int param);



class ScreenNav
{
public:
   ScreenNav();
   void start(screenHandlerFunc_t firstScreen);
   void sendEvent(int event, int param);
   void changeScreen(screenHandlerFunc_t nextScreen);
   
private:
   screenHandlerFunc_t currentScreen;
   screenHandlerFunc_t prevScreen;
};



#ifdef __cplusplus
}
#endif

#endif
