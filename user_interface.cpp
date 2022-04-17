//////////////////////////////////////////////////////////////////////////////
// 
// Author:           Caleb Sneath, Parker Hagmaier, MD Abubakkar
// Email:            ansengor@yahoo.com
// Label:            MTIL
// Course:           CMPS 5153
// Semester:         Spring 2022
// Date:             March 15, 2022
// File Description: This file contains the implementation details for the 
//                   user interface class along with other more general
//                   classes for the objects inside of a user interface, 
//                   such as a numeric input textbox, along with
//                   some basic shapes used as parent classes to create
//                   said items, such as a Circle class.
// Credit:           Much of the code on this page is heavily based upon
//                   code from Microsoft's online documentation and tutorial
//                   code for Windows.h and Direct 2D found here:
//                      https://docs.microsoft.com/en-us/windows/win32/
//                      learnwin32/learn-to-program-for-windows
//////////////////////////////////////////////////////////////////////////////

#pragma once

// Ensures UNICODE precompiler directive is activated
#ifdef _UNICODE
#ifndef UNICODE
#define UNICODE
#endif
#endif
#ifndef UNICODE
#define UNICODE
#endif


#include "integration_inputs.h"
#include "integration_outputs.h"
#include "user_interface.h"
#include "user_interface_primitives.h"
#include <iostream>
#include <thread>

// Precompiler directive in case anyone wants to modify relevant
// code to work for Mac or Linux builds. Commented out for now
// because it interfered with the warning squiggles that show up
// before you compile, which is likely a VS bug.
//#define WINDOWSRELEASE
//#ifdef WINDOWSRELEASE
#include <Windows.h>
#include <Windowsx.h>

// C RunTime Header Files:
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>

//#include "basewin.h"
#include <d2d1.h>
#include <dwmapi.h>


// Sometimes uncommenting one of these fixes user's include issues.
// Use the linker in the command line and comment these out
// if your compiler doesn't support pragma comment.
//#pragma comment(lib,"d2d1.lib")
//#pragma comment(lib, "d2d1")
#pragma comment(lib, "Dwrite")
#pragma comment(lib, "Dwmapi")

#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <Unknwn.h>


//////////////////////////////////////////////////////////////////////////////
//                             Normal Classes
//////////////////////////////////////////////////////////////////////////////

// TranslationStreamList

TranslationStreamList::TranslationStreamList()
{
    // Initialize 8 in/outputs streams
    for (int loopIndex = 0; loopIndex < 8; loopIndex++)
    {
        outputsArray[loopIndex] = new Integration_Output();
        inputsArray[loopIndex] = new Clipboard_Input();
        hDtKeys[loopIndex] = '`';// Arbitrary unset code
        active[loopIndex] = false;
    }
}
TranslationStreamList::TranslationStreamList(HWND inHandle, UINT inMessage, WPARAM inWParam,
    LPARAM inLongParam)
{
    hWnd = inHandle;
    uMsg = inMessage;
    wParam = inWParam;
    lParam = inLongParam;

    // Initialize 8 in/outputs streams
    for (int loopIndex = 0; loopIndex < 8; loopIndex++)
    {
        outputsArray[loopIndex] =  new 
            Vertical_Output(hWnd, uMsg, wParam, lParam);
        inputsArray[loopIndex] = new Clipboard_Input();
        hDtKeys[loopIndex] = '`';// Arbitrary unset code
        active[loopIndex] = false;
    }
}
void TranslationStreamList::cycleStreams(char inputKey, float inX, 
    float inY, HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget)
{

    // Iterate through list of in/outstreams
    for (int loopIndex = 0; loopIndex < 8; loopIndex++)
    {
        // Check if a hotkey matches the inputKey
        if (hDtKeys[loopIndex] == inputKey)
        {
            // If this is the first key set, go ahead and 
            // set this as the upper left coordinates
            if (bufferTopX == -1)
            {
                bufferTopX = inX;
                bufferTopY = inY;
            }
            // Otherwise, if the coordinates are above or
            // to the left, assume user is removing stream
            else if (inX < bufferTopX || inY < bufferTopY)
            {
                outputsArray[loopIndex]->setCoords(-1.0, -1.0, -1.0, -1.0);
            }
            else
            {
                // Set the desired coordinates and reset
                // the buffered variables
                outputsArray[loopIndex]->setCoords(bufferTopX, bufferTopY, inX, inY);
                bufferTopX = -1;
                bufferTopY = -1;
            }
        }

        // Draw outstream if it is active
        if (active[loopIndex] == true)
        {
            outputsArray[loopIndex]->cycleText(inputsArray[loopIndex]->getText(), hr, pRenderTarget);
        }
    }
}
void TranslationStreamList::setCoords(float inTopX, float inTopY, float inBottomX, 
    float inBottomY, int stream)
{
    if (stream < 8 && stream >= 0)
    {
        outputsArray[stream]->setCoords(inTopX, inTopY, inBottomX, inBottomY);
    }
}
void TranslationStreamList::setHotkey(char inText)
{
    hDtKeys[focus] = inText;
}
void TranslationStreamList::setFocus(const int inFocus)
{
    if ((inFocus < 8) && (inFocus >= 0))
    {
        focus = inFocus;
    }
}
void TranslationStreamList::setInStream(Integration_Input & inS)
{
    // Deallocate dynamic memory and adjust pointer. Activate stream.
    active[focus] = true;
    Integration_Input* deletePtr = inputsArray[focus];
    inputsArray[focus] = &inS;
    delete deletePtr;
}
void TranslationStreamList::setOutStream(Integration_Output & outS)
{
    // Deallocate dynamic memory and adjust pointer. Activate stream.
    active[focus] = true;
    Integration_Output* deletePtr = outputsArray[focus];
    outputsArray[focus] = &outS;
    delete deletePtr;
}
TranslationStreamList::~TranslationStreamList()
{
    // Deallocate memory
    for (int loopIndex = 0; loopIndex < 8; loopIndex++)
    {
        delete outputsArray[loopIndex];
        delete inputsArray[loopIndex];
    }
}


// MTILUIProgram

LRESULT CALLBACK MTILUIProgram::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    MTILUIProgram* pThis = NULL;

    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        pThis = (MTILUIProgram*)pCreate->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);

        pThis->m = hWnd;
    }
    else
    {
        pThis = (MTILUIProgram*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    }
    if (pThis)
    {
        return pThis->HandleMessage(uMsg, wParam, lParam);
    }
    else
    {
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

LRESULT CALLBACK MTILUIProgram::WindowProcChild(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    MTILUIProgram* pThis = NULL;

    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        pThis = (MTILUIProgram*)pCreate->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);

        pThis->screenWindow = hWnd;
    }
    else
    {
        pThis = (MTILUIProgram*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    }
    if (pThis)
    {
        return pThis->HandleMessage(uMsg, wParam, lParam);
    }
    else
    {
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

void MTILUIProgram::handleInOut()
{
    while(running == true)
    {

        // 0x8000 and 0x10000 are the values for 32 and 64 bit machines
        // for the left mouse being clicked. Does a bitwise
        // and and checks if its not equal to all zeros. Just
        // checking if the bitwise of 8000 is not all zeroes is probably
        // fastest and less architecture dependent.
        // The same is true for all others.
        if ((GetKeyState(VK_LBUTTON) & 0x8000) > 0)
        {
            clicked = true;
        }
        // Enter/Return
        else if ((GetKeyState(VK_RETURN) & 0x8000) > 0)
        {
            keyPressed = 'e';
        }
        else if ((GetKeyState('0') & 0x8000) > 0)
        {
            keyPressed = '0';
        }
        else if ((GetKeyState('1') & 0x8000) > 0)
        {
            keyPressed = '1';
        }
        else if ((GetKeyState('2') & 0x8000) > 0)
        {
            keyPressed = '2';
        }
        else if ((GetKeyState('3') & 0x8000) > 0)
        {
            keyPressed = '3';
        }
        else if ((GetKeyState('4') & 0x8000) > 0)
        {
            keyPressed = '4';
        }
        else if ((GetKeyState('5') & 0x8000) > 0)
        {
            keyPressed = '5';
        }
        else if ((GetKeyState('6') & 0x8000) > 0)
        {
            keyPressed = '6';
        }
        else if ((GetKeyState('7') & 0x8000) > 0)
        {
            keyPressed = '7';
        }
        else if ((GetKeyState('8') & 0x8000) > 0)
        {
            keyPressed = '8';
        }
        else if ((GetKeyState('9') & 0x8000) > 0)
        {
            keyPressed = '9';
        }
        // Not exact mappings from here down for 
        // keyPressed char sometimes
        // Backspace
        else if ((GetKeyState(VK_BACK) & 0x8000) > 0)
        {
            keyPressed = 'b';
        }
        // Shift + Buttons
        else if ((GetKeyState(VK_SHIFT) & 0x8000) > 0)
        {
            // - key
            if ((GetKeyState(VK_OEM_MINUS) & 0x8000) > 0)
            {
                keyPressed = 'q';
            }
            // = key
            else if ((GetKeyState(VK_OEM_PLUS) & 0x8000) > 0)
            {
                keyPressed = 'w';
            }
            // ] Key
            else if ((GetKeyState(VK_OEM_6) & 0x8000) > 0)
            {
                keyPressed = 'u';
            }
        }
        // Control + Buttons
        else if ((GetKeyState(VK_CONTROL) & 0x8000) > 0)
        {
            // - key
            if ((GetKeyState(VK_OEM_MINUS) & 0x8000) > 0)
            {
                keyPressed = 'r';
            }
            // = key
            else if ((GetKeyState(VK_OEM_PLUS) & 0x8000) > 0)
            {
                keyPressed = 't';
            }
            // ] Key
            else if ((GetKeyState(VK_OEM_6) & 0x8000) > 0)
            {
                keyPressed = 'y';
            }
        }

        Sleep(0.01);
    }
}

LRESULT MTILUIProgram::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {

    case WM_MOUSEMOVE:
        // Check for a mouse cursor and adjust if the program is clicked
        mouseX = GET_X_LPARAM(lParam);
        mouseY = GET_Y_LPARAM(lParam);
        break;
    case WM_CREATE:
        programLongParam = lParam;

        //
        //Initialize UI objects
        //

        // Title Bar
        titleBox = TextBox(m, uMsg, wParam, lParam);
        titleBox.setCoords(0.0, 0.0, 1.0, 0.2);
        titleBox.setText("Machine Translation Integration Layer");

        // Translation Stream Box
        tranStreamBorder = RectangularBorder(m, uMsg, wParam, lParam);
        tranStreamBorder.setCoords(0.1, 0.2, 0.4, 0.5);
        tranStreamLabel = TextBox(m, uMsg, wParam, lParam);
        tranStreamLabel.setCoords(0.1, 0.2, 0.4, 0.3);
        tranStreamLabel.setText("Translation Stream");
        tranStreamDrop = Dropdown(m, uMsg, wParam, lParam);
        tranStreamDrop.setCoords(0.13, 0.35, 0.37, 0.4);
        tranStreamDrop.appendItem("Stream 1");
        tranStreamDrop.appendItem("Stream 2");
        tranStreamDrop.appendItem("Stream 3");
        tranStreamDrop.appendItem("Stream 4");
        tranStreamDrop.appendItem("Stream 5");
        tranStreamDrop.appendItem("Stream 6");
        tranStreamDrop.appendItem("Stream 7");
        tranStreamDrop.appendItem("Stream 8");

        // Translation Stream Box
        inputStreamBorder = RectangularBorder(m, uMsg, wParam, lParam);
        inputStreamBorder.setCoords(0.1, 0.6, 0.4, 0.9);
        inputStreamLabel = TextBox(m, uMsg, wParam, lParam);
        inputStreamLabel.setCoords(0.1, 0.6, 0.4, 0.7);
        inputStreamLabel.setText("Input Stream");
        inputStreamDrop = Dropdown(m, uMsg, wParam, lParam);
        inputStreamDrop.setCoords(0.13, 0.75, 0.37, 0.8);
        inputStreamDrop.appendItem("Clipboard");
        inputStreamDrop.appendItem("Input File");

        // Output Box
        outputStreamBorder = RectangularBorder(m, uMsg, wParam, lParam);
        outputStreamBorder.setCoords(0.6, 0.2, 0.9, 0.9);
        // Output Location Section
        // Button Set Section
        outputLocationLabel = TextBox(m, uMsg, wParam, lParam);
        outputLocationLabel.setCoords(0.6, 0.2, 0.9, 0.3);
        outputLocationLabel.setText("Output Location");
        buttonSetRadio = ToggleButton(m, uMsg, wParam, lParam);
        buttonSetRadio.setCoords(0.65, 0.35, 0.06);
        buttonSetLabel = TextBox(m, uMsg, wParam, lParam);
        buttonSetLabel.setCoords(0.65, 0.30, 0.78, 0.4);
        buttonSetLabel.setText("Button Set");
        buttonSetDrop = Dropdown(m, uMsg, wParam, lParam);
        buttonSetDrop.setCoords(0.63, 0.38, 0.87, 0.43);
        // 'q'
        buttonSetDrop.appendItem("Set Button: Shift and -");
        // 'w'
        buttonSetDrop.appendItem("Set Button: Shift and =");
        // 'u'
        buttonSetDrop.appendItem("Set Button: Shift and ]");
        // 'r'
        buttonSetDrop.appendItem("Set Button: Control and -");
        // 't'
        buttonSetDrop.appendItem("Set Button: Control and =");
        // 'y'
        buttonSetDrop.appendItem("Set Button: Control and ]");
        buttonSetDrop.appendItem("Set Button: Enter");
        // Fixed Coordinates Section
        fixedSetRadio = ToggleButton(m, uMsg, wParam, lParam);
        fixedSetRadio.setCoords(0.65, 0.5, 0.045);
        fixedSetLabel = TextBox(m, uMsg, wParam, lParam);
        fixedSetLabel.setCoords(0.645, 0.47, 0.85, 0.53);
        fixedSetLabel.setText("Fixed Coordinates");
        fixedTLXLabel = TextBox(m, uMsg, wParam, lParam);
        fixedTLXLabel.setCoords(0.64, 0.55, 0.72, 0.60);
        fixedTLXLabel.setText("TL     X");
        fixedBRXLabel = TextBox(m, uMsg, wParam, lParam);
        fixedBRXLabel.setCoords(0.64, 0.62, 0.72, 0.67);
        fixedBRXLabel.setText("BR     X");
        topLXEntry = TextEnterInt(m, uMsg, wParam, lParam);
        topLXEntry.setCoords(0.72, 0.55, 0.78, 0.60);
        topLXEntry.setText("0");
        bottomRXEntry = TextEnterInt(m, uMsg, wParam, lParam);
        bottomRXEntry.setCoords(0.72, 0.62, 0.78, 0.67);
        bottomRXEntry.setText("0");
        fixedTLYLabel = TextBox(m, uMsg, wParam, lParam);
        fixedTLYLabel.setCoords(0.77, 0.55, 0.81, 0.60);
        fixedTLYLabel.setText("Y");
        fixedBRYLabel = TextBox(m, uMsg, wParam, lParam);
        fixedBRYLabel.setCoords(0.77, 0.62, 0.81, 0.67);
        fixedBRYLabel.setText("Y");
        topLYEntry = TextEnterInt(m, uMsg, wParam, lParam);
        topLYEntry.setCoords(0.80, 0.55, 0.86, 0.60);
        topLYEntry.setText("0");
        bottomRYEntry = TextEnterInt(m, uMsg, wParam, lParam);
        bottomRYEntry.setCoords(0.80, 0.62, 0.86, 0.67);
        bottomRYEntry.setText("0");
        outputMethodLabel = TextBox(m, uMsg, wParam, lParam);
        outputMethodLabel.setText("Output Method");
        outputMethodLabel.setCoords(0.6, 0.70, 0.9, 0.75);
        outputStreamDrop = Dropdown(m, uMsg, wParam, lParam);
        outputStreamDrop.setCoords(0.63, 0.75, 0.87, 0.8);
        outputStreamDrop.appendItem("Transparent");
        outputStreamDrop.appendItem("Solid Fill");
        outputStreamDrop.appendItem("Image");
        outputStreamDrop.appendItem("Horizontal Stretch");
        outputStreamDrop.appendItem("Vertical Stretch");
        // Bottom Left Logo Label
        logoLabel.setText("Labyrinth");
        logoLabel.setCoords(0.0, 0.9, 0.15, 1.0);

        // Translation Stream
        translationList = new TranslationStreamList(
            screenWindow, uMsg, wParam, lParam);


        //
        // End initializing UI Elements
        //

        break;
    case WM_CLOSE:
        DestroyWindow(m);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
        PAINTSTRUCT ps;
        PAINTSTRUCT screenPaintStruct;

        // Set up Direct 2D factory and other one time only things
        static ID2D1Factory* pFactory = NULL;
        if (FAILED(D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
        {
            // Fail drawing totally, exit.
            return DefWindowProc(m, uMsg, wParam, lParam);;  
        }
        static ID2D1HwndRenderTarget* pRenderTarget = NULL;
        static ID2D1HwndRenderTarget* screenRenderTarget = NULL;
        static ID2D1SolidColorBrush* pBrush = NULL;
        static ID2D1SolidColorBrush* screenBrush = NULL;
        static const D2D1_COLOR_F color = D2D1::ColorF(0.9, 0.9, 0.95);
        static const D2D1_COLOR_F screenColor = D2D1::ColorF(0.0, 0.0, 0.0);

        // Get the render target running as well as the background brush
        HRESULT hr = S_OK;
        if (pRenderTarget == NULL)
        {
            RECT rc;
            GetClientRect(m, &rc);
            GetClientRect(screenWindow, &screenRect);

            D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
            D2D1_SIZE_U screenSize = D2D1::SizeU(screenRect.right, screenRect.bottom);

            hr = pFactory->CreateHwndRenderTarget(
                D2D1::RenderTargetProperties(),
                D2D1::HwndRenderTargetProperties(m, size),
                &pRenderTarget);

            
            hr = pFactory->CreateHwndRenderTarget(
                D2D1::RenderTargetProperties(),
                D2D1::HwndRenderTargetProperties(screenWindow, screenSize),
                &screenRenderTarget);
            

            if (SUCCEEDED(hr))
            {
                // Get background brush.
                hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);
                hr = pRenderTarget->CreateSolidColorBrush(screenColor, &screenBrush);
            }
         }

        if (SUCCEEDED(hr))
        {

            //
            // All drawing for one frame should start here
            //

            HDC hdc;
            
            hdc = BeginPaint(m, & ps);
            pRenderTarget->BeginDraw();

            pRenderTarget->Clear(color);

            // Holds menu results
            int tempInt = -1;

            // Cycle UI objects
            // Title
            titleBox.cycleText(0.1, 0.1, 0.1, hr, pRenderTarget);
            // Translation Stream Box
            tranStreamBorder.cycleRectangle(0.1, 0.1, 0.1, hr, pRenderTarget);
            tranStreamLabel.cycleText(0.1, 0.1, 0.1, hr, pRenderTarget);
            // Input Source Box
            inputStreamBorder.cycleRectangle(0.1, 0.1, 0.1, hr, pRenderTarget);
            inputStreamLabel.cycleText(0.1, 0.1, 0.1, hr, pRenderTarget);
            tempInt = inputStreamDrop.cycleBox(mouseX, mouseY, clicked, hr, pRenderTarget);
            if (tempInt != -1)
            {
                // Make a clipboard or file input stream
                // Dynamic memory object life and deletion is handled by list
                // after creation here.
                if (tempInt == 0)
                {
                    // Pass new clipboard object to be taken by list.
                    Clipboard_Input *passClip = new Clipboard_Input();
                    translationList->setInStream(*passClip);
                }
                else
                {
                    // Pass new clipboard object to be taken by list.
                    File_Input* passFile = new File_Input();
                    translationList->setInStream(*passFile);
                }
            }
            // Output Box
            outputStreamBorder.cycleRectangle(0.1, 0.1, 0.1, hr, pRenderTarget);
            // Output Location Section
            outputLocationLabel.cycleText(0.1, 0.1, 0.1, hr, pRenderTarget);
            if (buttonSetRadio.cycleButton(mouseX, mouseY, clicked, hr, pRenderTarget))
            {
                // Untoggle mutually exclusive button
                fixedSetRadio.toggled = false;
            }
            buttonSetLabel.cycleText(0.1, 0.1, 0.1, hr, pRenderTarget);
            if (fixedSetRadio.cycleButton(mouseX, mouseY, clicked, hr, pRenderTarget))
            {
                // Untoggle mutually exclusive button
                buttonSetRadio.toggled = false;

                // In reality it sets it to "no" hotkey
                translationList->setHotkey('`');

                // Have coordinates of focused stream be locked to text entry
                translationList->setCoords(tempTopX, tempTopY, tempBottomX, 
                    tempBottomY, focus);
            }
            fixedSetLabel.cycleText(0.1, 0.1, 0.1, hr, pRenderTarget);
            fixedTLXLabel.cycleText(0.1, 0.1, 0.1, hr, pRenderTarget);
            fixedBRXLabel.cycleText(0.1, 0.1, 0.1, hr, pRenderTarget);
            // Update stored value for the text entry coordinate boxes
            // and convert them to a 0-1 scale
            tempTopX = float(topLXEntry.cycleBox(keyPressed, mouseX, mouseY, 
                clicked, hr, pRenderTarget)) / overlayWidth;
            tempBottomX = float(bottomRXEntry.cycleBox(keyPressed, mouseX, 
                mouseY, clicked, hr, pRenderTarget)) / overlayWidth;
            fixedTLYLabel.cycleText(0.1, 0.1, 0.1, hr, pRenderTarget);
            fixedBRYLabel.cycleText(0.1, 0.1, 0.1, hr, pRenderTarget);
            tempTopY = float(topLYEntry.cycleBox(keyPressed, mouseX, mouseY, 
                clicked, hr, pRenderTarget)) / overlayHeight;
            tempBottomY = float(bottomRYEntry.cycleBox(keyPressed, mouseX, 
                mouseY, clicked, hr, pRenderTarget)) / overlayHeight;
            // Output Method Section
            outputMethodLabel.cycleText(0.1, 0.1, 0.1, hr, pRenderTarget);
            tempInt = outputStreamDrop.cycleBox(mouseX, mouseY, clicked, hr, 
                pRenderTarget);
            if (tempInt != -1)
            {
                // Make a corresponding output stream
                // Dynamic memory object life and deletion is handled by list
                // after creation here.
                if (tempInt == 0)
                {
                    // Pass new transparent output object to be taken by list.
                    Integration_Output* passTransparent = new 
                        Integration_Output(m, uMsg, wParam, lParam);
                    translationList->setOutStream(*passTransparent);
                }
                else if (tempInt == 1)
                {
                    // Pass new solid output object to be taken by list.
                    Solid_Output* passSolid = new Solid_Output(
                        m, uMsg, wParam, lParam);
                    translationList->setOutStream(*passSolid);
                }
                else if (tempInt == 2)
                {
                    // Pass new image output object to be taken by list.
                    Solid_Output* passImage = new Solid_Output(
                        m, uMsg, wParam, lParam);
                    translationList->setOutStream(*passImage);
                }
                else if (tempInt == 3)
                {
                    // Pass new horizontal stretch output 
                    // object to be taken by list.
                    Horizontal_Output* passHorizontal = new Horizontal_Output(
                        m, uMsg, wParam, lParam);
                    translationList->setOutStream(*passHorizontal);
                }
                else
                {
                    // Pass new vertical stretch output 
                    // object to be taken by list.
                    Vertical_Output* passVertical = new Vertical_Output(
                        m, uMsg, wParam, lParam);
                    translationList->setOutStream(*passVertical);
                }
            }
            // Bottom Left Logo Label
            logoLabel.cycleText(0.1, 0.1, 0.1, hr, pRenderTarget);
            // Order Dependent Section
            // These items are on a section of the layout in which it matters
            // which order they are drawn in to prevent bugs. As such,
            // they are down here rather than in their logical section
            // above.
            tempInt = tranStreamDrop.cycleBox(mouseX, mouseY, clicked, hr, 
                pRenderTarget);
            // Change menu focuses if nondefault dropdown return (not -1)
            if (tempInt != -1)
            {
                focus = tempInt;
                translationList->setFocus(tempInt);
            }
            tempInt = buttonSetDrop.cycleBox(mouseX, mouseY, clicked, hr, 
                pRenderTarget);
            if (tempInt != -1)
            {
                // Sets hotkeys and toggles radio buttons
                fixedSetRadio.toggled = false;
                buttonSetRadio.toggled = true;

                // Switch would be better, but the compiler won't let me 
                // nest one for some reason. It's just 8 items though.
                // See dropDown creation for hotkey translations
                if (tempInt == 0)
                {
                    translationList->setHotkey('q');
                }
                else if (tempInt == 1)
                {
                    translationList->setHotkey('w');
                }
                else if (tempInt == 2)
                {
                    translationList->setHotkey('u');
                }
                else if (tempInt == 3)
                {
                    translationList->setHotkey('r');
                }
                else if (tempInt == 4)
                {
                    translationList->setHotkey('t');
                }
                else if (tempInt == 5)
                {
                    translationList->setHotkey('y');
                }
                else
                {
                    translationList->setHotkey('e');
                }
            }
            

            hr = pRenderTarget->EndDraw();

            // Make sure to resize objects
            if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
            {
                if (*&pRenderTarget)
                {
                    (*&pRenderTarget)->Release();
                    *&pRenderTarget = NULL;
                }
                if (*&pBrush)
                {
                    (*&pBrush)->Release();
                    *&pBrush = NULL;
                }
            }
            EndPaint(m, &ps);
            
            
            
            // Start drawing to screen overlay
            hdc = BeginPaint(screenWindow, &screenPaintStruct);
            screenRenderTarget->BeginDraw();
            screenRenderTarget->Clear(screenColor);
            //testOverlay2.cycleRectangle(0.0, 1.0, 1.0, hr, screenRenderTarget);
            //testOverlay.cycleText("Can you just make this easy for me and show up nice and easy.", hr, screenRenderTarget);
            translationList->cycleStreams(keyPressed, overlayX, overlayY, hr, screenRenderTarget);

            // Make sure to resize objects
            if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
            {
                if (*&pRenderTarget)
                {
                    (*&pRenderTarget)->Release();
                    *&pRenderTarget = NULL;
                }
                if (*&pBrush)
                {
                    (*&pBrush)->Release();
                    *&pBrush = NULL;
                }
            }

            // End screen overlay drawing
            hr = screenRenderTarget->EndDraw();
            EndPaint(screenWindow, &screenPaintStruct);
            
            //
            // All drawing should end here
            //

            // Reset status of controls and check again
            clicked = false;
            keyDown = false;
            keyPressed = 'a'; // Arbitrary noninput key
        }
    }
    return DefWindowProc(m, uMsg, wParam, lParam);
}

bool MTILUIProgram::ProcessMessages()
{
    MSG msg = {};


    while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return false;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return true;
}

BOOL MTILUIProgram::Create(
    PCWSTR lpWindowName,
    DWORD dwStyle,
    DWORD dwExStyle,
    int x,
    int y,
    int nWidth,
    int nHeight,
    HWND hWndParent,
    HMENU hMenu
)
{

    wc.lpfnWndProc = MTILUIProgram::WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"Machine Translation Integration Layer";

    screenWindowClass.hInstance = GetModuleHandle(NULL);
    screenWindowClass.lpszClassName = L"ChildClassW";
    screenWindowClass.lpfnWndProc = MTILUIProgram::WindowProcChild;

    // Set application icon
    wc.hIcon = (HICON)LoadImage(
        // Loading from image file, not resource file
        NULL, L"Labyrinth.ico", IMAGE_ICON,
        // Length and width
        256, 256,
        // Using a file, not a resource file, it could be an ico
        // derived from a transparent png, it should be the normal size
        // and it should close with the window without issue.
        LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED | LR_LOADTRANSPARENT
    );

    // Register the window class.
    // Note: class here refers to the windows.h
    // window class meaning, not the traditional C++
    // class meaning.
    RegisterClass(&wc);

    if (RegisterClass(&screenWindowClass) == 0)
    {
        std::cout << "Error registering overlay class." << std::endl;
    }

    // See: https://docs.microsoft.com/en-us/windows/
    //      win32/api/winuser/nf-winuser-createwindowexa
    // for more information on these parameters
    m = CreateWindowEx(
        //WS_EX_LAYERED |
        //WS_EX_TRANSPARENT | // Don't hittest this window
        WS_EX_TOPMOST | 
        //WS_EX_TOOLWINDOW | 
        dwExStyle,                          // Default exWindow style
        wc.lpszClassName,                   // Window class name
        lpWindowName,                       // Window name, using default
        dwStyle | 
        //WS_POPUP | 
        WS_VISIBLE |
        NULL
        ,                            // Windwo style, using default
        x, y,                               // Default screen coordinates            
        nWidth, nHeight,                    // Default window dimensions
        hWndParent, hMenu, GetModuleHandle(NULL), this
    );

    /*
    SetLayeredWindowAttributes(
        m,
        RGB(0, 0, 0),
        byte(255),
        LWA_ALPHA |
        LWA_COLORKEY
    );
    */
    
    MoveWindow(m,
        0, 0,                   // Top Left of screen
        800,    //
        600,
        TRUE);
    
    /*
    SetWindowLong(m, GWL_STYLE, 1);  // Without 1 point border = white rectangle 
    SetWindowPos(m, 0, 0, 0, 800, 600, SWP_FRAMECHANGED);
    
    ShowWindow(m, SW_SHOW);
    */



    /*
    screenWindow = CreateWindowEx(
        //WS_EX_LAYERED | // Layered Windows
        //WS_EX_TRANSPARENT | // Don't hittest this window
        //WS_EX_TOPMOST | WS_EX_TOOLWINDOW
        dwExStyle,
        L"MTIL Layer", L"MTIL Layer", WS_POPUP | WS_VISIBLE, 0,
        0, 800, 600, m, hMenu, GetModuleHandle(NULL), this
    );*/


    // Get the identifier for the whole screen
    systemContext = GetDC(NULL);
    systemHandle = GetDesktopWindow();

    //Adjust overlay window size to whole screen and show
    RECT screenRect;
    GetClientRect(systemHandle, &screenRect);
    
    // Keep a small border outside of the overlay to prevent
    // occasional issues
    int border = -10;
    int leftFactor = 1;
    int rightFactor = 1;
    int upFactor = 4;
    int downFactor = 2;

    
    //border = 0;


    screenWindow = CreateWindowEx(
        WS_EX_LAYERED | // Layered Windows
        WS_EX_TRANSPARENT | // Don't hittest this window
        WS_EX_TOPMOST |
        WS_EX_TOOLWINDOW 
        | dwExStyle
        ,screenWindowClass.lpszClassName,
        lpWindowName,
        dwStyle | 
        WS_VISIBLE | NULL,
        2 * border, 2 * border, 
        screenRect.right - 1 * border, screenRect.bottom - 1 * border,
        hWndParent, hMenu,
        GetModuleHandle(NULL),
        this);

    // Enforce proper overview size and transparency outside
    // of overlay effects.
    SetLayeredWindowAttributes(
        screenWindow,
        // This function works similar to a greenscreen.
        // All pixels of the below color will become transparent
        // on the overlay window.
        RGB(0, 0, 0),
        byte(200),
        LWA_ALPHA |
        LWA_COLORKEY
    );
    SetWindowLong(screenWindow, GWL_STYLE, 0);  
    MoveWindow(screenWindow,
        leftFactor * border, upFactor * border, // Almost top Left of screen
        screenRect.right - rightFactor * border,// Almost bottom right
        screenRect.bottom -  downFactor * border,
        TRUE);
    SetWindowPos(screenWindow, 0, leftFactor * border, upFactor * border, 
        screenRect.right - rightFactor * border, screenRect.bottom - 
        downFactor * border, SWP_FRAMECHANGED);
    

    ShowWindow(screenWindow, SW_SHOW);


    DrawIcon(hdc, 10, 20, wc.hIcon);

    return (m ? TRUE : FALSE);
}

void MTILUIProgram::runMTIL()
{
    //Create a window if no errors
    if (!this->Create(L"Machine Translation Integration Layer", WS_OVERLAPPEDWINDOW))
    {
        std::cout << "Error creating window" << std::endl;
        return;
    }
    else
    {
        // Second argument to this function seems to
        // mostly be antiquated these days,
        // SW_NORMAL just uses an OS default value.
        ShowWindow(m, SW_NORMAL);

        ShowWindow(screenWindow, SW_SHOW);

    }

    hdc = GetDC(m);
    POINT overlayMouseCoords;

    // Launch a thread to always listen for 
    // key/mouse
    //std::thread inOutThread(&handleInOut);
    std::thread inOutThread(&MTILUIProgram::handleInOut, this);

    // Measure overlay size
    RECT rc;
    GetClientRect(screenWindow, &rc);
    overlayWidth = rc.right - rc.left;
    overlayHeight = rc.bottom - rc.top;
    // Prevent possible / 0 bugs
    if (overlayWidth < 1 || overlayHeight < 1)
    {
        overlayWidth = overlayHeight = 1;
    }

    // Main event loop
    running = true;
    while (running)
    {
        // Update the mouse coordinate tracker for the overlay
        // since the other tracker only handles for the window
        GetCursorPos(&overlayMouseCoords);
        //std::cout << "Overlay: " << overlayMouseCoords.x << " " << overlayMouseCoords.y << std::endl;
        ScreenToClient(screenWindow, &overlayMouseCoords);
        //std::cout << "Screen: " << overlayMouseCoords.x << " " << overlayMouseCoords.y << std::endl;

        overlayX = double(overlayMouseCoords.x) / overlayWidth;
        overlayY = double(overlayMouseCoords.y) / overlayHeight;

        //std::cout << "Adjusted: " << overlayX << " " << overlayY << std::endl;

        // Update application icon
        //DrawIcon(hdc, 10, 20, wc.hIcon);


        if (!ProcessMessages())
        {
            running = false;
        }



        // Set the window to ready to draw for the next frame.
        RedrawWindow(m, NULL, NULL, RDW_INTERNALPAINT);
        RedrawWindow(screenWindow, NULL, NULL, RDW_INTERNALPAINT);




        PostMessage(m, WM_KEYDOWN, 0u, 0u);
        // Cap framerate at around 10 fps for performance.
        Sleep(0.1);
    }

    inOutThread.join();
}

void MTILUIProgram::resizeAll(int& inX, int& inY)
{
    
}

MTILUIProgram::~MTILUIProgram()
{
    delete translationList;
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