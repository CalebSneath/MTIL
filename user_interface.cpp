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
#include <math.h>

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
        //inputsArray[loopIndex] = new Integration_Output();
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
        outputsArray[loopIndex] =  new Integration_Output(hWnd, uMsg, wParam, lParam);
        active[loopIndex] = false;
    }
}
void TranslationStreamList::cycleStreams(std::string inputKey, int inX, 
    int inY, HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget)
{
    outputsArray[0]->setCoords(0.1, 0.1, 0.4, 0.4);
    outputsArray[0]->setText("Testing this");
    outputsArray[0]->cycleText("Test", hr, pRenderTarget);
}
void TranslationStreamList::setCoords(float inTopX, float inTopY, float inBottomX, 
    float inBottomY, int stream)
{
    if (stream > 8 && stream >= 0)
    {
        outputsArray[stream]->setCoords(inTopX, inTopY, inBottomX, inBottomY);
    }
}
void TranslationStreamList::setHotkey(std::string inText)
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
void TranslationStreamList::setInStream(Integration_Input* & inS)
{
    // Deallocate dynamic memory and adjust pointer. Activate stream.
    active[focus] = true;
    delete inputsArray[focus];
    inputsArray[focus] = inS;
}
void TranslationStreamList::setOutStream(Integration_Output* & outS)
{
    // Deallocate dynamic memory and adjust pointer. Activate stream.
    active[focus] = true;
    delete outputsArray[focus];
    outputsArray[focus] = outS;
}

TranslationStreamList::~TranslationStreamList()
{
    // Deallocate memory
    for (int loopIndex = 0; loopIndex < 8; loopIndex++)
    {
        delete outputsArray[loopIndex];
        //delete inputsArray[index];
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
        buttonSetDrop.appendItem("Set Button: \\");
        buttonSetDrop.appendItem("Set Button: =");
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
        topLXEntry.setCoords(0.72, 0.55, 0.76, 0.60);
        topLXEntry.setText("0");
        bottomRXEntry = TextEnterInt(m, uMsg, wParam, lParam);
        bottomRXEntry.setCoords(0.72, 0.62, 0.76, 0.67);
        bottomRXEntry.setText("0");
        fixedTLYLabel = TextBox(m, uMsg, wParam, lParam);
        fixedTLYLabel.setCoords(0.76, 0.55, 0.80, 0.60);
        fixedTLYLabel.setText("Y");
        fixedBRYLabel = TextBox(m, uMsg, wParam, lParam);
        fixedBRYLabel.setCoords(0.76, 0.62, 0.80, 0.67);
        fixedBRYLabel.setText("Y");
        topLYEntry = TextEnterInt(m, uMsg, wParam, lParam);
        topLYEntry.setCoords(0.80, 0.55, 0.84, 0.60);
        topLYEntry.setText("0");
        bottomRYEntry = TextEnterInt(m, uMsg, wParam, lParam);
        bottomRYEntry.setCoords(0.80, 0.62, 0.84, 0.67);
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
        translationList = new TranslationStreamList(screenWindow, uMsg, wParam, lParam);

        //
        // End initializing UI Elements
        //

        /*
        testCircle = Circle(m, uMsg, wParam, lParam);
        testCircle.setCoords(0.8, 0.8, 0.1);
        testRectangle = RectangleShape(m, uMsg, wParam, lParam);
        testRectangle.setCoords(0.4, 0.4, 0.6, 0.9);
        testButton = ToggleButton(m, uMsg, wParam, lParam);
        testButton.setCoords(0.5, 0.5, 0.2);

        testText = TextBox(m, uMsg, wParam, lParam);
        testText.setCoords(0.4, 0.4, 0.6, 0.9);
        testText.setText("Testing stuff");

        testNumBox = TextEnterInt(m, uMsg, wParam, lParam);
        testNumBox.setCoords(0.8, 0.1, 0.83, 0.13);

        testDrop = Dropdown(m, uMsg, wParam, lParam);
        testDrop.setCoords(0.5, 0.4, 0.7, 0.45);
        testDrop.appendItem("Test 1");
        testDrop.appendItem("Test 2");
        testDrop.appendItem("Test 3");
        testDrop.appendItem("Test 4");
        testDrop.appendItem("Test 5");
        */


        //screenRectTest = RectangleShape(systemHandle, uMsg, wParam, lParam);
        //screenRectTest.setCoords(0.2, 0.2, 0.4, 0.4);

        /*
        testOverlay = Vertical_Output(screenWindow, uMsg, wParam, lParam);
        testOverlay.setCoords(0.5, 0.5, 0.9, 0.9);
        testOverlay.setText("Testing others");
        testOverlay2 = RectangleShape(screenWindow, uMsg, wParam, lParam);
        testOverlay2.setCoords(0.3, 0.3, 0.6, 0.6);
        */

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


            // All drawing for one frame should start here
            HDC hdc;
            
            hdc = BeginPaint(m, & ps);
            pRenderTarget->BeginDraw();

            pRenderTarget->Clear(color);

            // Cycle UI objects
            // Title
            titleBox.cycleText(0.1, 0.1, 0.1, hr, pRenderTarget);
            // Translation Stream Box
            tranStreamBorder.cycleRectangle(0.1, 0.1, 0.1, hr, pRenderTarget);
            tranStreamLabel.cycleText(0.1, 0.1, 0.1, hr, pRenderTarget);
            // Input Source Box
            inputStreamBorder.cycleRectangle(0.1, 0.1, 0.1, hr, pRenderTarget);
            inputStreamLabel.cycleText(0.1, 0.1, 0.1, hr, pRenderTarget);
            inputStreamDrop.cycleBox(mouseX, mouseY, clicked, hr, pRenderTarget);
            // Output Box
            outputStreamBorder.cycleRectangle(0.1, 0.1, 0.1, hr, pRenderTarget);
            // Output Location Section
            outputLocationLabel.cycleText(0.1, 0.1, 0.1, hr, pRenderTarget);
            buttonSetRadio.cycleButton(mouseX, mouseY, clicked, hr, pRenderTarget);
            buttonSetLabel.cycleText(0.1, 0.1, 0.1, hr, pRenderTarget);
            fixedSetRadio.cycleButton(mouseX, mouseY, clicked, hr, pRenderTarget);
            fixedSetLabel.cycleText(0.1, 0.1, 0.1, hr, pRenderTarget);
            fixedTLXLabel.cycleText(0.1, 0.1, 0.1, hr, pRenderTarget);
            fixedBRXLabel.cycleText(0.1, 0.1, 0.1, hr, pRenderTarget);
            topLXEntry.cycleBox("", mouseX, mouseY, clicked, hr, pRenderTarget);
            bottomRXEntry.cycleBox("", mouseX, mouseY, clicked, hr, pRenderTarget);
            fixedTLYLabel.cycleText(0.1, 0.1, 0.1, hr, pRenderTarget);
            fixedBRYLabel.cycleText(0.1, 0.1, 0.1, hr, pRenderTarget);
            topLYEntry.cycleBox("", mouseX, mouseY, clicked, hr, pRenderTarget);
            bottomRYEntry.cycleBox("", mouseX, mouseY, clicked, hr, pRenderTarget);
            // Output Method Section
            outputMethodLabel.cycleText(0.1, 0.1, 0.1, hr, pRenderTarget);
            outputStreamDrop.cycleBox(mouseX, mouseY, clicked, hr, pRenderTarget);
            // Bottom Left Logo Label
            logoLabel.cycleText(0.1, 0.1, 0.1, hr, pRenderTarget);
            // Order Dependent Section
            // These items are on a section of the layout in which it matters
            // which order they are drawn in to prevent bugs. As such,
            // they are down here rather than in their logical section
            // above.
            tranStreamDrop.cycleBox(mouseX, mouseY, clicked, hr, pRenderTarget);
            buttonSetDrop.cycleBox(mouseX, mouseY, clicked, hr, pRenderTarget);


            /*
            testCircle.cycleCircle(0.0, 1.0, 1.0, hr, pRenderTarget);
            testRectangle.cycleRectangle(0.0, 1.0, 1.0, hr, pRenderTarget);
            //testButton.cycleButton(mouseX, mouseY, clicked, hr, pRenderTarget);
            //testText.cycleText(0.9, 0.1, 0.1, hr, pRenderTarget);
            testNumBox.cycleBox("", mouseX, mouseY, clicked, hr, pRenderTarget);
            //testNumBox.setText("125");
            //testNumBox.cycleText(0.9, 0.9, 0.1, hr, pRenderTarget);
            testDrop.cycleBox(mouseX, mouseY, clicked, hr, pRenderTarget);
            //screenRectTest.cycleRectangle(0.0, 1.0, 1.0, hr, pRenderTarget);
            //pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
            //pRenderTarget->FillRectangle(thisRect, pBrush);
            */
            

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
            translationList->cycleStreams("", mouseX, mouseY, hr, screenRenderTarget);

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

    bool running = true;

    // Main event loop
    while (running)
    {
        // Update application icon
        DrawIcon(hdc, 10, 20, wc.hIcon);

        if (!ProcessMessages())
        {
            running = false;
        }

        // Cap framerate at around 30 fps for performance, 
        // we're showing text, not fancy animations.
        Sleep(0.03);

        // Set the window to ready to draw for the next frame.
        RedrawWindow(m, NULL, NULL, RDW_INTERNALPAINT);
        RedrawWindow(screenWindow, NULL, NULL, RDW_INTERNALPAINT);

        // It'll get set to its true values from the messages
        clicked = false;
        // 0x8000 and 0x10000 are the values for 32 and 64 bit machines
        // for the left mouse being clicked. Does a bitwise
        // and and checks if its not equal to all zeros. Just
        // checking if the bitwise of 8000 is not all zeroes is probably
        // fastest and less architecture dependent.
        if ((GetKeyState(VK_LBUTTON) & 0x8000) > 0)
        {
            clicked = true;
        }
    }
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