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
#include <vector>

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
    void cycleCircle(float rValue, float gValue, float bValue, HRESULT& hr,
        ID2D1HwndRenderTarget*& pRenderTarget);
protected:
    HWND localHandle;
    float centerX = 0;
    float centerY = 0;
    float radius = 0;
    int xScale = 0;
    int yScale = 0;

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
        RectangleShape(HWND inHandle, UINT inMessage, WPARAM inWParam,
            LPARAM inLongParam);
        void setCoords(float inTopX, float inTopY, float inBottomX, float inBottomY);
        void setScale(int inWidth, int inHeight);
        void cycleRectangle(float rValue, float gValue, float bValue, HRESULT& hr,
            ID2D1HwndRenderTarget*& pRenderTarget);
    protected:
        HWND localHandle;
        float topX = 0;
        float topY = 0;
        float bottomX = 0;
        float bottomY = 0;
        int xScale = 0;
        int yScale = 0;

        HWND hWnd;
        UINT uMsg;
        WPARAM wParam;
        LPARAM lParam;
};


//////////////////////////////////////////////////////////////////////////////
//                             Child Classes
//////////////////////////////////////////////////////////////////////////////

class RectangularBorder :public RectangleShape
{
    public:
        RectangularBorder();// Mandatory.
        RectangularBorder(HWND inHandle, UINT inMessage, WPARAM inWParam,
            LPARAM inLongParam);
        void cycleRectangle(float rValue, float gValue, float bValue, HRESULT& hr,
            ID2D1HwndRenderTarget*& pRenderTarget);
};

class ToggleButton : public Circle
{
    public:
        ToggleButton();// Mandatory.
        ToggleButton(HWND inHandle, UINT inMessage, WPARAM inWidthParam,
            LPARAM inLengthParam);
        bool cycleButton(int inX, int inY, bool inClicked, HRESULT& hr,
            ID2D1HwndRenderTarget*& pRenderTarget);
        bool toggled = false;
    private:
        bool boundsCheck(const int& inX, const int& inY, D2D1_SIZE_F size);
};

class TextBox : public RectangleShape
{
    public:
        TextBox();// Mandatory.
        TextBox(HWND inHandle, UINT inMessage, WPARAM inWParam,
            LPARAM inLongParam);
        void cycleText(float rValue, float gValue, float bValue,
            HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget);
        void setText(std::string inText);
    protected:
        std::string text = "";
};

class TextEnterInt : public TextBox
{
    public:
        TextEnterInt();// Mandatory.
        TextEnterInt(HWND inHandle, UINT inMessage, WPARAM inWParam,
            LPARAM inLongParam);
        int cycleBox(std::string inputKey, int inX, int inY, bool inClicked,
            HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget);
        void setText(std::string inText);
    protected:
        bool boundsCheck(const int& inX, const int& inY, D2D1_SIZE_F size);
        void processKey(std::string inputKey);
        bool active = false;
        bool clicked = false;
        std::string buffer;
};

class Dropdown : public TextBox
{
public:
    Dropdown();// Mandatory.
    Dropdown(HWND inHandle, UINT inMessage, WPARAM inWParam,
        LPARAM inLongParam);
    int cycleBox(int inX, int inY, bool inClicked,
        HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget);
    void appendItem(std::string newItem);
protected:
    int boundsCheck(const int& inX, const int& inY, D2D1_SIZE_F size);
    int focus = 0;
    bool active = false;
    bool clicked = false;
    bool collapseWait = false;
    std::vector<std::string> prompts;
};