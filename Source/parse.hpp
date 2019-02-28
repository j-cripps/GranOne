//
//  parse.hpp
//  parse_flock_xml
//
//  Created by Vicky Filor on 18/02/2019.
//  Copyright © 2019 Vicky Filor. All rights reserved.
//

#ifndef parse_hpp
#define parse_hpp

#include <stdio.h>
#include <vector>
#include "tinyxml2.h"

class Boids
{
public:
    
    typedef struct neighbourhood_data_struct
    {
        uint32_t x_avg_coordinate;
        uint32_t y_avg_coordinate;
        uint32_t z_avg_coordinate;
        float x_avg_velocity;
        float y_avg_velocity;
        float z_avg_velocity;
        std::vector<int> neighbour_IDs;
    } neighbourhood_data_struct;
    
    typedef struct boid_struct
    {
        uint16_t ID;
        uint32_t x_coordinate;
        uint32_t y_coordinate;
        uint32_t z_coordinate;
        uint8_t active;
        uint8_t species;
        float x_velocity;
        float y_velocity;
        float z_velocity;
        neighbourhood_data_struct neighbourhood_data;
    } boid_struct;
    
    Boids();
    ~Boids();
    
    // Not included in my class but adding here for ease ?
    // Maximum coordinates
    uint32_t x_max_coordinate;
    uint32_t y_max_coordinate;
    uint32_t z_max_coordinate;
    // Maximum velocities (absolute value)
    float x_max_velocity;
    float y_max_velocity;
    float z_max_velocity;
    
    // This may be moved, depending on how advanced the different species become
    uint32_t radius;
};

// Parses the xml file, pass the file path as the argument
std::vector<Boids::boid_struct> parseXML(const char * file_path);

#endif /* parse_hpp */
