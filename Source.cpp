///////////////////////////////////////////////////////////////////////////////
// 
// Author:           Caleb Sneath, MD Abubakkar, Parker Hagmaier
// Email:            ansengor@yahoo.com
// Label:            Machine Translation Integration Layer
// Course:           CMPS 5153
// Semester:         Spring 2022
//
// Description:
//       This program is designed to make the translations for a windows
//       application appear more seamlessly within its intended application
//       
// Requirements (Visual Studios): 
//  - Create a new empty C++ program in Visual Studios.
//  - Copy files into Source FIles in Visual Studios. (.h and .cpp files)
//  - Copy .ico file to Resource Files.
//  - Hit local windows debugger to run as debug or build and press F5
// Requirements (Deprecated VS Code): 
//      - Ensure VS Code compiler is setup correctly, uses G++, C++ 20,
//        and that all files are in the correct folder, including arial.ttf.
// Compilation Instructions:
// - Open command terminal, navigate to this project directory. 
// - Run each of the following without parentheses:
//  - (g++ -c Source.cpp integration_inputs.cpp integration_outputs.cpp -O2 --inline)
//  - (g++ Source.o integration_inputs.o integration_outputs.o -o "MTIL" -O2 --inline)
//  - ("MTIL.exe")
//  - The game should now play and the executable can be renamed.
//      
// Files:            
//      main.cpp    : driver program 
//      arial.ttf   : font libray
// Font obtained from: 
//          https://github.com/zyedidia/SFML.jl/blob/master/assets/arial.ttf
//      assorted jpg files: image assets
//      test
// Credit: 
//      Extended sections of code were used from the Microsoft Windows.h
//      online documentation and tutorial. Source:
//          https://docs.microsoft.com/en-us/windows/win32/learnwin32
///////////////////////////////////////////////////////////////////////////////

#pragma once

// Ensure UNICODE precompiler directive is activated
#ifdef _UNICODE
#ifndef UNICODE
#define UNICODE
#endif
#endif
#ifndef UNICODE
#define UNICODE
#endif

//#include "integration_inputs.h"
//#include "integration_outputs.h"
#include "user_interface.h"
#include <iostream>

//#define WINDOWSRELEASE
//#ifdef WINDOWSRELEASE
#include <Windows.h>

// C RunTime Header Files:
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>

//#include "basewin.h"
#include <d2d1.h>
#pragma comment(lib,"d2d1.lib")
//#pragma comment(lib, "d2d1")
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>


int main(int flagCount, char* debugFlags[])
{
    bool debugSession = false;
    if (flagCount > 1)
    {
        std::string debugCheck = debugFlags[1];
        if (debugCheck == "-d")
        {
            std::cout << "Debug Mode" << std::endl;
            debugSession = true;
        }
    }
    else
    {
        //Remove the console so only the game shows, reducing clutter
        HWND noConsole = GetConsoleWindow();
        //ShowWindow(noConsole, SW_HIDE);
    }

    MTILUIProgram MTILInstance;
    MTILInstance.runMTIL();

    std::cout << "Exiting" << std::endl;
    return 0;
}
//#endif

// In case of future Mac port
// The idea is that #define WINDOWSRELEASE
// can just be commented out after code is
// added here and the appropriate version
// is uncommented.
//#define MACRELEASE
#ifdef MACRELEASE
int main(int flagCount, char* debugFlags[])
{
    bool debugSession = false;
    if (flagCount > 1)
    {
        std::string debugCheck = debugFlags[1];
        if (debugCheck == "-d")
        {
            std::cout << "Debug Mode" << std::endl;
            debugSession = true;
        }
    }

    return 0;
}
#endif

// In case of future Linux port
//#define LINUXRELEASE
#ifdef LINUXRELEASE
int main(int flagCount, char* debugFlags[])
{
    bool debugSession = false;
    if (flagCount > 1)
    {
        std::string debugCheck = debugFlags[1];
        if (debugCheck == "-d")
        {
            std::cout << "Debug Mode" << std::endl;
            debugSession = true;
        }
    }

    return 0;
}
#endif