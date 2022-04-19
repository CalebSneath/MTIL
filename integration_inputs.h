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
// Usage Information: Call the constructor and use getText()
// to grab from the clipboard. Note, guards are in place to prevent 
// the program from crashing due to receiving unexpected non-text
// input from the clipboard. In such a case, getText() will simply return
// the last text available to it. Regardless, in some cases, 
// mixed text format entries can cause the program to fail.
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
// Usage Information: Call the constructor and use getText()
// to grab from the input file.
// Only eight streams can usefully exist
// at once. Objects scroll through a list of names formatted 
// as "MTILFileX.in" where X ranges from 1 to 8. 
// Note: It is fine to change file extensions. The program
// doesn't actually use them (outside of as a file address).
// It's just a normal text file. Also, file names
// are chosen in order of creation down the list.
// Inheritance information: See Integration_Output for usage.
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

