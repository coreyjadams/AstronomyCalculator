//
//  astrocalc.cpp
//  astrocalc
//
//  Created by Corey Adams on 8/14/12.
//

#include <iostream>
#include "astrocalc.h"

 
    
//default constructor, sets position to fermilab, object to galactic center, time to current local time.  Altitude, azimuth, hourangle, etc set to zero.
Astrocalc::Astrocalc()
{
    latitude = 41.844054;
    longitude = -88.262426;
    declination = -25;
    rightAscension = 285;
    altitude = 0;
    azimuth = 0;
    hourangle = 0;
    
    LST = 0;
    GMST = 0;
    
    daysSinceJ2000 = 0;
    
    
    J2000.tm_year    = 100;             // initialize all of the J2000 parameters correctly
    J2000.tm_mon     = 0;
    J2000.tm_mday    = 1;
    J2000.tm_hour    = 12;
    J2000.tm_min     = 0;
    J2000.tm_sec     = 0;
    
    J2000raw = mktime(&J2000);          // creates the time_t object based of J2000 struct
                                        // contains number of seconds from some arbitrary start to J2000
    time(&nowraw);                      // sets nowTime to the current time
    now = *gmtime(&nowraw);             // fills in now with correct info
        
    setTime();
}

Astrocalc::Astrocalc(double lat, double lon, double dec, double ra)
{
    latitude = lat;
    longitude = lon;
    declination = dec;
    rightAscension = ra;
    altitude = 0;
    azimuth = 0;
    hourangle = 0;
    
    LST = 0;
    GMST = 0;
    
    daysSinceJ2000 = 0;
    
    
    J2000.tm_year    = 100;             // initialize all of the J2000 parameters correctly
    J2000.tm_mon     = 0;
    J2000.tm_mday    = 1;
    J2000.tm_hour    = 12;
    J2000.tm_min     = 0;
    J2000.tm_sec     = 0;
    
    J2000raw = mktime(&J2000);          // creates the time_t object based of J2000 struct
    // contains number of seconds from some arbitrary start to J2000
    time(&nowraw);                      // sets nowTime to the current time
    now = *gmtime(&nowraw);             // fills in now with correct info
    
    setTime();
}

//set target and targetTime to current local time, also UT
void Astrocalc::setTime(){
    
    time(&targetUTraw);
    targetUT = *gmtime(&targetUTraw);
    targetUT.tm_hour -= 1;  //this is an unbelievable error, I don't know why but unless you subtract 1 it becomes off by an hour.
    targetUTraw = mktime(&targetUT);

}

//set target and targetTime to specific local time, also initialize same time in UT
void Astrocalc::setTime(int month, int day, int year, int hour, int min, int sec){
    targetUT.tm_year    = year - 1900;      // set up as years from 1900
    targetUT.tm_mon     = month - 1;        //index starting at 0
    targetUT.tm_mday    = day;
    targetUT.tm_hour    = hour;
    targetUT.tm_min     = min;
    targetUT.tm_sec     = sec;
    
    targetUTraw = mktime(&targetUT);
    
}

void Astrocalc::refresh(){
    nowraw = time(NULL);                // sets nowTime to the current time
    now = *gmtime(&nowraw);             // fills in now with correct info
}


//by convention, positive number is after J2000
void Astrocalc::calcDaysSinceJ2000(){
    //difftime can calculate the difference in time between the target and J2000 in seconds
    
    double diff = difftime(targetUTraw, J2000raw);
    
    //convert the difference in seconds to days
    diff /= 60.0; //now in minutes 
    diff /= 60.0; //now in hours    
    diff /= 24.0; //now in days
    
    daysSinceJ2000 = diff;  //equal to number of days that have elapsed in Greenwich since J2000
    return;
    
}

void Astrocalc::calcSiderealTime(){
    GMST    = 18.697374558 + 24.06570982441908 * daysSinceJ2000; //in hours, convert to degrees
    
    while (GMST > 24) {
        GMST -= 24;
    }
    while (GMST < 0) {
        GMST += 24;
    } //adjust to appropriate range, 0 to 24 hours
    
    GMST   *= 15.0;
    
    //get local sidereal time by adding latitude to GMST
    LST     = GMST + longitude;
    while (LST > 360) {
        LST -= 360;
    }
    while (LST<0) {
        LST += 360;
    }
    return;
    
}

void Astrocalc::findTerrestrialCoordinates(){
    
    // in this case, know local sidereal time and also rightAscension and declination
    // find the altitude and azimuth, 
    
    calcDaysSinceJ2000();
    calcSiderealTime();
    hourangle = LST - rightAscension;
    
    double TanAz  = sin(hourangle*conv)/(cos(hourangle*conv)*sin(latitude*conv) - tan(declination*conv)*cos(latitude*conv));
    double SinAlt = sin(latitude*conv)*sin(declination*conv) + cos(latitude*conv)*cos(declination*conv)*cos(hourangle*conv);
        
    azimuth  = atan(TanAz)  / conv;
    altitude = asin(SinAlt) / conv;
    
    return;
    
}

void Astrocalc::findCelestialCoordinates(){
     
    // in this case, know the terrestrial coordinates and use that to find celestial coordinates
    calcDaysSinceJ2000();
    calcSiderealTime();
    
    double TanHA = sin(azimuth*conv)/(cos(azimuth*conv)*sin(latitude*conv) + tan(altitude*conv)*cos(latitude*conv));
    double SinDec = sin(latitude*conv)*sin(altitude*conv) - cos(latitude*conv)*cos(altitude*conv)*cos(azimuth*conv);
    
    hourangle   = atan(TanHA)/conv;
    declination = asin(SinDec)/conv;
    rightAscension = LST - hourangle;
    
    
    return;
}
