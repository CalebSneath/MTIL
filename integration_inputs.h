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
#include <iostream>
#include <windows.h>
#include <fstream>

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
        std::string getTextNoUpdate() { return inputText; }
    protected:
        std::string inputText = "";
};

///////////////////////////////////////////////////////////////////////////////
//                             Child Classes
///////////////////////////////////////////////////////////////////////////////

// Class: Clipboard_Input
// Purpose: A class to collect input from the clipboard for an application.
class Clipboard_Input : public Integration_Input
{
public:
    Clipboard_Input();
    void updateText();
    std::string getText();
    std::string getTextNoUpdate() {return inputText;}
private:
    // since getText Doesn't return we must save the 'text' variable
    //that outputs from the clipboard into a variable here called 'newText'
    std::string newText = "";
};

// Class: File_Input
// Purpose: A class to collect input from an input file.
class File_Input : public Integration_Input
{
public:
    File_Input();
    void updateText();
    std::string getText();
    std::string getTextNoUpdate() {return inputText;}
    ~File_Input() {files[index] = false;}

private:
    std::string fileName = "";
    static bool files[8];
    static std::string names[8];
    std::ofstream createFile;
    std::ifstream infile;
    std::string store = "";
    int index;
};

