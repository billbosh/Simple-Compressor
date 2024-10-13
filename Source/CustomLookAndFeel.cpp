/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 13 Oct 2024 4:05:26pm
    Author:  Josh Carroll

  ==============================================================================
*/

#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
    
    setColourScheme(getCustomColourScheme());
};

LookAndFeel_V4::ColourScheme CustomLookAndFeel::getCustomColourScheme()
{
    return LookAndFeel_V4::ColourScheme(
        Colour(0xff434343), // windowBackground
        Colour(0xff383838), // widgetBackground
        Colour(0xff434343), // menuBackground
        Colours::white, // outline
        Colours::white, // defaultText
        Colours::white, // defaultFill
        Colours::blue,  // highlightedText
        Colour(0xff383838),  // highlightedFill
        Colours::white // menuText
    );
}
