///////////////////////////////////////////////////////////////////////////////
// 
// Author:           Caleb Sneath, Parker, MD
// Email:            ansengor@yahoo.com
// Label:            MTIL
// Course:           CMPS 5153
// Semester:         Spring 2022
// File Description: This file contains the header information for the 
//                   translation input class along with all of 
//                   its  descendents.
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>

///////////////////////////////////////////////////////////////////////////////
//                             Interfaces
///////////////////////////////////////////////////////////////////////////////

// Interface class for any Integration_Input class

// Class: Integration_Input
// Purpose: Interface to demonstrate usage information on integration 
//          input classes.
// Usage:   Use updateText to refresh what text is by objects of this class.
//          Use getText to grab from an object of this class.
//          Use getTextNoUpdate to grab text from objects of this class 
//          without checking for updates from the text source.
class Integration_Input
{
    public:
        virtual void updateText()=0;
        virtual std::string getText()=0;
        virtual std::string getTextNoUpdate()=0;
    private:
        std::string inputText;
};

///////////////////////////////////////////////////////////////////////////////
//                             Child Classes
///////////////////////////////////////////////////////////////////////////////

// Class: Clipboard_Input
// Purpose: A class to collect input from the clipboard fo an application.
class Clipboard_Input: public Integration_Input
{
    public:
        void updateText();
        // Any necessary constructor overloads
        std::string getText();
        std::string getTextNoUpdate();
    private: 
        // Any internal implementation details
};

