//////////////////////////////////////////////////////////////////////////////
// 
// Author:           Caleb Sneath, Parker Hagmaier, MD Abubakkar
// Email:            ansengor@yahoo.com
// Label:            MTIL
// Course:           CMPS 5153
// Semester:         Spring 2022
// Date:             March 15, 2022
// File Description: This file contains the implementation details for the 
//                   translation output class along with all of 
//                   its  descendents.
//////////////////////////////////////////////////////////////////////////////

#include "integration_outputs.h"
#include <iostream>
#include <string>

// Windows Version
#define WINDOWSRELEASE
#ifdef WINDOWSRELEASE
#include <Windows.h>

using namespace std;


//////////////////////////////////////////////////////////////////////////////
//                             Abstract Classes
//////////////////////////////////////////////////////////////////////////////

// Integration_Output

inline void Integration_Output::setText(string inText)
{
    outputText = inText;
}
inline void Integration_Output::setCoordinates
(int topLeftX, int topLeftY, int lowRightX, int lowRightY)
{
    upperLeftX = topLeftX;
    upperLeftY = topLeftY;
    bottomRightX = lowRightX;
    bottomRightY = lowRightY;
}

//////////////////////////////////////////////////////////////////////////////
//                             Child Classes
//////////////////////////////////////////////////////////////////////////////

// Horizontal_Output

inline void Horizontal_Output::displayText(int topLeftX, int topLeftY, int lowRightX, int lowRightY)
{

}
inline void Horizontal_Output::displayText()
{
    
}

// Vertical_Output

inline void Vertical_Output::displayText(int topLeftX, int topLeftY, int lowRightX, int lowRightY)
{

}
inline void Vertical_Output::displayText()
{

}

// Transparent_Output

inline void Transparent_Output::displayText(int topLeftX, int topLeftY, int lowRightX, int lowRightY)
{
    
}
inline void Transparent_Output::displayText()
{

}

// Image_Output

inline void Image_Output::displayText(int topLeftX, int topLeftY, int lowRightX, int lowRightY)
{

}
inline void Image_Output::displayText()
{

}

// Solid_Output

inline void Solid_Output::displayText(int topLeftX, int topLeftY, int lowRightX, int lowRightY)
{

}
inline void Solid_Output::displayText()
{

}

#endif