/*!
\file geoAstro.c
\brief This file contains the wrapper routines to interface to the
       Novas (Naval Observatory Vector Astrometry Subroutine) Library.

       http://aa.usno.navy.mil/software/novas/novas_info.html

*/

#include <stdio.h>
#include <time.h>
#include <math.h>
#include "geoStars.h"


// return the julian day of the year
int jday(int month, int day)
{

    /* Days in each month 1-12 */
    int jday[] = {0,0,31,59,90,120,151,180,211,242,272,303,333 };

    /* Determin decimal years from date */
    return(jday[month]+day);
}

// integer part
double ip(double x)
{
  double dummy, tmp;

  dummy = modf(x, &tmp);
  return tmp;
}


double fractionalYearRad(int month, int day, int hour)
{
    double y;
    y =  (TWO_PI/365.0) * (jday(month,day)-1.0 +((hour-12.0)/24));
    return(y);
}

// Number of days from J2000
double n(int month, int day, int hour, int min, int sec)
{
    return(364.5 +jday(month,day) + (hour + min/60.0 + sec/3600.0)/24.0);
}

double JD(int year, int month, int day)
{
int A, B, m, y;

  if (month > 2)
  {
    y = year;
    m = month;
  }
  else
  {
    y = year - 1;
    m = month + 12;
  }
  A = y / 100;
  if ((year > 1582)  || ((year == 1582) && ((month > 10)
       || ((month == 10) && (day > 4)))))
    B = 2 - A + A / 4;
  else
    B = 0;
  return ip (365.25 * (y + 4716)) + ip (30.6001 * (m + 1)) + day + B - 1524.5 ;
}
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

int DLL_CALLCONV geoSunPosition(GEO_LOCATION *loc, double *az, double *el)
{
    struct tm *newtime;
    long ltime=0L;


    /* Obtain  time: */
    time( &ltime );
    newtime = gmtime( &ltime );

    geoSun(loc,newtime,az,el);
    return (GEO_OK );
}

int DLL_CALLCONV geoSun(GEO_LOCATION *loc, struct tm *newtime,double *az, double *el)
{
    double y,eqtime,decl,time_offset, tst,ha,theta,phi,sdecl;


    // This Sun position algorithm  is from :
    // http://www.esrl.noaa.gov/gmd/grad/solcalc/solareqns.PDF


    // First, the fractional year y is calculated, in radians.
    y = fractionalYearRad(newtime->tm_mon, newtime->tm_mday, newtime->tm_hour);

    // From y, we can estimate the equation of time (in minutes) and the
    // solar declination angle (in radians).
    eqtime = 229.18 * (0.000075 +
                      (0.001868 *  cos(y))-
                      (0.032077 *  sin(y))-
                      (0.014615 *  cos(2.0 * y)) -
                      (0.040849 *  sin(2.0 * y))  );


    decl = 0.006918 -
            (0.399912* cos(y)) +
            (0.070257* sin(y)) -
            (0.006758* cos(2.0 * y)) +
            (0.000907* sin(2.0 * y)) -
            (0.002697* cos(3.0 * y)) +
            (0.00148 * sin(3.0 * y));
    sdecl =  sin(decl);

    // Next, the true solar time is calculated in the following two equations.
    // First the time offset is found, in minutes, and then the true solar time, in minutes.
    time_offset = eqtime - (4.0 * loc->lon) + (60.0 * loc->timezone);

    // where eqtime is in minutes, longitude is in degrees, timezone is in hours
    // from UTC (Mountain Standard Time = +7 hours).
    tst = (newtime->tm_hour * 60.0) + newtime->tm_min + (newtime->tm_sec / 60.0) + time_offset;
    // where hr is the hour (0 - 23), mn is the minute (0 - 60), sc is the second (0 - 60).

    // The solar hour angle, in degrees, is:
    ha = (tst / 4.0) - 180.0;

    //The solar zenith angle (phi) can then be found from the following equation:
    phi = acos( (loc->slat * sdecl) + (loc->clat *  cos(decl) *  cos(ha*DEG_TO_RAD)));

    // And the solar azimuth (theta), clockwise from north) is:
   //  cos(180-theta) = (loc->slat *  cos(phi) -  sin(decl)) / (loc->clat *  sin(phi))
    theta = acos(((loc->slat *  cos(phi)) - sdecl) / (loc->clat *  sin(phi)));

    *el = 90.0 - (phi*RAD_TO_DEG);
    *az = theta * RAD_TO_DEG;

    printf("\nY=%f Eqtime=%f  decl=%f\n tOff=%f tst=%f  ha=%f tz=%f \n",y,eqtime,decl,time_offset,tst,ha,loc->timezone );
    printf("Phi=%f,   theta=%f\n", phi, theta);

    return (GEO_OK );
}

// ------------------------------------------------------------------------

int DLL_CALLCONV geoSunM(GEO_LOCATION *loc, struct tm *newtime,double *az, double *el)
{
/*
From
ftp://climate1.gsfc.nasa.gov/wiscombe/Solar_Rad/SunAngles/sunae.f


c     Calculates the local solar azimuth and elevation angles, and
c     the distance to and angle subtended by the Sun, at a specific
c     location and time using approximate formulas in The Astronomical
c     Almanac.  Accuracy of angles is 0.01 deg or better (the angular
c     width of the Sun is about 0.5 deg, so 0.01 deg is more than
c     sufficient for most applications).

c     Unlike many GCM (and other) sun angle routines, this
c     one gives slightly different sun angles depending on
c     the year.  The difference is usually down in the 4th
c     significant digit but can slowly creep up to the 3rd
c     significant digit after several decades to a century.

c     A refraction correction appropriate for the "US Standard
c     Atmosphere" is added, so that the returned sun position is
c     the APPARENT one.  The correction is below 0.1 deg for solar
c     elevations above 9 deg.  To remove it, comment out the code
c     block where variable REFRAC is set, and replace it with
c     REFRAC = 0.0.

c   References:

c     Michalsky, J., 1988: The Astronomical Almanac's algorithm for
c        approximate solar position (1950-2050), Solar Energy 40,
c        227-235 (but the version of this program in the Appendix
c        contains errors and should not be used)

c     The Astronomical Almanac, U.S. Gov't Printing Office, Washington,
c        D.C. (published every year): the formulas used from the 1995
c        version are as follows:
c        p. A12: approximation to sunrise/set times
c        p. B61: solar elevation ("altitude") and azimuth
c        p. B62: refraction correction
c        p. C24: mean longitude, mean anomaly, ecliptic longitude,
c                obliquity of ecliptic, right ascension, declination,
c                Earth-Sun distance, angular diameter of Sun
c        p. L2:  Greenwich mean sidereal time (ignoring T^2, T^3 terms)


c   Authors:  Dr. Joe Michalsky (joe@asrc.albany.edu)
c             Dr. Lee Harrison (lee@asrc.albany.edu)
c             Atmospheric Sciences Research Center
c             State University of New York
c             Albany, New York

c   Modified by:  Dr. Warren Wiscombe (wiscombe@climate.gsfc.nasa.gov)
c                 NASA Goddard Space Flight Center
c                 Code 913
c                 Greenbelt, MD 20771


c   WARNING:  Do not use this routine outside the year range
c             1950-2050.  The approximations become increasingly
c             worse, and the calculation of Julian date becomes
c             more involved.


          SUBROUTINE SUNAE( YEAR, DAY, HOUR, LAT, LONG,
         &                  AZ, EL, SOLDIA, SOLDST )


    c   Input:
    c      YEAR     year (INTEGER; range 1950 to 2050)
    c      DAY      day of year at LAT-LONG location (INTEGER; range 1-366)
    c      HOUR     hour of DAY [GMT or UT] (REAL; range -13.0 to 36.0)
    c               = (local hour) + (time zone number)
    c                 + (Daylight Savings Time correction; -1 or 0)
    C               where (local hour) range is 0 to 24,
    c               (time zone number) range is -12 to +12, and
    c               (Daylight Time correction) is -1 if on Daylight Time
    c               (summer half of year), 0 otherwise;
    c               Example: 8:30 am Eastern Daylight Time would be
    c
    c                           HOUR = 8.5 + 5 - 1 = 12.5
    c      LAT      latitude [degrees]
    c               (REAL; range -90.0 to 90.0; north is positive)
    c      LONG     longitude [degrees]
    c               (REAL; range -180.0 to 180.0; east is positive)
    c   Output:
    c      AZ       solar azimuth angle (measured east from north,
    c               0 to 360 degs)
    c      EL       solar elevation angle [-90 to 90 degs];
    c               solar zenith angle = 90 - EL
    c      SOLDIA   solar diameter [degs]
    c      SOLDST   distance to sun [Astronomical Units, AU]
    c               (1 AU = mean Earth-sun distance = 1.49597871E+11 m
    c                in IAU 1976 System of Astronomical Constants)
    c   Local Variables:
    c     DEC       Declination (radians)
    c     ECLONG    Ecliptic longitude (radians)
    c     GMST      Greenwich mean sidereal time (hours)
    c     HA        Hour angle (radians, -pi to pi)
    c     JD        Modified Julian date (number of days, including
    c               fractions thereof, from Julian year J2000.0);
    c               actual Julian date is JD + 2451545.0
    c     LMST      Local mean sidereal time (radians)
    c     MNANOM    Mean anomaly (radians, normalized to 0 to 2*pi)
    c     MNLONG    Mean longitude of Sun, corrected for aberration
    c               (deg; normalized to 0-360)
    c     OBLQEC    Obliquity of the ecliptic (radians)
    c     RA        Right ascension  (radians)
    c     REFRAC    Refraction correction for US Standard Atmosphere (degs)
    c --------------------------------------------------------------------
    */

    //c     .. Scalar Arguments ..

          int   YEAR, DAY;
          double      AZ, EL, AZr, ELr,  HOUR, SOLDIA, SOLDST;
    //c     ..
    //c     .. Local Scalars ..

          int    DELTA, LEAP;
          double  DEC, DEN, ECLONG, GMST, HA, JD, LMST,
                           MNANOM, MNLONG, NUM, OBLQEC, RA,
                           REFRAC=0.0, TIME;
    //c     ..

/*
          if( YEAR < 1950 .OR. YEAR.GT.2050 )
         &    STOP 'SUNAE--bad input variable YEAR'
          if( DAY < 1 .OR. DAY.GT.366 )
         &    STOP 'SUNAE--bad input variable DAY'
          if( HOUR < -13.0 .OR. HOUR.GT.36.0 )
         &    STOP 'SUNAE--bad input variable HOUR'
          if( LAT < -90.0 .OR. LAT.GT.90.0 )
         &    STOP 'SUNAE--bad input variable LAT'
          if( LONG < -180.0 .OR. LONG.GT.180.0 )
         &    STOP 'SUNAE--bad input variable LONG'

          if(PASS1) THEN
             PI     = 2.*A sin( 1.0 )
             TWOPI  = 2.*PI
             RPD    = PI / 180.
             PASS1 = .False.
          ENDIF
  */
          YEAR = newtime->tm_year+1900;
          DAY = jday(newtime->tm_mon, newtime->tm_mday);
          HOUR = newtime->tm_hour + (newtime->tm_min/60.0) + (newtime->tm_sec/3600.0) + loc->timezone - loc->dst;

          printf("Hour=%f\n",HOUR);

          // ** current Julian date (actually add 2,400,000
          // ** for true JD);  LEAP = leap days since 1949;
          // ** 32916.5 is midnite 0 jan 1949 minus 2.4e6
          DELTA  = YEAR - 1949;
          LEAP   = DELTA / 4;
          JD     = 32916.5 + ((DELTA * 365.0) + LEAP + DAY) + HOUR / 24.0 +1;

          // ** last yr of century not leap yr unless divisible
          // ** by 400 (not executed for the allowed YEAR range,
          // ** but left in so our successors can adapt this for
          // ** the following 100 years)

          if( ( ( YEAR % 100 ) == 0) &&  (( YEAR % 400 ) != 0) ) JD = JD - 1.0;

          // ** ecliptic coordinates
          // ** 51545.0 + 2.4e6 = noon 1 jan 2000
          TIME  = JD - 51545.0;

          printf("JD=%f  TIME=%f\n",JD,TIME);

          // ** force mean longitude between 0 and 360 degs
          MNLONG = 280.460 + 0.9856474 * TIME;
          MNLONG = fmod( MNLONG, 360.0 );
          if( MNLONG < 0.0 ) MNLONG = MNLONG + 360.0;

          // ** mean anomaly in radians between 0 and 2*pi
          MNANOM = 357.528 + 0.9856003 * TIME;
          MNANOM = fmod( MNANOM, 360.0 );
          if( MNANOM < 0.0) MNANOM = MNANOM + 360.0;
          printf("Mean Long=%f Mean Anom=%f\n",MNLONG,MNANOM);

          MNANOM = MNANOM * DEG_TO_RAD; //radians

          // ** ecliptic longitude and obliquity
          // ** of ecliptic in radians
          ECLONG = MNLONG + 1.915 *  sin( MNANOM ) + 0.020 *  sin( 2.0 * MNANOM );
          ECLONG = fmod( ECLONG, 360.0 );
          if( ECLONG < 0.0 ) ECLONG = ECLONG + 360.0;

          OBLQEC = 23.439 - 0.0000004 * TIME;
          ECLONG = ECLONG * DEG_TO_RAD;
          OBLQEC = OBLQEC * DEG_TO_RAD;
          printf("Ecc Long=%f Obliq=%f\n",ECLONG*RAD_TO_DEG,OBLQEC*RAD_TO_DEG);

          // ** right ascension
          NUM  =  cos( OBLQEC )* sin( ECLONG );
          DEN  =  cos( ECLONG );
          RA   =  atan2( NUM , DEN );
          printf("RA-raw = %f num=%f, den=%f\n",RA*RAD_TO_DEG,NUM,DEN);


          // ** Force right ascension between 0 and 2*pi
          if( DEN < 0.0 )              RA  = RA + M_PI;
          else if( NUM < 0.0 )         RA  = RA + TWO_PI;

          printf("RA = %f ",RA*RAD_TO_DEG);

          // ** declination
          DEC  = asin( sin(OBLQEC) * sin(ECLONG) );

          printf("DEC = %f \n",DEC*RAD_TO_DEG);

          // ** Greenwich mean sidereal time in hours
          GMST = 6.697375 + 0.0657098242*TIME + HOUR;

          // ** Hour not changed to sidereal time since
          // ** 'time' includes the fractional day
          GMST  = fmod( GMST, 24.0 );
          if( GMST < 0.0 ) GMST   = GMST + 24.0;

          // ** local mean sidereal time in radians
          LMST  = GMST + loc->lon / 15.0;
          LMST  = fmod( LMST, 24.0 );
          if( LMST < 0.0 ) LMST   = LMST + 24.0;

          LMST   = LMST*15.0 * DEG_TO_RAD;

          // ** hour angle in radians between -pi and pi
          HA  = LMST - RA;

          if( HA < -M_PI )  HA  = HA + TWO_PI;
          if( HA > M_PI )   HA  = HA - TWO_PI;

          printf("HA=%f deg\n",HA*RAD_TO_DEG);
          //HA = -7.99 * DEG_TO_RAD;


          // ** solar azimuth and elevation (radians)
          ELr  = asin(  sin( DEC )* loc->slat  +
                       cos( DEC )* loc->clat   * cos( HA ) );

      //    AZr  = asin( -cos( DEC )* sin( HA ) /  cos( ELr ) );
          AZr  = atan( sin( HA ) /  ((cos( HA ) * loc->slat) - (tan(DEC) * loc->clat)) );
           printf("raw az=%f DECL = %f\n",AZr*RAD_TO_DEG, DEC*RAD_TO_DEG);

          // ** Put azimuth between 0 and 2*pi radians
          if(  sin( DEC ) -  sin( ELr ) * loc->slat  >= 0.0 )
          {

              if(  sin(AZr) < 0.0) AZr  = AZr + TWO_PI;
              else                 AZr  = M_PI - AZr;

          }

          // ** Convert elevation and azimuth to degrees
          EL  = ELr * RAD_TO_DEG;
          AZ  = AZr * RAD_TO_DEG;

          //  ======== Refraction correction for U.S. Standard Atmos. ==========
          //      (assumes elevation in degs) (3.51823=1013.25 mb/288 K)
          if( EL >= 19.225 )      REFRAC = 0.00452 * 3.51823 / tan( ELr );
          else
          {
             if( (EL > -0.766) && (EL < 19.225) )
                REFRAC = 3.51823 * ( 0.1594 + EL*(0.0196 + 0.00002*EL) ) /
                     ( 1.0 + EL * (0.505 + 0.0845 * EL) );
             else if( EL <= -0.766 ) REFRAC = 0.0;
          }
          printf("Refrac = %f deg\n",REFRAC);
          EL  = EL + REFRAC;

          // ===================================================================
          // ** distance to sun in A.U. & diameter in degs

          SOLDST = 1.00014 - 0.01671 * cos(MNANOM) - 0.00014 * cos( 2.0 *MNANOM );
          SOLDIA = 0.5332 / SOLDST;

          if( EL < -90.0 || EL > 90.0 )  printf("SUNAE--output argument EL out of range(%f)\n",EL);
          if( AZ < 0.0   || AZ > 360.0 ) printf("SUNAE--output argument AZ out of range(%f)\n",AZ);

          *az = AZ;
          *el = EL;

          return(GEO_OK);
}

#define oe_base (23.0+(26.0/60.0)+(21.448/3600.0))

// t is in julian centuries from J2000
double obliquity(double t)
{
    double u,u2,u3,u4,u5,u6,u7,u8,u9,u10;
    u = t/100.0;
    u2= u*u;
    u3=u2*u;
    u4=u3*u;
    u5=u4*u;
    u6=u5*u;
    u7=u6*u;
    u8=u7*u;
    u9=u8*u;
    u10=u9*u;

    // high precision
    return(oe_base
           - (4680.93/3600.0  * u)
           - (1.55   /3600.0  * u2)
           + (1999.25/3600.0  * u3)
           - (51.38  /3600.0  * u4)
           - (249.67 /3600.0  * u5)
           - (39.05  /3600.0  * u6)
           + (7.12   /3600.0  * u7)
           + (27.87  /3600.0  * u8)
           + (5.79   /3600.0  * u9)
           + (2.45   /3600.0  * u10)
           );

    // low precision:
    //oe = oe_base -( 46.8150 / 3660.0 * t) - (0.00059 / 3660.0 * t2) + (0.001813 / 3660.0 *t3);

}

// this is the solar position algorithm from
// Astronomical Algorithms, by Jean Meeus
int DLL_CALLCONV geoSunAA(GEO_LOCATION *loc, struct tm *newtime,double *az, double *el)
{
    double jd,l0, t,t2,t3,m,m_r, e,c,r,
           trueLon,trueLon_r,trueLon2k,trueAnom,trueAnom_r,
           partDay,appLon,appLon_r,oe,oec,oe_r,oec_r,om,om_r,ra,dec;
    int year;

    year = newtime->tm_year+1900;
    partDay = (newtime->tm_hour + (newtime->tm_min/60.0) + (newtime->tm_sec / 3600.0) - loc->timezone)/24.0;

    // j2000 date
    jd = JD(year,newtime->tm_mon,newtime->tm_mday);
    printf("\nJD=%10.5f ",jd+partDay);

    // j2000 date in centuries
    t=(jd+partDay- 2451545.0)/36525.0;
    t2 = t*t;
    t3= t2*t;
    printf("T=%2.10f\n",t);

    // Geometric mean longitude referred to the mean equinox of the date
    l0 = 280.46645 + (36000.76983 * t) + (0.0003032 * t2);
    if(l0 < 0.0) l0 = fmod(l0,360.0) + 360.0;

    // mean anomoly of the Sun
    m = 357.52910 + (35999.05030 * t) - (0.0001559 * t2)  - (0.00000048 * t3);
 //   if(m < 0.0) m = fmod(m,360.0) + 360.0;
    m_r = m * DEG_TO_RAD;

    // eccentricity of the Earth's orbit
    e = 0.016708617 - (0.000042037 * t) - (0.0000001236 * t2);

    // Sun's equation of center
    c = (1.914600 - (0.004817 * t) - (0.000014 * t2)) * sin(m_r)  +
            (0.019993 - (0.000101 * t)) * sin(m_r+m_r) +
            (0.000290 * sin(m_r+m_r+m_r));

    // Sun true longitude & anomoly
    trueLon   = l0 + c;
    trueLon_r = trueLon * DEG_TO_RAD;
    trueAnom  = m + c;
    trueAnom_r= trueAnom * DEG_TO_RAD;
    trueLon2k =  trueLon - (0.01397 * (year - 2000));
    printf("\nl0= %f  m=%f e=%f  c=%f  trueLon=%f trueAnom=%f\n",l0,m,e,c,trueLon,trueAnom);

    // radius vector
    r = (1.000001018 * (1.0-(e*e)))/(1.0 + e *cos(trueAnom_r));

    // apparent longitude
    om      = 125.04 - 1934.136 * t;
    om_r    = om  * DEG_TO_RAD;
    appLon  = trueLon - 0.00569 - (0.00478 * sin(om_r));
    appLon_r= appLon * DEG_TO_RAD;

    // obliquity of ecliptic
    // and corrected value (oec)
    oe   = obliquity(t);
    oec  = oe + 0.00256 * cos(DEG_TO_RAD*(125.04-1934.1136*t)); // corrected
    oe_r = oe  * DEG_TO_RAD;
    oec_r= oec * DEG_TO_RAD;
    printf("\nom=%f  appLon=%f oe=%f oec=%f r=%f\n",om,appLon,oe,oec,r);

    ra = atan2(cos(oec_r)*sin(appLon_r),  cos(appLon_r))*RAD_TO_DEG;
 //   if(ra < 0.0) ra = fmod(ra,360.0); // + 360.0;

    dec = asin(sin(oec_r)*sin(appLon_r))*RAD_TO_DEG;
    printf("\nra=%f, dec=%f\n",ra, dec);

    //
    return 1;
}
