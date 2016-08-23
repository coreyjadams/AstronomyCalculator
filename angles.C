/*compute angles from microboone to the galactic center

*/

#include <iostream>
#include <string.h>
#include "astrocalc.h"

//time zone difference from UT (GMT) to FNAL
#define CST (-6)


//Coordinates of microboone
Double_t longitutde = -88.262426;
Double_t latitude = 41.844054;
Double_t zero_heading = -0.989;  //the angle that is uboone_phi =0, in degrees measured clockwise from north

//celestial coordinates of object in sky - currently sagittarius (galactic center)
Double_t declination = -25;
Double_t right_ascension = 285;


void run(){

	Astrocalc a(latitude, longitutde, declination, right_ascension);
    
	//by default, a has time set to the current time.  Override with:
	//a.setTime(month, day, year, hour, minute, sec); //sec is an optional parameter, default = 0;

	//run the calculation to find the coordinates
	a.findTerrestrialCoordinates();

    std::cout << "The object is currently " << a.getAltitude() << " degrees above the horizon." << endl;
    std::cout << "The object is currently " << a.getAzimuth() << " degrees away from north, rotating clockwise." << endl;
    
    Double_t uboone_theta, uboone_phi;
    
    //the altitude is the angle above the horizon to the object.  This is exactly the angle theta in the detector.
    uboone_theta = a.getAltitude();
    
    //the azimuth is the angle from north, and so uboone_phi will be the difference between zero_heading and azi.
    uboone_phi = a.getAzimuth() - zero_heading;
    
    std::cout << "In the detector, theta is " << uboone_theta << endl;
    std::cout << "In the detector, phi is " << uboone_phi << endl;
}

//function to fill a TTree with uboone_theta, uboone_phi over a period of time
void fill_tree(){
    TFile *savefile = new TFile("angles.root", "RECREATE");
	TTree *tree = new TTree("Angles","Angles"); 
    double ubooneTheta;
	double uboonePhi;
	//initialize the astrocalc object with celestial coordinates and geographic coordinates
	Astrocalc a(latitude, longitutde, declination, right_ascension);
    int year, month, day, hour, min; //time to start at
    
	//Stuff for 2D Root Histogram
	TH2F * ThetaVsPhi=new TH2F("ThetaVsPhi","ThetaVsPhi",360,-180,179,180,-90,89);
	//want phi on x and theta on y
	
	int minutecount = 0, daymax;
	
    //run for one full year, sample every minute?

    year = 2015;
	
	tree -> Branch("UbooneTheta",&ubooneTheta, "uboonePhi/D");
	tree -> Branch("UboonePhi",&uboonePhi, "uboonePhi/D");
	tree -> Branch("Minute", &minutecount, "i/I");
	
	//Check the histogram by looking at max values
	double thetamax, phimax;
    
//	//loop over months
//	for (month = 0; month < 12; month ++) {
//		
//		//determine how many days in the current month
//		if (month == 0 || month == 2 || month == 4 || month == 6 || month == 7 || month == 9 || month == 11) {
//			daymax = 31;
//		}
//		else if (month == 2 && year % 4 == 0){
//			daymax = 29;
//		}
//		else if (month == 2 && year % 4 != 0){
//			daymax = 28;
//		}
//		else daymax = 30;
//		
//		
//		//loop over days in this month
//		for (day = 0; day < daymax; day ++) {
			//loop over minutes in this day
			for (int i = 0; i < 1440; i ++) {
				//loop over declination region and right ascension region, add 1 degrees in steps of .25 to each
				
			for (double dec = -1.0; dec <= 1 ; dec += 0.25) {
			for (double ra = -1.0; ra <= 1; ra += 0.25) {
						
				
				//find the hour, minute corresponding to i
				hour = i / 60;
				min = i % 60;
				//reset the time for the astrocalc
				a.setTime(month, day, year, hour, min);
				//adjust the angles if necessary
				a.setRightAscension(right_ascension + ra);
				a.setDeclination(declination + dec);
				//solve for the coordinates in the terrestrial system
				a.findTerrestrialCoordinates();
				//convert to uboone coordinates
				ubooneTheta = a.getAltitude();
				uboonePhi   = a.getAzimuth() - zero_heading;
				//add to the histogram
				ThetaVsPhi -> Fill(uboonePhi, ubooneTheta);
				
				tree -> Fill();
				}
				}
			}
//		}
//	}

	
	//Now Plot the 2D histogram:
	ThetaVsPhi -> GetXaxis() -> SetTitle("Phi");
	ThetaVsPhi -> GetYaxis() -> SetTitle("Theta");

	
	TCanvas * c1=new TCanvas("ThetaVsPhiCan","ThetaVsPhiCan");
	ThetaVsPhi->DrawCopy("COLZ");
	
    tree -> Print();

	std::cout << "Theta max: " << thetamax << ", Phi max: " << phimax << endl;
    
	savefile -> Write();
    savefile -> Close();
}


void plot_maker(){
    TF1 *f2 = new TF1("altitude", func, 0, 360, 0);
    f2 -> SetTitle("altitude");
    f2 -> Draw();
}

Double_t func(Double_t *x, Double_t par[] = 0){
    return get_altitude(x[0]);
}

