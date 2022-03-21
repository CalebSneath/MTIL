//////////////////////////////////////////////////////////////////////////////
// 
// Author:           Caleb Sneath, Parker, MD
// Email:            ansengor@yahoo.com
// Label:            MTIL
// Course:           CMPS 5153
// Semester:         Spring 2022
// Date:             March 15, 2022
// File Description: This file contains the header information for the 
//                   translation output class along with all of 
//                   its  descendents.
//////////////////////////////////////////////////////////////////////////////


#pragma once

#include <string>

#pragma once

#include <string>

//////////////////////////////////////////////////////////////////////////////
//                             Abstract Classes
//////////////////////////////////////////////////////////////////////////////

// Interface class for any Integration_Input class

// Class: Integration_Output
// Purpose: Interface to demonstrate usage information on 
//          Integration_Output classes.
// Usage:   Use updateText to refresh what text is by objects of this class.
//          Use getText to grab from an object of this class.
//          Use getTextNoUpdate to grab text from objects of this class 
//          without checking for updates from the text source.
class Integration_Output
{
    public:
        void setText(std::string inText);
        void setCoordinates
            (int topLeftX, int topLeftY, int lowRightX, int lowRightY);
        virtual void displayText
            (int topLeftX, int topLeftY, int lowRightX, int lowRightY)=0;
        virtual void displayText();
    protected:
        std::string outputText;
        int upperLeftX;
        int upperLeftY;
        int bottomRightX;
        int bottomRightY;
};

//////////////////////////////////////////////////////////////////////////////
//                             Child Classes
//////////////////////////////////////////////////////////////////////////////

// Class: Horizontal_Output
// Purpose: A class to grab the left and right of the display area and
// stretch it across before displaying text.
class Horizontal_Output: public Integration_Output
{
    public:
        void displayText
            (int topLeftX, int topLeftY, int lowRightX, int lowRightY);
        void displayText();
    private: 
        // Any internal implementation details
};

class Vertical_Output : public Integration_Output
{
public:
    void displayText
    (int topLeftX, int topLeftY, int lowRightX, int lowRightY);
    void displayText();
private:
    // Any internal implementation details
};

class Transparent_Output : public Integration_Output
{
public:
    void displayText
    (int topLeftX, int topLeftY, int lowRightX, int lowRightY);
    void displayText();
private:
    // Any internal implementation details
};

class Image_Output : public Integration_Output
{
public:
    void displayText
    (int topLeftX, int topLeftY, int lowRightX, int lowRightY);
    void displayText();
private:
    // Any internal implementation details
};

class Solid_Output : public Integration_Output
{
public:
    void displayText
    (int topLeftX, int topLeftY, int lowRightX, int lowRightY);
    void displayText();
private:
    // Any internal implementation details
};
