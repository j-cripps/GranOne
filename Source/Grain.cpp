/*
  ==============================================================================

    Grain.cpp
    Created: 9 Feb 2019 4:43:00pm
    Author:  Jack Cripps

  ==============================================================================
*/

#include "Grain.h"

//==============================================================================

Grain::Grain(int boidID, bool isActive, unsigned long long int onset, const int length, int startPosition, envType env, float amplitude, float panPosition, float playbackRate, uint32_t xCoord, uint32_t yCoord, uint32_t zCoord)
    : boidID(boidID), onset(onset), length(length), startPosition(startPosition), envelope(env),
      amplitude(amplitude), panPosition(panPosition), playbackRate(playbackRate), xCoord(xCoord), yCoord(yCoord), zCoord(zCoord)
#if SCHED_ENV
, envBuffer(new Array<float, CriticalSection>)
#endif
{
    // Set amplitude values to be between 0.0 and 1.0 inclusive if constructed
    // values lie outside of this range
    if (amplitude > 1.0) amplitude = 1.0;
    if (amplitude < 0.0) amplitude = 0.0;
    
    // Bind pan position to -1.0, 1.0 range inclusive
    if (panPosition > 1.0) panPosition = 1.0;
    if (panPosition < -1.0) panPosition = -1.0;
    
    // No support for negative or 0 playback rates, so bind to sufficiently small value
    if (playbackRate < 0.001) playbackRate = 0.001;
    
    // If length is less than 10 samples then always use a rectangular envelope, as the functions differ very little
    // at this small number of samples
    if (length < 10) envelope = kRectangle;
    
    // Always starts off not playing
    isPlaying = false;
    
#if SCHED_ENV
    this->constructEnvelope(envBuffer);
#endif
    //std::cout << "grain created" << std::endl;
};

Grain::Grain()
#if SCHED_ENV
    :
envBuffer(new Array<float, CriticalSection>)
#endif
{
    onset = 0;
    length = 0;
    startPosition = 0;
    amplitude = 0.0;
    panPosition = 0.0;
    playbackRate = 1.0;
    isPlaying = false;
    envelope = kRectangle;
    xCoord = 0;
    yCoord = 0;
    zCoord = 0;
};

Grain::~Grain()
{

};

//==============================================================================
void Grain::processAudio(AudioBuffer<float>* bufferToFill,
                         AudioSampleBuffer &buffer,
                         int nInputChannels,
                         int nOutputChannels,
                         int nBufToFillSamples,
                         int nBufSamples,
                         unsigned long long int time)
{

    float azimuth = getAzimuthAngle(xCoord, yCoord, zCoord);
    float elevation = getElevationAngle(xCoord, yCoord, zCoord);
    float distance = getRadialDistance(xCoord, yCoord, zCoord, elevation);
    
    const auto* fileData = buffer.getReadPointer(0);
    
    float position = (time - onset) * playbackRate;
    int ceilSample = (int)std::ceil(position);
    
    float eta = ceilSample - position;
    
    float y0 = fileData[(ceilSample + startPosition) % nBufSamples];
    float y1 = fileData[((ceilSample + startPosition) - 1) % nBufSamples];
    float y2 = fileData[((ceilSample + startPosition) - 2) % nBufSamples];
    float y3 = fileData[((ceilSample + startPosition) - 3) % nBufSamples];
    
    //float outputSample = linearInterpolant(eta, currentSamp, previousSamp);
    float outputSample = cubicInterpolant(eta, y3, y2, y1, y0);
    
    float W = encodeW((outputSample * calculateEnvAtCurrentSample(time)));
    float X = encodeX((outputSample * calculateEnvAtCurrentSample(time)), azimuth, elevation, distance);
    float Y = encodeY((outputSample * calculateEnvAtCurrentSample(time)), azimuth, elevation, distance);
    float Z = encodeZ((outputSample * calculateEnvAtCurrentSample(time)), elevation, distance);
    float D = encodeD((outputSample * calculateEnvAtCurrentSample(time)), distance);
    
    for (auto channel = 8; channel < nOutputChannels; ++channel)
    {
        auto* channelData = bufferToFill->getWritePointer(channel);

#if DECODE_METHOD == 0
        if (channel == 0)
        {
        #if SCHED_ENV
            channelData[time % nBufToFillSamples] += outputSample *
                                                      this->envBuffer->getUnchecked((length - 1) - ((onset + length) - time))) *
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
                                                      this->envBuffer->getUnchecked((length - 1) - ((onset + length) - time))) *
            ((MathConstants<float>::sqrt2 / 2) *(cos(panPosition * (MathConstants<float>::pi / 4)) + sin(panPosition * (MathConstants<float>::pi / 4))));
        #else
            
            channelData[time % nBufToFillSamples] += outputSample *
                                                      calculateEnvAtCurrentSample(time) *
            ((MathConstants<float>::sqrt2 / 2) *(cos(panPosition * (MathConstants<float>::pi / 4)) + sin(panPosition * (MathConstants<float>::pi / 4))));
            
        #endif
        }
        else
        {
            channelData[time % nBufToFillSamples] = 0.0f;
        }
#elif DECODE_METHOD == 2
        
        
        if (channel == 8)
        {
            channelData[time % nBufToFillSamples] += decodedSpeakerSignal(W, X, Y, Z, D, 0.524, 0, distance);
        }
        else if (channel == 9)
        {
            channelData[time % nBufToFillSamples] += decodedSpeakerSignal(W, X, Y, Z, D, 1.047, 0, distance);
        }
        else if (channel == 10)
        {
            channelData[time % nBufToFillSamples] += decodedSpeakerSignal(W, X, Y, Z, D, 1.981, 0, distance);
        }
        else if (channel == 11)
        {
            channelData[time % nBufToFillSamples] += decodedSpeakerSignal(W, X, Y, Z, D, 2.775, 0, distance);
        }
        else if (channel == 12)
        {
            channelData[time % nBufToFillSamples] += decodedSpeakerSignal(W, X, Y, Z, D, -2.574, 0, distance); //3.709 +
        }
        else if (channel == 13)
        {
            channelData[time % nBufToFillSamples] += decodedSpeakerSignal(W, X, Y, Z, D, -1.309, 0, distance); //4.451 +
        }
        else if (channel == 14)
        {
            channelData[time % nBufToFillSamples] += decodedSpeakerSignal(W, X, Y, Z, D, -1.0472, 0, distance); //5.236 +
        }
        else if (channel == 15)
        {
            channelData[time % nBufToFillSamples] += decodedSpeakerSignal(W, X, Y, Z, D, -0.523, 0, distance); //5.760 +
        }
        else
        {
            channelData[time % nBufToFillSamples] = 0.0f;
            std::cout << "Ambisonic Decode Channel Error" << std::endl;
        }
        
#else
        
        std::cout << "Ambisonic Decode Compile Error" << std::endl;
        
#endif

    }
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

float Grain::calculateEnvAtCurrentSample(unsigned long long int time)
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
            // Basis of Tukey Window algorithm from:
            // http://michaelkrzyzaniak.com/AudioSynthesis/2_Audio_Synthesis/11_Granular_Synthesis/1_Window_Functions/
            // Slightly modified to fit implementation
            // [Accessed: 16 February 2019]
            float f = 1 / (2 * 0.5) * (1 - cos(2 * MathConstants<float>::pi *  ((length - 1) - ((onset + length) - time)) / length));
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
#if SCHED_ENV
    String string;
    for (auto i = 0; i < this->envBuffer->size(); ++i)
    {
        String temp(envBuffer->getUnchecked(i));
        string += temp;
        string += ", ";
    }
    std::cout << string << "\n\n" << std::endl;
#endif
}

//==============================================================================
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
