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
    // specify the number of input and output channels that we want to open
    setAudioChannels (0, 2);
    
    // Register audio formats
    formatManager.registerBasicFormats();
    
    // Initialise time to 0 and create a grain in the stack
    time = 0;
    //grainStack.add(Grain(88200, 44100, 0));
    
    // Start the thread to manage scheduler
    startThread();
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
    
    // Create local copy of the grainstack so that it cannot be changed by another thread mid-operation
    const Array<Grain> localGrainStack = grainStack;
    
    const auto nInputChannels = buffer->getNumChannels();
    const auto nOutputChannels = bufferToFill.buffer->getNumChannels();
    const auto nOutputSamplesRemaining = bufferToFill.numSamples;
    
    for (auto sample = 0; sample < nOutputSamplesRemaining; ++sample)
    {
        for (auto i = 0; i < localGrainStack.size(); ++i)
        {
            if (localGrainStack[i].onset < time)
            {
                if (time < (localGrainStack[i].onset + localGrainStack[i].length))
                {
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
        std::cout << "Grain Stack Size: " << grainStack.size() << std::endl;
        
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
                std::cout   << "hasEnded: "     << hasEnded
                            << "\tgrainEnd: "   << grainEnd
                            << "\ttime: "       << time
                            << std::endl;
            }
        }
        
        // Add grains
        if (currentBuffer != nullptr)
        {
            auto nSamples = currentBuffer->getAudioSampleBuffer()->getNumSamples();
            int onset = 1000;
            int length = 44100;
            int startPosition = -1000;
            grainStack.add(Grain((int)time + onset, length, wrap(startPosition, 0, nSamples)));
        }
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
