/*
  ==============================================================================

    Grain.cpp
    Created: 9 Feb 2019 4:43:00pm
    Author:  Jack Cripps

  ==============================================================================
*/

#include "Grain.h"

//==============================================================================

Grain::Grain(int onset, const int length, int startPosition, envType env, float amplitude, float panPosition, float playbackRate)
    : onset(onset), length(length), startPosition(startPosition), envelope(env),
      amplitude(amplitude), panPosition(panPosition), playbackRate(playbackRate), envBuffer(new Array<float, CriticalSection>)
{
    // Set amplitude values to be between 0.0 and 1.0 inclusive if constructed
    // values lie outside of this range
    if (amplitude > 1.0) amplitude = 1.0;
    if (amplitude < 0.0) amplitude = 0.0;
    
    // Bind pan position to -1.0, 1.0 range inclusive
    if (panPosition > 1.0) panPosition = 1.0;
    if (panPosition < -1.0) panPosition = -1.0;
    
    // No support for negative or 0 playback rates
    if (playbackRate < 0.001) playbackRate = 0.001;
    
    // If length is less than 10 samples then always use a rectangular envelope
    if (length < 10) envelope = kRectangle;
    
#if SCHED_ENV
    this->constructEnvelope(envBuffer);
#endif
    //std::cout << "grain created" << std::endl;
};

Grain::Grain()
    : envBuffer(new Array<float, CriticalSection>)
{
    onset = 0;
    length = 0;
    startPosition = 0;
    amplitude = 0.0;
    panPosition = 0.0;
    playbackRate = 1.0;
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
        
        float position = (time - onset) * playbackRate;
        int ceilSample = (int)std::ceil(position);
        
        float eta = ceilSample - position;
        
        float y0 = fileData[(ceilSample + startPosition) % nBufSamples];
        float y1 = fileData[((ceilSample + startPosition) - 1) % nBufSamples];
        float y2 = fileData[((ceilSample + startPosition) - 2) % nBufSamples];
        float y3 = fileData[((ceilSample + startPosition) - 3) % nBufSamples];
        
        //float outputSample = linearInterpolant(eta, currentSamp, previousSamp);
        float outputSample = cubicInterpolant(eta, y3, y2, y1, y0);
        
        if (channel == 0)
        {
#if SCHED_ENV
            channelData[time % nBufToFillSamples] += outputSample *
                                                      this->envBuffer->getUnchecked((length - 1) - ((onset + length) - (int)time))) *
            ((MathConstants<float>::sqrt2 / 2) *(cos(panPosition * (MathConstants<float>::pi / 4)) - sin(panPosition * (MathConstants<float>::pi / 4))));
#else
            
            channelData[time % nBufToFillSamples] += outputSample *
                                                      calculateEnvAtCurrentSample(time) *
            ((MathConstants<float>::sqrt2 / 2) *(cos(panPosition * (MathConstants<float>::pi / 4)) - sin(panPosition * (MathConstants<float>::pi / 4))));
            
#endif
        }
        else if (channel == 1)
        {
#if SCHED_ENV
            channelData[time % nBufToFillSamples] += outputSample *
                                                      this->envBuffer->getUnchecked((length - 1) - ((onset + length) - (int)time))) *
            ((MathConstants<float>::sqrt2 / 2) *(cos(panPosition * (MathConstants<float>::pi / 4)) + sin(panPosition * (MathConstants<float>::pi / 4))));
#else
            
            channelData[time % nBufToFillSamples] += outputSample *
                                                      calculateEnvAtCurrentSample(time) *
            ((MathConstants<float>::sqrt2 / 2) *(cos(panPosition * (MathConstants<float>::pi / 4)) + sin(panPosition * (MathConstants<float>::pi / 4))));
            
#endif
        }
        else
        {
            
        }
    }
    //std::cout <<"time: " << time << "onset + length: " << onset + length << "Array Index: " << (length - 1) - ((onset + length) - (int)time) << std::endl;
}

void Grain::constructEnvelope(std::shared_ptr<Array<float, CriticalSection>> envBuffer)
{
    int i = 0;
    switch (envelope)
    {
        case kRectangle:
            
            for (i = 0; i < length; ++i)
            {
                // Create rectangular window in envBuffer, so just amplitude * constant of 1
                envBuffer->add(amplitude * 1.0);
            }
            break;
            
        case kTukey:
            
            for (i = 0; i < length-1; ++i)
            {
                // Basis of Tukey Window algorithm from:
                // http://michaelkrzyzaniak.com/AudioSynthesis/2_Audio_Synthesis/11_Granular_Synthesis/1_Window_Functions/
                // Slightly modified to fit implementation
                // [Accessed: 16 February 2019]
                float truncationHeight = 0.5;
                float f = 1 / (2 * truncationHeight) * (1 - cos(2 * MathConstants<float>::pi * i / (length - 1)));
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

float Grain::calculateEnvAtCurrentSample(long long int time)
{
    switch (envelope)
    {
        case kRectangle:
        {
            return amplitude;
            break;
        }
        case kTukey:
        {
            float f = 1 / (2 * 0.5) * (1 - cos(2 * MathConstants<float>::pi *  ((length - 1) - ((onset + length) - (int)time)) / length));
            return amplitude * (f < 1 ? f : 1);
            break;
        }
        default:
        {
            std::cout << "Envelope Generation Error" << std::endl;
            break;
        }
    }
    
    return 0;
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

float Grain::linearInterpolant(float eta, float currentSample, float previousSample)
{
    // One-multiply linear interpolation from: https://www.dsprelated.com/freebooks/pasp/Linear_Interpolation.html
    
    float output = previousSample + eta * (currentSample - previousSample);
    //std::cout << "currentSamp: " << currentSample << " prevSamp: " << previousSample << " eta: " << eta << " out: " << output << std::endl;
    
    return output;
}

float Grain::cubicInterpolant(float eta, float y3, float y2, float y1, float y0)
{
    // This is nicked straight from supercollider implementation
    // 4-point, 3rd-order Hermite (x-form)
    float c0 = y1;
    float c1 = 0.5f * (y2 - y0);
    float c2 = y0 - 2.5f * y1 + 2.f * y2 - 0.5f * y3;
    float c3 = 0.5f * (y3 - y0) + 1.5f * (y1 - y2);
    
    return ((c3 * eta + c2) * eta + c1) * eta + c0;
}
