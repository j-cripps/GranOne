/*
  ==============================================================================

    AudioComponent.cpp
    Created: 12 Feb 2019 11:14:32am
    Author:  Jack Cripps

  ==============================================================================
*/

#include "AudioComponent.h"

AudioComponent::AudioComponent(guiMap_t* guiMap)
    : Thread("Network Thread"), masterGuiMap(guiMap)
{
    // Resize the array to be the correct size
    boidRanges.resize(numberOfMaxBindings);
    
#if FILE_BOIDS == 1
    // Add all the XML files to the boidStack
    for (auto i = 0; i < 3000; ++i)
    {
        const std::string tempString = ("/Users/jackcripps/Documents/MME_Proj/Flocking 270319/" + std::to_string(i) + "_flocking_data.xml");
        std::vector<std::vector<Boids::boidParam_t>> boidStruct = parseXMLBOID(tempString.data(), &boidRanges);
        boidStructStack.push_back(boidStruct);
    }
#else
    
    getBoidRanges(&boidRanges);
    
#endif

    // specify the number of input and output channels that we want to open
#if DECODE_METHOD == 1
    setAudioChannels (0, 4);
    file.create();
    writer.reset(wavFormat.createWriterFor(new FileOutputStream (file),
                                           44100,
                                           4,
                                           16,
                                           {},
                                           0));
#elif DECODE_METHOD == 2
    setAudioChannels(0, 16);
#else
    setAudioChannels(0,2);
#endif
    
    // Register audio formats
    formatManager.registerBasicFormats();
    
    // Initialise time to 0 and create a grain in the stack
    time = 0;
    boidFrame = 0;
    
    bool socketOpened = socket.bindToPort(80);
    if (socketOpened) std::cout << "Socket Opened" << std::endl;
    std::cout << "Host Name: " << socket.getHostName() << std::endl;
    
//    while (1)
//    {
//        AudioComponent::networkFunction();
//    }
    
    // Start the thread to manage scheduler
    startThread();
    
    // Create the grainStack from the first frame of the boids, before any audio starts playing.
    // Essentially starts the process from the beginning again if it's in offline mode (XML)
    refreshGuiMap();
    
#if FILE_BOIDS == 1
    if (!grainStack.empty()) grainStack.clear();
    
    for (auto i = 0; i < boidStructStack[boidFrame].size(); ++i)
    {
        grainStack.push_back(createGrainFromBoid(&boidStructStack[0][i], &boidRanges));
    }
    // Start the boid frame count again
    boidFrame = 1;
#endif
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
    
    //std::cout << "Number of Channels: " << bufferToFill.buffer->getNumChannels() << std::endl;
    
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
        
#if FILE_BOIDS == 1
        // Check if boid frame is less than boidStructStack size, if not then stop generating new grains
        if (boidFrame < boidStructStack.size())
        {
            // Refresh the GUI parameters
            refreshGuiMap();
            
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
#else
        refreshGuiMap();
        
        std::vector<std::vector<Boids::boidParam_t>> tempBoidStack;
        bool ret = boidQueue.pop(tempBoidStack);
        
        if (ret == false)
        {
            std::cout << "Cannot pop from network buffer" << std::endl;
        }
        else
        {
            if (grainStack.empty())
            {
                // Generate a new stack of grains from the boids
                for (auto i = 0; i < tempBoidStack.size(); ++i)
                {
                    grainStack.push_back(createGrainFromBoid(&tempBoidStack[i], &boidRanges));
                }
            }
            
            for (auto  i = 0; i < grainStack.size(); ++i)
            {
                auto grainEnd = grainStack[i].onset + grainStack[i].length;
                auto hasEnded = grainEnd < time;
                if (hasEnded)
                {
                    generateGrainFromBoid(&grainStack[i], &tempBoidStack[i], &boidRanges);
                }
                else
                {
                    updateGrainFromBoid(&grainStack[i], &tempBoidStack[i], &boidRanges);
                }
            }
        }
#endif
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
                    if (grainStack[i].isActive)
                    {
                    //std::cout << i << ": " << "time: " << time << "onset + length: " << (grainStack[i].onset + grainStack[i].length) << std::endl;
                        grainStack[i].processAudio(bufferToFill.buffer,
                                                   *buffer,
                                                   nInputChannels,
                                                   nOutputChannels,
                                                   nOutputSamplesRemaining,
                                                   nInputSamples,
                                                   time);
                    }
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
        std::cout << "Network Thread Entered" << std::endl;
        
        AudioComponent::networkFunction();
        
        std::cout << "Network Thread Exit" << std::endl;
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
void AudioComponent::setGuiMap(guiMap_t *guiMap)
{
    masterGuiMap = guiMap;
}

void AudioComponent::refreshGuiMap()
{
    // Ensure that this is an entire copy and not just a reference
    memcpy(&localGuiMap, masterGuiMap, sizeof(guiMap));
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
    if (outMin == outMax)
    {
        return outMin;
    }
    else
    {
        return (float)((x - inMin)) / (float)((inMax - inMin)) * (outMax - outMin) + outMin;
    }
}

float AudioComponent::floatMap(float x, float inMin, float inMax, float outMin, float outMax)
{
    if (outMin == outMax)
    {
        return outMin;
    }
    else
    {
        return (x - inMin) / (inMax - inMin) * (outMax - outMin) + outMin;
    }
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

void AudioComponent::networkFunction()
{
    
    // Timing
    double before = Time::getMillisecondCounterHiRes();
    socket.connect("192.168.0.2", 80, 1000);
    
    int ready = socket.waitUntilReady(false, 1000);
    if (ready == 1)
    {
        //std::cout << "Ready for Reading" << std::endl;
        // Ready for reading
        socket.read(&buffer[0], 50000, true);
        //std::cout << test << std::endl;
        std::string bufToDecode(&buffer[0]);
        std::stringstream ss(bufToDecode, std::ios_base::in|std::ios_base::out|std::ios_base::binary);
        //std::cout << "StringStream Length: " << ss.str().length() << std::endl;
        
        {
            boost::archive::text_iarchive ia(ss);
            // read class state from archive
            ia >> decoded;
        }
        
        // Convert the boid struct stack received from the network and push into the lock free ring buffer
        // so that the audio thread can pop it out the other side and use it to generate grains
        bool pushed = boidQueue.push(convertBoids(decoded));
        
        double after = Time::getMillisecondCounterHiRes();
        
        std::cout << "Elapsed Time (ms): " << after - before << std::endl;
        if (pushed == false)
        {
            std::cout << "Could not push boid struct stack into queue" << std::endl;
        }
    }
    else if (ready == 0)
    {
        // Time out
        std::cout << "Network Time Out" << std::endl;
    }
    else
    {
        // Error
        std::cout << "Network Error" << std::endl;
    }
    
    wait(40);
}

//==============================================================================
Grain AudioComponent::createGrainFromBoid(std::vector<Boids::boidParam_t>* boid, std::vector<Boids::boidParam_t>* range)
{
    // Get the parameter from the boid that has been selected in the GUI and map it to grain length
    Boids::boidParam_t lengthParam = (*boid)[localGuiMap.lengthBinding];
    int localLength;
    if (lengthParam.numType == Boids::intType)
    {
        localLength = intMap(lengthParam.intNum,
                             (*range)[(localGuiMap.lengthBinding * 2)].intNum,
                             (*range)[(localGuiMap.lengthBinding * 2) + 1].intNum,
                             (int)localGuiMap.grainLengthMin,
                             (int)localGuiMap.grainLengthMax);
    }
    else
    {
        localLength = floatMap(lengthParam.floatNum,
                               (*range)[(localGuiMap.lengthBinding * 2)].floatNum,
                               (*range)[(localGuiMap.lengthBinding * 2) + 1].floatNum,
                               (float)localGuiMap.grainLengthMin,
                               (float)localGuiMap.grainLengthMax);
    }

    
    // Get the parameter from the boid that has been selected in the GUI and map it to grain onset time
    Boids::boidParam_t onsetParam = (*boid)[localGuiMap.onsetBinding];
    int localOnset;
    if (onsetParam.numType == Boids::intType)
    {
        localOnset = intMap(onsetParam.intNum,
                            (*range)[(localGuiMap.onsetBinding * 2)].intNum,
                            (*range)[(localGuiMap.onsetBinding * 2) + 1].intNum,
                            (int)localGuiMap.grainOnsetMin,
                            (int)localGuiMap.grainOnsetMax);
    }
    else
    {
        localOnset = floatMap(onsetParam.floatNum,
                             (*range)[(localGuiMap.onsetBinding * 2)].floatNum,
                             (*range)[(localGuiMap.onsetBinding * 2) + 1].floatNum,
                             (float)localGuiMap.grainOnsetMin,
                             (float)localGuiMap.grainOnsetMax);
    }
    
    // Get the parameter from the boid that has been selected in the GUI and map it to grain start position
    Boids::boidParam_t startParam = (*boid)[localGuiMap.startBinding];
    int localStartPosition;
    if (currentBuffer != nullptr)
    {
        if (startParam.numType == Boids::intType)
        {
            localStartPosition = intMap(startParam.intNum,
                                        (*range)[(localGuiMap.startBinding * 2)].intNum,
                                        (*range)[(localGuiMap.startBinding * 2) + 1].intNum,
                                        (int)(localGuiMap.grainStartPosMin * currentBuffer->getAudioSampleBuffer()->getNumSamples()),
                                        (int)(localGuiMap.grainStartPosMax * currentBuffer->getAudioSampleBuffer()->getNumSamples()));
        }
        else
        {
            localStartPosition = floatMap(startParam.floatNum,
                                          (*range)[(localGuiMap.startBinding * 2)].floatNum,
                                          (*range)[(localGuiMap.startBinding * 2) + 1].floatNum,
                                          (float)localGuiMap.grainStartPosMin * currentBuffer->getAudioSampleBuffer()->getNumSamples(),
                                          (float)localGuiMap.grainStartPosMax * currentBuffer->getAudioSampleBuffer()->getNumSamples());
        }
    }
    else
    {
        // Must set local start position to 0 if there is no audio buffer yet
        localStartPosition = 0;
    }
    
    // Get grain envelope selection from GUI and set it to the current grain
    envType localEnv = localGuiMap.envelope;
    
    // Amplitude set by the master vol control, more to prevent clipping at output than to actually increase volume
    float localAmplitude = localGuiMap.grainMasterVol;
    
#if DECODE_METHOD == 0
    // TEMPORARY
    // Map X coordinate of grain to pan position until ambisonics is implemented
    Boids::boidParam_t panParam = (*boid)[xCoordinate];
    float localPanPosition = floatMap(panParam.intNum,
                                      0,
                                      2000,
                                      -1.0,
                                      1.0);
#else
    float localPanPosition = 0.0f;
#endif
    
    // GUI mapping for playback rate
    Boids::boidParam_t rateParam = (*boid)[localGuiMap.rateBinding];
    float localPlaybackRate;
    if (rateParam.numType == Boids::intType)
    {
        localPlaybackRate = floatMap((float)rateParam.intNum,
                                   (float)(*range)[(localGuiMap.rateBinding * 2)].intNum,
                                   (float)(*range)[(localGuiMap.rateBinding * 2) + 1].intNum,
                                   localGuiMap.grainRateMin,
                                   localGuiMap.grainRateMax);
    }
    else
    {
        localPlaybackRate = floatMap(rateParam.floatNum,
                                     (*range)[(localGuiMap.rateBinding * 2)].floatNum,
                                     (*range)[(localGuiMap.rateBinding * 2) + 1].floatNum,
                                     localGuiMap.grainRateMin,
                                     localGuiMap.grainRateMax);
    }
    
    return Grain((*boid)[boidID].intNum, (*boid)[isBoidActive].intNum, time + localOnset, localLength, localStartPosition, localEnv, localAmplitude, localPanPosition, localGuiMap.grainRateMax, (*boid)[xCoordinate].intNum, (*boid)[yCoordinate].intNum, (*boid)[zCoordinate].intNum);
}

void AudioComponent::generateGrainFromBoid(Grain* grain, std::vector<Boids::boidParam_t>* boid, std::vector<Boids::boidParam_t>* range)
{
    // Get the parameter from the boid that has been selected in the GUI and map it to grain length
    Boids::boidParam_t lengthParam = (*boid)[localGuiMap.lengthBinding];
    int localLength;
    if (lengthParam.numType == Boids::intType)
    {
        localLength = intMap(lengthParam.intNum,
                             (*range)[(localGuiMap.lengthBinding * 2)].intNum,
                             (*range)[(localGuiMap.lengthBinding * 2) + 1].intNum,
                             (int)localGuiMap.grainLengthMin,
                             (int)localGuiMap.grainLengthMax);
    }
    else
    {
        localLength = floatMap(lengthParam.floatNum,
                               (*range)[(localGuiMap.lengthBinding * 2)].floatNum,
                               (*range)[(localGuiMap.lengthBinding * 2) + 1].floatNum,
                               (float)localGuiMap.grainLengthMin,
                               (float)localGuiMap.grainLengthMax);
    }
    grain->length = localLength;
    
    // Get the parameter from the boid that has been selected in the GUI and map it to grain onset time
    Boids::boidParam_t onsetParam = (*boid)[localGuiMap.onsetBinding];
    int localOnset;
    if (onsetParam.numType == Boids::intType)
    {
        localOnset = intMap(onsetParam.intNum,
                            (*range)[(localGuiMap.onsetBinding * 2)].intNum,
                            (*range)[(localGuiMap.onsetBinding * 2) + 1].intNum,
                            (int)localGuiMap.grainOnsetMin,
                            (int)localGuiMap.grainOnsetMax);
    }
    else
    {
        localOnset = floatMap(onsetParam.floatNum,
                              (*range)[(localGuiMap.onsetBinding * 2)].floatNum,
                              (*range)[(localGuiMap.onsetBinding * 2) + 1].floatNum,
                              (float)localGuiMap.grainOnsetMin,
                              (float)localGuiMap.grainOnsetMax);
    }
    grain->onset = time + localOnset;
    
    // Get the parameter from the boid that has been selected in the GUI and map it to grain start position
    Boids::boidParam_t startParam = (*boid)[localGuiMap.startBinding];
    int localStartPosition;
    if (startParam.numType == Boids::intType)
    {
        localStartPosition = intMap(startParam.intNum,
                                    (*range)[(localGuiMap.startBinding * 2)].intNum,
                                    (*range)[(localGuiMap.startBinding * 2) + 1].intNum,
                                    (int)(localGuiMap.grainStartPosMin * currentBuffer->getAudioSampleBuffer()->getNumSamples()),
                                    (int)(localGuiMap.grainStartPosMax * currentBuffer->getAudioSampleBuffer()->getNumSamples()));
    }
    else
    {
        localStartPosition = floatMap(startParam.floatNum,
                                      (*range)[(localGuiMap.startBinding * 2)].floatNum,
                                      (*range)[(localGuiMap.startBinding * 2) + 1].floatNum,
                                      (float)localGuiMap.grainStartPosMin * currentBuffer->getAudioSampleBuffer()->getNumSamples(),
                                      (float)localGuiMap.grainStartPosMax * currentBuffer->getAudioSampleBuffer()->getNumSamples());
    }
    grain->startPosition = localStartPosition;
    
    // Get grain envelope selection from GUI and set it to the current grain
    grain->envelope = localGuiMap.envelope;
    
    // Amplitude set by the master vol control, more to prevent clipping at output than to actually increase volume
    float localAmplitude = localGuiMap.grainMasterVol;
    grain->amplitude = localAmplitude;
    
#if DECODE_METHOD == 0
    // TEMPORARY
    // Map X coordinate of grain to pan position until ambisonics is implemented
    Boids::boidParam_t panParam = (*boid)[xCoordinate];
    float localPanPosition = floatMap(panParam.intNum,
                                      0,
                                      2000,
                                      -1.0,
                                      1.0);
    grain->panPosition = localPanPosition;
#else
    grain->panPosition = 0.0f;
#endif
    
    // GUI mapping for playback rate
    Boids::boidParam_t rateParam = (*boid)[localGuiMap.rateBinding];
    float localPlaybackRate;
    if (rateParam.numType == Boids::intType)
    {
        localPlaybackRate = floatMap((float)rateParam.intNum,
                                     (float)(*range)[(localGuiMap.rateBinding * 2)].intNum,
                                     (float)(*range)[(localGuiMap.rateBinding * 2) + 1].intNum,
                                     localGuiMap.grainRateMin,
                                     localGuiMap.grainRateMax);
    }
    else
    {
        localPlaybackRate = floatMap(rateParam.floatNum,
                                     (*range)[(localGuiMap.rateBinding * 2)].floatNum,
                                     (*range)[(localGuiMap.rateBinding * 2) + 1].floatNum,
                                     localGuiMap.grainRateMin,
                                     localGuiMap.grainRateMax);
    }

    grain->playbackRate = localPlaybackRate;
    
    grain->isActive = (*boid)[isBoidActive].intNum;
    
    grain->xCoord = (*boid)[xCoordinate].intNum;
    
    grain->yCoord = (*boid)[yCoordinate].intNum;
    
    grain->zCoord = (*boid)[zCoordinate].intNum;
}

void AudioComponent::updateGrainFromBoid(Grain* grain, std::vector<Boids::boidParam_t>* boid, std::vector<Boids::boidParam_t>* range)
{
    // Only updates spatial position parameters
    
    // Amplitude set by the master vol control, more to prevent clipping at output than to actually increase volume
    float localAmplitude = localGuiMap.grainMasterVol;
    grain->amplitude = localAmplitude;
    
#if DECODE_METHOD == 0
    // TEMPORARY
    // Map X coordinate of grain to pan position until ambisonics is implemented
    Boids::boidParam_t panParam = (*boid)[xCoordinate];
    float localPanPosition = floatMap(panParam.intNum,
                                      0,
                                      2000,
                                      -1.0,
                                      1.0);
    grain->panPosition = localPanPosition;
#else
    grain->panPosition = 0.0f;
#endif
    
    grain->isActive = (*boid)[isBoidActive].intNum;
    
    grain->xCoord = (*boid)[xCoordinate].intNum;
    
    grain->yCoord = (*boid)[yCoordinate].intNum;
    
    grain->zCoord = (*boid)[zCoordinate].intNum;
}
