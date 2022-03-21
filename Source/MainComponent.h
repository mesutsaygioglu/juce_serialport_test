#pragma once

#include <JuceHeader.h>
#include "usb/juce_serialport.h"
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.

 
*/

class MainComponent  : public juce::Component,
                       public juce::ComboBox::Listener,
                       //public juce::Button::Listener,
                       public juce::Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
    ComboBox portBox;
    std::unique_ptr <SerialPortOutputStream> pOutputStream;
    std::unique_ptr <SerialPortInputStream> pInputStream;

    void comboBoxChanged(ComboBox* comboBoxThatHasChanged);
    void timerCallback() override;
    int selectedPortIndex=-1;
    std::unique_ptr <SerialPort> pSP;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
