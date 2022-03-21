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

// Ensures UNICODE precompiler directive is activated
#ifdef _UNICODE
#ifndef UNICODE
#define UNICODE
#endif
#endif
#ifndef UNICODE
#define UNICODE
#endif

#include "user_interface.h"
#include "integration_inputs.h"
#include "integration_outputs.h"
#include <iostream>

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

// Sometimes uncommenting one of these fixes user's include issues
// for direct2D.
//#pragma comment(lib,"d2d1.lib")
//#pragma comment(lib, "d2d1")

#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <Unknwn.h>

//////////////////////////////////////////////////////////////////////////////
//                             Parent Classes
//////////////////////////////////////////////////////////////////////////////

// Circle
Circle::Circle()
{
    // Only exists to be replaced by calling below constructor.
}
Circle::Circle(HWND inHandle, UINT inMessage, WPARAM inWidthParam, 
    LPARAM inLengthParam)
{
    hWnd = inHandle;
    uMsg = inMessage;
    wParam = inWidthParam;
    lParam = inLengthParam;
}
inline void Circle::setCoords(float inX, float inY, float inRad)
{
    centerX = inX;
    centerY = inY;
    radius = inRad;
}
inline void Circle::setScale(int inWidth, int inHeight)
{
    xScale = inWidth;
    yScale = inHeight;
}
inline void Circle::cycleCircle(float rValue, float gValue, float bValue, HRESULT &hr,
    ID2D1HwndRenderTarget*& pRenderTarget)
{
    D2D1_ELLIPSE ellipse;

    static ID2D1SolidColorBrush* pBrush = NULL;
    hr = S_OK;
    const D2D1_COLOR_F color = D2D1::ColorF(rValue, gValue, bValue);
    hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);

    D2D1_SIZE_F size = pRenderTarget->GetSize();
    const float x = size.width * centerX;
    const float y = size.height * centerY;
    const float adjustedRadius = min(x, y) * radius;
    ellipse = D2D1::Ellipse(D2D1::Point2F(x, y),
        adjustedRadius, adjustedRadius);

    if (SUCCEEDED(hr))
    {
        pRenderTarget->FillEllipse(ellipse, pBrush);
    }
}


// Rectangle
RectangleShape::RectangleShape()
{
    // Only exists to be replaced by calling below constructor.
}
RectangleShape::RectangleShape(HWND inHandle, UINT inMessage, WPARAM inWidthParam,
    LPARAM inLengthParam)
{
    hWnd = inHandle;
    uMsg = inMessage;
    wParam = inWidthParam;
    lParam = inLengthParam;
}
inline void RectangleShape::setCoords(float inTopX, float inTopY, float inBottomX, float inBottomY)
{
    topX = inTopX;
    topY = inTopY;
    bottomX = inBottomX;
    bottomY = inBottomY;
}

// TODO: Maybe a shape class would have been better if this is copied
//       It's just one function though, probably not worth an extra
//       parent and changing plans. Discuss later.
inline void RectangleShape::setScale(int inWidth, int inHeight)
{
    xScale = inWidth;
    yScale = inHeight;
}
inline void RectangleShape::cycleRectangle(float rValue, float gValue, float bValue, 
    HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget)
{
    D2D1_RECT_F thisRect;


    static ID2D1SolidColorBrush* pBrush = NULL;
    hr = S_OK;
    D2D1_SIZE_F size = pRenderTarget->GetSize();
    const float adjustedTopX = size.width * topX;
    const float adjustedTopY = size.height * topX;
    const float adjustedBottomX = size.width * bottomX;
    const float adjustedBottomY = size.height * bottomY;
    thisRect = D2D1::RectF(adjustedTopX, adjustedTopY, 
        adjustedBottomX, adjustedBottomY);
    const D2D1_COLOR_F color = D2D1::ColorF(rValue, gValue, bValue);
    hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);

   

    if (SUCCEEDED(hr))
    {
        pRenderTarget->FillRectangle(thisRect, pBrush);
    }
}

//////////////////////////////////////////////////////////////////////////////
//                             Normal Classes
//////////////////////////////////////////////////////////////////////////////

// TranslationStreamList

TranslationStreamList::TranslationStreamList()
{

}
inline void TranslationStreamList::cycleStreams(int inX, int inY)
{

}
inline void TranslationStreamList::setCoords(float inX, float inY, int inHeight, 
    float inWidth, int stream)
{

}
inline void TranslationStreamList::setText(std::string inText, int stream)
{

}
inline void TranslationStreamList::setInStream(Integration_Input *inS, int stream)
{

}
inline void TranslationStreamList::setOutStream(Integration_Output *outS, int stream)
{

}

// MTILUIProgram

inline LRESULT CALLBACK MTILUIProgram::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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


inline LRESULT MTILUIProgram::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

        //Initialize UI objects
        testCircle = Circle(m, uMsg, wParam, lParam);
        testCircle.setCoords(0.8, 0.8, 0.1);
        testRectangle = RectangleShape(m, uMsg, wParam, lParam);
        testRectangle.setCoords(0.2, 0.2, 0.4, 0.4);
        testButton = ToggleButton(m, uMsg, wParam, lParam);
        testButton.setCoords(0.5, 0.5, 0.2);

        break;
    case WM_CLOSE:
        DestroyWindow(m);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
        PAINTSTRUCT ps;

        // Set up Direct 2D factory and other one time only things
        static ID2D1Factory* pFactory = NULL;
        if (FAILED(D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
        {
            // Fail drawing totally, exit.
            return DefWindowProc(m, uMsg, wParam, lParam);;  
        }
        static ID2D1HwndRenderTarget* pRenderTarget = NULL;
        static ID2D1SolidColorBrush* pBrush = NULL;
        const D2D1_COLOR_F color = D2D1::ColorF(1.0, 0.1, 0.1);
        // Get the render target running as well as the background brush
        HRESULT hr = S_OK;
        if (pRenderTarget == NULL)
        {
            RECT rc;
            GetClientRect(m, &rc);

            D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

            hr = pFactory->CreateHwndRenderTarget(
                D2D1::RenderTargetProperties(),
                D2D1::HwndRenderTargetProperties(m, size),
                &pRenderTarget);

            if (SUCCEEDED(hr))
            {
                // Get background brush.
                hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);
            }
           }

        if (SUCCEEDED(hr))
        {


            // All drawing for one frame should start here
            HDC hdc = BeginPaint(m, & ps);
            pRenderTarget->BeginDraw();

            pRenderTarget->Clear(color);
            testCircle.cycleCircle(0.0, 1.0, 1.0, hr, pRenderTarget);
            testRectangle.cycleRectangle(0.0, 1.0, 1.0, hr, pRenderTarget);
            testButton.cycleButton(mouseX, mouseY, clicked, hr, pRenderTarget);
            //pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
            //pRenderTarget->FillRectangle(thisRect, pBrush);

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
        }
    }
    return DefWindowProc(m, uMsg, wParam, lParam);
}

inline bool MTILUIProgram::ProcessMessages()
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

inline BOOL MTILUIProgram::Create(
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

    // See: https://docs.microsoft.com/en-us/windows/
    //      win32/api/winuser/nf-winuser-createwindowexa
    // for more information on these parameters
    m = CreateWindowEx(
        dwExStyle,                          // Default exWindow style
        wc.lpszClassName,                   // Window class name
        lpWindowName,                       // Window name, using default
        dwStyle,                            // Windwo style, using default
        x, y,                               // Default screen coordinates            
        nWidth, nHeight,                    // Default window dimensions
        hWndParent, hMenu, GetModuleHandle(NULL), this
    );

    HDC hdc = GetDC(m);
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
    }

    HDC hdc = GetDC(m);

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

        //Cap framerate at 10 fps, we're showing text, not fancy animations.
        Sleep(0.1);

        // Set the window to ready to draw for the next frame.
        RedrawWindow(m, NULL, NULL, RDW_INTERNALPAINT);

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

inline void MTILUIProgram::resizeAll(int& inX, int& inY)
{
    
}

MTILUIProgram::~MTILUIProgram()
{

}

//////////////////////////////////////////////////////////////////////////////
//                             Child Classes
//////////////////////////////////////////////////////////////////////////////

// ToggleButton
inline bool ToggleButton::cycleButton(int inX, int inY, bool inClicked,
    HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget)
{
    D2D1_SIZE_F size = pRenderTarget->GetSize();
    const float adjustedRadius = min(size.width, size.height) * radius / 2;
    float useRadius;
    

    // Show either onClick state, or toggle state if clicked

    if (boundsCheck(inX, inY, size))
    {
        // Show onClick
        if (inClicked == true)
        {
            
            this->cycleCircle(0.5, 0.5, 0.8, hr, pRenderTarget);
            toggled = true;
        }
        // Show onHover
        else
        {
            float tempRad = radius;
            this->cycleCircle(0.7, 0.7, 0.7, hr, pRenderTarget);
            radius = radius * 0.67;
            this->cycleCircle(0.75, 0.75, 0.75, hr, pRenderTarget);
            radius = tempRad;
        }
    }
    else
    {
        // Show toggled
        if (toggled == true)
        {
            float tempRad = radius;
            this->cycleCircle(0.8, 0.8, 0.8, hr, pRenderTarget);
            radius = radius * 0.67;
            this->cycleCircle(0.4, 0.4, 0.4, hr, pRenderTarget);
            radius = tempRad;
        }
        // Show default
        else
        {
            float tempRad = radius;
            this->cycleCircle(0.8, 0.8, 0.8, hr, pRenderTarget);
            radius = radius * 0.67;
            this->cycleCircle(0.75, 0.75, 0.75, hr, pRenderTarget);
            radius = tempRad;
        }
    }

    return toggled;
}
inline bool ToggleButton::boundsCheck(const int& inX, const int& inY, 
    D2D1_SIZE_F size)
{
    const float adjustedRadius = min(size.width, size.height) * radius / 2;
    float useRadius;
    // Check cursor is within upper and lower bounds for X and Y
    if (((size.width * centerX - adjustedRadius) < inX) && 
        (((size.width * centerX) + adjustedRadius) > inX) && 
        (((size.height * centerY) - adjustedRadius) < inY) && 
        (((size.height * centerY) + adjustedRadius) > inY))
    {
        return true;
    }
    else
    {
        return false;
    }
}

// TextBox
inline void TextBox::setText(std::string inText)
{
    text = inText;
}
inline void TextBox::cycleText(HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget) 
{
    /*
    ID2D1Factory* pD2DFactory;
    hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        &pD2DFactory
    );
    IDWriteFactory* pDWriteFactory = nullptr;

    static const WCHAR msc_fontName[] = L"Verdana";
    static const FLOAT msc_fontSize = 50;
    ID2D1SolidColorBrush* textBoxBlackBrush = nullptr;
    IDWriteTextFormat* textBoxTextFormat = nullptr;

    // Create a text factory at most once per program per class.
    // For details see:
    //      https://docs.microsoft.com/en-us/windows/win32/direct2d/
    //      how-to--draw-text
    if (SUCCEEDED(hr))
    {
        hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(&pDWriteFactory));
    }
    if (SUCCEEDED(hr))
    {
        // Create a DirectWrite text format object.
        hr = pDWriteFactory->CreateTextFormat(
            msc_fontName,
            NULL,
            DWRITE_FONT_WEIGHT_REGULAR,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            msc_fontSize,
            L"en-us", //locale
            &textBoxTextFormat
        );
    }
    if (SUCCEEDED(hr))
    {
        textBoxTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
        textBoxTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    }


    //TODO: Replace with text to WCHAR
    WCHAR textBoxWCHAR[] = L"Testing";

    // Retrieve the size of the render target.
    D2D1_SIZE_F renderTargetSize = pRenderTarget->GetSize();


    pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());


    pRenderTarget->DrawText(
        textBoxWCHAR,
        ARRAYSIZE(textBoxWCHAR) - 1,
        textBoxTextFormat,
        D2D1::RectF(0, 0, renderTargetSize.width, renderTargetSize.height),
        textBoxBlackBrush
    );
    */

}

// TextEnterInt

inline void TextEnterInt::cycleText(HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget)
{

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