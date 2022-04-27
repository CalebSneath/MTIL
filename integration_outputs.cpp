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
#include <thread>

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
        xOffset = stoi(readString);

        // Read and skip the label to grab Y Offset
        confFile >> readString >> readString >> readString;
        yOffset = stoi(readString);
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
        xOffset = stoi(readString);

        // Read and skip the label to grab Y Offset
        confFile >> readString >> readString >> readString;
        yOffset = stoi(readString);
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
        POINT overlayAdjustCoords;
        // Grab the middle pixel of the overlay
        overlayAdjustCoords.x = adjustedTopX + 
            ((adjustedBottomX - adjustedTopX) / 2) + xOffset;
        overlayAdjustCoords.y = adjustedTopY + 
            ((adjustedBottomY - adjustedTopY) / 2) + yOffset;

        // Overlay isn't a 1:1 conversion of screen pixels, so
        // we now must adjust if we want to grab the actual
        // pixel color
        ScreenToClient(hWnd, &overlayAdjustCoords);

        // Try to find a good color for the text taking into 
        // account the color of the area
        // it will be displayed in, by looking at the center pixel.
        HDC surfaceContext = GetDC(NULL);
        COLORREF pixelColor = GetPixel(surfaceContext, 
            overlayAdjustCoords.x, overlayAdjustCoords.y);
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
        xOffset = stoi(readString);

        // Read and skip the label to grab Y Offset
        confFile >> readString >> readString >> readString;
        yOffset = stoi(readString);

        // Read and skip the label to grab pixel precision
        confFile >> readString >> readString >> readString;
        if (std::isdigit((unsigned char)readString[0]))
        {
            pixelScale = stoi(readString);
        }

        // Read and skip the label to grab polling scale
        confFile >> readString >> readString >> readString >> readString;
        if (std::isdigit((unsigned char)readString[0]))
        {
            waitScale = stof(readString);
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
        xOffset = stoi(readString);

        // Read and skip the label to grab Y Offset
        confFile >> readString >> readString >> readString;
        yOffset = stoi(readString);

        // Read and skip the label to grab pixel precision
        confFile >> readString >> readString >> readString;
        if (std::isdigit((unsigned char)readString[0]))
        {
            pixelScale = stoi(readString);
        }

        // Read and skip the label to grab polling scale
        confFile >> readString >> readString >> readString >> readString;
        if (std::isdigit((unsigned char)readString[0]))
        {
            waitScale = stof(readString);
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

    heldRenderTarget = pRenderTarget;

    // Calculate rectangle boundaries
    D2D1_RECT_F thisRect;

    ID2D1SolidColorBrush* pBrush = NULL;

    // Pixel polling is pretty inefficient until it is further optimized
    // so we want it to wait and do it only every few seconds rather
    // than once a frame.
    if (refreshThreadCalled == false)
    {
        refreshThreadCalled = true;

        refreshPollThread = std::thread(&Horizontal_Output::handlePixelPolling,
            this);

        pollingDone = true;

    }
    // Avoid timing issues
    while (pollingDone == false)
    {
        // Wait until one cycle of polling to move on
    }
    drawingDone = false;


    // Iterate through a row of pixels, drawing a matching color
    // rectangle down to the boundary
    int edgePixel = adjustedTopX;
    for (int index = 0; index < pixelColorList.size()
        ; index++)
    {
        // Early exit to avoid errors if no display and we still
        // made it here somehow due to thread
        if (pixelColorList.size() == 0)
        {
            return;
        }

        hr = pRenderTarget->CreateSolidColorBrush(
            pixelColorList[index], &pBrush);

        thisRect = D2D1::RectF(edgePixel, adjustedTopY,
            edgePixel + pixelScale, adjustedBottomY);
        pRenderTarget->FillRectangle(thisRect, pBrush);

        // Release resources
        (*&pBrush)->Release();
        pBrush = nullptr;

        edgePixel += pixelScale; //Increment edge pixels
    }
    // Let thread continue if locked
    drawingDone = true;

    Integration_Output::cycleText(inText, hr, pRenderTarget);
}
void Horizontal_Output::handlePixelPolling()
{
    while (refreshThreadCalled == true)
    {
        while (drawingDone == false)
        {
            // Wait until polling is done so we don't delete a vector
            // in use and cause a memory out of bounds error.
        }
        pollingDone = false;

        // Calculate rectangle boundaries
        D2D1_RECT_F thisRect;
        RECT rc;
        GetClientRect(hWnd, &rc);
        // Retrieve the size of the render target.
        D2D1_SIZE_F renderTargetSize = heldRenderTarget->GetSize();
        const int localWidth = rc.right - rc.left;
        const int localHeight = rc.bottom - rc.top;
        adjustedTopX = renderTargetSize.width * topX; // Border
        adjustedTopY = renderTargetSize.height * topY; // Border
        adjustedBottomX = renderTargetSize.width * bottomX;
        adjustedBottomY = renderTargetSize.height * bottomY;
        thisRect = D2D1::RectF(adjustedTopX, adjustedTopY,
            adjustedBottomX, adjustedBottomY);

        // Empty color list
        pixelColorList.clear();

        // Iterate through a row of pixels, grabbing a matching color
        // rectangle down to the boundary
        // Potential Optimizations: Grab screen area as bitmap all at once
        // and manipulate the bitmap. This is possibly better, although
        // no dev has experience dong as such beyond opening files.
        for (int edgePixel = adjustedTopX; edgePixel <= adjustedBottomX + pixelScale
            ; edgePixel += pixelScale)
        {
            POINT overlayAdjustCoords;
            overlayAdjustCoords.x = edgePixel;
            overlayAdjustCoords.y = adjustedTopY;
            ScreenToClient(hWnd, &overlayAdjustCoords);
            COLORREF unConvertedColor;

            // Grab some color information to know how to output
            HDC surfaceContext = GetDC(NULL);
            unConvertedColor = GetPixel(surfaceContext,
                overlayAdjustCoords.x + xOffset, 
                overlayAdjustCoords.y + yOffset);
            ReleaseDC(NULL, surfaceContext);

            BYTE rValue = GetRValue(unConvertedColor);
            BYTE gValue = GetGValue(unConvertedColor);
            BYTE bValue = GetBValue(unConvertedColor);

            // Convert some color values to the right format 
            // (0-255 scale to 0-1.0 scale)
            pixelColorList.push_back(D2D1::ColorF(float(rValue) / 255,
                float(gValue) / 255, float(bValue) / 255));
        }

        pollingDone = true;
        // Only poll occasionally.
        Sleep(waitScale);
    }
    threadDone = true;
}
Horizontal_Output::~Horizontal_Output()
{
    // Ensure refresh thread ends
    if (refreshThreadCalled == true)
    {
        refreshThreadCalled = false;
        while (!threadDone)
        {
            // Wait until thread ends
        }
        refreshPollThread.join();
    }
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
        xOffset = stoi(readString);

        // Read and skip the label to grab Y Offset
        confFile >> readString >> readString >> readString;
        yOffset = stoi(readString);

        // Read and skip the label to grab pixel precision
        confFile >> readString >> readString >> readString;
        if (std::isdigit((unsigned char)readString[0]))
        {
            pixelScale = stoi(readString);
        }

        // Read and skip the label to grab polling scale
        confFile >> readString >> readString >> readString >> readString;
        if (std::isdigit((unsigned char)readString[0]))
        {
            waitScale = stof(readString);
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
        xOffset = stoi(readString);

        // Read and skip the label to grab Y Offset
        confFile >> readString >> readString >> readString;
        yOffset = stoi(readString);

        // Read and skip the label to grab pixel precision
        confFile >> readString >> readString >> readString;
        if (std::isdigit((unsigned char)readString[0]))
        {
            pixelScale = stoi(readString);
        }

        // Read and skip the label to grab polling scale
        confFile >> readString >> readString >> readString >> readString;
        if (std::isdigit((unsigned char)readString[0]))
        {
            waitScale = stof(readString);
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

    heldRenderTarget = pRenderTarget;

    // Calculate rectangle boundaries
    D2D1_RECT_F thisRect;

    ID2D1SolidColorBrush* pBrush = NULL;

    // Pixel polling is pretty inefficient until it is further optimized
    // so we want it to wait and do it only every few seconds rather
    // than once a frame.
    if (refreshThreadCalled == false) 
    {
        refreshThreadCalled = true;
        
        refreshPollThread = std::thread(&Vertical_Output::handlePixelPolling,
            this);
        
        pollingDone = true;

    }
    // Avoid timing issues
    while (pollingDone == false)
    {
        // Wait until one cycle of polling to move on
    }
    drawingDone = false;

    // Iterate through a row of pixels, drawing a matching color
    // rectangle down to the boundary
    int edgePixel = adjustedTopY;
    for (int index = 0; index < pixelColorList.size(); index++)
    {
        // Early exit to avoid errors if no display and we still
        // made it here somehow due to thread
        if (pixelColorList.size() == 0)
        {
            return;
        }

        thisRect = D2D1::RectF(adjustedTopX, edgePixel, 
            adjustedBottomX, edgePixel + pixelScale);
        pRenderTarget->CreateSolidColorBrush(
            pixelColorList[index], &pBrush);
        pRenderTarget->FillRectangle(thisRect, pBrush);

        // Free resources
        (*&pBrush)->Release();
        pBrush = nullptr;

        edgePixel += pixelScale; // Increment edge pixel
    }
    // Let thread continue if locked
    drawingDone = true;

    Integration_Output::cycleText(inText, hr, pRenderTarget);
}
void Vertical_Output::handlePixelPolling()
{
    while (refreshThreadCalled == true)
    {
        while (drawingDone == false)
        {
            // Wait until polling is done so we don't delete a vector
            // in use and cause a memory out of bounds error.
        }
        pollingDone = false;

        // Calculate rectangle boundaries
        D2D1_RECT_F thisRect;
        RECT rc;
        GetClientRect(hWnd, &rc);
        // Retrieve the size of the render target.
        D2D1_SIZE_F renderTargetSize = heldRenderTarget->GetSize();
        const int localWidth = rc.right - rc.left;
        const int localHeight = rc.bottom - rc.top;
        adjustedTopX = renderTargetSize.width * topX; // Border
        adjustedTopY = renderTargetSize.height * topY; // Border
        adjustedBottomX = renderTargetSize.width * bottomX;
        adjustedBottomY = renderTargetSize.height * bottomY;
        thisRect = D2D1::RectF(adjustedTopX, adjustedTopY,
            adjustedBottomX, adjustedBottomY);

        // Empty color list
        pixelColorList.clear();

        // Iterate through a row of pixels, grabbing a matching color
        // rectangle down to the boundary
        // Potential Optimizations: Grab screen area as bitmap all at once
        // and manipulate the bitmap. This is possibly better, although
        // no dev has experience dong as such beyond opening files.
        for (int edgePixel = adjustedTopY; edgePixel <= adjustedBottomY 
            + pixelScale; edgePixel += pixelScale)
        {
            POINT overlayAdjustCoords;
            overlayAdjustCoords.x = adjustedTopX;
            overlayAdjustCoords.y = edgePixel;
            ScreenToClient(hWnd, &overlayAdjustCoords);
            COLORREF unConvertedColor;

            // Grab some color information to know how to output
            HDC surfaceContext = GetDC(NULL);
            unConvertedColor = GetPixel(surfaceContext, 
                overlayAdjustCoords.x + xOffset, 
                overlayAdjustCoords.y + yOffset);
            ReleaseDC(NULL, surfaceContext);

            BYTE rValue = GetRValue(unConvertedColor);
            BYTE gValue = GetGValue(unConvertedColor);
            BYTE bValue = GetBValue(unConvertedColor);

            // Convert some color values to the right format 
            // (0-255 scale to 0-1.0 scale)
            pixelColorList.push_back(D2D1::ColorF(float(rValue) / 255,
                float(gValue) / 255, float(bValue) / 255));
        }

        pollingDone = true;
        // Only poll occasionally.
        Sleep(waitScale);
    }
    threadDone = true;
}
Vertical_Output::~Vertical_Output()
{
    // Ensure refresh thread ends
    if (refreshThreadCalled == true)
    {
        refreshThreadCalled = false;
        while (!threadDone)
        {
            // Wait until thread ends
        }
        refreshPollThread.join();
    }
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
    std::ifstream sourceConfFile;
    sourceConfFile.open("Image Files.conf");

    int loopIndex = 0;
    // Guard against crashes
    if (sourceConfFile.is_open())
    {
        // Iterate through file
        // and load names into array
        while (sourceConfFile && loopIndex < 9)
        {
            std::string holdInput;
            sourceConfFile >> holdInput >> holdInput >> names[loopIndex];

            // Iterate loop control
            loopIndex++;
        }

        // Close file
        sourceConfFile.close();
    }

    // ConfFile Corrupted, try making a new one
    if (loopIndex != 9)
    {
        std::ofstream newConfFile;
        newConfFile.open("Image Files.conf");

        // Iterate through each row
        for (int newLoopIndex = 0; newLoopIndex < 9; newLoopIndex++)
        {
            newConfFile << "Image " << newLoopIndex + 1 << 
                ": Labyrinth.png" << std::endl;
        }

        // Close file
        newConfFile.close();
    }

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
    std::ifstream sourceConfFile;
    sourceConfFile.open("Image Files.conf");

    int loopIndex = 0;
    // Guard against crashes
    if (sourceConfFile.is_open())
    {
        // Iterate through file
        // and load names into array
        while (sourceConfFile && loopIndex < 9)
        {
            std::string holdInput;
            sourceConfFile >> holdInput >> holdInput >> names[loopIndex];

            // Iterate loop control
            loopIndex++;
        }

        // Close file
        sourceConfFile.close();
    }

    // ConfFile Corrupted, try making a new one
    if (loopIndex != 9)
    {
        std::ofstream newConfFile;
        newConfFile.open("Image Files.conf");

        // Iterate through each row
        for (int newLoopIndex = 0; newLoopIndex < 9; newLoopIndex++)
        {
            newConfFile << "Image " << newLoopIndex + 1 <<
                ": Labyrinth.png" << std::endl;
        }

        // Close file
        newConfFile.close();
    }

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
    D2D1_SIZE_F size = pRenderTarget->GetSize();
    const float adjustedTopX = size.width * topX; // Border
    const float adjustedTopY = size.height * topY; // Border
    const float adjustedBottomX = size.width * bottomX;
    const float adjustedBottomY = size.height * bottomY;
    thisRect = D2D1::RectF(adjustedTopX, adjustedTopY,
        adjustedBottomX, adjustedBottomY);





    /*
    // Calculate size for drawing
    D2D1_SIZE_F windowSize = pRenderTarget->GetSize();
    const float adjustedTopX = windowSize.width * topX;
    const float adjustedTopY = windowSize.height * topY;
    const float adjustedBottomX = windowSize.width * bottomX;
    const float adjustedBottomY = windowSize.height * bottomY;
    thisRect = D2D1::RectF(adjustedTopX, adjustedTopY,
        adjustedBottomX, adjustedBottomY);
    */

    // D2DBitmap may have been released due to device loss. 
    // If so, re-create it from the source bitmap
    pRenderTarget->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, 
            NULL, &m_pD2DBitmap);

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
    POINT overlayAdjustCoords;
    overlayAdjustCoords.x = adjustedTopX + xOffset;
    overlayAdjustCoords.y = adjustedTopY + yOffset;

    HDC surfaceContext = GetDC(NULL);
    COLORREF pixelColor = GetPixel(surfaceContext, overlayAdjustCoords.x, 
        overlayAdjustCoords.y);
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