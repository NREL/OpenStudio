/*!
\file   geoEllips.c
\brief  This file contains geo library location setup and initialization
            of ellipsoid values. There are 23 ellipsoids to choose from, of which
            the WGS 84 ellipsoid is the default ellipsoid.
*/


#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "geoStars.h"

#ifdef WIN32
#include <windows.h>
#endif

#ifdef WIN32
#ifndef GEO_LIB
BOOL APIENTRY
DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH :
            //	FreeImage_Initialise(FALSE);
            break;

        case DLL_PROCESS_DETACH :
            //	FreeImage_DeInitialise();
            break;

        case DLL_THREAD_ATTACH :
        case DLL_THREAD_DETACH :
            break;
    }

    return TRUE;
}
#endif // GEO_LIB
#endif // WIN32
//-----------------------------------------------------------------
/*!
\brief This structure array contains the all of the ellipsoids used in this library

*/
GEO_ELLIPSOID ellips[] =
{
/*         Ellipsoid Name                                    ID   Major Axis  Inverse Flattening */
/*   -----------------------------------------------------------------------------------------   */
{ "Default Ellipsoid (WGS 1984)            ","00", 6378137.0   ,298.257223563 },  /*!< Default datum (WGS84) */
{ "Airy 1830                               ","AA", 6377563.396 ,299.3249646   },
{ "Australian National                     ","AN", 6378160.0   ,298.25        },
{ "Bessel 1841                             ","BR", 6377397.155 ,299.1528128   },
{ "Bessel 1841 (Namibia)                   ","BN", 6377483.865 ,299.1528128   },
{ "Clarke 1866                             ","CC", 6378206.4   ,294.9786982   },
{ "Clarke 1880                             ","CD", 6378249.145 ,293.465       },
{ "Everest (Brunei, E. Malaysia)           ","EB", 6377298.556 ,300.8017      },
{ "Everest 1830                            ","EA", 6377276.345 ,300.8017      },
{ "Everest 1956 (India and Nepal)          ","EC", 6377301.243 ,300.8017      },
{ "Everest (Pakistan)                      ","EF", 6377309.613 ,300.8017      },
{ "Everest 1948 (W. Malaysia and Singapore)","EE", 6377304.063 ,300.8017      },
{ "Everest 1969 (W. Malaysia)              ","ED", 6377295.664 ,300.8017      },
{ "Geodetic Reference System 1980          ","RF", 6378137.0   ,298.257222101 },
{ "Helmert 1906                            ","HE", 6378200.0   ,298.3         },
{ "Hough 1960                              ","HO", 6378270.0   ,297.0         },
{ "Indonesian 1974                         ","ID", 6378160.0   ,298.247       },
{ "International 1924                      ","IN", 6378388.0   ,297.0         },
{ "Krassovsky 1940                         ","KA", 6378245.0   ,298.3         },
{ "Modified Airy                           ","AM", 6377340.189 ,299.3249646   },
{ "Modified Fischer 1960                   ","FA", 6378155.0   ,298.3         },
{ "South American 1969                     ","SA", 6378160.0   ,298.25        },
{ "WGS 1972                                ","WD", 6378135.0   ,298.26        },
{ "WGS 1984                                ","WE", 6378137.0   ,298.257223563 }

};




//-----------------------------------------------------------------
/*!
\brief This routine needs to be called when a site (or location) is initialized.
Several of the routines use the information in the structure that this routine fills

\param  GEO_LOCATION *l
\param  double lat
\param  double lon
\param  double hgt
\param  int datum
\param  char *name //Name of the site location
\retval GEO_OK on success
\retval GEO_ERROR on error
*/


int DLL_CALLCONV geoInitLocation(GEO_LOCATION *l, double lat, double lon, double hgt, int datum,  char *name)
{
    double N,a,e2,ee2, b, flat,Nh;
    double dec;
    //	struct tm *newtime;
    //    time_t aclock;


    /* Validate and get datum values */
    if(datum > GEO_DATUM_MAX) return(GEO_ERROR);
    geoGetEllipsoid(&a,&b,&e2,&ee2,&flat,datum);     /* Calls to read in "to" ellipsoid data */

    /* Initialize ellipsoid values in the location descriptor */
    l->datum.a    = a;
    l->datum.b    = b;
    l->datum.e2   = e2;
    l->datum.flat = flat ;
    l->datum.ee2  = ee2;
    l->datum.m1e2 = 1.0 - e2;


    /* Initialize lat/lon values */
    if(lon>180.0)lon=lon-360.0; // normalize to -180 to +180
    l->lat = lat;
    l->lon = lon;
    l->hgt = hgt;

    l->rlat = lat * DEG_TO_RAD; /* make radians */
    l->rlon = lon * DEG_TO_RAD;

    /* Precompute sin/cos values */
#ifdef HAVE_SINCOS
    sincos(l->rlat,&l->slat,&l->clat);
    sincos(l->rlon,&l->slon,&l->clon);
#else
    l->slat = sin(l->rlat);
    l->slon = sin(l->rlon);
    l->clat = cos(l->rlat);
    l->clon = cos(l->rlon);
#endif 
    l->tlat = tan(l->rlat);
    l->clonclat = l->clon * l->clat;
    l->slonslat = l->slon * l->slat;
    l->clonslat = l->clon * l->slat;
    l->slonclat = l->slon * l->clat;

    /* Compute geocentric coordinates */

    /* Compute the radius of curvature */
    N = a / (sqrt(1.0 - e2 * pow(l->slat,2.0)));

    /* Compute the EFG(XYZ) coordinates (earth centered) */
    Nh = N + hgt;
    l->e = Nh * l->clonclat;  //l->clat * l->clon;
    l->f = Nh * l->slonclat;  //l->clat * l->slon;
    l->g = (N * (l->datum.m1e2) + hgt) * l->slat;
    l->efg[GEO_E] = l->e;
    l->efg[GEO_F] = l->f;
    l->efg[GEO_G] = l->g;

    /* Compute the Magnetic Declination */
     geoMagFillDec(l,&dec);

    /* Time Zone */
    l->timezone = 0.0; //use geoSetTimeZone to set this value

    /* Save datum and site info */
    l->datum.datum_num = datum;
    strcpy(l->name, name);

    return(GEO_OK);
}

void DLL_CALLCONV geoSetTimeZone(GEO_LOCATION *l, double tz, int dst)
{
    l->timezone = tz;
    l->dst = dst;
}

//-----------------------------------------------------------------
/*!
\brief This routine computes essential datum values from basic parameters
obtained from the \e ellips structure.

\param double *a       : Major axis   ( Meters )
\param double *b       : Minor axis   ( Meters )
\param double *e2      : Eccentricity
\param double *ee2     : Eccentricity prime
\param double *f       : Flattening
\param int datum       : Datum to use
\return nothing
*/

void    DLL_CALLCONV geoGetEllipsoid(double *a,double *b,double *e2,double *ee2,double *f,int datum)
{
    /* - Major Axis. */
    *a   = ellips[datum].a;

    /* - Earth Flattening. */
    *f   = GEO_FL(ellips[datum].f1);

    /* - Minor axis value. */
    *b   = GEO_B(ellips[datum].a, (ellips[datum].f1));

    /* - Eccentricity squared. */
    *e2  = GEO_E2(ellips[datum].a, (ellips[datum].f1));

    /* - Eccentricity squared prime. */
    *ee2 = GEO_E2P(ellips[datum].a, (ellips[datum].f1));


#ifdef GEO_DEBUG
    printf("DEBUG: a=%f, b=%f, f=%f, e2=%f, e2p=%f \n",*a, *b, *f, *e2, *ee2);
#endif
}


/*
 Deflection of the vertical
                            latitude        longitude      Xi      Eta   Hor Lap
   Station Name          ddd mm ss.sssss ddd mm ss.sssss arc-sec arc-sec arc-sec
 USER LOCATION            31 17 32.00000  85 51  3.00000   -4.22    0.30   -0.18



*/

