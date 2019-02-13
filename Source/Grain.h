/*
  ==============================================================================

    Grain.h
    Created: 9 Feb 2019 4:43:00pm
    Author:  Jack Cripps

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Grain
{
public:
    Grain(int onset, int length, int startPosition);
    Grain();
    ~Grain();
    
    int onset;
    int length;
    int startPosition;
    
    void processAudio(const AudioSourceChannelInfo& bufferToFill,
                      AudioSampleBuffer& buffer,
                      int nInputChannels,
                      int nOutputChannels,
                      int nBufToFillSamples,
                      int nBufSamples,
                      long long int time);
};
