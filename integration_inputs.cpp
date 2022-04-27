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

#pragma once

#include "integration_inputs.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>

///////////////////////////////////////////////////////////////////////////////
//                             Child Classes
///////////////////////////////////////////////////////////////////////////////

// Clipboard_Input
Clipboard_Input::Clipboard_Input()
{
    getText();
}
void Clipboard_Input::updateText() {
    // Guard against unexpected input errors
    try
    {
        std::string holdText = "";

        OpenClipboard(nullptr);
        // Make sure the clipboard actually has text so we don't pass in any
        // noncharacter data to things expecting strings or chars. Otherwise,
        // just keep the old text.
        if (IsClipboardFormatAvailable(CF_TEXT))
        {
            HANDLE hData = GetClipboardData(CF_TEXT);

            char* pszText = static_cast<char*>(GlobalLock(hData));
            std::string text(pszText);

            GlobalUnlock(hData);

            holdText = text;
        }
        CloseClipboard();
        newText = holdText;
    }
    catch (...)
    {
        newText = inputText;
    }
}
std::string Clipboard_Input::getText() 
{
    updateText();
    inputText = newText;
    return inputText;
}

// File_Input
// Static Member Variables
bool File_Input::files[8] = { false, false, false, false, false, 
    false, false, false };
std::string File_Input::names[8] = { "MTILFile1.in", "MTILFile2.in", 
    "MTILFile3.in", "MTILFile4.in", "MTILFile5.in", "MTILFile6.in", 
    "MTILFile7.in", "MTILFile8.in" };
File_Input::File_Input() 
{
    std::ifstream confFile;
    confFile.open("Input Sources.conf");

    int loopIndex = 0;
    // Guard against crashes
    if (confFile.is_open())
    {
        // Iterate through file
        // and load names into array
        while (confFile && loopIndex < 9)
        {
            std::string holdInput;
            confFile >> holdInput >> holdInput >> names[loopIndex];

            // Iterate loop control
            loopIndex++;
        }

        // Close file
        confFile.close();
    }

    // ConfFile Corrupted, try making a new one
    if (loopIndex != 9)
    {
        std::ofstream newConfFile;
        newConfFile.open("Input Sources.conf");

        // Iterate through each row
        for (int newLoopIndex = 0; newLoopIndex < 9; newLoopIndex++)
        {
            newConfFile << "Input " << newLoopIndex + 1 <<
                ": MTILFile" << newLoopIndex + 1 << ".in" << std::endl;
        }

        // Close file
        newConfFile.close();
    }

    // Iterate through the input files
    // Grab the first and mark it as
    // in use to all other input streams.
    for (int i = 0; i < 8; i++) {
        if (files[i] == false) {
            fileName = names[i];
            files[i] = true;
            index = i;
            break;
        }
    }

    getText();
}
void File_Input::updateText() 
{
    std::string temp = "";
    store = "";
    infile.open(fileName);
    // Go through the input file
    // and add every character to a string.
    // Assuming it even opens.
    if (infile.is_open())
    {
        while (!infile.eof()) {
            infile >> temp;
            if (store[0]) {
                store += " " + temp + " ";
            }
            else {
                store += temp + " ";
            }
        }
        infile.close();
    }
}
std::string File_Input::getText() 
{
    updateText();
    inputText = store;
    return inputText;
}
