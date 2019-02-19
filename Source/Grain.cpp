/*
  ==============================================================================

    Grain.cpp
    Created: 9 Feb 2019 4:43:00pm
    Author:  Jack Cripps

  ==============================================================================
*/

#include "Grain.h"

//==============================================================================

Grain::Grain(int onset, const int length, int startPosition)
    : onset(onset), length(length), startPosition(startPosition), envPosition(10), envBuffer(new Array<float, CriticalSection>)
{
    this->constructEnvelope(envBuffer, kTukey, length, 1.0);
    //std::cout << "grain created" << std::endl;
};

Grain::Grain()
    : envBuffer(new Array<float, CriticalSection>)
{
    onset = 0;
    length = 0;
    startPosition = 0;
    envPosition = 0;
    
};

Grain::~Grain()
{

};

//==============================================================================
void Grain::processAudio(const AudioSourceChannelInfo& bufferToFill,
                         AudioSampleBuffer &buffer,
                         int nInputChannels,
                         int nOutputChannels,
                         int nBufToFillSamples,
                         int nBufSamples,
                         long long int time)
{
    for (auto channel = 0; channel < nOutputChannels; ++channel)
    {
        auto* channelData = bufferToFill.buffer->getWritePointer(channel);
        const auto* fileData = buffer.getReadPointer(channel % nInputChannels);
        
        auto position = (time - onset) + startPosition;
        
        channelData[time % nBufToFillSamples] += (fileData[position % nBufSamples] *
                                                  this->envBuffer->getUnchecked((length - 1) - ((onset + length) - (int)time)));
    }
    //std::cout <<"time: " << time << "onset + length: " << onset + length << "Array Index: " << (length - 1) - ((onset + length) - (int)time) << std::endl;
    envPosition++;
}

void Grain::constructEnvelope(std::shared_ptr<Array<float, CriticalSection>> envBuffer, envType env, const int nSamples, float amplitude)
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
                envBuffer->add(amplitude * 1.0);
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
                envBuffer->add(amplitude * (f < 1 ? f : 1));
            }
            // Fill last element with a value of 0
            envBuffer->add(amplitude * 0.0);
            break;
            
        default:
            
            std::cout << "Envelope Generation Error" << std::endl;
            break;
    }
}

void Grain::printEnvelope()
{
    String string;
    for (auto i = 0; i < this->envBuffer->size(); ++i)
    {
        String temp(envBuffer->getUnchecked(i));
        string += temp;
        string += ", ";
    }
    std::cout << string << "\n\n" << std::endl;
}
