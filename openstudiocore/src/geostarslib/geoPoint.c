/*! \file geoPoint.c
    \brief  This file contains the geo library routines for:
       \li Conversion between coordinate systems
       \li Angle conversions
       \li Tangential plane calculations

*/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "geoStars.h"


//-----------------------------------------------------------------
/*!
\brief This function returns the version of the GeoStarsLib.

\retval GEOSTARSLIB_VERSION as a double

*/

DLL_API double  DLL_CALLCONV geoVersion(void)
{
    return(GEOSTARSLIB_VERSION);
}


//-----------------------------------------------------------------
/*!
\brief This function returns the XYZ offset of the target point with
 respect to the source point, given the  Earth Fixed Geodetic
 coordinates of the points. The EFG  coordinates for the source
 must appear in the GEO_LOCATION  record, which must be built
 previous to the call to this procedure.

\param  GEO_LOCATION *src_desc
\param  GEO_LOCATION *tgt_desc
\param  double xyz_disp[]
\retval GEO_OK on success
\retval GEO_ERROR on error

\note This routine will allow you input site coordinates from two
different datums. It is up to the caller to make sure the datums are the
same (if it matters).
*/

int DLL_CALLCONV geoEfg2XyzDiff (GEO_LOCATION *src_desc,
GEO_LOCATION *tgt_desc,
double xyz_disp[])
{
    double delta_e, delta_f, delta_g, intermed_val;

    //   if(src_desc->datum != tgt_desc->datum) return(GEO_ERROR);

    delta_e = tgt_desc->e - src_desc->e;
    delta_f = tgt_desc->f - src_desc->f;
    delta_g = tgt_desc->g - src_desc->g;

    intermed_val = (-src_desc->clon * delta_e) -
    ( src_desc->slon * delta_f);

    xyz_disp[GEO_X] =
    (-src_desc->slon * delta_e) +
    ( src_desc->clon * delta_f);
    xyz_disp[GEO_Y] =
    (src_desc->slat * intermed_val) +
    (src_desc->clat * delta_g);
    xyz_disp[GEO_Z] =
    (-src_desc->clat * intermed_val) +
    ( src_desc->slat * delta_g);

    return(GEO_OK);

} /* End procedure site_xyz_diff */

/*!
\brief This function returns the XYZ offset of the target point with
 respect to the source point, given the  Earth Fixed Geodetic
 coordinates of the points. The EFG  coordinates for the source
 must appear in the GEO_LOCATION  record, which must be built
 previous to the call to this procedure.

\param  GEO_LOCATION *src_desc
\param  GEO_LOCATION *tgt_desc
\param  double xyz_disp[]
\retval GEO_OK on success
\retval GEO_ERROR on error

\note This routine will allow you input site coordinates from two
different datums. It is up to the caller to make sure the datums are the
same (if it matters).
*/
int DLL_CALLCONV geoEfg2XyzDiff_packed(GEO_LOCATION *src_desc, int count, double efg_xyz[])
{
    double delta_e, delta_f, delta_g, intermed_val;

    double* vector;

    // calculate end
    double* vend = efg_xyz + (3 * count);

    /* Convert all the sets. */
    for (vector = efg_xyz; vector < vend; vector += 3)
    {
        delta_e = vector[GEO_E] - src_desc->e;
        delta_f = vector[GEO_F] - src_desc->f;
        delta_g = vector[GEO_G] - src_desc->g;

        intermed_val = (-src_desc->clon * delta_e) -
        ( src_desc->slon * delta_f);

        vector[GEO_X] =
        (-src_desc->slon * delta_e) +
        ( src_desc->clon * delta_f);
        vector[GEO_Y] =
        (src_desc->slat * intermed_val) +
        (src_desc->clat * delta_g);
        vector[GEO_Z] =
        (-src_desc->clat * intermed_val) +
        ( src_desc->slat * delta_g);
    }

    return(GEO_OK);

} /* End procedure efg_xyz_diff */

double  DLL_CALLCONV geoLlh2DiffX (double lat1, double lon1, double hgt1,int datum1, double lat2, double lon2, double hgt2,int datum2)
{
    double xyz[3];
    GEO_LOCATION site1, site2;

    geoInitLocation(&site1, lat1, lon1, hgt1, datum1,  "site1");
    geoInitLocation(&site2, lat2, lon2, hgt2, datum2,  "site2");
    geoEfg2XyzDiff(&site1, &site2,xyz);
    return(xyz[GEO_X]);

}

double  DLL_CALLCONV geoLlh2DiffY (double lat1, double lon1, double hgt1,int datum1, double lat2, double lon2, double hgt2,int datum2)
{
    double xyz[3];
    GEO_LOCATION site1, site2;

    geoInitLocation(&site1, lat1, lon1, hgt1, datum1,  "site1");
    geoInitLocation(&site2, lat2, lon2, hgt2, datum2,  "site2");
    geoEfg2XyzDiff(&site1, &site2,xyz);
    return(xyz[GEO_Y]);

}
double  DLL_CALLCONV geoLlh2DiffZ (double lat1, double lon1, double hgt1,int datum1, double lat2, double lon2, double hgt2,int datum2)
{
    double xyz[3];
    GEO_LOCATION site1, site2;

    geoInitLocation(&site1, lat1, lon1, hgt1, datum1,  "site1");
    geoInitLocation(&site2, lat2, lon2, hgt2, datum2,  "site2");
    geoEfg2XyzDiff(&site1, &site2,xyz);
    return(xyz[GEO_Z]);

}
//-----------------------------------------------------------------
/*!
\brief This routine will convert geodetic coordinates (latitude
\f$\phi\f$, longitude \f$\lambda\f$, and ellipsoid height \f$h\f$)
into earth centered Cartesian coordinates (E,F,G).

 \param  double lat        : Latitude in RADIANS
 \param  double lon        : Longitude in RADIANS
 \param  double height     : Height in METERS
 \param  int datum1
 \param  double *e         : E(x) in METERS
 \param  double *f         : F(y) in METERS
 \param  double *g         : G(z) in METERS
 \return nothing
*/


void   DLL_CALLCONV  geoLlh2Efg (double lat, double lon, double height,int datum,
double *e,  double *f,  double *g)

{
    double N,a,e2,ee2, b, flat;
    double slat, clat;

    clat = cos(lat);
    slat = sin(lat);

    /* Get the ellipsoid parameters */
    geoGetEllipsoid(&a,&b,&e2,&ee2,&flat,datum);

    /* Compute the radius of curvature */
    N = a / (sqrt(1.0 - e2 * pow(slat,2.0)));

    /* Compute the EFG(XYZ) coordinates (earth centered) */
    *e = (N + height) * clat * cos(lon);
    *f = (N + height) * clat * sin(lon);
    *g = (N * (1.0 - e2) + height) * sin(lat);
}

/*!
\brief This routine will convert geodetic coordinates (latitude
\f$\phi\f$, longitude \f$\lambda\f$, and ellipsoid height \f$h\f$)
into earth centered Cartesian coordinates (E,F,G).

 \param  GEO_DATUM* datum Datum of the lat/lon/height
 \param int count Number of vertices
 \param double llh_efg[] Pointer to the first coordinate of the first vertex of
 the array.
 \return nothing
*/
void   DLL_CALLCONV  geoLlh2Efg_packed (GEO_DATUM* datum, int count, double llh_efg[])
{
    double N, slat, clat, slon, clon;

    double* vector;

    // calculate end
    double* vend = llh_efg + (3 * count);

    /* Convert all the sets. */
    for (vector = llh_efg; vector < vend; vector += 3)
    {
#ifdef HAVE_SINCOS
        sincos(vector[GEO_LAT], &slat, &clat);
        sincos(vector[GEO_LON], &slon, &clon);

#else
        slat = sin(vector[GEO_LAT]);
        clat = cos(vector[GEO_LAT]);
        slon = sin(vector[GEO_LON]);
        clon = cos(vector[GEO_LON]);
#endif

        /* Compute the radius of curvature */
        N = datum->a / (sqrt(1.0 - datum->e2 * slat * slat));

        /* Compute the EFG(XYZ) coordinates (earth centered) */
        vector[GEO_E] = (N + vector[GEO_HGT]) * clat * clon;
        vector[GEO_F] = (N + vector[GEO_HGT]) * clat * slon;
        vector[GEO_G] = (N * (1.0 - datum->e2) + vector[GEO_HGT]) * slat;
    }
}

//-----------------------------------------------------------------
/*!
\brief This routine will convert geodetic coordinates (latitude
\f$\phi\f$, longitude \f$\lambda\f$, and ellipsoid height \f$h\f$)
into earth centered Cartesian coordinates (E,F,G). This returns the
E component.

 \param  double lat        : Latitude in RADIANS
 \param  double lon        : Longitude in RADIANS
 \param  double height     : Height in METERS
 \param  int datum1
 \return  double *e         : E(x) in METERS
*/


double   DLL_CALLCONV  geoLlh2E (double lat, double lon, double hgt,int datum)

{
    GEO_LOCATION site;

    geoInitLocation(&site, lat, lon, hgt, datum,  "site");
    return(site.e);
}

//-----------------------------------------------------------------
/*!
\brief This routine will convert geodetic coordinates (latitude
\f$\phi\f$, longitude \f$\lambda\f$, and ellipsoid height \f$h\f$)
into earth centered Cartesian coordinates (E,F,G). This returns the
F component.

 \param  double lat        : Latitude in RADIANS
 \param  double lon        : Longitude in RADIANS
 \param  double height     : Height in METERS
 \param  int datum1
 \return  double *f        : F(x) in METERS
*/


double   DLL_CALLCONV  geoLlh2F (double lat, double lon, double hgt,int datum)

{
    GEO_LOCATION site;

    geoInitLocation(&site, lat, lon, hgt, datum,  "site");
    return(site.f);
}

//-----------------------------------------------------------------
/*!
\brief This routine will convert geodetic coordinates (latitude
\f$\phi\f$, longitude \f$\lambda\f$, and ellipsoid height \f$h\f$)
into earth centered Cartesian coordinates (E,F,G). This returns the
G component.

 \param  double lat        : Latitude in RADIANS
 \param  double lon        : Longitude in RADIANS
 \param  double height     : Height in METERS
 \param  int datum1
 \return  double *g         : G(x) in METERS
*/


double   DLL_CALLCONV  geoLlh2G (double lat, double lon, double hgt,int datum)

{
    GEO_LOCATION site;

    geoInitLocation(&site, lat, lon, hgt, datum,  "site");
    return(site.g);
}


//-----------------------------------------------------------------
/*!
 \brief Given the X, Y, and Z coordinates (in meters) of a point
 in space, the procedure geoXyz2Rae calculates the range,
 azimuth, and elevation to that point.

 \b Notes:
 \li X is understood to be the east-west displacement of the
 point, with east being the positive direction.
 \li Y is the north-south displacement, with north being positive.
 \li Z is the vertical displacement of the point.
 \li Range is in meters. Azimuth and elevation are in radians

\param  double xyz_in[]
\param  double rae_out[]
\return nothing

*/

void DLL_CALLCONV geoXyz2Rae (double xyz_in[],
double rae_out[])
{
    double horz_dist;

    /* Determine the range: */
    rae_out[GEO_RNG] =
    sqrt(pow(xyz_in[GEO_X],2.0) + pow(xyz_in[GEO_Y],2.0) + pow(xyz_in[GEO_Z],2.0));

    /* Determine the azimuth: */
    rae_out[GEO_AZ] = atan2(xyz_in[GEO_X], xyz_in[GEO_Y]);
    //if((xyz_in[GEO_X] >= 0.0) && (xyz_in[GEO_Y] < 0.0)) rae_out[GEO_AZ] += M_PI;
    if((xyz_in[GEO_X] <  0.0) && (xyz_in[GEO_Y] < 0.0)) rae_out[GEO_AZ] += (2.0 * M_PI);
    if((xyz_in[GEO_X] <  0.0) && (xyz_in[GEO_Y] > 0.0)) rae_out[GEO_AZ] += (2.0 * M_PI);

    /* Determine the elevation: */
    horz_dist = sqrt(pow(xyz_in[GEO_X],2.0) + pow(xyz_in[GEO_Y],2.0));
    rae_out[GEO_EL] = atan2(xyz_in[GEO_Z], horz_dist);
    if(rae_out[GEO_EL] < 0.0) rae_out[GEO_EL] += (2.0 * M_PI);

} /* End procedure xyz_to_rae */
//-----------------------------------------------------------------
/*!
 \brief Given the X, Y, and Z coordinates (in meters) of a point
 in space, the procedure geoXyz2Rae calculates the range,
 azimuth, and elevation to that point.

 \b Notes:
 \li X is understood to be the east-west displacement of the
 point, with east being the positive direction.
 \li Y is the north-south displacement, with north being positive.
 \li Z is the vertical displacement of the point.
 \li Range is in meters. Azimuth and elevation are in radians
 \param count Number of vertices
 \param xyz_rae[] Pointer to the first coordinate of the first vertex of
 the array.
\return nothing

*/
void DLL_CALLCONV geoXyz2Rae_packed(int count,
double xyz_rae[])
{
    double horz_dist;
    double rng, az;
    double* vector;

    // calculate end
    double* vend = xyz_rae + (3 * count);

    /* Convert all the sets. */
    for (vector = xyz_rae; vector < vend; vector += 3)
    {
        /* Determine the range: */
        rng =
        sqrt(vector[GEO_X]*vector[GEO_X] + vector[GEO_Y]*vector[GEO_Y] + vector[GEO_Z]*vector[GEO_Z]);

        /* Determine the azimuth: */
        az = atan2(vector[GEO_X], vector[GEO_Y]);

        //if((vector[GEO_X] <  0.0) && (vector[GEO_Y] < 0.0)) az += (2.0 * M_PI);
        //if((vector[GEO_X] <  0.0) && (vector[GEO_Y] > 0.0)) az += (2.0 * M_PI);
        if (az < -0.0f) az += (2.0 * M_PI);

        /* Determine the elevation: */
        horz_dist = sqrt(vector[GEO_X]*vector[GEO_X] + vector[GEO_Y]*vector[GEO_Y]);
        vector[GEO_RNG] = rng;
        vector[GEO_AZ] = az;
        vector[GEO_EL] = atan2(vector[GEO_Z], horz_dist);
        if(vector[GEO_EL] < -0.0f) vector[GEO_EL] += (2.0 * M_PI);
    }
} /* End procedure xyz_to_rae */


//-----------------------------------------------------------------
/*!
 \brief Given the X, Y, and Z coordinates (in meters) of a point
 in space, the procedure geoXyz2R calculates the range to that point.

 \b Notes:
 \li X is understood to be the east-west displacement of the
 point, with east being the positive direction.
 \li Y is the north-south displacement, with north being positive.
 \li Z is the vertical displacement of the point.
 \li Range is in meters.

\param  double xyz_in[]
\return double range

*/
double  DLL_CALLCONV geoXyz2R (double x, double y, double z)
{
    double rae[3],xyz[3];
    xyz[0]=x;
    xyz[1]=y;
    xyz[2]=z;
    geoXyz2Rae(xyz,rae);
    return(rae[GEO_RNG]);
}


//-----------------------------------------------------------------
/*!
 \brief Given the X, Y, and Z coordinates (in meters) of a point
 in space, the procedure geoXyz2R calculates the azimuth to that point.

 \b Notes:
 \li X is understood to be the east-west displacement of the
 point, with east being the positive direction.
 \li Y is the north-south displacement, with north being positive.
 \li Z is the vertical displacement of the point.
 \li Azimuth is in decimal degrees

\param  double xyz_in[]
\return double azimuth

*/
double  DLL_CALLCONV geoXyz2A (double x, double y, double z)
{
    double rae[3],xyz[3];
    xyz[0]=x;
    xyz[1]=y;
    xyz[2]=z;
    geoXyz2Rae(xyz,rae);
    return(rae[GEO_AZ]*RAD_TO_DEG);
}

//-----------------------------------------------------------------
/*!
 \brief Given the X, Y, and Z coordinates (in meters) of a point
 in space, the procedure geoXyz2R calculates the elevation angle to that point.

 \b Notes:
 \li X is understood to be the east-west displacement of the
 point, with east being the positive direction.
 \li Y is the north-south displacement, with north being positive.
 \li Z is the vertical displacement of the point.
 \li Elevation is in decimal degrees

\param  double xyz_in[]
\return double elevation

*/

double  DLL_CALLCONV geoXyz2E (double x, double y, double z)
{
    double rae[3],xyz[3];
    xyz[0]=x;
    xyz[1]=y;
    xyz[2]=z;
    geoXyz2Rae(xyz,rae);
    return(rae[GEO_EL]*RAD_TO_DEG);
}

//-----------------------------------------------------------------
/*!
\brief This routine converts from Range, Azimuth, and Elevation
 into Cartesian coordinates X,Y,Z.

 \param double rae_in[]
 \param double xyz_out[]
 \return nothing
*/
void DLL_CALLCONV geoRae2Xyz (double rae_in[],
double xyz_out[])
{
    double r_cos_e;

    r_cos_e = rae_in[GEO_RNG] * cos(rae_in[GEO_EL]);

    xyz_out[GEO_X] = sin(rae_in[GEO_AZ]) * r_cos_e;
    xyz_out[GEO_Y] = cos(rae_in[GEO_AZ]) * r_cos_e;
    xyz_out[GEO_Z] = rae_in[GEO_RNG] * sin(rae_in[GEO_EL]);
}

//-----------------------------------------------------------------
/*!
\brief This routine iterates over \a count vertices in the array \a rae_xyz converting it from Range, Azimuth, and Elevation into Cartesian coordinates X,Y,Z.

 \param count Number of vertices
 \param rae_xyz[] Pointer to the first coordinate of the first vertex of
 the array.
 \return nothing
*/
void DLL_CALLCONV geoRae2Xyz_packed(int count,
double rae_xyz[])
{
    double r_cos_e;
    double r_sin_e;
#ifdef HAVE_SINCOS
    double sin_a;
#endif
    double cos_a;
    double* vector;

    // calculate end
    double* vend = rae_xyz + (3 * count);

    /* Convert all the sets. */
    for (vector = rae_xyz; vector < vend; vector += 3)
    {
#ifdef HAVE_SINCOS
        sincos(vector[GEO_EL], &r_sin_e, &r_cos_e);
        sincos(vector[GEO_AZ], &sin_a, &cos_a);
        r_cos_e *= vector[GEO_RNG];
        r_sin_e *= vector[GEO_RNG];

        vector[GEO_X] = sin_a * r_cos_e;
        vector[GEO_Y] = cos_a * r_cos_e;
        vector[GEO_Z] = r_sin_e;
#else
        r_cos_e = vector[GEO_RNG] * cos(vector[GEO_EL]);
        r_sin_e *= vector[GEO_RNG] * sin(vector[GEO_EL]);
        cos_a = cos(vector[GEO_AZ]);

        vector[GEO_X] = sin(vector[GEO_AZ]) * r_cos_e;
        vector[GEO_Y] = cos_a * r_cos_e;
        vector[GEO_Z] = vector[GEO_RNG] ;
#endif
    }
}

//-----------------------------------------------------------------
/*!
 \brief Ingests X, Y, Z, and site info and returns the EFG coordinates.

 \param GEO_LOCATION *loc
 \param double xyz_in[]
 \param double efg_out[]
 \return nothing
*/

void DLL_CALLCONV geoXyz2Efg (GEO_LOCATION *loc,
double xyz_in[],
double efg_out[])
{
    double c1, c2, c3;

    /* Do the matrix multiplication: */

    c1 = -loc->slon * xyz_in[GEO_X] +
    -loc->clon * loc->slat * xyz_in[GEO_Y] +
    loc->clon * loc->clat * xyz_in[GEO_Z];

    c2 =  loc->clon * xyz_in[GEO_X] +
    -loc->slon * loc->slat * xyz_in[GEO_Y] +
    loc->slon * loc->clat * xyz_in[GEO_Z];

    c3 = loc->clat * xyz_in[GEO_Y] +
    loc->slat * xyz_in[GEO_Z];

    /* Add resultant matrix to local EFG to get remote EFG: */
    efg_out[GEO_E] = c1 + loc->e;
    efg_out[GEO_F] = c2 + loc->f;
    efg_out[GEO_G] = c3 + loc->g;

} /* End procedure aer_to_efg */

//-----------------------------------------------------------------
/*!
 \brief This routine takes site info and iterates over \a count vertices in the tightly packed array \a xyz_efg converting it from cartesian XYZ into geocentric EFG coordinates.

 \param loc The location that the XYZ grid is based around
 \param count The number of vertices in the array
 \param xyz_efg[] Pointer to the first vertex of the array
 \return nothing
*/

void DLL_CALLCONV geoXyz2Efg_packed(GEO_LOCATION *loc,
int count,
double xyz_efg[])
{
    double c1, c2, c3;
    double nclonslat = -loc->clon * loc->slat;
    double clonclat  =  loc->clon * loc->clat;
    double nslonslat = -loc->slon * loc->slat;
    double slonclat  =  loc->slon * loc->clat;
    double* vector;

    // calculate end
    double* vend = xyz_efg + (3 * count);

    /* Convert all the sets. */
    for (vector = xyz_efg; vector < vend; vector += 3)
    {
        /* Do the matrix multiplication: */
        c1 = -loc->slon * vector[GEO_X] +
        nclonslat * vector[GEO_Y] +
        clonclat * vector[GEO_Z];

        c2 =  loc->clon * vector[GEO_X] +
        nslonslat * vector[GEO_Y] +
        slonclat * vector[GEO_Z];

        c3 = loc->clat * vector[GEO_Y] +
        loc->slat * vector[GEO_Z];

        /* Add resultant matrix to local EFG to get remote EFG: */
        vector[GEO_E] = c1 + loc->e;
        vector[GEO_F] = c2 + loc->f;
        vector[GEO_G] = c3 + loc->g;
    }

} /* End procedure xyz_to_efg */

//-----------------------------------------------------------------
/*!
 \brief Ingests Range, Azimuth, Elevation and site info and returns the EFG coordinates
 that the RAE points to.

 \param GEO_LOCATION *loc
 \param double aer_in[]
 \param double efg_out[]
 \return nothing
*/

void DLL_CALLCONV geoRae2Efg (GEO_LOCATION *loc,
double aer_in[],
double efg_out[])
{
    double c1, c2, c3;
    double xyz_val[3];

    /* Convert the RAE value to XYZ: */
    geoRae2Xyz(aer_in, xyz_val);

    /* Do the matrix multiplication: */

    c1 = -loc->slon * xyz_val[GEO_X] +
    -loc->clon * loc->slat * xyz_val[GEO_Y] +
    loc->clon * loc->clat * xyz_val[GEO_Z];

    c2 =  loc->clon * xyz_val[GEO_X] +
    -loc->slon * loc->slat * xyz_val[GEO_Y] +
    loc->slon * loc->clat * xyz_val[GEO_Z];

    c3 = loc->clat * xyz_val[GEO_Y] +
    loc->slat * xyz_val[GEO_Z];

    /* Add resultant matrix to local EFG to get remote EFG: */
    efg_out[GEO_E] = c1 + loc->e;
    efg_out[GEO_F] = c2 + loc->f;
    efg_out[GEO_G] = c3 + loc->g;

} /* End procedure aer_to_efg */


//-----------------------------------------------------------------
/*!
\brief Converts degrees minutes seconds to radians.
\param double  deg, min, sec
\param char    sign            : N,E,S,W,-,+
\return radians
*/
double  DLL_CALLCONV geoDms2Rads(double deg, double min, double sec, char *sign)
{
    double direction;

    switch (toupper(*sign))
    {
        case   '-':
        case   'W':                    /* If coordinate is West or South, returned */
        case   'S':
            direction = -1.0;       /* radian will have negative value.         */
            break;
        case   '+':
        case   'E':                    /* If coordinate is East or North, returned */
        case   'N':
            direction = 1.0;        /* radian will have positive value.         */
            break;
        default:
            direction = 1.0;        /* If no compass direction entered, returned*/
    }                                /* radian will be assumed positive.         */

    /* Return radians to calling function. */
    return( direction * DEG_TO_RAD *
    ( fabs(deg) + (min * MIN_TO_DEG) + (sec * SEC_TO_DEG)));
}
//-----------------------------------------------------------------
/*!
\brief Converts degrees minutes seconds to Decimal Degrees
\param double  deg, min, sec
\param char    sign            : N,E,S,W,-,+
\return decimal degrees
*/
double  DLL_CALLCONV geoDms2DD(double deg, double min, double sec, char *sign)
{
    double direction;

    switch (toupper(*sign))
    {
        case   '-':
        case   'W':                    /* If coordinate is West or South, returned */
        case   'S':
            direction = -1.0;       /* radian will have negative value.         */
            break;
        case   '+':
        case   'E':                    /* If coordinate is East or North, returned */
        case   'N':
            direction = 1.0;        /* radian will have positive value.         */
            break;
        default:
            direction = 1.0;        /* If no compass direction entered, returned*/
    }                                /* radian will be assumed positive.         */

    /* Return radians to calling function. */
    return( direction *
    ( fabs(deg) + (min * MIN_TO_DEG) + (sec * SEC_TO_DEG)));
}



//-----------------------------------------------------------------
/*!
\brief  Convert decimal degrees, minutes, and seconds ("dmmss.s") to radians.
\param double in; // decimal deg,min,sec
\return radians
*/
double DLL_CALLCONV geoDecdms2Rads(double in)       /* minutes and seconds */
{
    double t1,m,d,s,sign;

    if (in < 0.0)
        sign = -1.0;
    else
        sign =  1.0;

    s = modf(fabs(in)/100.0, &t1) * 100.0;
    m = modf(t1/100.0,        &d) * 100.0;

    /* Return radians to calling function. */
    return( (d + (m/60.0) + (s/3600.0)) * sign * DEG_TO_RAD);
}

//-----------------------------------------------------------------
/*!
\brief Converts radians to degrees minutes seconds.
\param double rads
\param double  deg, min, sec
\param char    dir            : -1.0 or 1.0
\return nothing
*/


void  DLL_CALLCONV geoRads2Dms(double rads,
double *deg, double *min, double *sec, double *dir)
{
    double temp;
    double fraction;

    if (rads < 0.0)
        *dir = -1.0;
    else
        *dir =  1.0;

    rads = fabs (rads);

    temp = RAD_TO_DEG * rads;
    fraction = modf(temp,deg);

    temp = fraction * 60.0;
    fraction = modf(temp,min);

    *sec = fraction * 60.0;
}

//-----------------------------------------------------------------
/*!
\brief Convert radians to decimal degrees, minutes, and seconds ("dddmmss.s").
\param double rads
\return double Decimal Deg/min/sec
 */
double DLL_CALLCONV geoRads2Decdms(double rads)
{
    double d,m,s,sign;
    double frac;

    if (rads < 0.0)
        sign = -1.0;
    else
        sign =  1.0;

    frac = modf(fabs(rads * RAD_TO_DEG),&d);
    frac = modf(frac * 60.0,&m);
    s = frac * 60.0;

    /* Return dddmmss.s to calling function. */
    return(((d*10000.0)+(m*100)+s)*sign);
}

//-----------------------------------------------------------------
/*!
\brief Converts radians to Decimal Degrees
\param double radians
\return double decimal degrees
*/


double  DLL_CALLCONV geoRads2DD(double rads)
{
    return(rads * RAD_TO_DEG);
}

//-----------------------------------------------------------------
/*!
\brief Converts Decimal Degrees to radians
\param double decimal degrees
\return radians
*/


double  DLL_CALLCONV geoDD2Rads(double dd)
{
    return(dd * DEG_TO_RAD);
}


//-----------------------------------------------------------------
/*!
\brief Converts Decimal Degrees to degrees minutes seconds.
\param double decimal degrees
\param double  deg, min, sec
\param char    dir            : -1.0 or 1.0
\return nothing
*/


void  DLL_CALLCONV geoDD2Dms(double dd,
double *deg, double *min, double *sec, double *dir)
{
    double temp;
    double fraction;

    if (dd < 0.0)
        *dir = -1.0;
    else
        *dir =  1.0;

    dd = fabs (dd);

    //temp = RAD_TO_DEG * rads;
    fraction = modf(dd,deg);

    temp = fraction * 60.0;
    fraction = modf(temp,min);

    *sec = fraction * 60.0;
}


double DLL_CALLCONV geoDD2Deg(double dd)
{
    double deg, min, sec,dir;
    geoDD2Dms(dd,&deg, &min, &sec,&dir);
    return(deg*dir);
}
double DLL_CALLCONV geoDD2Min(double dd)
{
    double deg, min, sec,dir;
    geoDD2Dms(dd,&deg, &min, &sec,&dir);
    return(min);
}
double DLL_CALLCONV geoDD2Sec(double dd)
{
    double deg, min, sec,dir;
    geoDD2Dms(dd,&deg, &min, &sec,&dir);
    return(sec);
}

