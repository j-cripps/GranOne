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
//#include "cereal/cereal.hpp"
//#include <cereal/archives/binary.hpp>
//#include <cereal/types/vector.hpp>
#include <snappy.h>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#define DECODE_METHOD 2
#define TEST_SERIAL 1

class Boids
{
public:
    friend class boost::serialization::access;
    
    typedef struct neighbourhood_data_struct
    {
        int32_t x_avg_coordinate;
        int32_t y_avg_coordinate;
        int32_t z_avg_coordinate;
        float x_avg_velocity;
        float y_avg_velocity;
        float z_avg_velocity;
        std::vector<uint16_t> neighbour_IDs;
        
        /* Cereal version
        template <class Archive>
        void serialize( Archive & ar )
        {
            ar( x_avg_coordinate, y_avg_coordinate, z_avg_coordinate, x_avg_velocity, y_avg_velocity, z_avg_velocity, CEREAL_NVP(neighbour_IDs) );
        }
         */
        
        // Boost version
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
            ar & x_avg_coordinate;
            ar & y_avg_coordinate;
            ar & z_avg_coordinate;
            ar & x_avg_velocity;
            ar & y_avg_velocity;
            ar & z_avg_velocity;
            ar & neighbour_IDs;
        }
        
    } neighbourhood_data_struct;
    
    typedef struct boid_struct
    {
        uint16_t ID;
        int32_t x_coordinate;
        int32_t y_coordinate;
        int32_t z_coordinate;
        uint8_t active;
        uint8_t species;
        float x_velocity;
        float y_velocity;
        float z_velocity;
        neighbourhood_data_struct neighbourhood_data;
        
        /* Cereal version
        template <class Archive>
        void serialize( Archive & ar )
        {
            ar( ID, x_coordinate, y_coordinate, z_coordinate, active, species, x_velocity, y_velocity, z_velocity, CEREAL_NVP(neighbourhood_data) );
        }
        */
        
        // Boost version
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
            ar & ID;
            ar & x_coordinate;
            ar & y_coordinate;
            ar & z_coordinate;
            ar & active;
            ar & species;
            ar & x_velocity;
            ar & y_velocity;
            ar & z_velocity;
            ar & neighbourhood_data;
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
        
        /* Cereal version
        template <class Archive>
        void serialize( Archive & ar )
        {
            ar( numType, intNum, floatNum );
        }
        */
        
        // Boost version
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
            ar & numType;
            
            // Because this is a union, we only want to store the relevant
            // variable as the other contains junk
            if (numType == Boids::intType)
            {
                ar & intNum;
            }
            else
            {
                ar & floatNum;
            }
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

// Convert boid_struct to boidParam_t
std::vector<std::vector<Boids::boidParam_t>> convertBoids(std::vector<Boids::boid_struct>& boidStruct);

// Convert boidParam_t to boid_struct
std::vector<Boids::boid_struct> convertBoidsBack(std::vector<std::vector<Boids::boidParam_t>>& boidStack, std::vector<Boids::boid_struct>& nID);

// Set the boid ranghes

void getBoidRanges(std::vector<Boids::boidParam_t>* boidRange);

#endif /* parse_hpp */
