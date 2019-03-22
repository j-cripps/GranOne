//
//  Encoder.h
//  FirstOrderAmbisonicEncoder
//
//  Created by Angus Moore on 27/02/2019.
//  Copyright © 2019 Leeds.ac.uk. All rights reserved.
//

#ifndef Encoder_h
#define Encoder_h

int currentSample;
int nSamples;
int encodedW;
int encodedX;
int encodedY;
int encodedZ;

class Encoder
{

public:

float getAzimuthAngle(int x, int y, int z);

float getElevationAngle(int x, int y, int z);

void encodeW(float azymuth, float elevation);

void encodeX(float azymuth, float elevation);

void encodeY(float azymuth, float elevation);

void encodeZ(float azymuth, float elevation);

};

#endif /* Encoder_h */
