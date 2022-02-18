///////////////////////////////////////////////////////////////////////////////
// 
// Author:           Caleb Sneath, Parker, MD
// Email:            ansengor@yahoo.com
// Label:            MTIL
// Course:           CMPS 5153
// Semester:         Spring 2022
// File Description: This file contains the header information for the 
//                   translation output class along with all of 
//                   its  descendents.
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include <string>

#pragma once

#include <string>

///////////////////////////////////////////////////////////////////////////////
//                             Interfaces
///////////////////////////////////////////////////////////////////////////////

// Interface class for any Integration_Input class

// Class: Integration_Output
// Purpose: Interface to demonstrate usage information on 
//          Integration_Output classes.
// Usage:   Use updateText to refresh what text is by objects of this class.
//          Use getText to grab from an object of this class.
//          Use getTextNoUpdate to grab text from objects of this class 
//          without checking for updates from the text source.
class Integration_Output
{
    public:
        void setText(std::string inText);
        void setCoordinates();
        virtual void displayText
            (int topLeftX, int topLeftY, int lowRightX, int lowRightY)=0;
        virtual void displayText();
    private:
        std::string outputText;
        int upperLeftX;
        int upperRightY;
        int bottomLeftX;
        int bottomRightY;
};

///////////////////////////////////////////////////////////////////////////////
//                             Child Classes
///////////////////////////////////////////////////////////////////////////////

// Class: Clipboard_Input
// Purpose: A class to collect input from the clipboard fo an application.
class Horizontal_Output: public Integration_Output
{
    public:
        Horizontal_Output();

        void updateText();
        // Any necessary constructor overloads
        std::string getText();
        std::string getTextNoUpdate();
    private: 
        // Any internal implementation details
};
