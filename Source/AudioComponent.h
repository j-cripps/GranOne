/*
  ==============================================================================

    AudioComponent.h
    Created: 12 Feb 2019 11:14:32am
    Author:  Jack Cripps

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Grain.h"
#include "ReferenceCountedBuffer.h"
#include "sys/kdebug_signpost.h"
#include "parse.hpp"

#define SCHED_THREAD 0
#define GRAIN_LIMIT 100

class AudioComponent    : public AudioAppComponent,
                          private Thread
{
public:
    //==============================================================================
    AudioComponent();
    ~AudioComponent();
    
    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    //==============================================================================
    void run() override;
    
    //==============================================================================
    void setAudioBuffers(ReferenceCountedBuffer::Ptr newBuffer);
    void clearCurrentBuffer();
    void checkBuffers();
    
private:
    //==============================================================================
    int wrap(int val, const int low, const int high);
    int intMap(int x, int inMin, int inMax, int outMin, int outMax);
    float floatMap(float x, float inMin, float inMax, float outMin, float outMax);
    int intMapInverse(int x, int inMin, int inMax, int outMin, int outMax);
    float floatMapInverse(float x, float inMin, float inMax, float outMin, float outMax);
    
    //==============================================================================
    void setupGrainStack();
    
    //==============================================================================
    Grain createGrainFromBoid(Boids::boid_struct* boid, Boids::boid_range_t* range);
    void generateGrainFromBoid(Grain* grain, Boids::boid_struct* boid, Boids::boid_range_t* range);
    void updateGrainFromBoid(Grain* grain, Boids::boid_struct* boid, Boids::boid_range_t* range);
    
    //==============================================================================
    AudioFormatManager formatManager;
    
    unsigned long long int time;
#if SCHED_THREAD
    Array<Grain, CriticalSection> grainStack;
#else
    std::vector<Grain> grainStack;
#endif
    // Keeps hold of all buffers until certain they are no longer needed by the audio thread
    ReferenceCountedArray<ReferenceCountedBuffer> buffers;
    ReferenceCountedBuffer::Ptr currentBuffer;
    
    // Boid values
    std::vector<std::vector<Boids::boid_struct>> boidStructStack;
    Boids::boid_range_t boidRanges;
    unsigned int boidFrame;
    Time timeCheck;
    uint32_t millisecondCount;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioComponent);
};
