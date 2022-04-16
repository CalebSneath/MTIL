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
void Clipboard_Input::updateText() {
    OpenClipboard(nullptr);
    HANDLE hData = GetClipboardData(CF_TEXT);

    char* pszText = static_cast<char*>(GlobalLock(hData));
    std::string text(pszText);

    GlobalUnlock(hData);
    CloseClipboard();

    newText = text;
}
std::string Clipboard_Input::getText() 
{
    inputText = newText;
    return inputText;
}

// File_Input
File_Input::File_Input() 
{
    for (int i = 0; i < 8; i++) {
        if (files[i] == false) {
            fileName = names[i];
            files[i] = true;
            index = i;
            break;
        }
    }
    //Commented out don't need to create the file here
    //createFile.open(fileName);
    //createFile.close();
}
void File_Input::updateText() 
{
    std::string temp = "";
    store = "";
    infile.open(fileName);
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
std::string File_Input::getText() {
    inputText += " " + store;
    return inputText;
}
// Static Member Variables
bool File_Input::files[8] = { false, false, false, false, false, false, false, false };
std::string File_Input::names[8] = { "MTILFile1", "MTILFile2", "MTILFile3", "MTILFile4", "MTILFile5", "MTILFile6", "MTILFile7", "MTILFile8" };
