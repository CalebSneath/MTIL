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
// Legacy Code: All but HWND inHandle is obsolete as parameters
//          in the nondefault constructor.
//          Originally, when shapes were drawn a more inefficient way
//          during early program development, these parameters had
//          a slight use. Now, after drawing (cycleX) function optimizations,
//          they are no longer used. In refactoring these parameters
//          can be removed with no effect for Circle, RectangleShape,
//          and all descendents therein, however all dependent code
//          would need to be fixed as well.
class Circle
{
public:
    Circle();// Mandatory.
    Circle(HWND inHandle, UINT inMessage, WPARAM inWidthParam,
        LPARAM inLengthParam);
    void setCoords(float inX, float inY, float inRad);
    void cycleCircle(float rValue, float gValue, float bValue, HRESULT& hr,
        ID2D1HwndRenderTarget*& pRenderTarget);
protected:
    HWND localHandle;
    float centerX = 0;
    float centerY = 0;
    float radius = 0;

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
        void cycleRectangle(float rValue, float gValue, float bValue, HRESULT& hr,
            ID2D1HwndRenderTarget*& pRenderTarget);
    protected:
        HWND localHandle;
        float topX = 0;
        float topY = 0;
        float bottomX = 0;
        float bottomY = 0;

        HWND hWnd;
        UINT uMsg;
        WPARAM wParam;
        LPARAM lParam;
};


//////////////////////////////////////////////////////////////////////////////
//                             Child Classes
//////////////////////////////////////////////////////////////////////////////

// Class:   Rectangle
// Purpose: A class which can draw and store information relevant to a rect.
// Usage:   See RectangleShape
// Inheritance Notes: Inherits from RectangleShape and differs in drawing
// only an outline of a rectangle rather than a filled one
class RectangularBorder :public RectangleShape
{
    public:
        RectangularBorder();// Mandatory.
        RectangularBorder(HWND inHandle, UINT inMessage, WPARAM inWParam,
            LPARAM inLongParam);
        void cycleRectangle(float rValue, float gValue, float bValue, HRESULT& hr,
            ID2D1HwndRenderTarget*& pRenderTarget);
};

// Class:   ToggleButton
// Purpose: A class which can respond to passed in user input 
//          and enter an activated state, sending a true boolean
//          value. Can also be manually activated or deactivated
//          by setting the public variable "toggled"
// Usage:   Use the nondefault constructor to intitialize the button.
//          Use the setCoords function with coordinates ranging
//          from 0-1 to determine where the button is centered,
//          and then the radius in the same scale. Call cycleButton.
//          If the button gets toggled on through the outside program
//          or the passed user input, it will return true, otherwise false
// Inheritance: Inherits from Circle and implements some additional
//          logic useful for UI.
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

// Class:   TextBox
// Purpose: A class which can display set text. Outputted text will
//          be centered and logarithmically scale with its display
//          area and inversely with text length. Eventually it will
//          "bleed" through the text area if pushed to too high an
//          extreme, but for almost every reasonable case it will
//          resize and wrap well enough to make use of available space.
// Usage:   Use the nondefault constructor to intitialize the text.
//          Use the setCoords function with coordinates ranging
//          from 0-1 to determine where the text is bound.
//          Use setText to change the text to be displayed.
//          Call cycleText to display the text. The first three
//          float parameters will determine red, green, and
//          blue argument on a 0-1 scale.
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

// Class:   TextEnterInt
// Purpose: A class that responds to user numeric input as well
//          as clicks, the enter key, and backspace as a usual
//          menu entry item. 
// Usage:   Use the nondefault constructor to intitialize the button.
//          Use the setCoords function with coordinates ranging
//          from 0-1 to determine where the button is bounded.
//          Use setText to choose what to display.
//          The box will return with cycleBox what it previously
//          had stored as an integer since the user entered 
//          the enter key or clicked away.
//          Note: does not prevent overflow very well
//          Note, pass keyboard entry as 'e' for enter
//          and 'b' for backspace.
class TextEnterInt : public TextBox
{
    public:
        TextEnterInt();// Mandatory.
        TextEnterInt(HWND inHandle, UINT inMessage, WPARAM inWParam,
            LPARAM inLongParam);
        int cycleBox(char inputKey, int inX, int inY, bool inClicked,
            HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget);
        void setText(std::string inText);
        bool active = false;
    protected:
        bool boundsCheck(const int& inX, const int& inY, D2D1_SIZE_F size);
        void processKey(std::string inputKey);
        bool clicked = false;
        std::string buffer;
};

// Class:   DropDown
// Purpose: A class which can respond to passed in user input 
//          and enter an activated state, expanding and collapsing
//          as similar GUI dropdown menues to send a one time
//          per selection activation signals.
// Usage:   Set similar to parent RectangleShape.
//          Add items to appear in list with appendItem(string).
//          cycleBox will return -1 if nothing is selected that cycle,
//          otherwise, it will return the index of the clicked item and
//          collapse.
class Dropdown : public TextBox
{
public:
    Dropdown();// Mandatory.
    Dropdown(HWND inHandle, UINT inMessage, WPARAM inWParam,
        LPARAM inLongParam);
    int cycleBox(int inX, int inY, bool inClicked,
        HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget);
    void appendItem(std::string newItem);
    bool active = false;
protected:
    int boundsCheck(const int& inX, const int& inY, D2D1_SIZE_F size);
    int focus = 0;
    bool clicked = false;
    bool collapseWait = false;
    std::vector<std::string> prompts;
};