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

#include "integration_outputs.h"
#include "user_interface_primitives.h"
#include <iostream>
#include <fstream>
#include <string>

// Windows Version
#define WINDOWSRELEASE
//#ifdef WINDOWSRELEASE


// Ensures UNICODE precompiler directive is activated
#ifdef _UNICODE
#ifndef UNICODE
#define UNICODE
#endif
#endif
#ifndef UNICODE
#define UNICODE
#endif


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
#pragma comment(lib, "Windowscodecs")

#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <Unknwn.h>

//////////////////////////////////////////////////////////////////////////////
//                             Absstract Classes
//////////////////////////////////////////////////////////////////////////////

//Integration_Output
Integration_Output::Integration_Output()
{
    // Only exists to be replaced by calling below constructor.

    // Grab display reading offset values from config file
    std::ifstream confFile;
    confFile.open("Output Offset.conf");
    if (confFile)
    {
        // Read and skip the label to grab X Offset
        std::string readString;
        confFile >> readString >> readString >> readString;
        if (std::isdigit((unsigned char)readString[0]))
        {
            xOffset = stoi(readString);
        }


        // Read and skip the label to grab Y Offset
        confFile >> readString >> readString >> readString;
        if (std::isdigit((unsigned char)readString[0]))
        {
            yOffset = stoi(readString);
        }
    }
}
Integration_Output::Integration_Output(HWND inHandle, UINT inMessage, WPARAM inWParam,
    LPARAM inLongParam)
{
    hWnd = inHandle;
    uMsg = inMessage;
    wParam = inWParam;
    lParam = inLongParam;

    // Grab display reading offset values from config file
    std::ifstream confFile;
    confFile.open("Output Offset.conf");
    if (confFile)
    {
        // Read and skip the label to grab X Offset
        std::string readString;
        confFile >> readString >> readString >> readString;
        if (std::isdigit((unsigned char)readString[0]))
        {
            xOffset = stoi(readString);
        }


        // Read and skip the label to grab Y Offset
        confFile >> readString >> readString >> readString;
        if (std::isdigit((unsigned char)readString[0]))
        {
            yOffset = stoi(readString);
        }
    }
}

void Integration_Output::cycleText(std::string inText,
    HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget)
{
    // One time only initializations
    static ID2D1Factory* pD2DFactory = nullptr;
    static IDWriteFactory* pDWriteFactory = nullptr;
    static const WCHAR msc_fontName[] = L"Verdana";
    static ID2D1SolidColorBrush* textBoxBrush = nullptr;
    static IDWriteTextFormat* textBoxTextFormat;

    text = inText;

    // Early exit for invalid coordinates
    if (topX == -1)
    {
        return;
    }


    // Measure various sizes.
    // Sort out what rectangular coordinates text needs
    D2D1_RECT_F thisRect;
    RECT rc;
    GetClientRect(hWnd, &rc);
    // Retrieve the size of the render target.
    D2D1_SIZE_F renderTargetSize = pRenderTarget->GetSize();
    hr = S_OK;
    const int localWidth = rc.right - rc.left;
    const int localHeight = rc.bottom - rc.top;
    D2D1_SIZE_U size = D2D1::SizeU(localWidth, localHeight);
    const float adjustedTopX = size.width * topX;
    const float adjustedTopY = size.height * topY;
    const float adjustedBottomX = size.width * bottomX;
    const float adjustedBottomY = size.height * bottomY;
    thisRect = D2D1::RectF(adjustedTopX, adjustedTopY,
        adjustedBottomX, adjustedBottomY);

    // Convert to WCHAR and attempt to find a good font size
    int arraySize = text.size();
    // Cast text to wide string so it can be cast to a WCHAR
    std::wstring wideText = std::wstring(text.begin(), text.end());
    const WCHAR* convertedText = wideText.c_str();
    // Each font point is about 1/72 inch by definition
    // A digital inch is 96 pixels long by definition
    const float fontPoint = (1.0 / 72.0);
    const float digitalSquareInch = 96.0 * 96;
    // We want a logarithmic relationship since text shouldn't explode
    // in font size, but needs a minimum to be readable. Font
    // size is also already a bit exponential anyway.
    // Spaces also cause a fair bit of trouble by wasting space
    // working around them neatly for wrapping, so we throw in
    // the 0.75 * just to add some wiggle room.
    const FLOAT msc_fontSize = max((0.75 * log2(digitalSquareInch *
        (adjustedBottomY - adjustedTopY) * (adjustedBottomX - adjustedTopX) *
        fontPoint / (arraySize))), 1);
    text = "Alright but can you see how this really needs to fit but wont.";

    // Create factories on first object function call.
    // For details see:
    //      https://docs.microsoft.com/en-us/windows/win32/direct2d/
    //      how-to--draw-text
    if (pD2DFactory == nullptr)
    {
        D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED,
            &pD2DFactory);
        DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(&pDWriteFactory));

        // Create a DirectWrite text format object.
        pDWriteFactory->CreateTextFormat(
            msc_fontName,
            NULL,
            DWRITE_FONT_WEIGHT_REGULAR,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            msc_fontSize,
            L"en-us", //locale
            &textBoxTextFormat);


        textBoxTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
        textBoxTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    }

    if (!pRenderTarget)
    {
        // Create a Direct2D render target.
        hr = pD2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(
                hWnd,
                size
            ),
            &pRenderTarget);
    }


    // Actually draw the text
    if (pRenderTarget != nullptr)
    {
        // Try to find a good color for the text taking into account the color of the area
        // it will be displayed in, by looking at the center pixel.
        HDC surfaceContext = GetDC(NULL);
        COLORREF pixelColor = GetPixel(surfaceContext, 
            adjustedTopX + ((adjustedBottomX - adjustedTopX) / 2) + xOffset, 
            adjustedTopY + ((adjustedBottomY - adjustedTopY) / 2) + yOffset);

        BYTE inverseR = (BYTE)255 - GetRValue(pixelColor);
        BYTE inverseG = (BYTE)255 - GetGValue(pixelColor);
        BYTE inverseB = (BYTE)255 - GetBValue(pixelColor);

        float rValue = (float)inverseR / 255;
        float gValue = (float)inverseG / 255;
        float bValue = (float)inverseB / 255;


        ReleaseDC(NULL, surfaceContext);


        // Create a black brush.
        //const D2D1_COLOR_F thisColor = D2D1::ColorF(rValue, gValue, bValue);
        //hr = pRenderTarget->CreateSolidColorBrush(
        //    thisColor, &textBoxBrush);
        const D2D1_COLOR_F thisColor = D2D1::ColorF(rValue, gValue, bValue);
        hr = pRenderTarget->CreateSolidColorBrush(
            thisColor, &textBoxBrush);


        pRenderTarget->DrawText(
            convertedText,
            arraySize,
            textBoxTextFormat,
            thisRect,
            textBoxBrush);

    }


    // Free device resources
    if (FAILED(hr))
    {
        if (pRenderTarget)
        {
            (pRenderTarget)->Release();
            pRenderTarget = NULL;
        }

        if (textBoxBrush)
        {
            (textBoxBrush)->Release();
            textBoxBrush = NULL;
        }
    }

}

//////////////////////////////////////////////////////////////////////////////
//                             Child Classes
//////////////////////////////////////////////////////////////////////////////

// Horizontal_Output
Horizontal_Output::Horizontal_Output()
{
    // Only exists to be replaced by calling below constructor.

    // Grab display reading offset values from config file
    std::ifstream confFile;
    confFile.open("Output Offset.conf");
    if (confFile)
    {
        // Read and skip the label to grab X Offset
        std::string readString;
        confFile >> readString >> readString >> readString;
        if (std::isdigit((unsigned char)readString[0]))
        {
            xOffset = stoi(readString);
        }


        // Read and skip the label to grab Y Offset
        confFile >> readString >> readString >> readString;
        if (std::isdigit((unsigned char)readString[0]))
        {
            yOffset = stoi(readString);
        }
    }
}
Horizontal_Output::Horizontal_Output(HWND inHandle, UINT inMessage, WPARAM inWParam,
    LPARAM inLongParam)
{
    // Grab display reading offset values from config file
    std::ifstream confFile;
    confFile.open("Output Offset.conf");
    if (confFile)
    {
        // Read and skip the label to grab X Offset
        std::string readString;
        confFile >> readString >> readString >> readString;
        if (std::isdigit((unsigned char)readString[0]))
        {
            xOffset = stoi(readString);
        }


        // Read and skip the label to grab Y Offset
        confFile >> readString >> readString >> readString;
        if (std::isdigit((unsigned char)readString[0]))
        {
            yOffset = stoi(readString);
        }
    }

    hWnd = inHandle;
    uMsg = inMessage;
    wParam = inWParam;
    lParam = inLongParam;
}
void Horizontal_Output::cycleText(std::string inText,
    HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget)
{

    // Measure various sizes.
    // Sort out what rectangular coordinates text needs
    HDC systemContext = GetDC(NULL);
    HWND systemHandle = GetDesktopWindow();
    //Adjust overlay window size to whole screen and show
    RECT screenRect;
    GetClientRect(systemHandle, &screenRect);
    ReleaseDC(NULL, systemContext);

    D2D1_RECT_F thisRect;
    RECT rc;
    GetClientRect(hWnd, &rc);
    // Retrieve the size of the render target.
    D2D1_SIZE_F renderTargetSize = pRenderTarget->GetSize();
    hr = S_OK;

    const int localWidth = rc.right - rc.left;
    const int localHeight = rc.bottom - rc.top;
    D2D1_SIZE_U size = D2D1::SizeU(localWidth, localHeight);
    const float adjustedTopX = size.width * topX; // Border
    const float adjustedTopY = size.height * topY; // Border
    const float adjustedBottomX = size.width * bottomX;
    const float adjustedBottomY = size.height * bottomY;
    thisRect = D2D1::RectF(adjustedTopX, adjustedTopY,
        adjustedBottomX, adjustedBottomY);


    static ID2D1SolidColorBrush* pBrush = NULL;
    HDC surfaceContext;
    hr = S_OK;

    surfaceContext = GetDC(NULL);
    for (int edgePixel = adjustedTopX; edgePixel <= adjustedBottomX; edgePixel += 15)
    {
        // Grab some color information to know how to output
        COLORREF pixelColor = GetPixel(surfaceContext, edgePixel + xOffset, adjustedTopY + yOffset);

        BYTE rValue = GetRValue(pixelColor);
        BYTE gValue = GetGValue(pixelColor);
        BYTE bValue = GetBValue(pixelColor);
        thisRect = D2D1::RectF(edgePixel, adjustedTopY,
            edgePixel + 15, adjustedBottomY);
        D2D1_COLOR_F color = D2D1::ColorF(float(rValue) / 255, float(gValue) / 255, float(bValue) / 255);
        hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);
        if (SUCCEEDED(hr))
        {
            pRenderTarget->FillRectangle(thisRect, pBrush);
        }
    }
    ReleaseDC(NULL, surfaceContext);

    Integration_Output::cycleText(inText, hr, pRenderTarget);
}

// Vertical_Output
Vertical_Output::Vertical_Output()
{
    // Only exists to be replaced by calling below constructor.

    // Grab display reading offset values from config file
    std::ifstream confFile;
    confFile.open("Output Offset.conf");
    if (confFile)
    {
        // Read and skip the label to grab X Offset
        std::string readString;
        confFile >> readString >> readString >> readString;
        if (std::isdigit((unsigned char)readString[0]))
        {
            xOffset = stoi(readString);
        }
        

        // Read and skip the label to grab Y Offset
        confFile >> readString >> readString >> readString;
        if (std::isdigit((unsigned char)readString[0]))
        {
            yOffset = stoi(readString);
        }
    }
}
Vertical_Output::Vertical_Output(HWND inHandle, UINT inMessage, WPARAM inWParam,
    LPARAM inLongParam)
{
    // Grab display reading offset values from config file
    std::ifstream confFile;
    confFile.open("Output Offset.conf");
    if (confFile)
    {
        // Read and skip the label to grab X Offset
        std::string readString;
        confFile >> readString >> readString >> readString;
        if (std::isdigit((unsigned char)readString[0]))
        {
            xOffset = stoi(readString);
        }


        // Read and skip the label to grab Y Offset
        confFile >> readString >> readString >> readString;
        if (std::isdigit((unsigned char)readString[0]))
        {
            yOffset = stoi(readString);
        }
    }

    hWnd = inHandle;
    uMsg = inMessage;
    wParam = inWParam;
    lParam = inLongParam;
}
void Vertical_Output::cycleText(std::string inText,
    HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget)
{

    // Measure various sizes.
    // Sort out what rectangular coordinates text needs
    HDC systemContext = GetDC(NULL);
    HWND systemHandle = GetDesktopWindow();
    //Adjust overlay window size to whole screen and show
    RECT screenRect;
    GetClientRect(systemHandle, &screenRect);
    ReleaseDC(NULL, systemContext);

    D2D1_RECT_F thisRect;
    RECT rc;
    GetClientRect(hWnd, &rc);
    // Retrieve the size of the render target.
    D2D1_SIZE_F renderTargetSize = pRenderTarget->GetSize();
    hr = S_OK;

    /*
    Overlay border parameters
    const int border = -10;
    const int leftFactor = 1;
    const int rightFactor = 1;
    const int upFactor = 4;
    const int downFactor = 2;
    */



    const int localWidth = rc.right - rc.left;
    const int localHeight = rc.bottom - rc.top;
    D2D1_SIZE_U size = D2D1::SizeU(localWidth, localHeight);
    const float adjustedTopX = size.width * topX; // Border
    const float adjustedTopY = size.height * topY; // Border
    const float adjustedBottomX = size.width * bottomX;
    const float adjustedBottomY = size.height * bottomY;
    thisRect = D2D1::RectF(adjustedTopX, adjustedTopY,
        adjustedBottomX, adjustedBottomY);


    static ID2D1SolidColorBrush* pBrush = NULL;
    HDC surfaceContext;
    hr = S_OK;

    surfaceContext = GetDC(NULL);
    for (int edgePixel = adjustedTopY; edgePixel <= adjustedBottomY; edgePixel += 10)
    {

        // Grab some color information to know how to output
        COLORREF pixelColor = GetPixel(surfaceContext, adjustedTopX + xOffset, edgePixel + yOffset);

        BYTE rValue = GetRValue(pixelColor);
        BYTE gValue = GetGValue(pixelColor);
        BYTE bValue = GetBValue(pixelColor);
        thisRect = D2D1::RectF(adjustedTopX, edgePixel, adjustedBottomX, edgePixel + 10);

        // Convert some color values to the right format (0-255 scale to 0-1.0 scale)
        D2D1_COLOR_F color = D2D1::ColorF(float(rValue) / 255, float(gValue) / 255, float(bValue) / 255);
        hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);
        if (SUCCEEDED(hr))
        {
            pRenderTarget->FillRectangle(thisRect, pBrush);
        }
    }
    ReleaseDC(NULL, surfaceContext);

    Integration_Output::cycleText(inText, hr, pRenderTarget);
}


// Image_Output
// See:
// https://docs.microsoft.com/en-us/windows/win32/wic/-wic-bitmapsources-howto-drawusingd2d
void Image_Output::cycleText(std::string inText,
    HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget)
{
    // Create a decoder
    IWICBitmapDecoder* pDecoder = NULL;
    IWICImagingFactory* m_pIWICFactory = nullptr;

    hr = m_pIWICFactory->CreateDecoderFromFilename(
        L"Labyrinth.png",                      // Image to be decoded
        NULL,                            // Do not prefer a particular vendor
        GENERIC_READ,                    // Desired read access to the file
        WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
        &pDecoder                        // Pointer to the decoder
    );

    // Retrieve the first frame of the image from the decoder
    IWICBitmapFrameDecode* pFrame = NULL;

    if (SUCCEEDED(hr))
    {
        hr = pDecoder->GetFrame(0, &pFrame);
    }
    /*
    // Format convert the frame to 32bppPBGRA
    if (SUCCEEDED(hr))
    {
        if (m_pConvertedSourceBitmap)
        {
            (m_pConvertedSourceBitmap)->Release();
            m_pConvertedSourceBitmap = NULL;
        }
        hr = m_pIWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
    }

    if (SUCCEEDED(hr))
    {
        hr = m_pConvertedSourceBitmap->Initialize(
            pFrame,                          // Input bitmap to convert
            GUID_WICPixelFormat32bppPBGRA,   // Destination pixel format
            WICBitmapDitherTypeNone,         // Specified dither pattern
            NULL,                            // Specify a particular palette 
            0.f,                             // Alpha threshold
            WICBitmapPaletteTypeCustom       // Palette translation type
        );
    }

    // Create a D2D render target properties
    D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties = D2D1::RenderTargetProperties();

    // Set the DPI to be the default system DPI to allow direct mapping
    // between image pixels and desktop pixels in different system DPI settings
    renderTargetProperties.dpiX = DEFAULT_DPI;
    renderTargetProperties.dpiY = DEFAULT_DPI;

    // Create a D2D render target
    D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

    hr = m_pD2DFactory->CreateHwndRenderTarget(
        renderTargetProperties,
        D2D1::HwndRenderTargetProperties(hWnd, size),
        &m_pRT
    );

    // D2DBitmap may have been released due to device loss. 
// If so, re-create it from the source bitmap
    if (m_pConvertedSourceBitmap && !m_pD2DBitmap)
    {
        m_pRT->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &m_pD2DBitmap);
    }

    // Draws an image and scales it to the current window size
    if (m_pD2DBitmap)
    {
        m_pRT->DrawBitmap(m_pD2DBitmap, rectangle);
    }

    */
}

// Solid_Output
Solid_Output::Solid_Output()
{
    // Only exists to be replaced by calling below constructor.

    // Grab display reading offset values from config file
    std::ifstream confFile;
    confFile.open("Output Offset.conf");
    if (confFile)
    {
        // Read and skip the label to grab X Offset
        std::string readString;
        confFile >> readString >> readString >> readString;
        if (std::isdigit((unsigned char)readString[0]))
        {
            xOffset = stoi(readString);
        }


        // Read and skip the label to grab Y Offset
        confFile >> readString >> readString >> readString;
        if (std::isdigit((unsigned char)readString[0]))
        {
            yOffset = stoi(readString);
        }
    }
}
Solid_Output::Solid_Output(HWND inHandle, UINT inMessage, WPARAM inWParam,
    LPARAM inLongParam)
{
    // Grab display reading offset values from config file
    std::ifstream confFile;
    confFile.open("Output Offset.conf");
    if (confFile)
    {
        // Read and skip the label to grab X Offset
        std::string readString;
        confFile >> readString >> readString >> readString;
        if (std::isdigit((unsigned char)readString[0]))
        {
            xOffset = stoi(readString);
        }


        // Read and skip the label to grab Y Offset
        confFile >> readString >> readString >> readString;
        if (std::isdigit((unsigned char)readString[0]))
        {
            yOffset = stoi(readString);
        }
    }

    hWnd = inHandle;
    uMsg = inMessage;
    wParam = inWParam;
    lParam = inLongParam;
}
void Solid_Output::cycleText(std::string inText,
    HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget)
{
    // Measure various sizes.
    // Sort out what rectangular coordinates text needs
    D2D1_RECT_F thisRect;
    RECT rc;
    GetClientRect(hWnd, &rc);
    // Retrieve the size of the render target.
    D2D1_SIZE_F renderTargetSize = pRenderTarget->GetSize();
    hr = S_OK;
    const int localWidth = rc.right - rc.left;
    const int localHeight = rc.bottom - rc.top;
    D2D1_SIZE_U size = D2D1::SizeU(localWidth, localHeight);
    const float adjustedTopX = size.width * topX;
    const float adjustedTopY = size.height * topY;
    const float adjustedBottomX = size.width * bottomX;
    const float adjustedBottomY = size.height * bottomY;
    thisRect = D2D1::RectF(adjustedTopX, adjustedTopY,
        adjustedBottomX, adjustedBottomY);

    // Grab some color information to know how to output
    HDC surfaceContext = GetDC(NULL);
    COLORREF pixelColor = GetPixel(surfaceContext, adjustedTopX + xOffset, adjustedTopY + yOffset);
    BYTE rValue = GetRValue(pixelColor);
    BYTE gValue = GetGValue(pixelColor);
    BYTE bValue = GetBValue(pixelColor);

    ReleaseDC(NULL, surfaceContext);

    // Fill in a rectangular background, then draw the text
    cycleRectangle(float(rValue) / 255, float(gValue) / 255, float(bValue) / 255, hr, pRenderTarget);
    Integration_Output::cycleText(inText, hr, pRenderTarget);
}

//#endif