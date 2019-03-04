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
#include "parse.hpp"

#define SCHED_ENV 0

class Grain
{
public:
    //==============================================================================
    Grain(int boidID, bool isActive, unsigned long long int onset, const int length, int startPosition, envType env, float amplitude, float panPosition, float playbackRate);
    Grain();
    ~Grain();
    
    int boidID;
    unsigned long long int onset;
    int length;
    int startPosition;
    envType envelope;
    float amplitude;
    float panPosition;
    float playbackRate;
    bool isPlaying;
    bool isActive;
    
    
    //==============================================================================
    void processAudio(const AudioSourceChannelInfo& bufferToFill,
                      AudioSampleBuffer& buffer,
                      int nInputChannels,
                      int nOutputChannels,
                      int nBufToFillSamples,
                      int nBufSamples,
                      unsigned long long int time);
    
    void constructEnvelope(std::shared_ptr<Array<float, CriticalSection>> envBuffer);
    
    float calculateEnvAtCurrentSample(unsigned long long int time);
    
    void printEnvelope();
    
    //==============================================================================
    float linearInterpolant(float eta, float currentSample, float previousSample);
    
    float cubicInterpolant(float eta, float y3, float y2, float y1, float y0);
    
private:
    //==============================================================================
#if SCHED_ENV
    std::shared_ptr<Array<float, CriticalSection>> envBuffer;
#endif
};
