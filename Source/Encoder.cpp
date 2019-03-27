//
//  Encoder.cpp
//  FirstOrderAmbisonicEncoder
//
//  Created by Angus Moore on 27/02/2019.
//  Copyright © 2019 Leeds.ac.uk. All rights reserved.
//

#include "Encoder.h"
#include "math.h"
//1000 1000 is centre point offeset, 2000x2000 cube


//============Angles===========
float getAzimuthAngle(int x, int y, int z)
{
    float azimuth;
    
    float localX = (float)(x - 1000);
    float localZ = (float)(z - 1000);
    
    azimuth = atan2(localX, localZ);
    
    //if (azimuth < 0) azimuth += (2 * M_PI);
    
    return azimuth;
}

float getElevationAngle(int x, int y, int z)
{
    float elevation;
    
    float localY = (float)(y - 1000);
    float localZ = (float)(z - 1000);
    
    elevation = atan2(localY, localZ);
    
    //if (elevation < 0) elevation += (2 * M_PI);
    
    return elevation;
}



//=======Channel Encoding ===================
float encodeW(float currentSample, float theta, float phi)
{
    // float distanceScaling = sqrt((y^2) + (x^2));
    
    return currentSample * (1 / sqrt(2));
}

float encodeX(float currentSample, float theta, float phi)
{
    return currentSample * cos(theta) * cos(phi);
}

float encodeY(float currentSample, float theta, float phi)
{
    return currentSample * sin(theta) * cos(phi);
}

float encodeZ(float currentSample, float theta, float phi)
{
    return currentSample * sin(theta);
}
