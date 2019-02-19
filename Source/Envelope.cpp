/*
  ==============================================================================

    Envelope.cpp
    Created: 16 Feb 2019 11:51:05am
    Author:  Jack Cripps

  ==============================================================================
*/

#include "Envelope.h"

//==============================================================================

Envelope::Envelope()
{
    //validateParameters();
    //constructEnvelope();
};

Envelope::~Envelope()
{
    
};

//==============================================================================

void Envelope::validateParameters()
{
    /*
    // Set amplitude values to be between 0.0 and 1.0 inclusive if constructed
    // values lie outside of this range
    if (amplitude > 1.0) amplitude = 1.0;
    if (amplitude < 0.0) amplitude = 0.0;
    
    // If nSamples is less than 10 then only use a rectangular envelope
    if (nSamples < 10) env = kRectangle;
     */
}

void Envelope::constructEnvelope(std::shared_ptr<std::vector<float>> envBuffer, envType env, const int nSamples, float amplitude)
{
    // Set amplitude values to be between 0.0 and 1.0 inclusive if constructed
    // values lie outside of this range
    if (amplitude > 1.0) amplitude = 1.0;
    if (amplitude < 0.0) amplitude = 0.0;
    
    // If nSamples is less than 10 then only use a rectangular envelope
    if (nSamples < 10) env = kRectangle;
    
    int i = 0;
    switch (env)
    {
        case kRectangle:
            
            for (i = 0; i < nSamples; ++i)
            {
                // Create rectangular window in envBuffer, so just amplitude * constant of 1
                envBuffer->push_back(amplitude * 1.0);
            }
            break;
            
        case kTukey:
    
            for (i = 0; i < nSamples-1; ++i)
            {
                // Basis of Tukey Window algorithm from:
                // http://michaelkrzyzaniak.com/AudioSynthesis/2_Audio_Synthesis/11_Granular_Synthesis/1_Window_Functions/
                // Slightly modified to fit implementation
                // [Accessed: 16 February 2019]
                float truncationHeight = 0.5;
                float f = 1 / (2 * truncationHeight) * (1 - cos(2 * MathConstants<float>::pi * i / (nSamples - 1)));
                envBuffer->push_back(amplitude * (f < 1 ? f : 1));
            }
            // Fill last element with a value of 0
            envBuffer->push_back(amplitude * 0.0);
            break;
            
        default:
            
            std::cout << "Envelope Generation Error" << std::endl;
            break;
    }
}
