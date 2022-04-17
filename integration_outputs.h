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
//                             Abstract Classes
//////////////////////////////////////////////////////////////////////////////

// Interface class for any Integration_Input class

// Class: Integration_Output
// Purpose: Interface to demonstrate usage information on 
//          Integration_Output classes.
// Usage:   Use updateText to refresh what text is by objects of this class.
//          Use getText to grab from an object of this class.
//          Use getTextNoUpdate to grab text from objects of this class 
//          without checking for updates from the text source.
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
// Purpose: A class to grab the left and right of the display area and
// stretch it across before displaying text.
class Horizontal_Output: public Integration_Output
{
    public:
        Horizontal_Output();// Mandatory
        Horizontal_Output(HWND inHandle, UINT inMessage, WPARAM inWParam,
            LPARAM inLongParam);
        void cycleText(std::string inText,
            HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget);

};

class Vertical_Output : public Integration_Output
{
    public:
        Vertical_Output();// Mandatory
        Vertical_Output(HWND inHandle, UINT inMessage, WPARAM inWParam,
            LPARAM inLongParam);
        void cycleText(std::string inText,
            HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget);
};

class Image_Output : public Integration_Output
{
    public:
        // The parent constructor is fine to inherit.
        using Integration_Output::Integration_Output;
        void cycleText(std::string inText,
            HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget);
};

class Solid_Output : public Integration_Output
{
    public:
        Solid_Output();// Mandatory
        Solid_Output(HWND inHandle, UINT inMessage, WPARAM inWParam,
            LPARAM inLongParam);
        void cycleText(std::string inText,
            HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget);
};
