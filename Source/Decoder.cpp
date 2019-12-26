//
//  Decoder.cpp
//  FirstOrderAmbisonicEncoder
//
//  Created by Angus Moore on 22/02/2019.
//  Copyright © 2019 Leeds.ac.uk. All rights reserved.
//

#include "Decoder.h"
#include "math.h"


float decodedSpeakerSignal(float W, float X, float Y, float Z, float D, float speakerAzimuth, float speakerElevation, float radialDistance)
{
    auto num = (1.0f/8.0f) * ((W/(sqrt(2))) + (X * cos(speakerAzimuth) * cos(speakerElevation)) + (Y * sin(speakerAzimuth) *                   cos(speakerElevation)) + (Z * sin(speakerElevation)));
    
    return num * radialDistance;
}
