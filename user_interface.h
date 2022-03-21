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
//                             Parent Classes
//////////////////////////////////////////////////////////////////////////////

// Parent shape classes, such as shapes for the UI elements

// Class:   Circle
// Purpose: A class which can draw and store information relevant to a circle
// Usage:   Use the nondefault constructor to intitialize the circle.
//          Use setCoords with the new center and radius to change the 
//          circle to as desired. Coordinates are from 0 - 1 with 0
//          being the top left of the window and 1 being bottom right.
//          Note, radius is also 0-1, but goes with the smaller of
//          the height or width of the window.
//          Circles don't store color information,
//          their color is chosen on cycleCircle. cycleCircle draws
//          the circle and should ideally be called every time the 
//          screen is drawn once a frame inside the program control loop.
class Circle
{
    public:
        Circle();// Mandatory.
        Circle(HWND inHandle, UINT inMessage, WPARAM inWidthParam, 
            LPARAM inLengthParam);
        void setCoords(float inX, float inY, float inRad);
        void setScale(int inWidth, int inHeight);
        void cycleCircle(float rValue, float gValue, float bValue, HRESULT &hr, 
            ID2D1HwndRenderTarget* & pRenderTarget);
    protected:
        HWND localHandle;
        float centerX;
        float centerY;
        float radius;
        int xScale;
        int yScale;

        HWND hWnd;
        UINT uMsg;
        WPARAM wParam;
        LPARAM lParam;
};

// Class:   Rectangle
// Purpose: A class which can draw and store information relevant to a rect.
// Usage:   Use the nondefault constructor to intitialize the rectangle.
//          Use setCoords with the top left and bottom right coordinates of
//          the rectangle as desired. Coordinates are from 0 - 1 with 0
//          being the top left of the window and 1 being bottom right.
//          Note, radius is also 0-1, but goes with the smaller of
//          the height or width of the window.
//          Circles don't store color information,
//          their color is chosen on cycleCircle. cycleCircle draws
//          the circle and should ideally be called every time the 
//          screen is drawn once a frame inside the program control loop.
class RectangleShape
{
public:
    RectangleShape();// Mandatory.
    RectangleShape(HWND inHandle, UINT inMessage, WPARAM inWidthParam,
        LPARAM inLengthParam);
    void setCoords(float inTopX, float inTopY, float inBottomX, float inBottomY);
    void setScale(int inWidth, int inHeight);
    void cycleRectangle(float rValue, float gValue, float bValue, HRESULT &hr,
        ID2D1HwndRenderTarget*& pRenderTarget);
private:
    HWND localHandle;
    float topX;
    float topY;
    float bottomX;
    float bottomY;
    int xScale;
    int yScale;

    HWND hWnd;
    UINT uMsg;
    WPARAM wParam;
    LPARAM lParam;
};


//////////////////////////////////////////////////////////////////////////////
//                             Child Classes
//////////////////////////////////////////////////////////////////////////////

class ToggleButton : public Circle
{
public:
    // Circle's constructor is fine to inherit.
    using Circle::Circle;
    bool cycleButton(int inX, int inY, bool inClicked, HRESULT& hr,
        ID2D1HwndRenderTarget*& pRenderTarget);
    bool toggled = false;
private:
    // Grab all of Circle's functionality, but drawing a circle should
    // now be private.
    using Circle::cycleCircle;
    bool boundsCheck(const int& inX, const int& inY, D2D1_SIZE_F size);
};

class TextBox : public RectangleShape
{
public:
    // Rectangle's constructor is fine to inherit.
    using RectangleShape::RectangleShape;
    void cycleText(HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget); 
    void setText(std::string inText);
private:
    // Grab all of Circle's functionality, but drawing a circle should
    // now be private.
    using RectangleShape::cycleRectangle;
    std::string text;
};

class TextEnterInt : public TextBox
{
public:
    // TextBox's constructor is fine to inherit.
    using TextBox::TextBox;
    void cycleText(HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget);
private:
    bool active;
    bool clicked;
    std::string buffer = "";
};


//////////////////////////////////////////////////////////////////////////////
//                             Normal Classes
//////////////////////////////////////////////////////////////////////////////


class TranslationStreamList
{
public:
    TranslationStreamList();
    void cycleStreams(int inX, int inY);
    void setCoords(float inX, float inY, int inHeight, float inWidth, int stream);
    void setText(std::string inText, int stream);
    void setInStream(Integration_Input* inS, int stream);
    void setOutStream(Integration_Output* outS, int stream);
private:
    Integration_Input* inputsArray[8];
    Integration_Output* outputsArray[8];
    std::string hDtKeys [8];
    bool active [8];
    std::string buffer = "";
};

class MTILUIProgram
{
    public:
        MTILUIProgram() : m(NULL) { };
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
    private: 
        WNDCLASS wc = { 0 };
        HWND m;
        int mouseX;
        int mouseY;
        bool clicked;
        LPARAM programLongParam;
        void resizeAll(int &inX, int &inY);
        // Any internal implementation details

        // Temporary testing objects
        Circle testCircle;
        RectangleShape testRectangle;
        ToggleButton testButton;
};

