/*
  ==============================================================================

    Envelope.h
    Created: 16 Feb 2019 11:51:05am
    Author:  Jack Cripps

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

typedef enum
{
    kRectangle,
    kTukey
} envType;

class Envelope
{
public:
    //==============================================================================
    Envelope();
    ~Envelope();
    
    //==============================================================================
    void validateParameters();
    void constructEnvelope(std::shared_ptr<std::vector<float>> envBuffer, envType env, const int length, const float amplitude);
    
private:
    /*
    envType env;
    int nSamples;
    float amplitude;
    std::unique_ptr<std::vector<float>> envBuffer;
    */
};
