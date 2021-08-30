#include <time.h>

#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <thread>
#include <string>
#include <functional>

#include "stdafx.h"
#include "AA+.h"

using namespace std;

#ifdef _MSC_VER
#pragma warning(push)  // We're not interested in unreferrenced variables in this app
#pragma warning(disable : 4101)
#pragma warning(disable : 4189)
#endif


void run(double start_date, double period, vector<string> &event_list) {
    int n = 0;
    int old_n = 0;

    for ( double x = start_date; x< (start_date+period); x+=(1.0/(24*60)) ) {  // res = 1m
        CAAEllipticalPlanetaryDetails JupiterDetails =
                CAAElliptical::Calculate(x, CAAElliptical::EllipticalObject::JUPITER, false);
        CAAGalileanMoonsDetails GalileanDetails = CAAGalileanMoons::Calculate(x, false);
        n = GalileanDetails.Satellite1.bInShadowTransit +
        + GalileanDetails.Satellite2.bInShadowTransit
        + GalileanDetails.Satellite3.bInShadowTransit
        + GalileanDetails.Satellite4.bInShadowTransit
        + GalileanDetails.Satellite3.bInTransit
        + GalileanDetails.Satellite4.bInTransit;

        const int minimum_number_of_events = 2;
        if ( (n >= minimum_number_of_events) && (old_n != n) ) {
            // Turnhout -4.95, 51.32
            // to calculate altitude of planet and sun
            CAAEllipticalPlanetaryDetails SunDetails =
                    CAAElliptical::Calculate(x, CAAElliptical::EllipticalObject::SUN, false);
            double AST = CAASidereal::ApparentGreenwichSiderealTime(x);
            double LongtitudeAsHourAngle =
            CAACoordinateTransformation::DegreesToHours(
                        CAACoordinateTransformation::DMSToDegrees(-4, 95, 0));

            double Alpha = JupiterDetails.ApparentGeocentricRA;
            double LocalHourAngle = AST - LongtitudeAsHourAngle - Alpha;
            CAA2DCoordinate JupiterHorizontal =
            CAACoordinateTransformation::Equatorial2Horizontal(LocalHourAngle,
            JupiterDetails.ApparentGeocentricDeclination,
            CAACoordinateTransformation::DMSToDegrees(51, 32, 0) );

            Alpha = SunDetails.ApparentGeocentricRA;
            LocalHourAngle = AST - LongtitudeAsHourAngle - Alpha;
            CAA2DCoordinate SunHorizontal =
                    CAACoordinateTransformation::Equatorial2Horizontal(
                        LocalHourAngle,
                        SunDetails.ApparentGeocentricDeclination,
                        CAACoordinateTransformation::DMSToDegrees(51, 32, 0) );

            // if ( (JupiterHorizontal.Y>0) && (SunHorizontal.Y<0) ) {
            if ( (JupiterHorizontal.Y > 0) ) {
                CAADate nu(x, true);
                ostringstream os;
                os <<  right << setw(2) << nu.Day() << "/"
                << setw(2) << nu.Month() << "/"
                << setw(4) << nu.Year() << " , "
                << setw(2) << nu.Hour() << "h"
                << setw(2) << nu.Minute() << "m"
                << setw(4) << " alt jup =  " << setw(8) << (JupiterHorizontal.Y)
                << setw(4) << " alt sun =  " << setw(8) << (SunHorizontal.Y)
                << " ";
                // io europa ganymedes callisto
                if (GalileanDetails.Satellite1.bInShadowTransit) os << "I";
                if (GalileanDetails.Satellite2.bInShadowTransit) os << "E";
                if (GalileanDetails.Satellite3.bInShadowTransit) os << "G";
                if (GalileanDetails.Satellite4.bInShadowTransit) os << "C";
                os << " ";
                if (GalileanDetails.Satellite3.bInTransit) os << "Gt";
                if (GalileanDetails.Satellite4.bInTransit) os << "Ct";
                os    << endl;
                //                cout << os.str();
                event_list.push_back(os.str());
            }
        }
        old_n = n;
    }
}

int main(int argc, char *argv[]) {
    clock_t start, end;
    start = clock();
    unsigned int startyear = 2014;
    unsigned int endyear = 2014;

    if ( argc != 3 ) {
        cout << "usage: " << argv[0] << " startyear endyear\n";
        exit(1);
    } else {
        startyear = atoi(argv[1]);
        endyear = atoi(argv[2]);
    }

    CAADate begin_date(startyear, 1, 1, 0, 0, 0, true);
    CAADate end_date(endyear, 12, 31, 23, 59, 59, true);
    double periode = end_date.Julian() - begin_date.Julian();

    vector<string> event_list1, event_list2, event_list3, event_list4;
    vector<string>::iterator pos_iter;

    thread thrd1(bind(&run, begin_date.Julian()              , periode/4. , ref(event_list1) ));
    thread thrd2(bind(&run, (begin_date.Julian()+periode/4.) , periode/4. , ref(event_list2) ));
    thread thrd3(bind(&run, (begin_date.Julian()+2*periode/4.) , periode/4. , ref(event_list3) ));
    thread thrd4(bind(&run, (begin_date.Julian()+3*periode/4.) , periode/4. , ref(event_list4) ));
    thrd1.join();
    thrd2.join();
    thrd3.join();
    thrd4.join();

    // iterate over container and print out
    for (pos_iter = event_list1.begin(); pos_iter != event_list1.end(); pos_iter++) {
        cout << *pos_iter;
    }
    for (pos_iter = event_list2.begin(); pos_iter != event_list2.end(); pos_iter++) {
        cout << *pos_iter;
    }
    for (pos_iter = event_list3.begin(); pos_iter != event_list3.end(); pos_iter++) {
        cout << *pos_iter;
    }
    for (pos_iter = event_list4.begin(); pos_iter != event_list4.end(); pos_iter++) {
        cout << *pos_iter;
    }

    end = clock();

    cout << "Time required for execution: "
    << (double)(end-start)/CLOCKS_PER_SEC
    << " seconds." << "\n\n";

    return 0;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
