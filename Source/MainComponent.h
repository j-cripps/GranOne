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
#include "GuiParameters.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public Component,
                        private Thread,
                        private ChangeListener,
                        private Slider::Listener,
                        private ComboBox::Listener
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
     * @note    Implementation is thread safe via the Juce Tutorial.
     */
    void openButtonClicked();
    
    void clearButtonClicked();
    
    void checkForPathToOpen();
    
    void changeListenerCallback(ChangeBroadcaster* source) override;
    
    void thumbnailChanged();
    
    void sliderValueChanged(Slider* slider) override;
    
    void comboBoxChanged(ComboBox* comboBox) override;
    
    guiMap_t guiMap;
    
    // Audio Sample Buffer
    String chosenPath;
    AudioFormatManager formatManager;
    
    // Audio Component
    AudioComponent* audioComponent;
    
    // GUI Elements
    int numSections = 8;
    
    TextButton openButton;
    TextButton clearButton;
    
    AudioThumbnailCache thumbnailCache;
    AudioThumbnail thumbnail;
    
    Label masterGrainVolTitLab, masterGrainVolValLab;
    Slider masterGrainVolSli;
    
    Label envSelectTitLab, settingTwoTitLab, settingThreeTitLab;
    ComboBox envSelectBox, settingTwoBox, settingThreeBox;
    
    Label grainStartTitLab, grainStartMinLab, grainStartMaxLab;
    ComboBox grainStartBox;
    Slider grainStartSlider;
    
    Label grainOnsetTitLab, grainOnsetMinLab, grainOnsetMaxLab;
    ComboBox grainOnsetBox;
    Slider grainOnsetSlider;
    
    Label grainLengthTitLab, grainLengthMinLab, grainLengthMaxLab;
    ComboBox grainLengthBox;
    Slider grainLengthSlider;
    
    Label grainRateTitLab, grainRateMinLab, grainRateMaxLab;
    ComboBox grainRateBox;
    Slider grainRateSlider;
    
 
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
