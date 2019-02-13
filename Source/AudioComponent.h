/*
  ==============================================================================

    AudioComponent.h
    Created: 12 Feb 2019 11:14:32am
    Author:  Jack Cripps

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Grain.h"
#include "ReferenceCountedBuffer.h"

class AudioComponent    : public AudioAppComponent,
                          private Thread
{
public:
    //==============================================================================
    AudioComponent();
    ~AudioComponent();
    
    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    //==============================================================================
    void run() override;
    
    //==============================================================================
    void setAudioBuffers(ReferenceCountedBuffer::Ptr newBuffer);
    void clearCurrentBuffer();
    void checkBuffers();
    
private:
    AudioFormatManager formatManager;
    
    long long int time;
    Array<Grain> grainStack;
    
    // Keeps hold of all buffers until certain they are no longer needed by the audio thread
    ReferenceCountedArray<ReferenceCountedBuffer> buffers;
    ReferenceCountedBuffer::Ptr currentBuffer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioComponent);
};