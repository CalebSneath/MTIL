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

#include <iostream>
#include <string>
#include "integration_inputs.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////////
//                             Abstract Classes
//////////////////////////////////////////////////////////////////////////////

// Integration_Input

std::string Integration_Input::getTextNoUpdate()
{
    return inputText;
}


//////////////////////////////////////////////////////////////////////////////
//                             Child Classes
//////////////////////////////////////////////////////////////////////////////


//Clipboard_Input

inline void Clipboard_Input::updateText()
{
    // TODO: Implementation
}
inline std::string Clipboard_Input::getText()
{
    // TODO: Implementation
    return inputText;
}

// File_Input

File_Input::File_Input()
{
    fileName = "";
}
File_Input::File_Input(std::string inFileName)
{
    fileName = inFileName;
}
inline void File_Input::updateText()
{
    // TODO: Implementation
}
inline std::string File_Input::getText()
{
    // TODO: Implementation
    return inputText;
}
