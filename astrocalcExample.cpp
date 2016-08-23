//
//  main.cpp
//  astrocalc
//
//  Created by Corey Adams on 8/14/12.
//

#include <iostream>
#include "astrocalc.h"

int main(int argc, const char * argv[])
{
    
    double uboone_heading = -30;
    
    // Example of how to use this code in microboone
    
    //initialize an instance of the astrocalc:
    
    Astrocalc a; //default constructor, so coordinates set to uboone lat/lon, and celestial object set to be the galactic center
    
    //to override these defaults, use:
    // Astrocalc a(lat, lon, dec, rightA);
    
    //upon initialization, astrocalc a sets the target time to the current time.  Override this:
    // a.setTime(month, day, year, hour, minute, sec); //sec is an optional parameter, default = 0;
    
    //suppose we want to know the terrestrial angles from microboone to GC.  Use:
    
    a.findTerrestrialCoordinates();
    
    double uboone_theta = a.getAltitude();
    double uboone_phi = a.getAzimuth() - uboone_heading;
    
    std::cout << "Currently, the galactic center is at uboone angles: theta = " << uboone_theta << ", phi = " << uboone_phi << std::endl;
    
    //to find the coordinates of an abritrary object at an arbitrary location, use the following:
    
//    a.setAltitude(alt);
//    a.setAzimuth(az);
//    a.setDeclination(dec);
//    a.setRightAscension(ra);
//    a.setLatitude(lat);
//    a.setLongitude(lon);
    
    
    //the function a.findCelestialCoordinates() will use alt, az, lon, lat and the targetUT time (which is now by default, can be overriden with setTime()) to find declination, hourangle, and right ascension
    
    return 0;
}

