/*
  ==============================================================================

    GuiParameters.h
    Created: 9 Mar 2019 8:43:49pm
    Author:  Jack Cripps

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Grain.h"

#define SAMPLE_RATE 44100
#define SAMPLE_RATE_MS (SAMPLE_RATE / 1000)

#define RANGE_MASTER_VOL_MIN 0.0f
#define RANGE_MASTER_VOL_MAX 1.0f
#define INIT_MASTER_VOL 0.3f

#define INIT_ENV_TYPE kTukey

#define RANGE_START_POS_MIN 0.0f
#define RANGE_START_POS_MAX 1.0f
#define INIT_START_POS_MIN 0.0f
#define INIT_START_POS_MAX 1.0f
#define INIT_START_POS_BIND xCoordinate

#define RANGE_ONSET_MIN 1
#define RANGE_ONSET_MAX 8000
#define INIT_ONSET_MIN 50
#define INIT_ONSET_MAX 500
#define INIT_ONSET_BIND yCoordinate

#define RANGE_LENGTH_MIN 5
#define RANGE_LENGTH_MAX 2000
#define INIT_LENGTH_MIN 200
#define INIT_LENGTH_MAX 1000
#define INIT_LENGTH_BIND xVelocity

#define RANGE_RATE_MIN 0.1f
#define RANGE_RATE_MAX 4.0f
#define INIT_RATE_MIN 1.0f
#define INIT_RATE_MAX 1.0f
#define INIT_RATE_BIND yVelocity

static Array<String> boidStrings = {
    String("X Coordinate"),
    String("Y Coordinate"),
    String("Z Coordinate"),
    String("X Velocity"),
    String("Y Velocity"),
    String("Z Velocity")
};

typedef enum boidBinding {
    xCoordinate = 0,
    yCoordinate,
    zCoordinate,
    xVelocity,
    yVelocity,
    zVelocity,
    boidID,
    isBoidActive,
    boidSpecies,
    numOfBindings
    
}boidBinding_t;

typedef enum boidMaxBinding {
    xCoordMin = 0,
    xCoordMax,
    yCoordMin,
    yCoordMax,
    zCoordMin,
    zCoordMax,
    xVelMin,
    xVelMax,
    yVelMin,
    yVelMax,
    zVelMin,
    zVelMax,
    numberOfMaxBindings
    
} boidMaxBinding_t;

typedef struct guiMap
{
    float grainMasterVol;
    envType envelope;
    float grainStartPosMin;
    float grainStartPosMax;
    int startBinding;
    int grainOnsetMin;
    int grainOnsetMax;
    int onsetBinding;
    int grainLengthMin;
    int grainLengthMax;
    int lengthBinding;
    float grainRateMin;
    float grainRateMax;
    int rateBinding;
} guiMap_t;
