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
float getAzimuthAngle(int x, int y, int z){
   
    
    
    
    float azimuth;
    
    if (x == 0 && z > 0){
        azimuth = 360;
    }
    else if (x == 0 && z < 0){
        azimuth = 90;
    }
    else
    {
    azimuth = atan(z/x);
    }
    return azimuth;
    
}

float getElevationAngle(int x, int y, int z, float azimuth){
    
    float elevation = asin((y*sin(azimuth))/z);
    return elevation;
    
}



//=======Channel Encoding ===================
void encodeW(float theta, float phi){
    
   // float distanceScaling = sqrt((y^2) + (x^2));
    
    //for(int i = 0; i < nSamples; i++){
  
    encodedW = currentSample*(1/sqrt(2));
        
    //}

}

void encodeX(float theta, float phi){
    
    encodedX = currentSample*((sqrt(3))*(cos(theta))*(cos(phi)));
    
}

void encodeY(float theta, float phi){

    encodedY = currentSample*((sqrt(3))*(sin(theta))*(cos(phi)));
    
}

void encodeZ(float theta, float phi){
    
    encodedZ = currentSample*((sqrt(3))*(sin(theta)));
    
}
