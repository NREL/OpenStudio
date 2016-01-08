/*! \file geoEfg2Llh.c
    \brief  This file contains the geo library routines for
        conversion between Geocentric and Geodetic coordinates

  Methods avalailable:
       \li geoStarLib method
       \li Hirvonen & Moritz method
       \li Torge method
       \li Astronomical Almanac 2002 method
       \li Borkowski method
       \li Bowring method

*/
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "geoStars.h"

double geoAccuracy = GEO_EFG2LLH_ACCURACY;

static int its;   // keep track of the number of iterations

int    DLL_CALLCONV geoEfg2LlhGetIts (void)
{
    return(its);
}

//-----------------------------------------------------------------
/*! \brief This routine will set the accuracy of the iterative
Efg2Llh routines (geoEfg2Llh_hm, geoEfg2Llh_torge, geoEfg2Llh_bowring,
 geoEfg2Llh_aa)

 \param  double acc   : Accuracy in degrees
\retval GEO_OK on success
\retval GEO_ERROR on error

  Several definitions may be used with this function:
 GEO_EFG2LLH_ACCURACY_METER = 0.00001 degrees
 GEO_EFG2LLH_ACCURACY_CM    = 0.0000001 degrees
 GEO_EFG2LLH_ACCURACY_MM    = 0.00000001 degrees

*/
int DLL_CALLCONV geoSetAccuracy(double acc)
{
    if (acc <= 1.0) // make sure the accuracy is sane
    {
        geoAccuracy = acc;
        return(GEO_OK);
    }
    return(GEO_ERROR);
}

//-----------------------------------------------------------------
/*! \brief This routine will convert earth centered Cartesian
coordinates (E,F,G), into geodetic coordinates (latitude
\f$\phi\f$, longitude \f$\lambda\f$, and ellipsoid height \f$h\f$).

 \param  int datum
 \param  double efg[]   : EFG(xyz) in METERS
 \param  double *lat    : Latitude in RADIANS
 \param  double *lon    : Longitude in RADIANS
 \param  double *hgt    : Height in METERS
 \return nothing

\note This routine will be exact only for WGS84 coordinates. All other
datums will be slightly off.

*/

void    DLL_CALLCONV geoEfg2Llh (int datum,  double efg[],
double *lat, double *lon, double *hgt)
{
    double p, u, u_prime, a, flat, b, e2, ee2,sign = 1.0,cu_prime,su,cu,t1;

    /* Get the ellipsoid parameters */
    geoGetEllipsoid (&a, &b, &e2, &ee2, &flat, datum);
    its=0;

    // Computes EFG to lat, lon & height
    p = sqrt(sqr(efg[GEO_E]) + sqr(efg[GEO_F]));
    u = atan((efg[GEO_G]/p)  * (a/b));
    //    u = atan2(efg[GEO_G] * a , p * b);
    su = sin(u);
    cu = cos(u);

    *lat = atan((efg[GEO_G] + ee2 * b * cube(su) ) /
    ( p - e2 * a * cube(cu) ) );

    u_prime =  atan((1.0 - flat) * tan(*lat));
    cu_prime = p - a * cos(u_prime);
    if(cu_prime < 0.0) sign= -1.0; // determine sign

    //     *hgt = p / cos(*lat) - ( a / (sqrt(1.0 - e2 * pow(sin(*lat),2.0))));   //same results
    t1 = efg[GEO_G] - b * sin(u_prime);
    *hgt =  sign * sqrt( sqr(cu_prime) + sqr(t1) );
    *lon = atan2(efg[GEO_F],efg[GEO_E]);  // atan(f/e)

}

double DLL_CALLCONV geoEfg2Lat (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lat);
}

double DLL_CALLCONV geoEfg2Lon (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lon);
}
double DLL_CALLCONV geoEfg2Hgt (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh (datum,efg,&lat,&lon,&hgt);
    return(hgt);
}


/*! \brief Hirvonen & Moritz Method - This routine will convert earth centered Cartesian
coordinates (E,F,G), into geodetic coordinates (latitude
\f$\phi\f$, longitude \f$\lambda\f$, and ellipsoid height \f$h\f$).

 \param  int datum
 \param  double efg[]   : EFG(xyz) in METERS
 \param  double *lat    : Latitude in RADIANS
 \param  double *lon    : Longitude in RADIANS
 \param  double *hgt    : Height in METERS
 \return nothing

\note This method is a successive approximation

*/

void    DLL_CALLCONV geoEfg2Llh_hm (int datum,  double efg[],
double *lat, double *lon, double *hgt)
{
    double  a, flat, b, e2, ee2;
    double lat1,tmp,n=0.0,delta,p,slat;
    //    int done=1;
    int i;

    if(efg[GEO_G] == 0.0) efg[GEO_G] = 0.00000001; //cheat to avoid bad numbers

    /* Get the ellipsoid parameters */
    geoGetEllipsoid (&a, &b, &e2, &ee2, &flat, datum);
    p = sqrt(sqr(efg[GEO_E])+ sqr(efg[GEO_F]));
    *lon = atan2(efg[GEO_F],efg[GEO_E]);  // atan(f/e)
    its=0;

    // Prepare the initial value of lat
    tmp = efg[GEO_G] /  p;
    lat1 = atan((1.0 / (1.0 - e2))*tmp);
    *lat=lat1;

    // iterate until delta lat is tiny
    for(i=0;i<GEO_EFG2LLH_MAX_ITS;i++)
    {
        its++;
        slat = sin(lat1);
        n= a/sqrt(1.0 - e2 * sqr(slat));
        *lat=atan(tmp*(1.0 + (e2*n*slat)/efg[GEO_G]));
        delta = *lat - lat1;
        if(fabs(delta) <= geoAccuracy) break;
        //        if(fabs(delta) == 0.0) break;
        lat1=*lat;
    }
    *hgt = (p / cos(*lat)) - n;

}

double DLL_CALLCONV geoEfg2Lat_hm (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_hm (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lat);
}

double DLL_CALLCONV geoEfg2Lon_hm (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_hm (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lon);
}
double DLL_CALLCONV geoEfg2Hgt_hm (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_hm (datum,efg,&lat,&lon,&hgt);
    return(hgt);
}

double DLL_CALLCONV geoEfg2Hgt_hm_its (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_hm (datum,efg,&lat,&lon,&hgt);

    return((double)geoEfg2LlhGetIts());
}

/*! \brief Torge Method - This routine will convert earth centered Cartesian
coordinates (E,F,G), into geodetic coordinates (latitude
\f$\phi\f$, longitude \f$\lambda\f$, and ellipsoid height \f$h\f$).

 \param  int datum
 \param  double efg[]   : EFG(xyz) in METERS
 \param  double *lat    : Latitude in RADIANS
 \param  double *lon    : Longitude in RADIANS
 \param  double *hgt    : Height in METERS
 \return nothing

\note This method is a successive approximation

*/

void    DLL_CALLCONV geoEfg2Llh_torge (int datum,  double efg[],
double *lat, double *lon, double *hgt)
{
    double  a, flat, b, e2, ee2;
    double lat1,hgt1,tmp,n=0.0,deltal,deltah,p,slat;
            //,clat;
    //    int done=1;
    int i;

    /* Get the ellipsoid parameters */
    geoGetEllipsoid (&a, &b, &e2, &ee2, &flat, datum);
    p = sqrt(sqr(efg[GEO_E])+ sqr(efg[GEO_F]));
    *lon = atan2(efg[GEO_F],efg[GEO_E]);  // atan(f/e)
    its=0;

    // Prepare the initial value of lat
    tmp = efg[GEO_G] /  p;
    //    lat1 = atan(tmp/(1.0 - e2));  //assume hgt = 0 to start
    lat1 = atan(tmp);  //assume hgt = 0 to start
    *lat=lat1;
    *hgt=0.0;


    // iterate until delta lat is tiny
    for(i=0;i<GEO_EFG2LLH_MAX_ITS;i++)
    {
        its++;
        lat1=*lat;
        hgt1 = *hgt;

        slat = sin(lat1);
        //clat = cos(lat1);

        // radius of curvature estimate
        n= a/sqrt(1.0 - e2 * sqr(slat));

        // Next: estimates of lat/hgt
        *lat=atan2(tmp , (1.0 - (e2 * ( n / (n + *hgt)) ) ) );
        *hgt = (p / cos(*lat)) - n;
        deltal = *lat - lat1;
        deltah = *hgt - hgt1;
        if((fabs(deltah) <= .00001) && (fabs(deltal)  <= geoAccuracy)) break;
    }

}

double DLL_CALLCONV geoEfg2Lat_torge (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_torge (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lat);
}

double DLL_CALLCONV geoEfg2Lon_torge (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_torge (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lon);
}
double DLL_CALLCONV geoEfg2Hgt_torge (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_torge (datum,efg,&lat,&lon,&hgt);
    return(hgt);
}

double DLL_CALLCONV geoEfg2Hgt_torge_its (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_torge (datum,efg,&lat,&lon,&hgt);

    return((double)geoEfg2LlhGetIts());
}

//-----------------------------------------------------------------
/*! \brief Bowring Method - This routine will convert earth centered Cartesian
coordinates (E,F,G), into geodetic coordinates (latitude
\f$\phi\f$, longitude \f$\lambda\f$, and ellipsoid height \f$h\f$).

 \param  int datum
 \param  double efg[]   : EFG(xyz) in METERS
 \param  double *lat    : Latitude in RADIANS
 \param  double *lon    : Longitude in RADIANS
 \param  double *hgt    : Height in METERS
 \return nothing

\note This method is a successive approximation

*/
void    DLL_CALLCONV geoEfg2Llh_bowring (int datum,  double efg[],
double *lat, double *lon, double *hgt)
{
    double  a, flat, b, e2, ee2;
    double tmp,n,b0,b1,p,slat,clat,ee2b,ae2,boa;
    //    int done=1;
    int i;

    /* Get the ellipsoid parameters */
    geoGetEllipsoid (&a, &b, &e2, &ee2, &flat, datum);
    *lon = atan2(efg[GEO_F],efg[GEO_E]);  // atan(f/e)
    p = sqrt(sqr(efg[GEO_E])+ sqr(efg[GEO_F]));
    tmp = efg[GEO_G] /  p;
    its=0;

    // temps pulled out of the loop
    ae2  = a * e2;
    ee2b = ee2 * b;
    boa = b/a;

    // Prepare the initial value of lat
    b0 = atan((a/b)*tmp);

    // iterate until delta lat is tiny
    for(i=0;i<GEO_EFG2LLH_MAX_ITS;i++)
    {
        its++;
        b1=b0;
        slat = sin(b1);
        clat = cos(b1);
        *lat = atan((efg[GEO_G]+ ee2b * cube(slat) ) /
        (p - ae2 * cube(clat) ) );
        b0 = atan((boa) * tan(*lat));
        if(fabs(b0-b1)  <= geoAccuracy) break;
        //        if(fabs(b1-b0)== 0.0) break;
        //        if(b1-b0== 0.0) break;
    }
    slat = sin(*lat);
    n= a/sqrt(1.0-e2*sqr(slat));

    *hgt = (p / cos(*lat)) - n;

}

double DLL_CALLCONV geoEfg2Lat_bowring (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_bowring (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lat);
}

double DLL_CALLCONV geoEfg2Lon_bowring (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_bowring (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lon);
}
double DLL_CALLCONV geoEfg2Hgt_bowring (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_bowring (datum,efg,&lat,&lon,&hgt);
    return(hgt);
}

double DLL_CALLCONV geoEfg2Hgt_bowring_its (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_bowring (datum,efg,&lat,&lon,&hgt);
    return((double)geoEfg2LlhGetIts());
}

//-----------------------------------------------------------------
/*! \brief Astronomical Almanac 2002 Method - This routine will convert earth centered Cartesian
coordinates (E,F,G), into geodetic coordinates (latitude
\f$\phi\f$, longitude \f$\lambda\f$, and ellipsoid height \f$h\f$).

 \param  int datum
 \param  double efg[]   : EFG(xyz) in METERS
 \param  double *lat    : Latitude in RADIANS
 \param  double *lon    : Longitude in RADIANS
 \param  double *hgt    : Height in METERS
 \return nothing

\note This method is a successive approximation from Astronomical Almanac 2002 K11-12

*/
void    DLL_CALLCONV geoEfg2Llh_aa (int datum,  double efg[],
double *lat, double *lon, double *hgt)
{
    double phi,p,n=1.0,phi1;
    int i;

    double a, flat, b, e2, ee2;
    double slat;

    /* Get the ellipsoid parameters */
    geoGetEllipsoid (&a, &b, &e2, &ee2, &flat, datum);

    // Computes EFG to lat, lon & height
    // from the Astronomical Almanac 2002 - K11-12
    *lon = atan2(efg[GEO_F],efg[GEO_E]);  // atan(f/e)
    p = sqrt(sqr(efg[GEO_E]) + sqr(efg[GEO_F]));

    phi = atan(efg[GEO_G]/p); // first approximation
    //	 phi = atan(efg[GEO_G]/((1.0-e2)*p)); // faster

    its=0;

    // compute until the error in phi is no more
    for(i=0;i<GEO_EFG2LLH_MAX_ITS;i++)
    {
        its++;
        phi1 = phi;
        slat = sin(phi1);
        //        c = 1.0/sqrt(1.0 - e2 * sqr(slat));
        //        phi = atan((efg[GEO_G] + a * c * e2 * slat)/r);
        n = a/sqrt(1.0 - e2 * sqr(slat));
        phi = atan((efg[GEO_G] + n * e2 * slat)/p);
        if( fabs(phi1-phi)  <= geoAccuracy) break;
    }
    *lat = phi;
    //     *hgt = r / cos(phi) - a * c;
    *hgt = p / cos(phi) - n;

}

double DLL_CALLCONV geoEfg2Lat_aa (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_aa (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lat);
}

double DLL_CALLCONV geoEfg2Lon_aa (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_aa (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lon);
}
double DLL_CALLCONV geoEfg2Hgt_aa (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_aa (datum,efg,&lat,&lon,&hgt);
    return(hgt);
}

double DLL_CALLCONV geoEfg2Hgt_aa_its (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_aa (datum,efg,&lat,&lon,&hgt);
    return((double)geoEfg2LlhGetIts());
}


//-----------------------------------------------------------------
/*! \brief Borkowski Method - This routine will convert earth centered Cartesian
coordinates (E,F,G), into geodetic coordinates (latitude
\f$\phi\f$, longitude \f$\lambda\f$, and ellipsoid height \f$h\f$).

 \param  int datum
 \param  double efg[]   : EFG(xyz) in METERS
 \param  double *lat    : Latitude in RADIANS
 \param  double *lon    : Longitude in RADIANS
 \param  double *hgt    : Height in METERS
 \return nothing

\note This method is closed exact method from Bull. Geod., 63 (1989), pp. 50-56

*/
void    DLL_CALLCONV geoEfg2Llh_borkowski (int datum,  double efg[],
double *lat, double *lon, double *hgt)
{
    double a, flat, b, e2, ee2;
    double r,e,f,p,q,d,v,g,t,sign,bg,samb,sqrtd,sqre;


    /* Get the ellipsoid parameters */
    geoGetEllipsoid (&a, &b, &e2, &ee2, &flat, datum);
    its=0;

    // Computes EFG to lat, lon & height
    // from Bull. Geod., 63 (1989), pp. 50-56
    // Accurate Algorithms to Transform Geocentric to Geodetic Coordinates
    // By K.M. Borkowski

    *lon = atan2(efg[GEO_F],efg[GEO_E]);  // atan(f/e)

    //use the sign for Z
    if(efg[GEO_G] < 0.0) sign = -1.0;
    else sign = 1.0;
    b = fabs(b) * sign;
    bg = b * efg[GEO_G];  //temp
    samb = sqr(a)-sqr(b) ;//temp
    r = efg[GEO_E] / cos(*lon);
    e = (bg - samb) / (a*r);
    sqre = sqr(e);
    f = (bg + samb) / (a*r);
    p = 4.0/3.0*(e*f+1.0);
    q = 2.0 * (sqre-sqr(f));
    d = cube(p) + sqr(q);
    sqrtd = sqrt(d);
    if(d < 0.0)
        v = 2.0 * sqrt(-p) * cos(acos(q/pow(-p,3.0/2.0))/3.0);
    else
        v = pow((sqrtd - q),1.0/3.0) - pow((sqrtd + q),1.0/3.0);
    g = (sqrt(sqre+v) + e ) / 2.0;
    t = sqrt(sqr(g)+((f-v*g)/(2.0*g-e))) - g;
    *lat = atan(a*(1.0-sqr(t))/(2.0*b*t));

    *hgt = (r - a*t) * cos(*lat) + (efg[GEO_G]-b)*sin(*lat);

}

double DLL_CALLCONV geoEfg2Lat_borkowski (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_borkowski (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lat);
}

double DLL_CALLCONV geoEfg2Lon_borkowski (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_borkowski (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lon);
}
double DLL_CALLCONV geoEfg2Hgt_borkowski (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_borkowski (datum,efg,&lat,&lon,&hgt);
    return(hgt);
}


//-----------------------------------------------------------------
/*! \brief Vermeille Method - This routine will convert earth centered Cartesian
coordinates (E,F,G), into geodetic coordinates (latitude
\f$\phi\f$, longitude \f$\lambda\f$, and ellipsoid height \f$h\f$).

 \param  int datum
 \param  double efg[]   : EFG(xyz) in METERS
 \param  double *lat    : Latitude in RADIANS
 \param  double *lon    : Longitude in RADIANS
 \param  double *hgt    : Height in METERS
 \return nothing

\note This method is closed exact method from ....

*/
void    DLL_CALLCONV geoEfg2Llh_vermeille (int datum,  double efg[],
double *lat, double *lon, double *hgt)
{
    double a, flat, b, e2, ee2;
    double a2,x2,y2,z2,d2,e4,p,q,r,s,t,u,v,w,k,d,sqrtx2py2,sqrtd2pz2;


    /* Get the ellipsoid parameters */
    geoGetEllipsoid (&a, &b, &e2, &ee2, &flat, datum);
    its=0;

    // precompute variables
    x2 = sqr(efg[GEO_E]);
    y2 = sqr(efg[GEO_F]);
    z2 = sqr(efg[GEO_G]);
    a2 = sqr(a);
    e4 = sqr(e2);
    sqrtx2py2 = sqrt(x2+y2);

    // main algorithm
    p = (x2 + y2)/ a2;
    q = ((1.0-e2)/ a2)*z2;
    r = (p+q-e4)/6.0;
    s = e4 * (p*q)/(4.0*pow(r,3.0));
    t = pow(1.0+s+sqrt(s*(2.0+s)),1.0/3.0);
    u = r*(1.0+t+(1.0/t));
    v = sqrt(sqr(u)+e4*q);
    w = e2*(u+v-q)/(2.0*v);
    k = sqrt(u+v+sqr(w)) - w;
    d = k*sqrtx2py2/(k+e2);
    d2 = sqr(d);
    sqrtd2pz2 = sqrt(d2+z2);

    *lon = 2 * atan(efg[GEO_F]/(efg[GEO_E] + sqrtx2py2));

    *lat = 2 * atan(efg[GEO_G]/(d + sqrtd2pz2));

    *hgt = ((k + e2 - 1.0) / k) * sqrtd2pz2;

}

double DLL_CALLCONV geoEfg2Lat_vermeille (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_vermeille (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lat);
}

double DLL_CALLCONV geoEfg2Lon_vermeille (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_vermeille (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lon);
}
double DLL_CALLCONV geoEfg2Hgt_vermeille (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_vermeille (datum,efg,&lat,&lon,&hgt);
    return(hgt);
}


//-----------------------------------------------------------------
/*! \brief heikkinen Method - This routine will convert earth centered Cartesian
coordinates (E,F,G), into geodetic coordinates (latitude
\f$\phi\f$, longitude \f$\lambda\f$, and ellipsoid height \f$h\f$).

 \param  int datum
 \param  double efg[]   : EFG(xyz) in METERS
 \param  double *lat    : Latitude in RADIANS
 \param  double *lon    : Longitude in RADIANS
 \param  double *hgt    : Height in METERS
 \return nothing

\note This method is closed exact method from ....

*/
void    DLL_CALLCONV geoEfg2Llh_heikkinen (int datum,  double efg[],
double *lat, double *lon, double *hgt)
{
    double a, flat, b, e2, ee2;
    double a2,b2,x2,y2,z2,d,d2,e4,m1e2,m1e2z2,
    W,W2,F,G,p,q,q1,s,sp,W0,wew0,U,V,Z0;


    /* Get the ellipsoid parameters */
    geoGetEllipsoid (&a, &b, &e2, &ee2, &flat, datum);
    its=0;

    // precompute variables
    x2 = sqr(efg[GEO_E]);
    y2 = sqr(efg[GEO_F]);
    z2 = sqr(efg[GEO_G]);
    a2 = sqr(a);
    b2 = sqr(b);
    e4 = sqr(e2);
    m1e2 = 1.0 - e2;
    m1e2z2 = m1e2*z2;

    // main algorithm
    W = sqrt(x2+y2);
    W2= W*W;

    F = 54.0*b2*z2;

    G = W2 + m1e2z2 - e2*(a2-b2);
    d = e4*F*W2/(G*G*G);
    d2=d*d;

    s = pow(1.0+d+sqrt(d2+2*d),1.0/3.0);
    sp=s+1.0/s+1.0;
    p = F/(3.0*sqr(sp)*G*G);
    q = sqrt(1.0+2.0*e4*p);
    q1 = q+1.0;

    W0= (-p*e2*W / q1) + sqrt(0.5*a2*(1.0+1.0/q)-(p*m1e2z2/(q*q1))-(p*W2/2.0));
    wew0 = W-e2*W0;
    U = sqrt(sqr(wew0)+z2);
    V = sqrt(sqr(wew0)+m1e2z2);
    Z0= b2*efg[GEO_G]/(a*V);
    *hgt = U*(1.0-(b2/(a*V)));
    *lat = atan2((efg[GEO_G]+ee2*Z0),W);
    *lon = atan2(efg[GEO_F],efg[GEO_E]);


}

double DLL_CALLCONV geoEfg2Lat_heikkinen (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_heikkinen (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lat);
}

double DLL_CALLCONV geoEfg2Lon_heikkinen (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_heikkinen (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lon);
}
double DLL_CALLCONV geoEfg2Hgt_heikkinen (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_heikkinen (datum,efg,&lat,&lon,&hgt);
    return(hgt);
}

//-----------------------------------------------------------------
/*! \brief toms Method - This routine will convert earth centered Cartesian
coordinates (E,F,G), into geodetic coordinates (latitude
\f$\phi\f$, longitude \f$\lambda\f$, and ellipsoid height \f$h\f$).

 \param  int datum
 \param  double efg[]   : EFG(xyz) in METERS
 \param  double *lat    : Latitude in RADIANS
 \param  double *lon    : Longitude in RADIANS
 \param  double *hgt    : Height in METERS
 \return nothing

\note This method is closed exact method from ....

*/
#define aDc 1.0026000

void    DLL_CALLCONV geoEfg2Llh_toms (int datum,  double efg[],
double *lat, double *lon, double *hgt)
{
    double a, flat, b, e2, ee2;
    double x2,y2,W,W2,t0,s0,sbo,cbo,t1,wae,s1,sin1,cos1,n;


    /* Get the ellipsoid parameters */
    geoGetEllipsoid (&a, &b, &e2, &ee2, &flat, datum);
    its=0;

    // precompute variables
    x2 = sqr(efg[GEO_E]);
    y2 = sqr(efg[GEO_F]);
    //	z2 = sqr(efg[GEO_G]);

    // main algorithm
    W = sqrt(x2+y2);
    W2= W*W;

    t0 = efg[GEO_G] * aDc;
    s0 = sqrt(sqr(t0)+W2);
    sbo= t0/s0;
    cbo= W/s0;

    t1= efg[GEO_G] + b * ee2 * cube(sbo);
    wae = W - a * e2 * cube(cbo);
    s1 = sqrt(sqr(t1) + sqr(wae));
    sin1 = t1/s1;
    cos1 = wae/s1;
    n = a/sqrt(1.0-e2*sqr(sin1));
    if(sin1 >= 0.3826834323650897717284599840304)
    {
        sin1 = sqrt(sqr(sin1));
        *hgt = efg[GEO_G]/sin1 + n*(e2-1.0);
    }
    else
    {
        *hgt = W/cos1 - n;
    }

    *lat = atan2(sin1,cos1);
    *lon = atan2(efg[GEO_F],efg[GEO_E]);


}

double DLL_CALLCONV geoEfg2Lat_toms (int datum, double e, double f, double g)
{
    double lat, lon, hgt;
    double efg[] = {e,f,g};
//    efg[GEO_E] = e;
//    efg[GEO_F] = f;
//    efg[GEO_G] = g;

    geoEfg2Llh_toms (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lat);
}

double DLL_CALLCONV geoEfg2Lon_toms (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_toms (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lon);
}
double DLL_CALLCONV geoEfg2Hgt_toms (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_toms (datum,efg,&lat,&lon,&hgt);
    return(hgt);
}



