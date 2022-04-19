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

// Integration_Output

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
Integration_Output::Integration_Output(HWND inHandle, UINT inMessage, 
    WPARAM inWParam, LPARAM inLongParam)
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
    D2D1_SIZE_F renderTargetSize = pRenderTarget->GetSize();
    hr = S_OK; 
    D2D1_SIZE_F size = pRenderTarget->GetSize();
    const int localWidth = rc.right - rc.left;
    const int localHeight = rc.bottom - rc.top;
    const float adjustedTopX = size.width * topX;
    const float adjustedTopY = size.height * topY;
    const float adjustedBottomX = size.width * bottomX;
    const float adjustedBottomY = size.height * bottomY;
    thisRect = D2D1::RectF(adjustedTopX, adjustedTopY,
        adjustedBottomX, adjustedBottomY);

    // Actually draw the text
    if (pRenderTarget != nullptr)
    {
        // Try to find a good color for the text taking into 
        // account the color of the area
        // it will be displayed in, by looking at the center pixel.
        HDC surfaceContext = GetDC(NULL);
        COLORREF pixelColor = GetPixel(surfaceContext, 
            adjustedTopX + ((adjustedBottomX - adjustedTopX) / 2) + xOffset, 
            adjustedTopY + ((adjustedBottomY - adjustedTopY) / 2) + yOffset);
        ReleaseDC(NULL, surfaceContext);

        // Color is scaled either 0-255 bits or floating 0-1.
        BYTE inverseR = (BYTE)255 - GetRValue(pixelColor);
        BYTE inverseG = (BYTE)255 - GetGValue(pixelColor);
        BYTE inverseB = (BYTE)255 - GetBValue(pixelColor);
        float rValue = (float)inverseR / 255;
        float gValue = (float)inverseG / 255;
        float bValue = (float)inverseB / 255;

        // Actually draw text.
        TextBox::cycleText(rValue, gValue, bValue, hr, pRenderTarget);
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
Horizontal_Output::Horizontal_Output(HWND inHandle, UINT inMessage, 
    WPARAM inWParam, LPARAM inLongParam)
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
    // Early exit for invalid coordinates
    if (topX == -1)
    {
        return;
    }

    // Calculate boundaries
    D2D1_RECT_F thisRect;
    RECT rc;
    GetClientRect(hWnd, &rc);
    // Retrieve the size of the render target.
    D2D1_SIZE_F renderTargetSize = pRenderTarget->GetSize();
    hr = S_OK;
    const int localWidth = rc.right - rc.left;
    const int localHeight = rc.bottom - rc.top;
    const float adjustedTopX = renderTargetSize.width * topX; // Border
    const float adjustedTopY = renderTargetSize.height * topY; // Border
    const float adjustedBottomX = renderTargetSize.width * bottomX;
    const float adjustedBottomY = renderTargetSize.height * bottomY;
    thisRect = D2D1::RectF(adjustedTopX, adjustedTopY,
        adjustedBottomX, adjustedBottomY);


    static ID2D1SolidColorBrush* pBrush = NULL;

    HDC surfaceContext = GetDC(NULL);
    // Iterate through a row of pixels, drawing a matching color
    // rectangle down to the boundary
    for (int edgePixel = adjustedTopX; edgePixel <= adjustedBottomX + 15
        ; edgePixel += 15)
    {
        // Grab some color information to know how to output
        COLORREF pixelColor = GetPixel(surfaceContext, edgePixel + xOffset, 
            adjustedTopY + yOffset);

        BYTE rValue = GetRValue(pixelColor);
        BYTE gValue = GetGValue(pixelColor);
        BYTE bValue = GetBValue(pixelColor);
        thisRect = D2D1::RectF(edgePixel, adjustedTopY,
            edgePixel + 15, adjustedBottomY);
        D2D1_COLOR_F color = D2D1::ColorF(float(rValue) / 255, 
            float(gValue) / 255, float(bValue) / 255);
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
Vertical_Output::Vertical_Output(HWND inHandle, UINT inMessage, 
    WPARAM inWParam, LPARAM inLongParam)
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
    // Early exit for invalid coordinates
    if (topX == -1)
    {
        return;
    }

    // Calculate rectangle boundaries
    D2D1_RECT_F thisRect;
    RECT rc;
    GetClientRect(hWnd, &rc);
    // Retrieve the size of the render target.
    D2D1_SIZE_F renderTargetSize = pRenderTarget->GetSize();
    hr = S_OK;
    const int localWidth = rc.right - rc.left;
    const int localHeight = rc.bottom - rc.top;
    const float adjustedTopX = renderTargetSize.width * topX; // Border
    const float adjustedTopY = renderTargetSize.height * topY; // Border
    const float adjustedBottomX = renderTargetSize.width * bottomX;
    const float adjustedBottomY = renderTargetSize.height * bottomY;
    thisRect = D2D1::RectF(adjustedTopX, adjustedTopY,
        adjustedBottomX, adjustedBottomY);


    static ID2D1SolidColorBrush* pBrush = NULL;

    HDC surfaceContext;
    // Iterate through a row of pixels, drawing a matching color
    // rectangle down to the boundary
    // Potential Optimizations: Grab screen area as bitmap all at once
    // and manipulate the bitmap. This is possibly better, although
    // no dev has experience dong as such beyond opening files.
    // Alternatively, using a thread to fire off a sleep function
    // and only readjust pixels every few seconds rather than X
    // times per second.
    for (int edgePixel = adjustedTopY; edgePixel <= adjustedBottomY + 15
        ; edgePixel += 15)
    {
        // Grab some color information to know how to output
        surfaceContext = GetDC(NULL);
        COLORREF pixelColor = GetPixel(surfaceContext, 
            adjustedTopX + xOffset, edgePixel + yOffset);
        ReleaseDC(NULL, surfaceContext);


        BYTE rValue = GetRValue(pixelColor);
        BYTE gValue = GetGValue(pixelColor);
        BYTE bValue = GetBValue(pixelColor);
        thisRect = D2D1::RectF(adjustedTopX, edgePixel, 
            adjustedBottomX, edgePixel + 15);

        // Convert some color values to the right format 
        // (0-255 scale to 0-1.0 scale)
        D2D1_COLOR_F color = D2D1::ColorF(float(rValue) / 255, 
            float(gValue) / 255, float(bValue) / 255);
        hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);
        if (SUCCEEDED(hr))
        {
            pRenderTarget->FillRectangle(thisRect, pBrush);
        }
    }

    Integration_Output::cycleText(inText, hr, pRenderTarget);
}


// Image_Output
// Static Member Variables
bool Image_Output::files[9] = { false, false, false, false, false,
    false, false, false };
std::string Image_Output::names[9] = { "MTILFILE1.png", "Labyrinth.png", 
    "MTILFile2.png", "MTILFile3.png", "MTILFile4.png", "MTILFile5.png", 
    "MTILFile6.png", "MTILFile7.png", "MTILFile8.png" };
Image_Output::Image_Output()
{
    // Only exists to be replaced by calling below constructor.

    // Iterate through the input files
    // Grab the first and mark it as
    // in use to all other input streams.
    for (int i = 0; i < 9; i++) {
        if (files[i] == false) {
            fileName = names[i];
            files[i] = true;
            index = i;
            break;
        }
    }

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
Image_Output::Image_Output(HWND inHandle, UINT inMessage,
    WPARAM inWParam, LPARAM inLongParam)
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

    // Iterate through the input files
    // Grab the first and mark it as
    // in use to all other input streams.
    for (int i = 0; i < 8; i++) {
        if (files[i] == false) {
            fileName = names[i];
            files[i] = true;
            index = i;
            break;
        }
    }
}
// Credit: Partially made from code obtained here.
// See:
// https://docs.microsoft.com/en-us/windows/win32/wic/
// -wic-bitmapsources-howto-drawusingd2d
void Image_Output::cycleText(std::string inText,
    HRESULT& hr, ID2D1HwndRenderTarget*& pRenderTarget)
{
    // Declare/Initialize statics
    static IWICBitmapDecoder* pDecoder = nullptr;
    static IWICImagingFactory* m_pIWICFactory;
    static ID2D1Bitmap* m_pD2DBitmap = nullptr;
    static IWICFormatConverter* m_pConvertedSourceBitmap = nullptr;
    static IWICBitmapFrameDecode* pFrame = NULL;
    static D2D1_RECT_F thisRect;
    static RECT rc;

    // Early exit for invalid coordinates
    if (topX == -1)
    {
        return;
    }


    // Convert filename to WCHAR *
    std::wstring wideName = std::wstring(fileName.begin(), fileName.end());
    const WCHAR* convertedName = wideName.c_str();

    hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&m_pIWICFactory)
    );
    hr = m_pIWICFactory->CreateDecoderFromFilename(
        convertedName,                      // Image to be decoded
        NULL,                            // Do not prefer a particular vendor
        GENERIC_READ,                    // Desired read access to the file
        WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
        &pDecoder                        // Pointer to the decoder
    );


    if (SUCCEEDED(hr))
    {
        hr = pDecoder->GetFrame(0, &pFrame);
    }
    
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

    // D2DBitmap may have been released due to device loss. 
    // If so, re-create it from the source bitmap
    if (m_pConvertedSourceBitmap && !m_pD2DBitmap)
    {
        pRenderTarget->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, 
            NULL, &m_pD2DBitmap);
    }

    // Draws an image and scales it to the current window size
    if (m_pD2DBitmap)
    {
        pRenderTarget->DrawBitmap(m_pD2DBitmap, thisRect);
    }

    // Print the text
    Integration_Output::cycleText(inText, hr, pRenderTarget);
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
    COLORREF pixelColor = GetPixel(surfaceContext, adjustedTopX + xOffset, 
        adjustedTopY + yOffset);
    BYTE rValue = GetRValue(pixelColor);
    BYTE gValue = GetGValue(pixelColor);
    BYTE bValue = GetBValue(pixelColor);

    ReleaseDC(NULL, surfaceContext);

    // Fill in a rectangular background, then draw the text
    cycleRectangle(float(rValue) / 255, float(gValue) / 255, 
        float(bValue) / 255, hr, pRenderTarget);
    Integration_Output::cycleText(inText, hr, pRenderTarget);
}

//#endif