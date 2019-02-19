/*
  ==============================================================================

    Grain.h
    Created: 9 Feb 2019 4:43:00pm
    Author:  Jack Cripps

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Envelope.h"

class Grain
{
public:
    //==============================================================================
    Grain(int onset, const int length, int startPosition);
    Grain();
    ~Grain();
    
    int onset;
    int length;
    int startPosition;
    
    //==============================================================================
    void processAudio(const AudioSourceChannelInfo& bufferToFill,
                      AudioSampleBuffer& buffer,
                      int nInputChannels,
                      int nOutputChannels,
                      int nBufToFillSamples,
                      int nBufSamples,
                      long long int time);
    
    void constructEnvelope(std::shared_ptr<Array<float, CriticalSection>> envBuffer,
                           envType env,
                           const int nSamples,
                           float amplitude);
    
    void printEnvelope();
    
private:
    //==============================================================================
    unsigned int envPosition;
    std::shared_ptr<Array<float, CriticalSection>> envBuffer;
};
