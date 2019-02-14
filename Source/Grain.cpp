/*
  ==============================================================================

    Grain.cpp
    Created: 9 Feb 2019 4:43:00pm
    Author:  Jack Cripps

  ==============================================================================
*/

#include "Grain.h"

Grain::Grain(int onset, int length, int startPosition)
    : onset(onset), length(length), startPosition(startPosition)
{
    
};

Grain::Grain()
{
    onset = 0;
    length = 0;
    startPosition = 0;
}

Grain::~Grain()
{
    
};

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
        
        channelData[time % nBufToFillSamples] += fileData[position/*time*/ % nBufSamples];
    }
}
