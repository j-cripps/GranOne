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
#include "GuiParameters.h"
#include <boost/lockfree/spsc_queue.hpp>

#define SCHED_THREAD 0
#define FILE_BOIDS 0
#define GRAIN_LIMIT 100

// 0 - Stereo Decode
// 1 - Ambisonic Decode to Disk
// 2 - Ambisonic Decode to Output

class AudioComponent    : public AudioAppComponent,
                          private Thread
{
public:
    //==============================================================================
    AudioComponent(guiMap_t* guiMap);
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
    
    //==============================================================================
    void setGuiMap(guiMap_t* guiMap);
    void refreshGuiMap();
    
private:
    //==============================================================================
    int wrap(int val, const int low, const int high);
    int intMap(int x, int inMin, int inMax, int outMin, int outMax);
    float floatMap(float x, float inMin, float inMax, float outMin, float outMax);
    int intMapInverse(int x, int inMin, int inMax, int outMin, int outMax);
    float floatMapInverse(float x, float inMin, float inMax, float outMin, float outMax);
    
    //==============================================================================
    void setupGrainStack();
    
    void networkFunction();
    
    //==============================================================================
    Grain createGrainFromBoid(std::vector<Boids::boidParam_t>* boid, std::vector<Boids::boidParam_t>* range);
    void generateGrainFromBoid(Grain* grain, std::vector<Boids::boidParam_t>* boid, std::vector<Boids::boidParam_t>* range);
    void updateGrainFromBoid(Grain* grain, std::vector<Boids::boidParam_t>* boid, std::vector<Boids::boidParam_t>* range);
    
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
    std::vector<std::vector<std::vector<Boids::boidParam_t>>> boidStructStack;
    
    std::vector<Boids::boidParam_t> boidRanges;
    unsigned int boidFrame;
    Time timeCheck;
    uint32_t millisecondCount;
    guiMap_t localGuiMap;
    guiMap_t* masterGuiMap;
    
    StreamingSocket socket;
    boost::lockfree::spsc_queue<std::vector<std::vector<Boids::boidParam_t>>> boidQueue{4};
    char buffer[50000];
    std::vector<Boids::boid_struct> decoded;
    
#if DECODE_METHOD == 1
    WavAudioFormat wavFormat;
    std::unique_ptr<AudioFormatWriter> writer;
    File file;
#endif
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioComponent);
};
