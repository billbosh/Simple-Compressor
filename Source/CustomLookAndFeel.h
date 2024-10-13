/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 13 Oct 2024 4:05:26pm
    Author:  Josh Carroll

  ==============================================================================
*/

#include <JuceHeader.h>

class CustomLookAndFeel : public LookAndFeel_V4
{
public:
    CustomLookAndFeel();
    
    static LookAndFeel_V4::ColourScheme getCustomColourScheme();
};
