/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Grain.h"
#include "ReferenceCountedBuffer.h"
#include "AudioComponent.h"
#include "parse.hpp"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public Component,
                        private Thread
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();
    
    //==============================================================================
    void run() override;
    void checkForBuffersToFree();

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    /**
     * @brief   Function called whenever the 'open' button is pressed in the GUI
     *
     * @note    Implementation not thread safe so audio shuts down when pressed,
     *          this can be avoided by following looping audio sample tutorial
     *          on the JUCE website
     */
    void openButtonClicked();
    
    void clearButtonClicked();
    
    void checkForPathToOpen();
    
    // GUI Elements
    TextButton openButton;
    TextButton clearButton;
    Slider densitySlider, offsetSlider, lengthSlider, startSlider, panSlider, rateSlider;
    
    // Audio Sample Buffer
    String chosenPath;
    AudioFormatManager formatManager;
    
    // Audio Component
    AudioComponent* audioComponent;
    
    // Boid values
    std::vector<std::vector<Boids::boid_struct>> boidStructStack;
    Boids::boid_range_t boidRanges;
 
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
