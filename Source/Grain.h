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

#define SCHED_ENV 0

class Grain
{
public:
    //==============================================================================
    Grain(int onset, const int length, int startPosition, envType env, float amplitude, float panPosition, float playbackRate);
    Grain();
    ~Grain();
    
    int onset;
    int length;
    int startPosition;
    envType envelope;
    float amplitude;
    float panPosition;
    float playbackRate;
    
    
    //==============================================================================
    void processAudio(const AudioSourceChannelInfo& bufferToFill,
                      AudioSampleBuffer& buffer,
                      int nInputChannels,
                      int nOutputChannels,
                      int nBufToFillSamples,
                      int nBufSamples,
                      long long int time);
    
    void constructEnvelope(std::shared_ptr<Array<float, CriticalSection>> envBuffer);
    
    float calculateEnvAtCurrentSample(long long int time);
    
    void printEnvelope();
    
    float linearInterpolant(float eta, float currentSample, float previousSample);
    
    float cubicInterpolant(float eta, float y3, float y2, float y1, float y0);
    
private:
    //==============================================================================
    std::shared_ptr<Array<float, CriticalSection>> envBuffer;
};
