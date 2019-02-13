/*
  ==============================================================================

    ReferenceCountedBuffer.h
    Created: 10 Feb 2019 11:27:37am
    Author:  Jack Cripps

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class ReferenceCountedBuffer : public ReferenceCountedObject
{
public:
    typedef ReferenceCountedObjectPtr<ReferenceCountedBuffer> Ptr;
    
    ReferenceCountedBuffer(const String& nameToUse, int nChannels, int nSamples);
    ~ReferenceCountedBuffer();
    
    AudioSampleBuffer* getAudioSampleBuffer();
    
    int position = 0;
    
private:
    String name;
    AudioSampleBuffer buffer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReferenceCountedBuffer)
};
