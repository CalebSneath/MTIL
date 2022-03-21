///////////////////////////////////////////////////////////////////////////////
// 
// Author:           Caleb Sneath, Parker Hagmaier, MD Abubakkar
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
//                             Abstract Classes
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
    virtual void updateText() = 0;
    virtual std::string getText() = 0;
    std::string getTextNoUpdate();
protected:
    std::string inputText;
};

///////////////////////////////////////////////////////////////////////////////
//                             Child Classes
///////////////////////////////////////////////////////////////////////////////

// Class: Clipboard_Input
// Purpose: A class to collect input from the clipboard for an application.
class Clipboard_Input : public Integration_Input
{
public:
    void updateText();
    // Any necessary constructor overloads
    std::string getText();
    std::string getTextNoUpdate();
private:
    // Any internal implementation details
};

// Class: File_Input
// Purpose: A class to collect input from an input file.
class File_Input : public Integration_Input
{
public:
    File_Input();
    File_Input(std::string);
    void updateText();
    // Any necessary constructor overloads
    std::string getText();
    std::string getTextNoUpdate();
private:
    std::string fileName;
    // Any internal implementation details
};
