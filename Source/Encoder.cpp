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

float getRadialDistance(int x, int y, int z, float elevation){
    
    float localX = (float)(x - 1000);
    float localY = (float)(y - 1000);
    float localZ = (float)(z - 1000);
    
    
    
    float radialDistance;
    
    if (localY != 0){
        
        radialDistance = (localY/sin(elevation));
        
    } else
    {
        radialDistance = sqrt((localX*localX) + (localZ*localZ));
        
    }
    
    radialDistance = 1 - ((radialDistance/(1000/1)));
    
    
    return radialDistance; //(1000/(0.5 * M_PI));
    
}


//=======Channel Encoding =================== theta = elevation, theta = rotation, rho = radialDistance
float encodeW(float currentSample)
{
    return currentSample * (1 / sqrt(2));
}

float encodeX(float currentSample, float theta, float phi, float rho)
{
    return currentSample * cos(theta) * cos(phi);// * sin(rho);
}

float encodeY(float currentSample, float theta, float phi, float rho)
{
    return currentSample * sin(theta) * cos(phi);// * sin(rho);
}

float encodeZ(float currentSample, float phi, float rho)
{
    return currentSample * sin(phi);// * sin(rho);
}

//Additional channel D for distance encoding.
float encodeD(float currentSample, float rho)
{
    return currentSample * cos(rho);
    
}
