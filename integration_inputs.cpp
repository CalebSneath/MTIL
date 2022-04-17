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
    OpenClipboard(nullptr);
    // Make sure the clipboard actually has text so we don't pass in any
    // noncharacter data to things expecting strings or chars. Otherwise,
    // just keep the old text.
    if (IsClipboardFormatAvailable(CF_TEXT) || 
        IsClipboardFormatAvailable(CF_UNICODETEXT))
    {
        HANDLE hData = GetClipboardData(CF_TEXT);

        char* pszText = static_cast<char*>(GlobalLock(hData));
        std::string text(pszText);

        GlobalUnlock(hData);

        newText = text;
    }
    CloseClipboard();
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
