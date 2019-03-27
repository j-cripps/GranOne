//
//  parse.cpp
//  parse_flock_xml
//
//  Created by Vicky Filor on 18/02/2019.
//  Copyright © 2019 Vicky Filor. All rights reserved.
//

#include "parse.hpp"
#include "stdint.h"
#define debug_comments 0
using namespace tinyxml2;

template<typename T>
bool isEqual(std::vector<T> const &v1, std::vector<T> const &v2)
{
    return (v1.size() == v2.size() &&
            std::equal(v1.begin(), v1.end(), v2.begin()));
}

Boids::Boids(){
    //Object created
    // I don't really know if anything should be here??
};

Boids::~Boids(){
    // Object destroyed
    // Anything here either??
};


/*
 * Implemented using the following tutorial
 * https://shilohjames.wordpress.com/2014/04/27/tinyxml2-tutorial/#XML-CreateXMLDocument
 */
std::vector<std::vector<Boids::boidParam_t>> parseXMLBOID(const char * file_path, std::vector<Boids::boidParam_t>* boidRange)
{
    // Declares an instance of the boid class
    Boids boid;
    
    // Declares an instance of the xmlDoc for use with tinyxml2
    tinyxml2::XMLDocument xmlDoc;
    
    // Loads the file given via the file_path argument
    tinyxml2::XMLError err = xmlDoc.LoadFile(file_path);
    
    // 0 = success, not 0 = failed
    printf("Load file error code: %i\n", err);
    
    
    //-------------------------------------Root Node-------------------------------------//
   
    tinyxml2::XMLNode* root = xmlDoc.FirstChildElement("flock_data");
    
    //-------------------------------------General Data-------------------------------------//
    
    XMLElement * radius_data = root->FirstChildElement("radius");
    XMLElement * max_velocities_data = root->FirstChildElement("max_velocities");
    XMLElement * max_coordinates_data = root->FirstChildElement("max_coordinates");
    
    int radius_attribute, xm_coordinate_attribute,  ym_coordinate_attribute, zm_coordinate_attribute;
    float xm_velocity_attribute, ym_velocity_attribute, zm_velocity_attribute;
    
    // Radius
    radius_data->QueryAttribute("radius",&radius_attribute);
    boid.radius = (uint32_t)radius_attribute;
    
#if debug_comments
    printf("radius: \t%i\n", boid.radius);
#endif
    
    // Maximum Velocities
    max_velocities_data->QueryAttribute("x_max_velocity", &xm_velocity_attribute);
    boid.x_max_velocity = (float)xm_velocity_attribute;
    //boidRange->x_max_velocity = boid.x_max_velocity;
    //boidRange->x_min_velocity = -boid.x_max_velocity;
    Boids::boidParam_t xVelMaxParam;
    xVelMaxParam.numType = Boids::floatType;
    xVelMaxParam.floatNum = boid.x_max_velocity;
    boidRange->at(xVelMax) = xVelMaxParam;
    
    Boids::boidParam_t xVelMinParam;
    xVelMinParam.numType = Boids::floatType;
    xVelMinParam.floatNum = -boid.x_max_velocity;
    boidRange->at(xVelMin) = xVelMinParam;
    
    max_velocities_data->QueryAttribute("y_max_velocity", &ym_velocity_attribute);
    boid.y_max_velocity = (float)ym_velocity_attribute;
    //boidRange->y_max_velocity = boid.y_max_velocity;
    //boidRange->y_min_velocity = -boid.y_max_velocity;
    Boids::boidParam_t yVelMaxParam;
    yVelMaxParam.numType = Boids::floatType;
    yVelMaxParam.floatNum = boid.y_max_velocity;
    boidRange->at(yVelMax) = yVelMaxParam;
    
    Boids::boidParam_t yVelMinParam;
    yVelMinParam.numType = Boids::floatType;
    yVelMinParam.floatNum = -boid.y_max_velocity;
    boidRange->at(yVelMin) = yVelMinParam;
    
    max_velocities_data->QueryAttribute("z_max_velocity", &zm_velocity_attribute);
    boid.z_max_velocity = (float)zm_velocity_attribute;
    //boidRange->z_max_velocity = boid.z_max_velocity;
    //boidRange->z_min_velocity = -boid.z_max_velocity;
    Boids::boidParam_t zVelMaxParam;
    zVelMaxParam.numType = Boids::floatType;
    zVelMaxParam.floatNum = boid.z_max_velocity;
    boidRange->at(zVelMax) = zVelMaxParam;
    
    Boids::boidParam_t zVelMinParam;
    zVelMinParam.numType = Boids::floatType;
    zVelMinParam.floatNum = -boid.z_max_velocity;
    boidRange->at(zVelMin) = yVelMinParam;
    
    // Maximum Coordinates
    max_coordinates_data->QueryAttribute("x_max_coordinate", &xm_coordinate_attribute);
    boid.x_max_coordinate = (uint32_t)xm_coordinate_attribute;
    //boidRange->x_max_coordinate = boid.x_max_coordinate;
    //boidRange->x_min_coordinate = 0;
    Boids::boidParam_t xCoordMaxParam;
    xCoordMaxParam.numType = Boids::intType;
    xCoordMaxParam.intNum = boid.x_max_coordinate;
    boidRange->at(xCoordMax) = xCoordMaxParam;
    
    Boids::boidParam_t xCoordMinParam;
    xCoordMinParam.numType = Boids::intType;
    xCoordMinParam.intNum = 0;
    boidRange->at(xCoordMin) = xCoordMinParam;
    
    max_coordinates_data->QueryAttribute("y_max_coordinate", &ym_coordinate_attribute);
    boid.y_max_coordinate = (uint32_t)ym_coordinate_attribute;
    //boidRange->y_max_coordinate = boid.y_max_coordinate;
    //boidRange->y_min_coordinate = 0;
    Boids::boidParam_t yCoordMaxParam;
    yCoordMaxParam.numType = Boids::intType;
    yCoordMaxParam.intNum = boid.y_max_coordinate;
    boidRange->at(yCoordMax) = yCoordMaxParam;
    
    Boids::boidParam_t yCoordMinParam;
    yCoordMinParam.numType = Boids::intType;
    yCoordMinParam.intNum = 0;
    boidRange->at(yCoordMin) = yCoordMinParam;
    
    max_coordinates_data->QueryAttribute("z_max_coordinate", &zm_coordinate_attribute);
    boid.z_max_coordinate = (uint32_t)zm_coordinate_attribute;
    //boidRange->z_max_coordinate = boid.z_max_coordinate;
    //boidRange->z_min_coordinate = 0;
    Boids::boidParam_t zCoordMaxParam;
    zCoordMaxParam.numType = Boids::intType;
    zCoordMaxParam.intNum = boid.z_max_coordinate;
    boidRange->at(zCoordMax) = zCoordMaxParam;
    
    Boids::boidParam_t zCoordMinParam;
    zCoordMinParam.numType = Boids::intType;
    zCoordMinParam.intNum = 0;
    boidRange->at(zCoordMin) = zCoordMinParam;
    
#if debug_comments
    printf("vel: \txmv: %f\t ymv: %f\t zmv:%f\n", boid.x_max_velocity, boid.y_max_velocity, boid.z_max_velocity);
    printf("coord: \txmc: %i\t ymc: %i\t zmc:%i\n", boid.x_max_coordinate, boid.y_max_coordinate, boid.z_max_coordinate);
#endif
    
    //-------------------------------------Boid Data-------------------------------------//
   
    // Gets the first occurance of the boid node in the file
    XMLElement * boid_data = root->FirstChildElement("boid");
    //std::vector<Boids::boid_struct> flock_data;
    std::vector<std::vector<Boids::boidParam_t>> flockData;
    
    // boid_data will become nullptr when there are no sibling elements left
    while (boid_data != nullptr)
    {
        
        //Boids::boid_struct individual_boid;
        std::vector<Boids::boidParam_t> individualBoid;
        individualBoid.resize(numOfBindings);
        
        // Core boid data
        
        XMLElement * id = boid_data->FirstChildElement("id");
        XMLElement * x_coordinate = boid_data->FirstChildElement("x_coordinate");
        XMLElement * y_coordinate = boid_data->FirstChildElement("y_coordinate");
        XMLElement * z_coordinate = boid_data->FirstChildElement("z_coordinate");
        XMLElement * x_velocity = boid_data->FirstChildElement("x_velocity");
        XMLElement * y_velocity = boid_data->FirstChildElement("y_velocity");
        XMLElement * z_velocity = boid_data->FirstChildElement("z_velocity");
        XMLElement * active = boid_data->FirstChildElement("active");
        XMLElement * species = boid_data->FirstChildElement("species");
        
        int id_value, x_coordinate_data, y_coordinate_data, z_coordinate_data, active_data, species_data;
        float x_velocity_data, y_velocity_data, z_velocity_data;
        
        // ID
        id->QueryIntText(&id_value);
        //individual_boid.ID = (uint32_t)id_value;
        Boids::boidParam_t idParam;
        idParam.numType = Boids::intType;
        idParam.intNum = id_value;
        individualBoid[boidID] = idParam;
        
        // Coordinate data
        x_coordinate->QueryIntText(&x_coordinate_data);
        //individual_boid.x_coordinate = (uint32_t)x_coordinate_data;
        Boids::boidParam_t xCoordParam;
        xCoordParam.numType = Boids::intType;
        xCoordParam.intNum = x_coordinate_data;
        individualBoid[xCoordinate] = xCoordParam;
        
        y_coordinate->QueryIntText(&y_coordinate_data);
        //individual_boid.y_coordinate = (uint32_t)y_coordinate_data;
        Boids::boidParam_t yCoordParam;
        yCoordParam.numType = Boids::intType;
        yCoordParam.intNum = y_coordinate_data;
        individualBoid[yCoordinate] = yCoordParam;
        
        z_coordinate->QueryIntText(&z_coordinate_data);
        //individual_boid.z_coordinate = (uint32_t)z_coordinate_data;
        Boids::boidParam_t zCoordParam;
        zCoordParam.numType = Boids::intType;
        zCoordParam.intNum = z_coordinate_data;
        individualBoid[zCoordinate] = zCoordParam;
        
        // Velocity Data
        x_velocity->QueryFloatText(&x_velocity_data);
        //individual_boid.x_velocity = (float)x_velocity_data;
        Boids::boidParam_t xVelParam;
        xVelParam.numType = Boids::floatType;
        xVelParam.floatNum = x_velocity_data;
        individualBoid[xVelocity] = xVelParam;
        
        y_velocity->QueryFloatText(&y_velocity_data);
        //individual_boid.y_velocity = (float)y_velocity_data;
        Boids::boidParam_t yVelParam;
        yVelParam.numType = Boids::floatType;
        yVelParam.floatNum = y_velocity_data;
        individualBoid[yVelocity] = yVelParam;
        
        z_velocity->QueryFloatText(&z_velocity_data);
        //individual_boid.z_velocity = (float)z_velocity_data;
        Boids::boidParam_t zVelParam;
        zVelParam.numType = Boids::floatType;
        zVelParam.floatNum = z_velocity_data;
        individualBoid[zVelocity] = zVelParam;
        
        // Active
        active->QueryIntText(&active_data);
        //individual_boid.active = (uint32_t)active_data;
        Boids::boidParam_t boidActive;
        boidActive.numType = Boids::intType;
        boidActive.intNum = active_data;
        individualBoid[isBoidActive] = boidActive;
        
        // Species
        species->QueryIntText(&species_data);
        //individual_boid.species = (uint32_t)species_data;
        Boids::boidParam_t boid_Species;
        boid_Species.numType = Boids::intType;
        boid_Species.intNum = species_data;
        individualBoid[boidSpecies] = boid_Species;
        
#if debug_comments
        printf("\n\nID: %i\n", individual_boid.ID);
        printf("vel: \txv: %f\t yv: %f\t zv:%f\n", individual_boid.x_velocity, individual_boid.y_velocity, individual_boid.z_velocity);
        printf("coord: \txc: %i\t yc: %i\t zc:%i\n", individual_boid.x_coordinate, individual_boid.y_coordinate, individual_boid.z_coordinate);
        printf("active: %i \tspecies: \t%i\n", individual_boid.active, individual_boid.species);
#endif
        
        // Neighbourhood Data
        
        XMLElement * neighbourhood_data = boid_data->FirstChildElement("neighbourhood_data");
        
        XMLElement * x_avg_coordinate = neighbourhood_data->FirstChildElement("x_avg_coordinate");
        XMLElement * y_avg_coordinate = neighbourhood_data->FirstChildElement("y_avg_coordinate");
        XMLElement * z_avg_coordinate = neighbourhood_data->FirstChildElement("z_avg_coordinate");
        XMLElement * x_avg_velocity = neighbourhood_data->FirstChildElement("x_avg_velocity");
        XMLElement * y_avg_velocity = neighbourhood_data->FirstChildElement("y_avg_velocity");
        XMLElement * z_avg_velocity = neighbourhood_data->FirstChildElement("z_avg_velocity");
        
        int x_avg_coordinate_data, y_avg_coordinate_data, z_avg_coordinate_data;
        float x_avg_velocity_data, y_avg_velocity_data, z_avg_velocity_data;
        
        // Average Coordinate Data
        x_avg_coordinate->QueryIntText(&x_avg_coordinate_data);
        //individual_boid.neighbourhood_data.x_avg_coordinate = (uint32_t)x_avg_coordinate_data;
        
        y_avg_coordinate->QueryIntText(&y_avg_coordinate_data);
        //individual_boid.neighbourhood_data.y_avg_coordinate = (uint32_t)y_avg_coordinate_data;
        
        z_avg_coordinate->QueryIntText(&z_avg_coordinate_data);
        //individual_boid.neighbourhood_data.z_avg_coordinate = (uint32_t)z_avg_coordinate_data;
        
        // Average Velocity Data
        x_avg_velocity->QueryFloatText(&x_avg_velocity_data);
        //individual_boid.neighbourhood_data.x_avg_velocity = (float)x_avg_velocity_data;
        
        y_avg_velocity->QueryFloatText(&y_avg_velocity_data);
        //individual_boid.neighbourhood_data.y_avg_velocity = (float)y_avg_velocity_data;
        
        z_avg_velocity->QueryFloatText(&z_avg_velocity_data);
        //individual_boid.neighbourhood_data.z_avg_velocity = (float)z_avg_velocity_data;
        
#if debug_comments
        printf("neighbourhood: \tavg_x_vel: %f \tavg_y_vel: %f \tavg_z_vel: %f\n",individual_boid.neighbourhood_data.x_avg_velocity, individual_boid.neighbourhood_data.y_avg_velocity, individual_boid.neighbourhood_data.z_avg_velocity);
        printf("neighbourhood: \tavg_x_coord: %i \tavg_y_coord: %i \tavg_z_coord: %i\n", individual_boid.neighbourhood_data.x_avg_coordinate, individual_boid.neighbourhood_data.y_avg_coordinate, individual_boid.neighbourhood_data.z_avg_coordinate);
#endif
        
        // Neighbour IDs
        XMLElement * neighbour_id_list = neighbourhood_data->FirstChildElement("neighbour_id_list");
        
        // if neighbour_id_list exists
        if (neighbour_id_list)
        {
            XMLElement * neighbour_id = neighbour_id_list->FirstChildElement("neighbour_id");
            std::vector<int> id_list;
            // Gets the list of neighbour IDs
            while ( neighbour_id != nullptr)
            {
                int neighbour_id_value;
                neighbour_id->QueryIntText(&neighbour_id_value);
                id_list.push_back(neighbour_id_value);
                neighbour_id = neighbour_id->NextSiblingElement("neighbour_id");
            }
            // Assigns them to the struct
            //individual_boid.neighbourhood_data.neighbour_IDs = id_list;
            
#if debug_comments
            printf("neighbour ids:");
            for (int i = 0; i < id_list.size(); i++)
            {
                printf("\t%i", id_list[i]);
            }
            printf("\n");
#endif
            
        }
        
        // Finds if there is a sibling element and reassigns boid_data to it
        boid_data = boid_data->NextSiblingElement("boid");
        
        //flock_data.push_back(individual_boid);
        flockData.push_back(individualBoid);
    }
    printf("Finished parsing\n");
    
    flockData.shrink_to_fit();
    /*
    std::stringstream ss;
    
    // save data to archive
    {
        boost::archive::text_oarchive oa(ss);
        // write class instance to archive
        oa << flockData;
        // archive and stream closed when destructors are called
    }

    {
        cereal::BinaryOutputArchive oarchive(ss); // Create an output archive
        
        oarchive(flockData); // Write the data to the archive
    } // archive goes out of scope, ensuring all contents are flushed

    std::string s = ss.str();
    long int sSize = s.length();
    std::cout << "Uncompressed Size: " << sSize << std::endl;
    
    /*
    std::string compressed;
    snappy::Compress(s.data(), s.size(), &compressed);
    long int compressSize = compressed.length();
    std::cout << "Compressed Size: " << compressSize << std::endl;
    
    std::string decompressed;
    snappy::Uncompress(compressed.data(), compressed.size(), &decompressed);
    ss.str() = decompressed;
    
    
    std::vector<std::vector<Boids::boidParam_t>> flockDataDecoded;
    {
        boost::archive::text_iarchive ia(ss);
        // read class state from archive
        ia >> flockDataDecoded;
    }
    {
        cereal::BinaryInputArchive iarchive(ss); // Create an input archive
        
        iarchive(flockDataDecoded); // Read the data from the archive
    }
*/
    //return flock_data;
    return flockData;
};

std::vector<std::vector<Boids::boidParam_t>> convertBoids(std::vector<Boids::boid_struct>& boidStructStack)
{
    std::vector<std::vector<Boids::boidParam_t>> boidStack;
    
    // Loop through the passed in vector of boids and convert them to the correct format for this program
    for (auto i = 0; i < boidStructStack.size(); ++i)
    {
        std::vector<Boids::boidParam_t> individualBoid;
        individualBoid.resize(numOfBindings);
        
        individualBoid[xCoordinate].numType = Boids::intType;
        individualBoid[xCoordinate].intNum = boidStructStack[i].x_coordinate;
        
        individualBoid[yCoordinate].numType = Boids::intType;
        individualBoid[yCoordinate].intNum = boidStructStack[i].y_coordinate;
        
        individualBoid[zCoordinate].numType = Boids::intType;
        individualBoid[zCoordinate].intNum = boidStructStack[i].z_coordinate;
        
        individualBoid[xVelocity].numType = Boids::floatType;
        individualBoid[xVelocity].floatNum = boidStructStack[i].x_velocity;
        
        individualBoid[yVelocity].numType = Boids::floatType;
        individualBoid[yVelocity].floatNum = boidStructStack[i].y_velocity;
        
        individualBoid[zVelocity].numType = Boids::floatType;
        individualBoid[zVelocity].floatNum = boidStructStack[i].z_velocity;
        
        individualBoid[boidID].numType = Boids::intType;
        individualBoid[boidID].intNum = boidStructStack[i].ID;
        
        individualBoid[boidSpecies].numType = Boids::intType;
        individualBoid[boidSpecies].intNum = boidStructStack[i].species;
        
        individualBoid[isBoidActive].numType = Boids::intType;
        individualBoid[isBoidActive].intNum = boidStructStack[i].active;
        
        boidStack.push_back(individualBoid);
    }
    
    return boidStack;
}
