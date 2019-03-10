/*
  ==============================================================================

    GuiParameters.h
    Created: 9 Mar 2019 8:43:49pm
    Author:  Jack Cripps

  ==============================================================================
*/

#pragma once

#define SAMPLE_RATE 44100
#define SAMPLE_RATE_MS (SAMPLE_RATE / 1000)

#define RANGE_MASTER_VOL_MIN 0.0
#define RANGE_MASTER_VOL_MAX 1.0
#define INIT_MASTER_VOL 1.0

#define INIT_ENV_TYPE kTukey

#define RANGE_START_POS_MIN 0
#define RANGE_START_POS_MAX 100
#define INIT_START_POS_MIN 0
#define INIT_START_POS_MAX 100

#define RANGE_ONSET_MIN 10
#define RANGE_ONSET_MAX 2000
#define INIT_ONSET_MIN 50
#define INIT_ONSET_MAX 500

#define RANGE_LENGTH_MIN 10
#define RANGE_LENGTH_MAX 2000
#define INIT_LENGTH_MIN 200
#define INIT_LENGTH_MAX 1000

#define RANGE_RATE_MIN 0.1
#define RANGE_RATE_MAX 4.0
#define INIT_RATE_MIN 1.0
#define INIT_RATE_MAX 1.0

typedef struct guiMap
{
    float grainMasterVol;
    envType envelope;
    int grainStartPosMin;
    int grainStartPosMax;
    int grainOnsetMin;
    int grainOnsetMax;
    int grainLengthMin;
    int grainLengthMax;
    float grainRateMin;
    float grainRateMax;
    
} guiMap_t;
