/*
  ==============================================================================

    AudioComponent.cpp
    Created: 12 Feb 2019 11:14:32am
    Author:  Jack Cripps

  ==============================================================================
*/

#include "AudioComponent.h"

AudioComponent::AudioComponent()
    : Thread("Scheduler Thread")
{
    // Add all the XML files to the boidStack
    for (auto i = 0; i < 2400; ++i)
    {
        const std::string tempString = ("/Users/jackcripps/Documents/MME_Proj/Flocking data 1335 010319/" + std::to_string(i) + "_flocking_data.xml");
        std::vector<Boids::boid_struct> boidStruct = parseXMLBOID(tempString.data(), &boidRanges);
        boidStructStack.push_back(boidStruct);
    }
    
    // specify the number of input and output channels that we want to open
    setAudioChannels (0, 2);
    
    // Register audio formats
    formatManager.registerBasicFormats();
    
    // Initialise time to 0 and create a grain in the stack
    time = 0;
    boidFrame = 0;
    
    // Start the thread to manage scheduler
    startThread(8);
    
    // Create the grainStack from the first frame of the boids, before any audio starts playing.
    // Essentially starts the process from the beginning again if it's in offline mode (XML)
    if (!grainStack.empty()) grainStack.clear();
    
    for (auto i = 0; i < /*boidStructStack[boidFrame].size()*/2; ++i)
    {
        grainStack.push_back(createGrainFromBoid(&boidStructStack[0][i], &boidRanges));
    }
    // Start the boid frame count again
    boidFrame = 1;
}

AudioComponent::~AudioComponent()
{
    // This shuts down the audio device stops scheduler thread.
    shutdownAudio();
    stopThread(500);
}

void AudioComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.
    
    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.
    
    // For more details, see the help for AudioProcessor::prepareToPlay()
    std::cout << samplesPerBlockExpected << ", " << sampleRate << std::endl;
}

void AudioComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    kdebug_signpost_start(2, 0, 0, 0, 3);
    // This is the part that seperates this thread from the previous by making a new
    // copy, then checking if buffer is not null pointer
    ReferenceCountedBuffer::Ptr retainedBuffer(currentBuffer);
    if (retainedBuffer == nullptr) return;
    
    // Get the audio buffer from the reference counted buffer
    AudioSampleBuffer* buffer = retainedBuffer->getAudioSampleBuffer();
    
    const auto nInputSamples = buffer->getNumSamples();
    if (nInputSamples == 0)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
#if SCHED_THREAD
    // Create local copy of the grainstack so that it cannot be changed by another thread mid-operation
    const Array<Grain, CriticalSection> localGrainStack(grainStack);
    
    const auto nInputChannels = buffer->getNumChannels();
    const auto nOutputChannels = bufferToFill.buffer->getNumChannels();
    const auto nOutputSamplesRemaining = bufferToFill.numSamples;
    
    for (auto sample = 0; sample < nOutputSamplesRemaining; ++sample)
    {
        for (auto i = 0; i < localGrainStack.size(); ++i)
        {
            if (localGrainStack[i].onset < time)
            {
                if (time <= (localGrainStack[i].onset + localGrainStack[i].length))
                {
                    //std::cout << i << ": " << "time: " << time << "onset + length: " << (localGrainStack[i].onset + localGrainStack[i].length) << std::endl;
                    localGrainStack[i].processAudio(bufferToFill,
                                                    *buffer,
                                                    nInputChannels,
                                                    nOutputChannels,
                                                    nOutputSamplesRemaining,
                                                    nInputSamples,
                                                    time);
                }
            }
        }
        ++time;
    }
#else
    
    // This needs to be run to as close as every 50ms as possible
    uint32_t currentTime = timeCheck.getMillisecondCounter();
    if ((currentTime - millisecondCount) >= 50)
    {
        // 'Reset' the counter
        millisecondCount = currentTime;
        
        // Check if boid frame is less than boidStructStack size, if not then stop generating new grains
        if (boidFrame < boidStructStack.size())
        {
            // Process the next frame of boids
            for (auto i = 0; i < grainStack.size(); ++i)
            {
                auto grainEnd = grainStack[i].onset + grainStack[i].length;
                auto hasEnded = grainEnd < time;
                if (hasEnded)
                {
                    generateGrainFromBoid(&grainStack[i], &boidStructStack[boidFrame][i], &boidRanges);
                }
                else
                {
                    updateGrainFromBoid(&grainStack[i], &boidStructStack[boidFrame][i], &boidRanges);
                }
            }
            ++boidFrame;
        }
        
        // Iterate over all the boids
        // If the boid is not active then set the grain to not active
        // Once the scheduler has finished do boidFrame++
        
        /*
        // Delete grains
        if (grainStack.size() > 0)
        {
            for (auto i = grainStack.size() - 1; i >= 0; --i)
            {
                auto grainEnd = grainStack[i].onset + grainStack[i].length;
                auto hasEnded = grainEnd < time;
                if (hasEnded)
                {
                    grainStack.remove(i);
                }
                //std::cout   << "hasEnded: "     << hasEnded
                //      << "\tgrainEnd: "   << grainEnd
                //      << "\ttime: "       << time
                //      << std::endl;
            }
        }
        
        // Add grains
        if (currentBuffer != nullptr)
        {
            if (grainStack.size() < GRAIN_LIMIT)
            {
                
                auto nSamples = currentBuffer->getAudioSampleBuffer()->getNumSamples();
                int onset = offsetNum;
                int length = lengthNum;
                int startPosition = startNum;
                for (int i = 0; i < 1; ++i)
                {
                    //grainStack.add(Grain((int)time + onset, length, wrap(startPosition, 0, nSamples), kTukey, 0.1, panNum, rateNum));
                }
                std::cout << grainStack.size() << std::endl;
            }
         }
         */
    }
    
    const auto nInputChannels = buffer->getNumChannels();
    const auto nOutputChannels = bufferToFill.buffer->getNumChannels();
    const auto nOutputSamplesRemaining = bufferToFill.numSamples;
    //std::cout << grainStack.size() << std::endl;
    
    // Fill the output buffer
    for (auto sample = 0; sample < nOutputSamplesRemaining; ++sample)
    {
        for (auto i = 0; i < grainStack.size(); ++i)
        {
            // Only start playing a grain if it's onset time has been 
            if (grainStack[i].onset < time)
            {
                if (time <= (grainStack[i].onset + grainStack[i].length))
                {
                    //std::cout << i << ": " << "time: " << time << "onset + length: " << (grainStack[i].onset + grainStack[i].length) << std::endl;
                    grainStack[i].processAudio(bufferToFill,
                                                    *buffer,
                                                    nInputChannels,
                                                    nOutputChannels,
                                                    nOutputSamplesRemaining,
                                                    nInputSamples,
                                                    time);
                }
            }
        }
        ++time;
    }
#endif
    kdebug_signpost_end(2, 0, 0, 0, 3);
}

void AudioComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
    
    // For more details, see the help for AudioProcessor::releaseResources()
    
    // Reseting the sample buffer
    //buffer.setSize (0, 0);
}

//==============================================================================

void AudioComponent::run()
{
    while (!threadShouldExit())
    {
#if SCHED_THREAD
        double time1 = Time::getMillisecondCounterHiRes();

        //std::cout << "Grain Stack Size: " << grainStack.size() << std::endl;
        kdebug_signpost_start(1,0,0,0,0);
        
        // Delete grains
        if (grainStack.size() > 0)
        {
            for (auto i = grainStack.size() - 1; i >= 0; --i)
            {
                auto grainEnd = grainStack[i].onset + grainStack[i].length;
                auto hasEnded = grainEnd < time;
                if (hasEnded)
                {
                    grainStack.remove(i);
                }
                //std::cout   << "hasEnded: "     << hasEnded
                      //      << "\tgrainEnd: "   << grainEnd
                       //      << "\ttime: "       << time
                      //      << std::endl;
            }
        }
        
        // Add grains
        if (currentBuffer != nullptr)
        {
            // Make local copy of grain stack to avoid blocking audio thread
            // Array<Grain, CriticalSection> localGrainStack(grainStack);
            
            auto nSamples = currentBuffer->getAudioSampleBuffer()->getNumSamples();
            int onset = 1000;
            int length = 4410;
            int startPosition = -1000;
            for (int i = 0; i < 1; ++i)
            {
                grainStack.push_back(Grain(time + onset, length, wrap(startPosition, 0, nSamples), kTukey, 1.0));
            }
            // Once all new grains have been created then replace grainStack with localGrainStack
            // grainStack = localGrainStack;
        }
        double time2 = Time::getMillisecondCounterHiRes();
        std::cout << "Scheduler Elapsed: " << time2 - time1 << "\t" << "Stack Size: " << grainStack.size() << std::endl;
        kdebug_signpost_end(1, 0, 0, 0, 0);
#endif
        wait(250);
    }
}

//==============================================================================

void AudioComponent::setAudioBuffers(ReferenceCountedBuffer::Ptr newBuffer)
{
    currentBuffer = newBuffer;
    buffers.add(newBuffer);
}

void AudioComponent::clearCurrentBuffer()
{
    currentBuffer = nullptr;
}

void AudioComponent::checkBuffers()
{
    for (auto i = buffers.size(); --i >= 0;)
    {
        // Retain a copy of the buffer at the specified index
        ReferenceCountedBuffer::Ptr buffer (buffers.getUnchecked(i));
        
        // Check if the retained buffer has a reference count of 2, if so the audio thread cannot
        // be using it and so it can be removed from the array
        if (buffer->getReferenceCount() == 2)
        {
            buffers.remove(i);
        }
    }
}

//==============================================================================
int AudioComponent::wrap(int val, const int low, const int high)
{
    auto rangeSize = high - low + 1;
    
    if (val < low)
    {
        val += rangeSize * ((low - val) / rangeSize + 1);
    }
    
    return low + (val - low) % rangeSize;
}

int AudioComponent::intMap(int x, int inMin, int inMax, int outMin, int outMax)
{
    return inMin + ((outMax - outMin) / (inMax - inMin)) * (x - inMin);
}

float AudioComponent::floatMap(float x, float inMin, float inMax, float outMin, float outMax)
{
    return inMin + ((outMax - outMin) / (inMax - inMin)) * (x - inMin);
}

int AudioComponent::intMapInverse(int x, int inMin, int inMax, int outMin, int outMax)
{
    return outMax - ((outMax - outMin) / (inMax - inMin)) * (x - inMin);
}

float AudioComponent::floatMapInverse(float x, float inMin, float inMax, float outMin, float outMax)
{
    return outMax - ((outMax - outMin) / (inMax - inMin)) * (x - inMin);
}

//==============================================================================
void AudioComponent::setupGrainStack()
{
    for (auto i = 0; i < boidStructStack.size(); ++i)
    {
        
    }
}

//==============================================================================
Grain AudioComponent::createGrainFromBoid(Boids::boid_struct* boid, Boids::boid_range_t* range)
{
    // Keep fixed at 500ms
    // Value in terms of samples @ 44.1kHz
    int localLength = 22050;
    
    // Map to y coordinate of boids
    // Value mapped from boid range to 0 to 1000 samples
    int localOnset = intMap(boid->y_coordinate, range->y_min_coordinate, range->y_max_coordinate, 0, 2000);
    
    // Keep fixed at start of the sample buffer
    int localStartPosition = 0;
    
    // Set envtype to Tukey
    envType localEnv = kTukey;
    
    // Map to Z coordinate
    // Value mapped in inverse, so the smaller the z coordinate, the larger the output amplitude
    float localAmplitude = floatMapInverse((float)boid->z_coordinate, (float)range->z_min_coordinate, (float)range->z_max_coordinate, 0.1, 1.0);
    
    // Map to X coordinate
    float localPanPosition = floatMap((float)boid->x_coordinate, (float)range->x_min_coordinate, (float)range->x_max_coordinate, 0.0, 2.0) - 1.0;
    
    // Change based on velocity
    float localPlaybackRate;
    if (abs(boid->x_velocity) >= 4 || abs(boid->y_velocity) >= 4 || abs(boid->z_velocity) >= 4)
    {
        localPlaybackRate = 2.0;
    }
    else if (abs(boid->x_velocity) >= 2 || abs(boid->y_velocity) >= 2 || abs(boid->z_velocity) >= 2)
    {
        localPlaybackRate = 1.0;
    }
    else
    {
        localPlaybackRate = 0.5;
    }
    
    return Grain(boid->ID, boid->active, time + localOnset, localLength, localStartPosition, localEnv, localAmplitude, localPanPosition, localPlaybackRate);
}

void AudioComponent::generateGrainFromBoid(Grain* grain, Boids::boid_struct* boid, Boids::boid_range_t* range)
{
    // Keep fixed at 500ms
    // Value in terms of samples @ 44.1kHz
    int localLength = 44100;
    grain->length = localLength;
    
    // Map to y coordinate of boids
    // Value mapped from boid range to 0 to 1000 samples
    int localOnset = intMap(boid->y_coordinate, range->y_min_coordinate, range->y_max_coordinate, 0, 2000);
    grain->onset = time + localOnset;
    
    // Keep fixed at start of the sample buffer
    int localStartPosition = 0;
    grain->startPosition = localStartPosition;
    
    // Set envtype to Tukey
    envType localEnv = kTukey;
    grain->envelope = localEnv;
    
    // Map to Z coordinate
    // Value mapped in inverse, so the smaller the z coordinate, the larger the output amplitude
    float localAmplitude = floatMapInverse((float)boid->z_coordinate, (float)range->z_min_coordinate, (float)range->z_max_coordinate, 0.1, 1.0);
    grain->amplitude = localAmplitude;
    
    // Map to X coordinate
    float localPanPosition = floatMap((float)boid->x_coordinate, (float)range->x_min_coordinate, (float)range->x_max_coordinate, 0.0, 2.0) - 1.0;
    grain->panPosition = localPanPosition;
    
    // Change based on velocity
    float localPlaybackRate;
    if (abs(boid->x_velocity) >= 4 || abs(boid->y_velocity) >= 4 || abs(boid->z_velocity) >= 4)
    {
        localPlaybackRate = 2.0;
    }
    else if (abs(boid->x_velocity) >= 2 || abs(boid->y_velocity) >= 2 || abs(boid->z_velocity) >= 2)
    {
        localPlaybackRate = 1.0;
    }
    else
    {
        localPlaybackRate = 0.5;
    }
    grain->playbackRate = localPlaybackRate;
    
    grain->isActive = boid->active;
}

void AudioComponent::updateGrainFromBoid(Grain* grain, Boids::boid_struct* boid, Boids::boid_range_t* range)
{
    // Only updates spatial position parameters
    
    // Map to Z coordinate
    // Value mapped in inverse, so the smaller the z coordinate, the larger the output amplitude
    float localAmplitude = floatMapInverse(boid->z_coordinate, (float)range->z_min_coordinate, (float)range->z_max_coordinate, 0.1, 1.0);
    grain->amplitude = localAmplitude;
    
    // Map to X coordinate
    float localPanPosition = floatMap(boid->x_coordinate, (float)range->x_min_coordinate, (float)range->x_max_coordinate, 0.0, 2.0) - 1.0;
    grain->panPosition = localPanPosition;
    
    // Change based on velocity
    float localPlaybackRate;
    if (abs(boid->x_velocity) >= 4 || abs(boid->y_velocity) >= 4 || abs(boid->z_velocity) >= 4)
    {
        localPlaybackRate = 2.0;
    }
    else if (abs(boid->x_velocity) >= 3 || abs(boid->y_velocity) >= 3 || abs(boid->z_velocity) >= 3)
    {
        localPlaybackRate = 1.0;
    }
    else
    {
        localPlaybackRate = 0.5;
    }
    grain->playbackRate = localPlaybackRate;
    
    grain->isActive = boid->active;
}
