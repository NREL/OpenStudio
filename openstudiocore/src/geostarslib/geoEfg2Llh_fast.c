/*! \file geoEfg2Llh_fast.c
    \brief  This file contains the geo library routines for
        conversion between Geocentric and Geodetic coordinates. These routines are
  optimised for quick execution at the expense of precision.

  Methods avalailable:
       \li geoStarLib method
       \li Hirvonen & Moritz method
       \li Torge method
    \li Astronomical Almanac 2002 method
    \li Borkowski method
    \li Bowring method
    \li Vermeille method

*/
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "geoStars.h"

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

void    DLL_CALLCONV geoEfg2Llh_fast (int datum,  double efg[],
double *lat, double *lon, double *hgt)
{
    double p, u, su,cu,bee2,ae2,aob,slat;

    bee2 = GEO_WGS84_ee2 * GEO_WGS84_b;
    ae2 = GEO_WGS84_e2 * GEO_WGS84_a;
    aob = GEO_WGS84_a / GEO_WGS84_b;

    // Computes EFG to lat, lon & height
    p = sqrt(sqr(efg[GEO_E]) + sqr(efg[GEO_F]));
    u = atan((efg[GEO_G]/p)  * aob);

    su = sin(u);
    cu = cos(u);

    *lat = atan((efg[GEO_G] + bee2 * cube(su) ) /
    ( p - ae2 * cube(cu) ) );
    slat = sin(*lat);

    *hgt = p / cos(*lat) - ( GEO_WGS84_a / (sqrt(1.0 - GEO_WGS84_e2 * sqr(slat))));   //same results
    *lon = atan2(efg[GEO_F],efg[GEO_E]);  // atan(f/e)

}

double DLL_CALLCONV geoEfg2Lat_fast (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_fast (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lat);
}

double DLL_CALLCONV geoEfg2Lon_fast (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_fast (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lon);
}
double DLL_CALLCONV geoEfg2Hgt_fast (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_fast (datum,efg,&lat,&lon,&hgt);
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

void    DLL_CALLCONV geoEfg2Llh_hm_fast (int datum,  double efg[],
double *lat, double *lon, double *hgt)
{
    //    double  a, flat, b, e2, ee2;
    double tmp,n,p,slat;

    p = sqrt(sqr(efg[GEO_E])+ sqr(efg[GEO_F]));
    *lon = atan2(efg[GEO_F],efg[GEO_E]);  // atan(f/e)

    // Prepare the initial value of lat
    tmp = efg[GEO_G] /  p;
    *lat = atan((1.0 / (1.0 - GEO_WGS84_e2))*tmp);

    slat = sin(*lat);
    n= GEO_WGS84_a/sqrt(1.0 - GEO_WGS84_e2 * sqr(slat));
    //        *lat=atan(tmp*(1.0 + (e2*n*slat)/efg[GEO_G]));
    //        lat1=*lat;
    *hgt = (p / cos(*lat)) - n;

}

double DLL_CALLCONV geoEfg2Lat_hm_fast (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_hm_fast (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lat);
}

double DLL_CALLCONV geoEfg2Lon_hm_fast (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_hm_fast (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lon);
}
double DLL_CALLCONV geoEfg2Hgt_hm_fast (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_hm_fast (datum,efg,&lat,&lon,&hgt);
    return(hgt);
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

void    DLL_CALLCONV geoEfg2Llh_torge_fast (int datum,  double efg[],
double *lat, double *lon, double *hgt)
{
    double tmp,n,p,slat;

    p = sqrt(sqr(efg[GEO_E])+ sqr(efg[GEO_F]));
    *lon = atan2(efg[GEO_F],efg[GEO_E]);  // atan(f/e)

    // Prepare the initial value of lat
    tmp = efg[GEO_G] /  p;
    *lat = atan(tmp/(1.0 - GEO_WGS84_e2));  //assume hgt = 0 to start


    slat = sin(*lat);
    //clat = cos(*lat);

    // radius of curvature estimate
    n= GEO_WGS84_a/sqrt(1.0 - GEO_WGS84_e2 * sqr(slat));

    // Next estimates of lat/hgt
    *hgt = (p / cos(*lat)) - n;
    *lat=atan(tmp / (1.0 - (GEO_WGS84_e2 * ( n / (n + *hgt)) ) ) );

}

double DLL_CALLCONV geoEfg2Lat_torge_fast (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_torge_fast (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lat);
}

double DLL_CALLCONV geoEfg2Lon_torge_fast (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_torge_fast (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lon);
}
double DLL_CALLCONV geoEfg2Hgt_torge_fast (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_torge_fast (datum,efg,&lat,&lon,&hgt);
    return(hgt);
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
void    DLL_CALLCONV geoEfg2Llh_bowring_fast (int datum,  double efg[],
double *lat, double *lon, double *hgt)
{
    double tmp,n,b0,p,slat,clat,ee2b,ae2;
    //double boa;

    *lon = atan2(efg[GEO_F],efg[GEO_E]);  // atan(f/e)
    p = sqrt(sqr(efg[GEO_E])+ sqr(efg[GEO_F]));
    tmp = efg[GEO_G] /  p;

    // temps pulled out of the loop
    ae2  = GEO_WGS84_a * GEO_WGS84_e2;
    ee2b = GEO_WGS84_ee2 * GEO_WGS84_b;
    //boa = GEO_WGS84_b/GEO_WGS84_a;

    // Prepare the initial value of lat
    b0 = atan((GEO_WGS84_a/GEO_WGS84_b)*tmp);

    // iterate until delta lat is tiny
    slat = sin(b0);
    clat = cos(b0);
    *lat = atan((efg[GEO_G]+ ee2b * cube(slat) ) /
    (p - ae2 * cube(clat) ) );
    //        b0 = atan((boa) * tan(*lat));
    slat = sin(*lat);
    n= GEO_WGS84_a / sqrt(1.0-GEO_WGS84_e2*sqr(slat));

    *hgt = (p / cos(*lat)) - n;

}

double DLL_CALLCONV geoEfg2Lat_bowring_fast (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_bowring_fast (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lat);
}

double DLL_CALLCONV geoEfg2Lon_bowring_fast (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_bowring_fast (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lon);
}
double DLL_CALLCONV geoEfg2Hgt_bowring_fast (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_bowring_fast (datum,efg,&lat,&lon,&hgt);
    return(hgt);
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
void    DLL_CALLCONV geoEfg2Llh_aa_fast (int datum,  double efg[],
double *lat, double *lon, double *hgt)
{
    double phi,p,n;
    double slat;

    // Computes EFG to lat, lon & height
    // from the Astronomical Almanac 2002 - K11-12
    *lon = atan2(efg[GEO_F],efg[GEO_E]);  // atan(f/e)
    p = sqrt(sqr(efg[GEO_E]) + sqr(efg[GEO_F]));

    //     phi = atan(efg[GEO_G]/p); // first approximation
    phi = atan(efg[GEO_G]/((1.0-GEO_WGS84_e2)*p)); // faster

    slat = sin(phi);
    n = GEO_WGS84_a/sqrt(1.0 - GEO_WGS84_e2 * sqr(slat));
    *lat = atan((efg[GEO_G] + n * GEO_WGS84_e2 * slat)/p);

    *hgt = p / cos(*lat) - n;

}

double DLL_CALLCONV geoEfg2Lat_aa_fast (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_aa_fast (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lat);
}

double DLL_CALLCONV geoEfg2Lon_aa_fast (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_aa_fast (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lon);
}
double DLL_CALLCONV geoEfg2Hgt_aa_fast (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_aa_fast (datum,efg,&lat,&lon,&hgt);
    return(hgt);
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
void    DLL_CALLCONV geoEfg2Llh_borkowski_fast (int datum,  double efg[],
double *lat, double *lon, double *hgt)
{
    double a,b,r,e,f,p,q,d,v,g,t,sign,bg,samb,sqrtd,sqre,ar;
    //double e2;

    /* Get the ellipsoid parameters */
    //    geoGetEllipsoid (&a, &b, &e2, &ee2, &flat, datum);
    a=GEO_WGS84_a;
    //	 b=GEO_WGS84_b;
    // e2=GEO_WGS84_e2;

    // Computes EFG to lat, lon & height
    // from Bull. Geod., 63 (1989), pp. 50-56
    // Accurate Algorithms to Transform Geocentric to Geodetic Coordinates
    // By K.M. Borkowski

    *lon = atan2(efg[GEO_F],efg[GEO_E]);  // atan(f/e)

    //use the sign for Z
    if(efg[GEO_G] < 0.0) sign = -1.0;
    else sign = 1.0;
    b =  GEO_WGS84_b * sign;
    bg = b * efg[GEO_G];  //temp
    samb = sqr(a)-sqr(b) ;//temp
    r = efg[GEO_E] / cos(*lon);
    ar = a*r;
    e = (bg - samb) / ar;
    sqre = sqr(e);
    f = (bg + samb) / ar;
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

double DLL_CALLCONV geoEfg2Lat_borkowski_fast (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_borkowski_fast (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lat);
}

double DLL_CALLCONV geoEfg2Lon_borkowski_fast (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_borkowski_fast (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lon);
}
double DLL_CALLCONV geoEfg2Hgt_borkowski_fast (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_borkowski_fast (datum,efg,&lat,&lon,&hgt);
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
void    DLL_CALLCONV geoEfg2Llh_vermeille_fast (int datum,  double efg[],
double *lat, double *lon, double *hgt)
{
    double fr,a2,x2,y2,z2,d2,e4,p,q,r,s,t,u,v,w,k,d,sqrtx2py2,sqrtd2pz2;



    // precompute variables
    x2 = sqr(efg[GEO_E]);
    y2 = sqr(efg[GEO_F]);
    z2 = sqr(efg[GEO_G]);
    a2 = sqr(GEO_WGS84_a);
    e4 = sqr(GEO_WGS84_e2);
    sqrtx2py2 = sqrt(x2+y2);
    fr = (1.0-GEO_WGS84_e2)/(sqr(GEO_WGS84_a));

    // main algorithm
    p = (x2 + y2)/ a2;
    q = fr*z2;
    r = (p+q-e4)/6.0;
    s = e4 * (p*q)/(4.0*cube(r));
    t = pow(1.0+s+sqrt(s*(2.0+s)),1.0/3.0);
    u = r*(1.0+t+(1.0/t));
    v = sqrt(sqr(u)+e4*q);
    w = GEO_WGS84_e2*(u+v-q)/(2.0*v);
    k = sqrt(u+v+sqr(w)) - w;
    d = k*sqrtx2py2/(k+GEO_WGS84_e2);
    d2 = sqr(d);
    sqrtd2pz2 = sqrt(d2+z2);

    *lon = 2.0 * atan(efg[GEO_F]/(efg[GEO_E] + sqrtx2py2));
    *lat = 2.0 * atan(efg[GEO_G]/(d + sqrtd2pz2));
    *hgt = ((k + GEO_WGS84_e2 - 1.0) / k) * sqrtd2pz2;

}

double DLL_CALLCONV geoEfg2Lat_vermeille_fast (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_vermeille_fast (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lat);
}

double DLL_CALLCONV geoEfg2Lon_vermeille_fast (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_vermeille_fast (datum,efg,&lat,&lon,&hgt);
    return(RAD_TO_DEG * lon);
}
double DLL_CALLCONV geoEfg2Hgt_vermeille_fast (int datum, double e, double f, double g)
{
    double efg[3];
    double lat, lon, hgt;
    efg[GEO_E] = e;
    efg[GEO_F] = f;
    efg[GEO_G] = g;

    geoEfg2Llh_vermeille_fast (datum,efg,&lat,&lon,&hgt);
    return(hgt);
}
