/*
  ==============================================================================

    ReferenceCountedBuffer.cpp
    Created: 10 Feb 2019 11:27:37am
    Author:  Jack Cripps

  ==============================================================================
*/

#include "ReferenceCountedBuffer.h"

ReferenceCountedBuffer::ReferenceCountedBuffer(const String& nameToUse, int nChannels, int nSamples)
    : name(nameToUse), buffer(nChannels, nSamples)
{
    DBG(String ("Buffer named '") + name + "' constructed. nChannels = " + String (nChannels) + ", nSamples = " + String (nSamples));
}

ReferenceCountedBuffer::~ReferenceCountedBuffer()
{
    DBG(String ("Buffer named '") + name + "' destroyed");
}

AudioSampleBuffer* ReferenceCountedBuffer::getAudioSampleBuffer()
{
    return &buffer;
}
