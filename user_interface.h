//////////////////////////////////////////////////////////////////////////////
// 
// Author:           Caleb Sneath, Parker Hagmaier, MD Abubakkar
// Email:            ansengor@yahoo.com
// Label:            MTIL
// Course:           CMPS 5153
// Semester:         Spring 2022
// Date:             March 15, 2022
// File Description: This file contains the header information for the 
//                   classes necessary for the various elements inside 
//                   a user interface.
//                   The user interface classes on this page should be
//                   so general that they can be used in many other programs.
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include "user_interface_primitives.h"
#include "integration_inputs.h"
#include "integration_outputs.h"

#include <Windows.h>

//#include "basewin.h"
#include <d2d1.h>

// Sometimes uncommenting one of these fixes user's include issues
// for direct2D.
//#pragma comment(lib,"d2d1.lib")
//#pragma comment(lib, "d2d1")





//////////////////////////////////////////////////////////////////////////////
//                             Normal Classes
//////////////////////////////////////////////////////////////////////////////

class TranslationStreamList
{
public:
    TranslationStreamList();
    TranslationStreamList(HWND inHandle, UINT inMessage, WPARAM inWParam,
        LPARAM inLongParam);
    void cycleStreams(char inputKey, float inX,
        float inY, HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget);
    void setCoords(float inTopX, float inTopY, float inBottomX, float inBottomY, int stream);
    void setHotkey(char inText);
    void setFocus(const int inFocus);
    void setInStream(Integration_Input& inS);
    void setOutStream(Integration_Output& outS);
    ~TranslationStreamList();
private:
    Integration_Input* inputsArray[8];
    Integration_Output* outputsArray[8];
    char hDtKeys[8];
    bool active[8];
    int focus = 0;
    std::string buffer = "";
    float bufferTopX = -1;
    float bufferTopY = -1;
    float bufferBottomX = -1;
    float bufferBottomY = -1;
    

    // Window variables
    HWND hWnd;
    UINT uMsg;
    WPARAM wParam;
    LPARAM lParam;
};

class MTILUIProgram
{
    public:
        MTILUIProgram() : m(NULL), screenWindow(NULL) { };
        void runMTIL();
        ~MTILUIProgram();

        BOOL Create(
            PCWSTR lpWindowName,
            DWORD dwStyle,
            DWORD dwExStyle = 0,
            int x = CW_USEDEFAULT,
            int y = CW_USEDEFAULT,
            int nWidth = CW_USEDEFAULT,
            int nHeight = CW_USEDEFAULT,
            HWND hWndParent = 0,
            HMENU hMenu = 0
        );
        bool ProcessMessages();
        LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
        static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, 
            WPARAM wParam, LPARAM lParam);
        static LRESULT CALLBACK WindowProcChild(HWND hWnd, UINT uMsg,
            WPARAM wParam, LPARAM lParam);
    private: 
        bool running = true;
        // Determines which stream is selected in the GUI
        int focus = 0;

        // Overlay size variables
        // Absolute pixel values (integers)
        int overlayWidth = 0;
        int overlayHeight = 0;

        // Converted coordinate entry numbers
        // Uses a 0 - 1 scale.
        float tempTopX = 0;
        float tempTopY = 0;
        float tempBottomX = 0;
        float tempBottomY = 0;

        // Mouse Coordinates for overlay
        // Uses a 0 - 1 scale
        float overlayX = 0;
        float overlayY = 0;

        
        void handleInOut();

        // UI Window Variables
        WNDCLASS wc = { 0 };
        HWND m;
        char keyPressed = 'a';
        bool keyDown = false;
        int mouseX;
        int mouseY;
        bool clicked;
        LPARAM programLongParam;
        void resizeAll(int &inX, int &inY);
        // Any internal implementation details

        // Screen Overlay Window Variables
        RECT screenRect;
        WNDCLASS screenWindowClass = { 0 };
        HWND screenWindow;
        HDC systemContext;
        HWND systemHandle;
        HDC hdc;

        // UI Objects
        // Title
        TextBox titleBox;
        // Translation Stream Box
        RectangularBorder tranStreamBorder;
        TextBox tranStreamLabel;
        Dropdown tranStreamDrop;
        // Translation Stream Box
        RectangularBorder inputStreamBorder;
        TextBox inputStreamLabel;
        Dropdown inputStreamDrop;
        // Output Stream Box
        RectangularBorder outputStreamBorder;
        TextBox outputLocationLabel;
        ToggleButton buttonSetRadio;
        TextBox buttonSetLabel;
        Dropdown buttonSetDrop;
        ToggleButton fixedSetRadio;
        TextBox fixedSetLabel;
        TextBox fixedTLXLabel;
        TextBox fixedBRXLabel;
        TextEnterInt topLXEntry;
        TextEnterInt bottomRXEntry;
        TextBox fixedTLYLabel;
        TextBox fixedBRYLabel;
        TextEnterInt topLYEntry;
        TextEnterInt bottomRYEntry;
        TextBox outputMethodLabel;
        Dropdown outputStreamDrop;
        TextBox logoLabel;


        // Translation Stream List Object
        TranslationStreamList* translationList;
};




