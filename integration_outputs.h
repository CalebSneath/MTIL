//////////////////////////////////////////////////////////////////////////////
// 
// Author:           Caleb Sneath, Parker, MD
// Email:            ansengor@yahoo.com
// Label:            MTIL
// Course:           CMPS 5153
// Semester:         Spring 2022
// Date:             March 15, 2022
// File Description: This file contains the header information for the 
//                   translation output class along with all of 
//                   its  descendents.
//////////////////////////////////////////////////////////////////////////////


#pragma once

#include <string>
#include <Windows.h>
#include <d2d1.h>
#include "user_interface_primitives.h"

//////////////////////////////////////////////////////////////////////////////
//                             Parent Classes
//////////////////////////////////////////////////////////////////////////////

// Interface class for any Integration_Input class

// Class: Integration_Output
// Purpose: Displays text and attempts to find a suitable text color to
//          display the text on. Also meant to act as a parent class
//          which all other output classes can inherit from
// Usage:   Call the nondefault constructor. Afterward, call setCoords
//          to choose display coordinates. Call cycleText to run,
//          passing in the texxt to display. Note, sometimes text
//          shows up in a hard to read color. This is a sign that the 
//          display settings are causing an application error in reading
//          pixels in the correct area. Fortunately, this can
//          be usually fixed by adjusting the overlay offset settings
//          int the "Output Offset.conf" file.
// Inheritance Information: Set RectangleShape for more usage info.
class Integration_Output: public TextBox
{
    public:
        Integration_Output();// Mandatory.
        Integration_Output(HWND inHandle, UINT inMessage, WPARAM inWParam,
            LPARAM inLongParam);
        virtual void cycleText(std::string inText,
            HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget);
    protected:
        int xOffset = 0;
        int yOffset = 0;
};

//////////////////////////////////////////////////////////////////////////////
//                             Child Classes
//////////////////////////////////////////////////////////////////////////////

// Class: Horizontal_Output
// Purpose: A class to grab the top of the display area and
// stretch it across before displaying text.
// Inheritance information: See Integration_Output for usage.
class Horizontal_Output: public Integration_Output
{
    public:
        Horizontal_Output();// Mandatory
        Horizontal_Output(HWND inHandle, UINT inMessage, WPARAM inWParam,
            LPARAM inLongParam);
        void cycleText(std::string inText,
            HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget);

};

// Class: Horizontal_Output
// Purpose: A class to grab the left of the display area and
// stretch it across before displaying text.
// Inheritance information: See Integration_Output for usage.
class Vertical_Output : public Integration_Output
{
    public:
        Vertical_Output();// Mandatory
        Vertical_Output(HWND inHandle, UINT inMessage, WPARAM inWParam,
            LPARAM inLongParam);
        void cycleText(std::string inText,
            HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget);
};

// Class: Image_Output
// Purpose: A class to grab an image and display it as a background
// for some text.
// Usage Information: Only nine streams can usefully exist
// at once. Objects scroll through a list of names formatted 
// as "MTILFileX.png" where X ranges from 1 to 8. File two
// is "Labyrinth.png" and can be used to show the program logo.
// Note: It is fine to change file extensions. The program
// doesn't actually use them (outside of as a file address).
// Most common formats should work fine with the program
// after being renamed to X.png. Also, file names
// are chosen in order of creation down the list.
// Inheritance information: See Integration_Output for usage.
class Image_Output : public Integration_Output
{
    public:
        Image_Output();// Mandatory
        Image_Output(HWND inHandle, UINT inMessage, WPARAM inWParam,
            LPARAM inLongParam);
        // The parent constructor is fine to inherit.
        void cycleText(std::string inText,
            HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget);
        ~Image_Output() { files[index] = false; }
    protected:
        static bool files[9];
        static std::string names[9];
        std::string fileName;
        int index = 0;
};

// Class: Horizontal_Output
// Purpose: A class to grab a pixel near the display
// area and color its background as such.
// Inheritance information: See Integration_Output for usage.
class Solid_Output : public Integration_Output
{
    public:
        Solid_Output();// Mandatory
        Solid_Output(HWND inHandle, UINT inMessage, WPARAM inWParam,
            LPARAM inLongParam);
        void cycleText(std::string inText,
            HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget);
};
