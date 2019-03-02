/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
    : Thread("Background Thread")
{
    // Must set size of child components before size of main component
    addAndMakeVisible (openButton);
    openButton.setButtonText ("Open...");
    openButton.onClick = [this] { openButtonClicked(); };   // Lambda function
    
    addAndMakeVisible (clearButton);
    clearButton.setButtonText ("Clear Buffer");
    clearButton.onClick = [this] { clearButtonClicked(); };   // Lambda function
    
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
    
    // Register audio formats
    formatManager.registerBasicFormats();
    
    // Create a new audioComponent
    audioComponent = new AudioComponent();
    
    // Start the thread to manage opening files safely
    startThread();
    
    // Add all the XML files to the boidStack
    for (auto i = 0; i < 72; ++i)
    {
        const std::string tempString = ("/Users/jackcripps/Documents/MME_Proj/Flocking data 9030 250219/" + std::to_string(i) + "_flocking_data.xml");
        std::vector<Boids::boid_struct> boidStruct = parseXMLBOID(tempString.data(), &boidRanges);
        boidStructStack.push_back(boidStruct);
    }
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
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    
    openButton.setBounds (10, 10, getWidth() - 20, 20);
    clearButton.setBounds(10, 40, getWidth() - 20, 20);
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
