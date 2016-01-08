/*!
\file geoAstro.c
\brief This file contains the wrapper routines to interface to the
       Novas (Naval Observatory Vector Astrometry Subroutine) Library.

       http://aa.usno.navy.mil/software/novas/novas_info.html

*/

#include <stdio.h>
#include <time.h>
#include "geoStars.h"
#include "novas.h"

int geoSunError = 0;
struct tm tError;


//-----------------------------------------------------------------
/*!
\brief this routine uses ANSI C time routines to obtain the current time
and then get the az and el of the sun at this location

\param  GEO_LOCATION *loc    : Location structure
\param  double *az           : Azimuth in decimal degrees
\param  double *el           : Elevation in decimal degrees
\retval GEO_OK on success
\retval GEO_ERROR on error
*/

int DLL_CALLCONV geoSunAzElNow(GEO_LOCATION *loc, double *az, double *el)
{
    struct tm *newtime;
    long ltime=0L;

    /* Obtain coordinated universal time: */
    time( &ltime );
    newtime = gmtime( &ltime );

    return ( geoSunAzEltm(loc, az, el, newtime) );
}

//-----------------------------------------------------------------
/*!
\brief this routine uses the location and current time
to get the elevation of the sun at this location

\param  double lat    : Latitude in degrees
\param  double lon    : Longitude in degrees
\param  double hgt    : Height in meters
\retval  double       : Elevation in decimal degrees
*/

double DLL_CALLCONV geoSunNowEl(double lat, double lon, double hgt)
{
    GEO_LOCATION loc;
    double az,el;

    geoInitLocation(&loc, lat, lon, hgt, GEO_DATUM_DEFAULT,  "now");
    geoSunAzElNow(&loc, &az, &el);
    return(el);
}

//-----------------------------------------------------------------
/*!
\brief this routine uses the location and current time
to get the azimuth of the sun at this location

\param  double lat    : Latitude in degrees
\param  double lon    : Longitude in degrees
\param  double hgt    : Height in meters
\retval  double       : Azimuth in decimal degrees
*/

double DLL_CALLCONV geoSunNowAz(double lat, double lon, double hgt)
{
    GEO_LOCATION loc;
    double az,el;

    geoInitLocation(&loc, lat, lon, hgt, GEO_DATUM_DEFAULT,  "now");
    geoSunAzElNow(&loc, &az, &el);
    return(az);
}


//-----------------------------------------------------------------
/*!
\brief this routine ingests an ANSI C time structure and then gets the
az and el of the sun at this location based on this time.

\param  GEO_LOCATION *loc    : Location structure
\param  double *az           : Azimuth in decimal degrees
\param  double *el           : Elevation in decimal degrees
\retval GEO_OK on success
\retval GEO_ERROR on error
*/

int DLL_CALLCONV geoSunAzEltm(GEO_LOCATION *loc, double *az, double *el, struct tm *newtime)
{
    /* Obtain coordinated universal time: */
    double tjd_now;

    tError.tm_year=newtime->tm_year;
    tError.tm_mon=newtime->tm_mon;
    tError.tm_mday=newtime->tm_mday;

    tjd_now = julian_date((short)(newtime->tm_year+1900),
    (short)(newtime->tm_mon+1),
    (short)newtime->tm_mday,
    newtime->tm_hour + (newtime->tm_min/60.0)+(newtime->tm_sec/3600.0));

    return ( geoSunAzElJD(loc, az, el,tjd_now) );
}


int DLL_CALLCONV geoGettm(int part)
{
    switch(part)
    {
        case 1: return(tError.tm_year);
        case 2: return(tError.tm_mon);
        case 3: return(tError.tm_mday);
    }
    return(-999);
}


#define MAJOR_PLANET 0    //! \internal  major planet, sun or moon
#define BODY_EARTH   3    //! \internal  Earth
#define BODY_SUN     10   //! \internal  Sun
#define REDUCED_ACC  1    //! \internal  Reduced Accuracy (0=full, 1=reduced)


//-----------------------------------------------------------------
/*!
\brief This routine uses a previously determined \e Julian \e date and then gets the
Az and El of the sun at this location based on the Julian date.

\param  GEO_LOCATION *loc    : Location structure
\param  double *az           : Azimuth in decimal degrees
\param  double *el           : Elevation in decimal degrees
\retval GEO_OK on success
\retval GEO_ERROR on error
*/


int DLL_CALLCONV geoSunAzElJD(GEO_LOCATION *loc, double *az, double *el, double tjd_now)
{
    on_surface geo_loc;
    cat_entry  dummy_star;
    object     sun;
    double deltat = 60.0;     // 'deltat' is the difference in time scales, TT - UT1.
    double ra, dec, dis;
    double rar,decr;
    //   int error;

    /* Convert geo library info into Novas info */
    // geo_loc.latitude  =    loc->lat;
    // geo_loc.longitude =    loc->lon;
    // geo_loc.height    =    loc->hgt;
    // geo_loc.temperature =  30.0;     // degrees celcius
    // geo_loc.pressure  =    1000.0;   // pressure in millibars
    make_on_surface(loc->lat, loc->lon, loc->hgt, 30.0, 1000.0, &geo_loc);


    /* Set the planetary bodies to be worked with */
    /*geoSunError = set_body (MAJOR_PLANET,BODY_EARTH,"Earth", &earth);
   if(geoSunError)
   {
    //  printf ("Error %d from set_body.\n", geoSunError);
      return(GEO_ERROR);
   }
*/

    geoSunError = make_object (MAJOR_PLANET,BODY_SUN,"Sun",&dummy_star, &sun);
    if(geoSunError)
    {
        //printf ("Error %d from make_object.\n", geoSunError);
        return(GEO_ERROR);
    }

    /* Compute the Sun's position */
    //   geoSunError = topo_planet (tjd_now,&sun,&earth,deltat,&geo_loc, &ra,&dec,&dis);
    geoSunError = topo_planet (tjd_now,&sun,deltat,&geo_loc, REDUCED_ACC, &ra,&dec,&dis);
    if(geoSunError)
    {
        //  printf ("Error %d from topo_planet.\n", geoSunError);
        return(GEO_ERROR);
    }

    /* Convert to Azimuth and Elevation */
    equ2hor(tjd_now,deltat,REDUCED_ACC,0,0,&geo_loc,ra,dec,0,el,az,&rar,&decr);
    *el = 90.0 - *el; // correct elevation from zenith

    return(GEO_OK);
}

int DLL_CALLCONV  geoGetSunError(void)
{
    return(geoSunError);
}



