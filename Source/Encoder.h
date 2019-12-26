////
////  Encoder.h
////  FirstOrderAmbisonicEncoder
////
////  Created by Angus Moore on 27/02/2019.
////  Copyright © 2019 Leeds.ac.uk. All rights reserved.
////
//
//#ifndef Encoder_h
//#define Encoder_h
//
//#include "stdint.h"
//
////int currentSample;
////int nSamples;
////int encodedW;
////int encodedX;
////int encodedY;
////int encodedZ;
//
//float getAzimuthAngle(int x, int y, int z);
//
//float getElevationAngle(int x, int y, int z);
//
//float encodeW(float currentSample, float azymuth, float elevation);
//
//float encodeX(float currentSample, float azymuth, float elevation);
//
//float encodeY(float currentSample, float azymuth, float elevation);
//
//float encodeZ(float currentSample, float azymuth, float elevation);
//
////float encodeW(float currentSample, uint32_t x, uint32_t y, uint32_t z);
////
////float encodeX(float currentSample, int x, int y, int z);
////
////float encodeY(float currentSample, float azymuth, float elevation);
////
////float encodeZ(float currentSample, float azymuth, float elevation);
///*
//class Encoder
//{
//
//public:
//
//};
// */
//
//#endif /* Encoder_h */

//
//  Encoder.h
//  FirstOrderAmbisonicEncoder
//
//  Created by Angus Moore on 27/02/2019.
//  Copyright © 2019 Leeds.ac.uk. All rights reserved.
//

#ifndef Encoder_h
#define Encoder_h

#include "stdint.h"

float getAzimuthAngle(int x, int y, int z);

float getElevationAngle(int x, int y, int z);

float getRadialDistance(int x, int y, int z, float elevation);

float encodeW(float currentSample);

float encodeX(float currentSample, float theta, float phi, float rho);

float encodeY(float currentSample, float theta, float phi, float rho);

float encodeZ(float currentSample, float theta, float rho);

float encodeD(float currentSample, float rho);



#endif /* Encoder_h */

