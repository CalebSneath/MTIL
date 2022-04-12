//Uploading so I know what to change and what is innacurate 

#include <iostream>
using namespace std;
#include <string>
#include <windows.h>
#include <fstream>
//I got this from the .h file that is stored in the MTIL
//Sction of your C drive should be abe to access it from the vs code
//You are just building the code all in one file cause fuck it


//this is my version of the program just implimenting the basics



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
        void updateText() {
            OpenClipboard(nullptr);
            HANDLE hData = GetClipboardData(CF_TEXT);

            char* pszText = static_cast<char*>(GlobalLock(hData));
            std::string text(pszText);

            GlobalUnlock(hData);
            CloseClipboard();

            newText = text;
        }
        // Any necessary constructor overloads
        std::string getText() {
            inputText = newText;
            return inputText;
        }
        std::string getTextNoUpdate() {
            return inputText;
        }
    private:
        // since getText Doesn't return we must save the 'text' variable
        //that outputs from the clipboard into a variable here called 'newText'
        string newText;
    };

    // Class: File_Input
    // Purpose: A class to collect input from an input file.
    class File_Input : public Integration_Input
    {
    public:
        File_Input() {
            for (int i = 0; i < 8; i++) {
                if (files[i] == false) {
                    fileName = names[i];
                    files[i] = true;
                    index = i;
                    break;
                }
            }
            createFile.open(fileName);
            createFile.close();
        }
       
        void updateText() {
            string temp = "";
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


        std::string getText() {
            inputText += " " + store;
            return inputText;

        }


        std::string getTextNoUpdate() {
            return inputText;
        }

        ~File_Input() {
            files[index] = false;
        }

    private:
        std::string fileName;
        static bool files[8];
        static string names[8];
        ofstream createFile;
        ifstream infile;
        string store;
        int index;
        // Any internal implementation details
    };
   
    bool File_Input::files[8] = { false, false, false, false, false, false, false, false };
    string File_Input::names[8] = { "MTILFile1", "MTILFile2", "MTILFile3", "MTILFile4", "MTILFile5", "MTILFile6", "MTILFile7", "MTILFile8" };

    // Get ClipBoard funcion
    /*
    std::string GetClipboardText()
    {
        OpenClipboard(nullptr);
        HANDLE hData = GetClipboardData(CF_TEXT);

        char* pszText = static_cast<char*>(GlobalLock(hData));
        std::string text(pszText);

        GlobalUnlock(hData);
        CloseClipboard();

        return text;
    }
    */
