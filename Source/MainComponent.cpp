/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
    : Thread("Background Thread"), thumbnailCache(5), thumbnail(512, formatManager, thumbnailCache)
{
    // Must set size of child components before size of main component
    addAndMakeVisible (openButton);
    openButton.setButtonText ("Choose Audio");
    openButton.onClick = [this] { openButtonClicked(); };   // Lambda function
    
    addAndMakeVisible (clearButton);
    clearButton.setButtonText ("Clear Audio");
    clearButton.onClick = [this] { clearButtonClicked(); };   // Lambda function
    
    // Setup the audio thumbnail as per the JUCE tutorial
    thumbnail.addChangeListener(this);
    
    // Setup Master Volume Section
    addAndMakeVisible(masterGrainVolTitLab);
    masterGrainVolTitLab.setText("Master Volume", NotificationType::dontSendNotification);
    masterGrainVolTitLab.setJustificationType(Justification::centred);
    
    addAndMakeVisible(masterGrainVolValLab);
    masterGrainVolValLab.setText("Label", NotificationType::dontSendNotification);
    masterGrainVolValLab.setJustificationType(Justification::centred);
    masterGrainVolValLab.setColour(Label::ColourIds::outlineColourId, Colour(144, 152, 154));
    
    addAndMakeVisible(masterGrainVolSli);
    masterGrainVolSli.setSliderStyle(Slider::LinearHorizontal);
    masterGrainVolSli.setSkewFactor(0.4);
    masterGrainVolSli.setRange(RANGE_MASTER_VOL_MIN, RANGE_MASTER_VOL_MAX);
    masterGrainVolSli.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 1, 1);
    masterGrainVolSli.setValue(INIT_MASTER_VOL);
    guiMap.grainMasterVol = INIT_MASTER_VOL;
    masterGrainVolSli.addListener(this);
    
    // Setup discrete box selection
    addAndMakeVisible(envSelectTitLab);
    envSelectTitLab.setText("Grain Envelope", NotificationType::dontSendNotification);
    envSelectTitLab.setJustificationType(Justification::centred);
    
    addAndMakeVisible(envSelectBox);
    envSelectBox.addSectionHeading("Envelope Shape");
    envSelectBox.addItem("Rectangle", 1);
    envSelectBox.addItem("Tukey", 2);
    envSelectBox.setSelectedId(2);
    
    // Setup grain start position
    addAndMakeVisible(grainStartTitLab);
    grainStartTitLab.setText("Grain Start Position", NotificationType::dontSendNotification);
    grainStartTitLab.setJustificationType(Justification::centred);
    
    addAndMakeVisible(grainStartBox);
    grainStartBox.addSectionHeading("Boid Parameter to Bind to Start Position");
    grainStartBox.addItemList(StringArray(boidStrings), 1);
    grainStartBox.setSelectedId(1);
    
    addAndMakeVisible(grainStartMinLab);
    grainStartMinLab.setText("Min Label", NotificationType::dontSendNotification);
    grainStartMinLab.setJustificationType(Justification::centred);
    grainStartMinLab.setColour(Label::ColourIds::outlineColourId, Colour(144, 152, 154));
    
    addAndMakeVisible(grainStartSlider);
    grainStartSlider.setSliderStyle(Slider::TwoValueHorizontal);
    grainStartSlider.setRange(RANGE_START_POS_MIN, RANGE_START_POS_MAX);
    grainStartSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 1, 1);
    grainStartSlider.setMaxValue(INIT_START_POS_MAX);
    grainStartSlider.setMinValue(INIT_START_POS_MIN);
    guiMap.grainStartPosMax = INIT_START_POS_MAX;
    guiMap.grainStartPosMin = INIT_START_POS_MIN;
    grainStartSlider.addListener(this);
    
    addAndMakeVisible(grainStartMaxLab);
    grainStartMaxLab.setText("Max Label", NotificationType::dontSendNotification);
    grainStartMaxLab.setJustificationType(Justification::centred);
    grainStartMaxLab.setColour(Label::ColourIds::outlineColourId, Colour(144, 152, 154));
    
    // Setup grain inital onset
    addAndMakeVisible(grainOnsetTitLab);
    grainOnsetTitLab.setText("Grain Onset Time", NotificationType::dontSendNotification);
    grainOnsetTitLab.setJustificationType(Justification::centred);
    
    addAndMakeVisible(grainOnsetBox);
    grainOnsetBox.addSectionHeading("Boid Parameter to Bind to Onset Time");
    grainOnsetBox.addItemList(StringArray(boidStrings), 1);
    grainOnsetBox.setSelectedId(1);
    
    addAndMakeVisible(grainOnsetMinLab);
    grainOnsetMinLab.setText("Min Label", NotificationType::dontSendNotification);
    grainOnsetMinLab.setJustificationType(Justification::centred);
    grainOnsetMinLab.setColour(Label::ColourIds::outlineColourId, Colour(144, 152, 154));
    
    addAndMakeVisible(grainOnsetSlider);
    grainOnsetSlider.setSliderStyle(Slider::TwoValueHorizontal);
    grainOnsetSlider.setRange(RANGE_ONSET_MIN, RANGE_ONSET_MAX);
    grainOnsetSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 1, 1);
    grainOnsetSlider.setMaxValue(INIT_ONSET_MAX);
    grainOnsetSlider.setMinValue(INIT_ONSET_MIN);
    guiMap.grainOnsetMax = INIT_ONSET_MAX * SAMPLE_RATE_MS;
    guiMap.grainOnsetMin = INIT_ONSET_MIN * SAMPLE_RATE_MS;
    grainOnsetSlider.addListener(this);
    
    addAndMakeVisible(grainOnsetMaxLab);
    grainOnsetMaxLab.setText("Max Label", NotificationType::dontSendNotification);
    grainOnsetMaxLab.setJustificationType(Justification::centred);
    grainOnsetMaxLab.setColour(Label::ColourIds::outlineColourId, Colour(144, 152, 154));
    
    // Setup grain length
    addAndMakeVisible(grainLengthTitLab);
    grainLengthTitLab.setText("Grain Length", NotificationType::dontSendNotification);
    grainLengthTitLab.setJustificationType(Justification::centred);
    
    addAndMakeVisible(grainLengthBox);
    grainLengthBox.addSectionHeading("Boid Parameter to Bind to Length");
    grainLengthBox.addItemList(StringArray(boidStrings), 1);
    grainLengthBox.setSelectedId(1);
    
    addAndMakeVisible(grainLengthMinLab);
    grainLengthMinLab.setText("Min Label", NotificationType::dontSendNotification);
    grainLengthMinLab.setJustificationType(Justification::centred);
    grainLengthMinLab.setColour(Label::ColourIds::outlineColourId, Colour(144, 152, 154));
    
    addAndMakeVisible(grainLengthSlider);
    grainLengthSlider.setSliderStyle(Slider::TwoValueHorizontal);
    grainLengthSlider.setRange(RANGE_LENGTH_MIN, RANGE_LENGTH_MAX);
    grainLengthSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 1, 1);
    grainLengthSlider.setMaxValue(INIT_LENGTH_MAX);
    grainLengthSlider.setMinValue(INIT_LENGTH_MIN);
    guiMap.grainLengthMax = INIT_LENGTH_MAX * SAMPLE_RATE_MS;
    guiMap.grainLengthMin = INIT_LENGTH_MIN * SAMPLE_RATE_MS;
    grainLengthSlider.addListener(this);
    
    addAndMakeVisible(grainLengthMaxLab);
    grainLengthMaxLab.setText("Max Label", NotificationType::dontSendNotification);
    grainLengthMaxLab.setJustificationType(Justification::centred);
    grainLengthMaxLab.setColour(Label::ColourIds::outlineColourId, Colour(144, 152, 154));
    
    // Setup grain rate
    addAndMakeVisible(grainRateTitLab);
    grainRateTitLab.setText("Grain Transposition", NotificationType::dontSendNotification);
    grainRateTitLab.setJustificationType(Justification::centred);
    
    addAndMakeVisible(grainRateBox);
    grainRateBox.addSectionHeading("Boid Parameter to Bind to Transposition");
    grainRateBox.addItemList(StringArray(boidStrings), 1);
    grainRateBox.setSelectedId(1);
    
    addAndMakeVisible(grainRateMinLab);
    grainRateMinLab.setText("Min Label", NotificationType::dontSendNotification);
    grainRateMinLab.setJustificationType(Justification::centred);
    grainRateMinLab.setColour(Label::ColourIds::outlineColourId, Colour(144, 152, 154));
    
    addAndMakeVisible(grainRateSlider);
    grainRateSlider.setSliderStyle(Slider::TwoValueHorizontal);
    grainRateSlider.setRange(RANGE_RATE_MIN, RANGE_RATE_MAX);
    grainRateSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 1, 1);
    grainRateSlider.setMaxValue(INIT_RATE_MAX);
    grainRateSlider.setMinValue(INIT_RATE_MIN);
    guiMap.grainRateMax = INIT_RATE_MAX;
    guiMap.grainRateMin = INIT_RATE_MIN;
    grainRateSlider.addListener(this);
    
    addAndMakeVisible(grainRateMaxLab);
    grainRateMaxLab.setText("Max Label", NotificationType::dontSendNotification);
    grainRateMaxLab.setJustificationType(Justification::centred);
    grainRateMaxLab.setColour(Label::ColourIds::outlineColourId, Colour(144, 152, 154));
    
    
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 800);
    
    // Register audio formats
    formatManager.registerBasicFormats();
    
    // Create a new audioComponent
    audioComponent = new AudioComponent();
    
    // Start the thread to manage opening files safely
    startThread();
}

MainComponent::~MainComponent()
{
    // Destroy audioComponent which then shuts down audio and stops scheduler thread
    delete audioComponent;
    stopThread(500);
}

//==============================================================================

void MainComponent::run()
{
    // Check whether there are any buffers to be free.
    
    // Because it is running in a different thread we can wait for 500ms here without
    // blocking any other code from executing
    while (!threadShouldExit())
    {
        //std::cout << "Background Thread Run called" << std::endl;
        checkForPathToOpen();
        checkForBuffersToFree();
        wait(500);
    }
}

void MainComponent::checkForBuffersToFree()
{
    audioComponent->checkBuffers();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
    
    // Get area of current window
    auto r = getLocalBounds();
    auto amountPerSection = r.getHeight() / numSections;
    
    // Draw dividing line between sections
    auto localRecSize = r.removeFromTop(amountPerSection);
    g.setColour (Colours::black);
    for (auto i = 1; i < numSections; ++i)
    {
        Line<float> line (Point<float> (localRecSize.getX() + 100, localRecSize.getBottom() * i),
                          Point<float> (localRecSize.getRight() - 100, localRecSize.getBottom() * i));
        g.drawLine (line, 1.0f);
    }
    
    // Get the local dimensions and the size of each GUI section
    r = getLocalBounds();
    
    // Must take away top section before the thumbnail (not sure of a way round this yet)
    r.removeFromTop(amountPerSection);
    // Get the thumbnail section
    auto audioThumbSection = r.removeFromTop(amountPerSection);
    
    if (thumbnail.getNumChannels() == 0)
    {
        g.setColour(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
        g.fillRect(audioThumbSection.reduced(10));
        g.setColour(Colours::white);
        g.drawFittedText("No File Loaded", audioThumbSection.reduced(10), Justification::centred, 1.0f);
    }
    else
    {
        g.setColour(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
        g.fillRect(audioThumbSection.reduced(10));
        g.setColour(Colours::white);                                    
        thumbnail.drawChannels (g,
                                audioThumbSection.reduced(10),
                                0.0,                                    // start time
                                thumbnail.getTotalLength(),             // end time
                                1.0f);                                  // vertical zoom
    }
    
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    
    // Get the local dimensions of the main compotent so we can divide the GUI up inside
    auto r = getLocalBounds();
    // Currently 8 elements so split up the height of the main component (r) into 8
    auto amountPerSection = r.getHeight() / numSections;
    
    // Setup open and clear buffer button dimensions
    auto openSection = r.removeFromTop(amountPerSection / 2);
    openButton.setBounds(openSection.getX() + 10, openSection.getY() + 10, openSection.getWidth() - 20, openSection.getHeight() - 15);
    
    auto clearSection = r.removeFromTop(amountPerSection / 2);
    clearButton.setBounds(clearSection.getX() + 10, clearSection.getY() + 5, clearSection.getWidth() - 20, clearSection.getHeight() - 15);
    
    // Deal with audio thumbnail section (Since this is drawn in the paint() function this is just a dud section)
    r.removeFromTop(amountPerSection);
    
    // Setup master volume section
    auto masterVolTitSection = r.removeFromTop(amountPerSection / 2);
    masterGrainVolTitLab.setBounds(masterVolTitSection.getX() + 10, masterVolTitSection.getY() + 10,
                                   masterVolTitSection.getWidth() - 20, masterVolTitSection.getHeight() - 15);
    
    auto masterVolSliSection = r.removeFromTop(amountPerSection / 2);
    auto masterVolWidth = masterVolSliSection.getWidth();
    auto masterVolLabelSection = masterVolSliSection.removeFromLeft(masterVolWidth / 8);
    masterGrainVolValLab.setBounds(masterVolLabelSection.getX() + 10, masterVolLabelSection.getY() + 5,
                                   masterVolLabelSection.getWidth() - 15, masterVolLabelSection.getHeight() - 15);
    
    masterGrainVolSli.setBounds(masterVolSliSection.getX(), masterVolSliSection.getY() + 5,
                                masterVolSliSection.getWidth() - 15, masterVolSliSection.getHeight() - 15);
    
    // Setup envelope selection and other combobox's that may be necessary
    auto envelopeTitSection = r.removeFromTop(amountPerSection / 2);
    envSelectTitLab.setBounds(envelopeTitSection.getX() + 10, envelopeTitSection.getY() + 10,
                              envelopeTitSection.getWidth() - 20, envelopeTitSection.getHeight() - 15);
    
    auto envelopeSelSection = r.removeFromTop(amountPerSection / 2);
    envSelectBox.setBounds(envelopeSelSection.getX() + 10, envelopeSelSection.getY() + 5,
                           envelopeSelSection.getWidth() - 20, envelopeSelSection.getHeight() - 15);
    
    
    // Setup start position section

    auto startPosTitSection = r.removeFromTop(amountPerSection / 2);
    
    auto startPosTitSectionLeft = startPosTitSection.removeFromLeft(startPosTitSection.getWidth() / 2);
    grainStartTitLab.setBounds(startPosTitSectionLeft.getX() + 10, startPosTitSectionLeft.getY() + 10,
                                   startPosTitSectionLeft.getWidth() - 15, startPosTitSectionLeft.getHeight() - 15);
    
    auto startPosTitSectionRight = startPosTitSection;
    grainStartBox.setBounds(startPosTitSectionRight.getX() + 5, startPosTitSectionRight.getY() + 10,
                            startPosTitSectionRight.getWidth() - 15, startPosTitSectionRight.getHeight() - 15);
    
    auto startPosSliSection = r.removeFromTop(amountPerSection / 2);
    auto startPosWidth = startPosSliSection.getWidth();
    auto startPosLeftLabelSection = startPosSliSection.removeFromLeft(startPosWidth / 8);
    grainStartMinLab.setBounds(startPosLeftLabelSection.getX() + 10, startPosLeftLabelSection.getY() + 5,
                               startPosLeftLabelSection.getWidth() - 15, startPosLeftLabelSection.getHeight() - 15);
    
    auto startPosCentreSection = startPosSliSection.removeFromLeft((startPosWidth / 8) * 6);
    grainStartSlider.setBounds(startPosCentreSection.getX(), startPosCentreSection.getY() + 5,
                                startPosCentreSection.getWidth(), startPosCentreSection.getHeight() - 15);
    
    grainStartMaxLab.setBounds(startPosSliSection.getX() + 5, startPosSliSection.getY() + 5,
                               startPosSliSection.getWidth() - 15, startPosSliSection.getHeight() - 15);
    
    // Setup initialOnset slider
    auto onsetTitSection = r.removeFromTop(amountPerSection / 2);
    
    auto onsetTitSectionLeft = onsetTitSection.removeFromLeft(onsetTitSection.getWidth() / 2);
    grainOnsetTitLab.setBounds(onsetTitSectionLeft.getX() + 10, onsetTitSectionLeft.getY() + 10,
                               onsetTitSectionLeft.getWidth() - 15, onsetTitSectionLeft.getHeight() - 15);
    
    auto onsetTitSectionRight = onsetTitSection;
    grainOnsetBox.setBounds(onsetTitSectionRight.getX() + 5, onsetTitSectionRight.getY() + 10,
                            onsetTitSectionRight.getWidth() - 15, onsetTitSectionRight.getHeight() - 15);
    
    auto onsetSliSection = r.removeFromTop(amountPerSection / 2);
    auto onsetWidth = onsetSliSection.getWidth();
    auto onsetLeftLabelSection = onsetSliSection.removeFromLeft(onsetWidth / 8);
    grainOnsetMinLab.setBounds(onsetLeftLabelSection.getX() + 10, onsetLeftLabelSection.getY() + 5,
                               onsetLeftLabelSection.getWidth() - 15, onsetLeftLabelSection.getHeight() - 15);
    
    auto onsetCentreSection = onsetSliSection.removeFromLeft((onsetWidth / 8) * 6);
    grainOnsetSlider.setBounds(onsetCentreSection.getX(), onsetCentreSection.getY() + 5,
                               onsetCentreSection.getWidth(), onsetCentreSection.getHeight() - 15);
    
    grainOnsetMaxLab.setBounds(onsetSliSection.getX() + 5, onsetSliSection.getY() + 5,
                               onsetSliSection.getWidth() - 15, onsetSliSection.getHeight() - 15);
     
    
    // Setup length slider
    auto LengthTitSection = r.removeFromTop(amountPerSection / 2);
    
    auto LengthTitSectionLeft = LengthTitSection.removeFromLeft(LengthTitSection.getWidth() / 2);
    grainLengthTitLab.setBounds(LengthTitSectionLeft.getX() + 10, LengthTitSectionLeft.getY() + 10,
                               LengthTitSectionLeft.getWidth() - 15, LengthTitSectionLeft.getHeight() - 15);
    
    auto LengthTitSectionRight = LengthTitSection;
    grainLengthBox.setBounds(LengthTitSectionRight.getX() + 5, LengthTitSectionRight.getY() + 10,
                            LengthTitSectionRight.getWidth() - 15, LengthTitSectionRight.getHeight() - 15);
    
    auto LengthSliSection = r.removeFromTop(amountPerSection / 2);
    auto LengthWidth = LengthSliSection.getWidth();
    auto LengthLeftLabelSection = LengthSliSection.removeFromLeft(LengthWidth / 8);
    grainLengthMinLab.setBounds(LengthLeftLabelSection.getX() + 10, LengthLeftLabelSection.getY() + 5,
                               LengthLeftLabelSection.getWidth() - 15, LengthLeftLabelSection.getHeight() - 15);
    
    auto LengthCentreSection = LengthSliSection.removeFromLeft((LengthWidth / 8) * 6);
    grainLengthSlider.setBounds(LengthCentreSection.getX(), LengthCentreSection.getY() + 5,
                               LengthCentreSection.getWidth(), LengthCentreSection.getHeight() - 15);
    
    grainLengthMaxLab.setBounds(LengthSliSection.getX() + 5, LengthSliSection.getY() + 5,
                               LengthSliSection.getWidth() - 15, LengthSliSection.getHeight() - 15);
    
    
    // Setup rate slider
    auto RateTitSection = r.removeFromTop(amountPerSection / 2);
    
    auto RateTitSectionLeft = RateTitSection.removeFromLeft(RateTitSection.getWidth() / 2);
    grainRateTitLab.setBounds(RateTitSectionLeft.getX() + 10, RateTitSectionLeft.getY() + 10,
                                RateTitSectionLeft.getWidth() - 15, RateTitSectionLeft.getHeight() - 15);
    
    auto RateTitSectionRight = RateTitSection;
    grainRateBox.setBounds(RateTitSectionRight.getX() + 5, RateTitSectionRight.getY() + 10,
                             RateTitSectionRight.getWidth() - 15, RateTitSectionRight.getHeight() - 15);
    
    auto RateSliSection = r.removeFromTop(amountPerSection / 2);
    auto RateWidth = RateSliSection.getWidth();
    auto RateLeftLabelSection = RateSliSection.removeFromLeft(RateWidth / 8);
    grainRateMinLab.setBounds(RateLeftLabelSection.getX() + 10, RateLeftLabelSection.getY() + 5,
                                RateLeftLabelSection.getWidth() - 15, RateLeftLabelSection.getHeight() - 15);
    
    auto RateCentreSection = RateSliSection.removeFromLeft((RateWidth / 8) * 6);
    grainRateSlider.setBounds(RateCentreSection.getX(), RateCentreSection.getY() + 5,
                                RateCentreSection.getWidth(), RateCentreSection.getHeight() - 15);
    
    grainRateMaxLab.setBounds(RateSliSection.getX() + 5, RateSliSection.getY() + 5,
                                RateSliSection.getWidth() - 15, RateSliSection.getHeight() - 15);
    
}

void MainComponent::openButtonClicked()
{
    FileChooser chooser("Choose a WAV file shorter than 20 seconds", {}, "*.wav", "*.aiff");
    
    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        auto path = file.getFullPathName();
        chosenPath.swapWith(path);
        // Wake up the background thread so that it can load the audio file
        notify();
    }
}

void MainComponent::clearButtonClicked()
{
    audioComponent->clearCurrentBuffer();
}

void MainComponent::checkForPathToOpen()
{
    String pathToOpen;
    pathToOpen.swapWith(chosenPath);
    
    if (pathToOpen.isNotEmpty())
    {
        File file(pathToOpen);
        std::unique_ptr<AudioFormatReader> reader(formatManager.createReaderFor(file));
        
        if (reader.get() != nullptr)
        {
            auto duration = reader->lengthInSamples/ reader->sampleRate;
            
            if (duration < 20)
            {
                ReferenceCountedBuffer::Ptr newBuffer = new ReferenceCountedBuffer(file.getFileName(),
                                                                                   reader->numChannels,
                                                                                   (int)reader->lengthInSamples);
                reader->read(newBuffer->getAudioSampleBuffer(),
                             0,
                             (int)reader->lengthInSamples,
                             0,
                             true,
                             true);
                
            
                // Use method in AudioComponent to set a component in here
                audioComponent->setAudioBuffers(newBuffer);
                
                // Set the audio thumbnail to the new waveform
                thumbnail.setSource(new FileInputSource(file));
                
                //std::cout << "samples in buffer: " << currentBuffer->getNumSamples() << std::endl;
            }
            else
            {
                // File longer than 20 seconds so error
                std::cerr << "File longer than set limit of 20s" << std::endl;
            }
        }
    }
}

void MainComponent::changeListenerCallback(ChangeBroadcaster *source)
{
    // If a callback is received from the thumbnail then call the thumbnail changed
    if (source == &thumbnail) thumbnailChanged();
}

void MainComponent::thumbnailChanged()
{
    // When the thumbnail is changed then call resize as this has the drawing code for the thumbnail
    repaint();
}

void MainComponent::sliderValueChanged(Slider* slider)
{
    // Since this is called any time any slider is changed, we must iterate through all sliders to check which
    // one is responsible for the callback.
    // Once we have identified the calling slider, set the relevant labels and move the value into the GUI values
    // reference struct so that the audio thread can access it.
    if (slider == &masterGrainVolSli)
    {
        // Get the value from the slider
        float sliderVal = slider->getValueObject().getValue();
        
        // Set the slider label in decibels
        auto decibel = Decibels::gainToDecibels(sliderVal);
        masterGrainVolValLab.setText(Decibels::toString(decibel), NotificationType::dontSendNotification);
        
        // This one-thumb slider has a label built in so just set value in guiMap
        guiMap.grainMasterVol = sliderVal;
    }
    else if (slider == &grainStartSlider)
    {
        // Get the values from the slider
        int sliderMin = (int)slider->getMinValueObject().getValue();
        int sliderMax = (int)slider->getMaxValueObject().getValue();
        
        // Set the slider labels with the values from the slider
        grainStartMinLab.setText(String(sliderMin) + "%", NotificationType::dontSendNotification);
        grainStartMaxLab.setText(String(sliderMax) + "%", NotificationType::dontSendNotification);
        
        // Set the relevant value in the GUI values struct
        guiMap.grainStartPosMin = sliderMin;
        guiMap.grainStartPosMax = sliderMax;
    }
    else if (slider == &grainOnsetSlider)
    {
        // Get the values from the slider
        int sliderMin = (int)slider->getMinValueObject().getValue();
        int sliderMax = (int)slider->getMaxValueObject().getValue();
        
        // Set the slider labels with the valiues from the slider
        grainOnsetMinLab.setText(String(sliderMin) + "ms", NotificationType::dontSendNotification);
        grainOnsetMaxLab.setText(String(sliderMax) + "ms", NotificationType::dontSendNotification);
        
        // Set the relevant value in the GUI values struct
        guiMap.grainOnsetMin = sliderMin * SAMPLE_RATE_MS;
        guiMap.grainOnsetMax = sliderMax * SAMPLE_RATE_MS;
    }
    else if (slider == &grainLengthSlider)
    {
        // Get the values from the slider
        int sliderMin = (int)slider->getMinValueObject().getValue();
        int sliderMax = (int)slider->getMaxValueObject().getValue();
        
        // Set the slider labels with the values from the slider
        grainLengthMinLab.setText(String(sliderMin) + "ms", NotificationType::dontSendNotification);
        grainLengthMaxLab.setText(String(sliderMax) + "ms", NotificationType::dontSendNotification);
        
        // Set the relevant values in the GUI values struct
        guiMap.grainLengthMin = sliderMin * SAMPLE_RATE_MS;
        guiMap.grainLengthMax = sliderMax * SAMPLE_RATE_MS;
    }
    else if (slider == &grainRateSlider)
    {
        // Get the values from the slider
        float sliderMin = slider->getMinValueObject().getValue();
        float sliderMax = slider->getMaxValueObject().getValue();
        
        // Set the slider labels with the values from the slider
        grainRateMinLab.setText(String(sliderMin, 1), NotificationType::dontSendNotification);
        grainRateMaxLab.setText(String(sliderMax, 1), NotificationType::dontSendNotification);
        
        // Set the relevant values in the GUI values struct
        guiMap.grainRateMin = sliderMin;
        guiMap.grainRateMax = sliderMax;
    }
    else
    {
        std::cout << "Slider not recognized" << std::endl;
    }
}

guiMap_t* MainComponent::getGuiMap()
{
    return &this->guiMap;
}
