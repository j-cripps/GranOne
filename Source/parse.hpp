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
#include "GuiParameters.h"
#include "cereal/cereal.hpp"
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <snappy.h>

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
        
        template <class Archive>
        void serialize( Archive & ar )
        {
            ar( x_avg_coordinate, y_avg_coordinate, z_avg_coordinate, x_avg_velocity, y_avg_velocity, z_avg_velocity, neighbour_IDs );
        }
        
    } neighbourhood_data_struct;
    
    typedef struct boid_struct
    {
        uint32_t ID;
        uint32_t x_coordinate;
        uint32_t y_coordinate;
        uint32_t z_coordinate;
        uint32_t active;
        uint32_t species;
        float x_velocity;
        float y_velocity;
        float z_velocity;
        neighbourhood_data_struct neighbourhood_data;
        
        template <class Archive>
        void serialize( Archive & ar )
        {
            ar( ID, x_coordinate, y_coordinate, z_coordinate, active, species, x_velocity, y_velocity, z_velocity, neighbourhood_data );
        }
    } boid_struct;
    
    typedef enum
    {
        intType = 0,
        floatType
    } numberType_t;
    
    typedef struct
    {
        numberType_t numType;
        
        union
        {
            int intNum;
            float floatNum;
        };
        
        template <class Archive>
        void serialize( Archive & ar )
        {
            ar( numType, intNum, floatNum );
        }
    } boidParam_t;
    
    typedef struct boid_range
    {
        uint32_t x_max_coordinate;
        uint32_t y_max_coordinate;
        uint32_t z_max_coordinate;
        float x_max_velocity;
        float y_max_velocity;
        float z_max_velocity;
        uint32_t x_min_coordinate;
        uint32_t y_min_coordinate;
        uint32_t z_min_coordinate;
        float x_min_velocity;
        float y_min_velocity;
        float z_min_velocity;
    } boid_range_t;
    
    Boids();
    ~Boids();
    
    //std::vector<Boids::boidParam_t> getBoidRange();
    
    std::vector<Boids::boidParam_t> localBoidRange;
    
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
std::vector<std::vector<Boids::boidParam_t>> parseXMLBOID(const char * file_path, std::vector<Boids::boidParam_t>* boidRange);

#endif /* parse_hpp */
