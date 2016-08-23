//
//  astrocalc.h
//  astrocalc
//
//  Created by Corey Adams on 8/14/12.
//
//   
//  Class is not related to microboone, argoneut or LBNE in anyway, simply takes in coordinates
//  and transforms them to another system.  See main.cpp in this folder for an example of how to
//  use this class.

#include <iostream>
#include "time.h"
#include "math.h"

#ifndef _astrocalc_h
#define _astrocalc_h

#define conv (0.01745329251)


class Astrocalc {
    
public:
    
    void setLatitude(double lat)        {latitude = lat; return;}
    void setLongitude(double lon)       {longitude = lon; return;}
    
    void setDeclination(double dec)     {declination = dec; return;}
    void setRightAscension(double ra)   {rightAscension = ra; return;}
    
    void setAzimuth(double az)          {azimuth = az; return;}
    void setAltitude(double alt)        {altitude = alt; return;}
	
	void printTargetTime()				{std::cout << ctime(&targetUTraw); return;}
    
    
    double getLatitude()                {return latitude;}
    double getLongitude()               {return longitude;}
    double getAltitude()                {return altitude;}
    double getAzimuth()                 {return azimuth;}
    double getDeclination()             {return declination;}
    double getRightAscension()          {return rightAscension;}
    
    //returns the Local Sidereal Time for the latitude
    double getLST()                     {return LST;}
    //returns the Greenwich Mean Sidereal Time
    double getGMST()                    {return GMST;}
    
    void refresh();     //resets now and nowTime to current time
    
    void setTime();  //sets target to current time;

    //set target time to specified time
    void setTime(int month, int day, int year, int hour, int min, int sec=0);
    
    void findTerrestrialCoordinates();  //uses lat and lon already defined
    
    void findCelestialCoordinates(); //return celes. coord. from 

    
    Astrocalc();
    Astrocalc(double lat, double lon, double dec, double ra);
    
    
private:
    
    double latitude, longitude;         // coordinates of the position on earth
    double declination, rightAscension; // celestial coordinates of interest
    double altitude, azimuth;           // coordinates in the horizontal frame
    double hourangle;                   // local hour angle
    
    double LST, GMST;                   // sidereal times
    
    
    long double daysSinceJ2000;         // days since Jan 1, 2000 1200h in the UTC zone

    tm J2000, now, targetUT;            // structure to hold information about the time
                                        // in an easy to use form

    time_t J2000raw, nowraw, targetUTraw;      // time_t objects, same thing

    
    void calcSiderealTime();            // calculates the Sidereal Times
    void calcDaysSinceJ2000();          // calculates days since J2000 at GMT;
    
    
};




#endif
