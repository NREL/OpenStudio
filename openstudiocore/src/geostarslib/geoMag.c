/*!
\file	geoMag.c
\brief	This file contains the source for deriving geomagnetic variables
  from geodetic coordinates
*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "geoStars.h"


#define wmmData wmmDta2010
#define EPOCH   2010.0

/*!
\brief This structure array contains the all of the coefficients for WMM-2000 used in this library


This is documentation that came with the WMM. It is included here for completeness.
\verbatim
C     MODEL:  THE WMM SERIES GEOMAGNETIC MODELS ARE COMPOSED
C             OF TWO PARTS:  THE MAIN FIELD MODEL, WHICH IS
C             VALID AT THE BASE EPOCH OF THE CURRENT MODEL AND
C             A SECULAR VARIATION MODEL, WHICH ACCOUNTS FOR SLOW
C             TEMPORAL VARIATIONS IN THE MAIN GEOMAGNETIC FIELD
C             FROM THE BASE EPOCH TO A MAXIMUM OF 5 YEARS BEYOND
C             THE BASE EPOCH.  FOR EXAMPLE, THE BASE EPOCH OF
C             THE WMM-2005 MODEL IS 2005.0.  THIS MODEL IS THEREFORE
C             CONSIDERED VALID BETWEEN 2005.0 AND 2010.0. THE
C             COMPUTED MAGNETIC PARAMETERS ARE REFERENCED TO THE
C             WGS-84 ELLIPSOID.
C
C***********************************************************************
C
C     ACCURACY:  IN OCEAN AREAS AT THE EARTH'S SURFACE OVER THE
C                ENTIRE 5 YEAR LIFE OF THE DEGREE AND ORDER 12
C                SPHERICAL HARMONIC MODEL WMM-2005, THE ESTIMATED
C                MAXIMUM RMS ERRORS FOR THE VARIOUS MAGNETIC COMPONENTS
C                ARE:
C
C                DEC  -   0.5 Degrees
C                DIP  -   0.5 Degrees
C                TI   - 280.0 nanoTeslas (nT)
C                GV   -   0.5 Degrees
C
C                OTHER MAGNETIC COMPONENTS THAT CAN BE DERIVED FROM
C                THESE FOUR BY SIMPLE TRIGONOMETRIC RELATIONS WILL
C                HAVE THE FOLLOWING APPROXIMATE ERRORS OVER OCEAN AREAS:
C
C                X    - 140 nT (North)
C                Y    - 140 nT (East)
C                Z    - 200 nT (Vertical) Positive is down
C                H    - 200 nT (Horizontal)
C
C                OVER LAND THE MAXIMUM RMS ERRORS ARE EXPECTED TO BE
C                HIGHER, ALTHOUGH THE RMS ERRORS FOR DEC, DIP, AND GV
C                ARE STILL ESTIMATED TO BE LESS THAN 1.0 DEGREE, FOR
C                THE ENTIRE 5-YEAR LIFE OF THE MODEL AT THE EARTH's
C                SURFACE.  THE OTHER COMPONENT ERRORS OVER LAND ARE
C                MORE DIFFICULT TO ESTIMATE AND SO ARE NOT GIVEN.
C
C                THE ACCURACY AT ANY GIVEN TIME FOR ALL OF THESE
C                GEOMAGNETIC PARAMETERS DEPENDS ON THE GEOMAGNETIC
C                LATITUDE.  THE ERRORS ARE LEAST FROM THE EQUATOR TO
C                MID-LATITUDES AND GREATEST NEAR THE MAGNETIC POLES.
C
C                IT IS VERY IMPORTANT TO NOTE THAT A DEGREE AND
C                ORDER 12 MODEL, SUCH AS WMM-2005, DESCRIBES ONLY
C                THE LONG WAVELENGTH SPATIAL MAGNETIC FLUCTUATIONS
C                DUE TO EARTH'S CORE.  NOT INCLUDED IN THE WMM SERIES
C                MODELS ARE INTERMEDIATE AND SHORT WAVELENGTH
C                SPATIAL FLUCTUATIONS OF THE GEOMAGNETIC FIELD
C                WHICH ORIGINATE IN THE EARTH'S MANTLE AND CRUST.
C                CONSEQUENTLY, ISOLATED ANGULAR ERRORS AT VARIOUS
C                POSITIONS ON THE SURFACE (PRIMARILY OVER LAND, IN
C                CONTINENTAL MARGINS AND OVER OCEANIC SEAMOUNTS,
C                RIDGES AND TRENCHES) OF SEVERAL DEGREES MAY BE
C                EXPECTED. ALSO NOT INCLUDED IN THE MODEL ARE
C                NONSECULAR TEMPORAL FLUCTUATIONS OF THE GEOMAGNETIC
C                FIELD OF MAGNETOSPHERIC AND IONOSPHERIC ORIGIN.
C                DURING MAGNETIC STORMS, TEMPORAL FLUCTUATIONS CAN
C                CAUSE SUBSTANTIAL DEVIATIONS OF THE GEOMAGNETIC
C                FIELD FROM MODEL VALUES.  IN ARCTIC AND ANTARCTIC
C                REGIONS, AS WELL AS IN EQUATORIAL REGIONS, DEVIATIONS
C                FROM MODEL VALUES ARE BOTH FREQUENT AND PERSISTENT.
C
C                IF THE REQUIRED DECLINATION ACCURACY IS MORE
C                STRINGENT THAN THE WMM SERIES OF MODELS PROVIDE, THEN
C                THE USER IS ADVISED TO REQUEST SPECIAL (REGIONAL OR
C                LOCAL) SURVEYS BE PERFORMED AND MODELS PREPARED.
C                REQUESTS OF THIS NATURE SHOULD BE MADE TO NIMA
C                AT THE ADDRESS ABOVE.
\endverbatim
*/

static WMM_DATA wmmDta2000[] =
{
/*-----n---m------gnm--------hnm---------dgnm---------dhnm---*/
{  1,  0,  -29616.0,       0.0,       14.7,        0.0 },
{  1,  1,   -1722.7,    5194.5,       11.1,      -20.4 },
{  2,  0,   -2266.7,       0.0,      -13.6,        0.0 },
{  2,  1,    3070.2,   -2484.8,       -0.7,      -21.5 },
{  2,  2,    1677.6,    -467.9,       -1.8,       -9.6 },
{  3,  0,    1322.4,       0.0,        0.3,        0.0 },
{  3,  1,   -2291.5,    -224.7,       -4.3,        6.4 },
{  3,  2,    1255.9,     293.0,        0.9,       -1.3 },
{  3,  3,     724.8,    -486.5,       -8.4,      -13.3 },
{  4,  0,     932.1,       0.0,       -1.6,        0.0 },
{  4,  1,     786.3,     273.3,        0.9,        2.3 },
{  4,  2,     250.6,    -227.9,       -7.6,        0.7 },
{  4,  3,    -401.5,     120.9,        2.2,        3.7 },
{  4,  4,     106.2,    -302.7,       -3.2,       -0.5 },
{  5,  0,    -211.9,       0.0,       -0.9,        0.0 },
{  5,  1,     351.6,      42.0,       -0.2,        0.0 },
{  5,  2,     220.8,     173.8,       -2.5,        2.1 },
{  5,  3,    -134.5,    -135.0,       -2.7,        2.3 },
{  5,  4,    -168.8,     -38.6,       -0.9,        3.1 },
{  5,  5,     -13.3,     105.2,        1.7,        0.0 },
{  6,  0,      73.8,       0.0,        1.2,        0.0 },
{  6,  1,      68.2,     -17.4,        0.2,       -0.3 },
{  6,  2,      74.1,      61.2,        1.7,       -1.7 },
{  6,  3,    -163.5,      63.2,        1.6,       -0.9 },
{  6,  4,      -3.8,     -62.9,       -0.1,       -1.0 },
{  6,  5,      17.1,       0.2,       -0.3,       -0.1 },
{  6,  6,     -85.1,      43.0,        0.8,        1.9 },
{  7,  0,      77.4,       0.0,       -0.4,        0.0 },
{  7,  1,     -73.9,     -62.3,       -0.8,        1.4 },
{  7,  2,       2.2,     -24.5,       -0.2,        0.2 },
{  7,  3,      35.7,       8.9,        1.1,        0.7 },
{  7,  4,       7.3,      23.4,        0.4,        0.4 },
{  7,  5,       5.2,      15.0,        0.0,       -0.3 },
{  7,  6,       8.4,     -27.6,       -0.2,       -0.8 },
{  7,  7,      -1.5,      -7.8,       -0.2,       -0.1 },
{  8,  0,      23.3,       0.0,       -0.3,        0.0 },
{  8,  1,       7.3,      12.4,        0.6,       -0.5 },
{  8,  2,      -8.5,     -20.8,       -0.8,        0.1 },
{  8,  3,      -6.6,       8.4,        0.3,       -0.2 },
{  8,  4,     -16.9,     -21.2,       -0.2,        0.0 },
{  8,  5,       8.6,      15.5,        0.5,        0.1 },
{  8,  6,       4.9,       9.1,        0.0,       -0.1 },
{  8,  7,      -7.8,     -15.5,       -0.6,        0.3 },
{  8,  8,      -7.6,      -5.4,        0.1,        0.2 },
{  9,  0,       5.7,       0.0,        0.0,        0.0 },
{  9,  1,       8.5,     -20.4,        0.0,        0.0 },
{  9,  2,       2.0,      13.9,        0.0,        0.0 },
{  9,  3,      -9.8,      12.0,        0.0,        0.0 },
{  9,  4,       7.6,      -6.2,        0.0,        0.0 },
{  9,  5,      -7.0,      -8.6,        0.0,        0.0 },
{  9,  6,      -2.0,       9.4,        0.0,        0.0 },
{  9,  7,       9.2,       5.0,        0.0,        0.0 },
{  9,  8,      -2.2,      -8.4,        0.0,        0.0 },
{  9,  9,      -6.6,       3.2,        0.0,        0.0 },
{ 10,  0,      -2.2,       0.0,        0.0,        0.0 },
{ 10,  1,      -5.7,       0.9,        0.0,        0.0 },
{ 10,  2,       1.6,      -0.7,        0.0,        0.0 },
{ 10,  3,      -3.7,       3.9,        0.0,        0.0 },
{ 10,  4,      -0.6,       4.8,        0.0,        0.0 },
{ 10,  5,       4.1,      -5.3,        0.0,        0.0 },
{ 10,  6,       2.2,      -1.0,        0.0,        0.0 },
{ 10,  7,       2.2,      -2.4,        0.0,        0.0 },
{ 10,  8,       4.6,       1.3,        0.0,        0.0 },
{ 10,  9,       2.3,      -2.3,        0.0,        0.0 },
{ 10, 10,       0.1,      -6.4,        0.0,        0.0 },
{ 11,  0,       3.3,       0.0,        0.0,        0.0 },
{ 11,  1,      -1.1,      -1.5,        0.0,        0.0 },
{ 11,  2,      -2.4,       0.7,        0.0,        0.0 },
{ 11,  3,       2.6,      -1.1,        0.0,        0.0 },
{ 11,  4,      -1.3,      -2.3,        0.0,        0.0 },
{ 11,  5,      -1.7,       1.3,        0.0,        0.0 },
{ 11,  6,      -0.6,      -0.6,        0.0,        0.0 },
{ 11,  7,       0.4,      -2.8,        0.0,        0.0 },
{ 11,  8,       0.7,      -1.6,        0.0,        0.0 },
{ 11,  9,      -0.3,      -0.1,        0.0,        0.0 },
{ 11, 10,       2.3,      -1.9,        0.0,        0.0 },
{ 11, 11,       4.2,       1.4,        0.0,        0.0 },
{ 12,  0,      -1.5,       0.0,        0.0,        0.0 },
{ 12,  1,      -0.2,      -1.0,        0.0,        0.0 },
{ 12,  2,      -0.3,       0.7,        0.0,        0.0 },
{ 12,  3,       0.5,       2.2,        0.0,        0.0 },
{ 12,  4,       0.2,      -2.5,        0.0,        0.0 },
{ 12,  5,       0.9,      -0.2,        0.0,        0.0 },
{ 12,  6,      -1.4,       0.0,        0.0,        0.0 },
{ 12,  7,       0.6,      -0.2,        0.0,        0.0 },
{ 12,  8,      -0.6,       0.0,        0.0,        0.0 },
{ 12,  9,      -1.0,       0.2,        0.0,        0.0 },
{ 12, 10,      -0.3,      -0.9,        0.0,        0.0 },
{ 12, 11,       0.3,      -0.2,        0.0,        0.0 },
{ 12, 12,       0.4,       1.0,        0.0,        0.0 }
};


/*!
\brief This structure array contains the all of the coefficients for WMM-2005 used in this library

*/

static WMM_DATA wmmDta2005[] =
{
/*-----n---m------gnm--------hnm---------dgnm---------dhnm---*/
{  1,  0,  -29556.8,       0.0,        8.0,        0.0},
{  1,  1,   -1671.7,    5079.8,       10.6,      -20.9},
{  2,  0,   -2340.6,       0.0,      -15.1,        0.0},
{  2,  1,    3046.9,   -2594.7,       -7.8,      -23.2},
{  2,  2,    1657.0,    -516.7,       -0.8,      -14.6},
{  3,  0,    1335.4,       0.0,        0.4,        0.0},
{  3,  1,   -2305.1,    -199.9,       -2.6,        5.0},
{  3,  2,    1246.7,     269.3,       -1.2,       -7.0},
{  3,  3,     674.0,    -524.2,       -6.5,       -0.6},
{  4,  0,     919.8,       0.0,       -2.5,        0.0},
{  4,  1,     798.1,     281.5,        2.8,        2.2},
{  4,  2,     211.3,    -226.0,       -7.0,        1.6},
{  4,  3,    -379.4,     145.8,        6.2,        5.8},
{  4,  4,     100.0,    -304.7,       -3.8,        0.1},
{  5,  0,    -227.4,       0.0,       -2.8,        0.0},
{  5,  1,     354.6,      42.4,        0.7,        0.0},
{  5,  2,     208.7,     179.8,       -3.2,        1.7},
{  5,  3,    -136.5,    -123.0,       -1.1,        2.1},
{  5,  4,    -168.3,     -19.5,        0.1,        4.8},
{  5,  5,     -14.1,     103.6,       -0.8,       -1.1},
{  6,  0,      73.2,       0.0,       -0.7,        0.0},
{  6,  1,      69.7,     -20.3,        0.4,       -0.6},
{  6,  2,      76.7,      54.7,       -0.3,       -1.9},
{  6,  3,    -151.2,      63.6,        2.3,       -0.4},
{  6,  4,     -14.9,     -63.4,       -2.1,       -0.5},
{  6,  5,      14.6,      -0.1,       -0.6,       -0.3},
{  6,  6,     -86.3,      50.4,        1.4,        0.7},
{  7,  0,      80.1,       0.0,        0.2,        0.0},
{  7,  1,     -74.5,     -61.5,       -0.1,        0.6},
{  7,  2,      -1.4,     -22.4,       -0.3,        0.4},
{  7,  3,      38.5,       7.2,        1.1,        0.2},
{  7,  4,      12.4,      25.4,        0.6,        0.3},
{  7,  5,       9.5,      11.0,        0.5,       -0.8},
{  7,  6,       5.7,     -26.4,       -0.4,       -0.2},
{  7,  7,       1.8,      -5.1,        0.6,        0.1},
{  8,  0,      24.9,       0.0,        0.1,        0.0},
{  8,  1,       7.7,      11.2,        0.3,       -0.2},
{  8,  2,     -11.6,     -21.0,       -0.4,        0.1},
{  8,  3,      -6.9,       9.6,        0.3,        0.3},
{  8,  4,     -18.2,     -19.8,       -0.3,        0.4},
{  8,  5,      10.0,      16.1,        0.2,        0.1},
{  8,  6,       9.2,       7.7,        0.4,       -0.2},
{  8,  7,     -11.6,     -12.9,       -0.7,        0.4},
{  8,  8,      -5.2,      -0.2,        0.4,        0.4},
{  9,  0,       5.6,       0.0,        0.0,        0.0},
{  9,  1,       9.9,     -20.1,        0.0,        0.0},
{  9,  2,       3.5,      12.9,        0.0,        0.0},
{  9,  3,      -7.0,      12.6,        0.0,        0.0},
{  9,  4,       5.1,      -6.7,        0.0,        0.0},
{  9,  5,     -10.8,      -8.1,        0.0,        0.0},
{  9,  6,      -1.3,       8.0,        0.0,        0.0},
{  9,  7,       8.8,       2.9,        0.0,        0.0},
{  9,  8,      -6.7,      -7.9,        0.0,        0.0},
{  9,  9,      -9.1,       6.0,        0.0,        0.0},
{ 10,  0,      -2.3,       0.0,        0.0,        0.0},
{ 10,  1,      -6.3,       2.4,        0.0,        0.0},
{ 10,  2,       1.6,       0.2,        0.0,        0.0},
{ 10,  3,      -2.6,       4.4,        0.0,        0.0},
{ 10,  4,       0.0,       4.8,        0.0,        0.0},
{ 10,  5,       3.1,      -6.5,        0.0,        0.0},
{ 10,  6,       0.4,      -1.1,        0.0,        0.0},
{ 10,  7,       2.1,      -3.4,        0.0,        0.0},
{ 10,  8,       3.9,      -0.8,        0.0,        0.0},
{ 10,  9,      -0.1,      -2.3,        0.0,        0.0},
{ 10, 10,      -2.3,      -7.9,        0.0,        0.0},
{ 11,  0,       2.8,       0.0,        0.0,        0.0},
{ 11,  1,      -1.6,       0.3,        0.0,        0.0},
{ 11,  2,      -1.7,       1.2,        0.0,        0.0},
{ 11,  3,       1.7,      -0.8,        0.0,        0.0},
{ 11,  4,      -0.1,      -2.5,        0.0,        0.0},
{ 11,  5,       0.1,       0.9,        0.0,        0.0},
{ 11,  6,      -0.7,      -0.6,        0.0,        0.0},
{ 11,  7,       0.7,      -2.7,        0.0,        0.0},
{ 11,  8,       1.8,      -0.9,        0.0,        0.0},
{ 11,  9,       0.0,      -1.3,        0.0,        0.0},
{ 11, 10,       1.1,      -2.0,        0.0,        0.0},
{ 11, 11,       4.1,      -1.2,        0.0,        0.0},
{ 12,  0,      -2.4,       0.0,        0.0,        0.0},
{ 12,  1,      -0.4,      -0.4,        0.0,        0.0},
{ 12,  2,       0.2,       0.3,        0.0,        0.0},
{ 12,  3,       0.8,       2.4,        0.0,        0.0},
{ 12,  4,      -0.3,      -2.6,        0.0,        0.0},
{ 12,  5,       1.1,       0.6,        0.0,        0.0},
{ 12,  6,      -0.5,       0.3,        0.0,        0.0},
{ 12,  7,       0.4,       0.0,        0.0,        0.0},
{ 12,  8,      -0.3,       0.0,        0.0,        0.0},
{ 12,  9,      -0.3,       0.3,        0.0,        0.0},
{ 12, 10,      -0.1,      -0.9,        0.0,        0.0},
{ 12, 11,      -0.3,      -0.4,        0.0,        0.0},
{ 12, 12,      -0.1,       0.8,        0.0,        0.0}
};




static WMM_DATA wmmDta2010[] =
{
/*-----n---m------gnm--------hnm---------dgnm---------dhnm---*/
{  1,  0,  -29496.6,       0.0,       11.6,        0.0},
{  1,  1,   -1586.3,    4944.4,       16.5,      -25.9},
{  2,  0,   -2396.6,       0.0,      -12.1,        0.0},
{  2,  1,    3026.1,   -2707.7,       -4.4,      -22.5},
{  2,  2,    1668.6,    -576.1,        1.9,      -11.8},
{  3,  0,    1340.1,       0.0,        0.4,        0.0},
{  3,  1,   -2326.2,    -160.2,       -4.1,        7.3},
{  3,  2,    1231.9,     251.9,       -2.9,       -3.9},
{  3,  3,     634.0,    -536.6,       -7.7,       -2.6},
{  4,  0,     912.6,       0.0,       -1.8,        0.0},
{  4,  1,     808.9,     286.4,        2.3,        1.1},
{  4,  2,     166.7,    -211.2,       -8.7,        2.7},
{  4,  3,    -357.1,     164.3,        4.6,        3.9},
{  4,  4,      89.4,    -309.1,       -2.1,       -0.8},
{  5,  0,    -230.9,       0.0,       -1.0,        0.0},
{  5,  1,     357.2,      44.6,        0.6,        0.4},
{  5,  2,     200.3,     188.9,       -1.8,        1.8},
{  5,  3,    -141.1,    -118.2,       -1.0,        1.2},
{  5,  4,    -163.0,       0.0,        0.9,        4.0},
{  5,  5,      -7.8,     100.9,        1.0,       -0.6},
{  6,  0,      72.8,       0.0,       -0.2,        0.0},
{  6,  1,      68.6,     -20.8,       -0.2,       -0.2},
{  6,  2,      76.0,      44.1,       -0.1,       -2.1},
{  6,  3,    -141.4,      61.5,        2.0,       -0.4},
{  6,  4,     -22.8,     -66.3,       -1.7,       -0.6},
{  6,  5,      13.2,       3.1,       -0.3,        0.5},
{  6,  6,     -77.9,      55.0,        1.7,        0.9},
{  7,  0,      80.5,       0.0,        0.1,        0.0},
{  7,  1,     -75.1,     -57.9,       -0.1,        0.7},
{  7,  2,      -4.7,     -21.1,       -0.6,        0.3},
{  7,  3,      45.3,       6.5,        1.3,       -0.1},
{  7,  4,      13.9,      24.9,        0.4,       -0.1},
{  7,  5,      10.4,       7.0,        0.3,       -0.8},
{  7,  6,       1.7,     -27.7,       -0.7,       -0.3},
{  7,  7,       4.9,      -3.3,        0.6,        0.3},
{  8,  0,      24.4,       0.0,       -0.1,        0.0},
{  8,  1,       8.1,      11.0,        0.1,       -0.1},
{  8,  2,     -14.5,     -20.0,       -0.6,        0.2},
{  8,  3,      -5.6,      11.9,        0.2,        0.4},
{  8,  4,     -19.3,     -17.4,       -0.2,        0.4},
{  8,  5,      11.5,      16.7,        0.3,        0.1},
{  8,  6,      10.9,       7.0,        0.3,       -0.1},
{  8,  7,     -14.1,     -10.8,       -0.6,        0.4},
{  8,  8,      -3.7,       1.7,        0.2,        0.3},
{  9,  0,       5.4,       0.0,        0.0,        0.0},
{  9,  1,       9.4,     -20.5,       -0.1,        0.0},
{  9,  2,       3.4,      11.5,        0.0,       -0.2},
{  9,  3,      -5.2,      12.8,        0.3,        0.0},
{  9,  4,       3.1,      -7.2,       -0.4,       -0.1},
{  9,  5,     -12.4,      -7.4,       -0.3,        0.1},
{  9,  6,      -0.7,       8.0,        0.1,        0.0},
{  9,  7,       8.4,       2.1,       -0.1,       -0.2},
{  9,  8,      -8.5,      -6.1,       -0.4,        0.3},
{  9,  9,     -10.1,       7.0,       -0.2,        0.2},
{ 10,  0,      -2.0,       0.0,        0.0,        0.0},
{ 10,  1,      -6.3,       2.8,        0.0,        0.1},
{ 10,  2,       0.9,      -0.1,       -0.1,       -0.1},
{ 10,  3,      -1.1,       4.7,        0.2,        0.0},
{ 10,  4,      -0.2,       4.4,        0.0,       -0.1},
{ 10,  5,       2.5,      -7.2,       -0.1,       -0.1},
{ 10,  6,      -0.3,      -1.0,       -0.2,        0.0},
{ 10,  7,       2.2,      -3.9,        0.0,       -0.1},
{ 10,  8,       3.1,      -2.0,       -0.1,       -0.2},
{ 10,  9,      -1.0,      -2.0,       -0.2,        0.0},
{ 10, 10,      -2.8,      -8.3,       -0.2,       -0.1},
{ 11,  0,       3.0,       0.0,        0.0,        0.0},
{ 11,  1,      -1.5,       0.2,        0.0,        0.0},
{ 11,  2,      -2.1,       1.7,        0.0,        0.1},
{ 11,  3,       1.7,      -0.6,        0.1,        0.0},
{ 11,  4,      -0.5,      -1.8,        0.0,        0.1},
{ 11,  5,       0.5,       0.9,        0.0,        0.0},
{ 11,  6,      -0.8,      -0.4,        0.0,        0.1},
{ 11,  7,       0.4,      -2.5,        0.0,        0.0},
{ 11,  8,       1.8,      -1.3,        0.0,       -0.1},
{ 11,  9,       0.1,      -2.1,        0.0,       -0.1},
{ 11, 10,       0.7,      -1.9,       -0.1,        0.0},
{ 11, 11,       3.8,      -1.8,        0.0,       -0.1},
{ 12,  0,      -2.2,       0.0,        0.0,        0.0},
{ 12,  1,      -0.2,      -0.9,        0.0,        0.0},
{ 12,  2,       0.3,       0.3,        0.1,        0.0},
{ 12,  3,       1.0,       2.1,        0.1,        0.0},
{ 12,  4,      -0.6,      -2.5,       -0.1,        0.0},
{ 12,  5,       0.9,       0.5,        0.0,        0.0},
{ 12,  6,      -0.1,       0.6,        0.0,        0.1},
{ 12,  7,       0.5,       0.0,        0.0,        0.0},
{ 12,  8,      -0.4,       0.1,        0.0,        0.0},
{ 12,  9,      -0.4,       0.3,        0.0,        0.0},
{ 12, 10,       0.2,      -0.9,        0.0,        0.0},
{ 12, 11,      -0.8,      -0.2,       -0.1,        0.0},
{ 12, 12,       0.0,       0.9,        0.1,        0.0}
};

static int started = 0;  //! \internal Flag for limiting multiple initializations

#define snorm p //! \internal redefinition to fit original fortran code
#define MAXDEG 12  //! \internal Maximum degrees


// Static Prototypes
static void  geoMagInit(void);



static double
c[13][13],      //!<       C      - GAUSS COEFFICIENTS OF MAIN GEOMAGNETIC MODEL (NT)
cd[13][13],     //!<       CD     - GAUSS COEFFICIENTS OF SECULAR GEOMAGNETIC MODEL (NT/YR)
tc[13][13],     //!<       TC     - TIME ADJUSTED GEOMAGNETIC GAUSS COEFFICIENTS (NT)
dp[13][13],     //!<       DP(N,M)- THETA DERIVATIVE OF P(N,M) (UNNORMALIZED)
snorm[13][13],  //!<       SNORM  - SCHMIDT NORMALIZATION FACTORS
sp[13],         //!<       SP(M)  - SINE OF (M*SPHERICAL COORD. LONGITUDE)
cp[13],         //!<       CP(M)  - COSINE OF (M*SPHERICAL COORD. LONGITUDE)
fn[13], fm[13],
pp[13],         //!<       PP(N)  - ASSOCIATED LEGENDRE POLYNOMIALS FOR M=1 (UNNORMALIZED)
k[13][13],
epoch;
//		 otime,          //!<       OTIME  - TIME ON PREVIOUS CALL TO GEOMAG (YRS)
//		 oalt,           //!<       OALT   - GEODETIC ALTITUDE ON PREVIOUS CALL TO GEOMAG (YRS)
//	     olat,           //!<       OLAT   - GEODETIC LATITUDE ON PREVIOUS CALL TO GEOMAG (DEG.)
//		 olon;           //!<       OLON   - GEODETIC LONGITUDE ON PREVIOUS CALL TO GEOMAG (DEG.)

static int maxord;   //!<       MAXORD - MAXIMUM ORDER OF SPHERICAL HARMONIC MODEL

/*************************************************************************/

static void geoMagInit(void)
{
    int n,m,i,j;
    double flnmj,n2m1,nm12,n2m1stuff;

    /* Only do this once */
    if(started) return;
    started = 1;

    // INITIALIZE CONSTANTS
    maxord = MAXDEG;
    sp[0] = 0.0;
    cp[0] = p[0][0] = pp[0] = 1.0;
    dp[0][0] = 0.0;

    // READ WORLD MAGNETIC MODEL SPHERICAL HARMONIC COEFFICIENTS
    c[0][0]  = 0.0;
    cd[0][0] = 0.0;

    // Load the WMM Coefficients
    epoch=EPOCH;
    for( i=0;i<90;i++)
    {
        n=wmmData[i].n;
        m=wmmData[i].m;
        if (m <= n)
        {
            c[m][n] = wmmData[i].gnm;
            cd[m][n] = wmmData[i].dgnm;
            if (m != 0)
            {
                c[n][m-1] = wmmData[i].hnm;
                cd[n][m-1] = wmmData[i].dhnm;
            }
        }
    } //end for


    /* CONVERT SCHMIDT NORMALIZED GAUSS COEFFICIENTS TO UNNORMALIZED */

    snorm[0][0] = 1.0;
    for (n=1; n<=maxord; n++)
    {
        nm12 = (n-1)*(n-1);
        n2m1 = (double)2*n-1;
        n2m1stuff = (double)(n2m1*(2*n-3));
        snorm[0][n] = snorm[0][n-1]*n2m1/(double)n;
        j = 2;
        for (m=0;m<=n;m++)
        {
            k[m][n] = (double)(nm12-(m*m))/n2m1stuff;
            if (m > 0)
            {
                flnmj = (double)((n-m+1)*j)/(double)(n+m);
                snorm[m][n] = snorm[m-1][n]*sqrt(flnmj);
                j = 1;
                c[n][m-1] = snorm[m][n]*c[n][m-1];
                cd[n][m-1] = snorm[m][n]*cd[n][m-1];
            }
            c[m][n] = snorm[m][n]*c[m][n];
            cd[m][n] = snorm[m][n]*cd[m][n];
        } //end for m
        fn[n] = (double)(n+1);
        fm[n] = (double)n;
    } //end for n
    k[1][1] = 0.0;


}



// Main GeoMag routine
/*! \brief This routine computes all of the relevant geomagnetic data

\param double alt    : in: altitude in meters
\param double glat   : in: latitude in decimal degrees
\param double glon   : in: longitude in decimal degrees
\param double time   : in: time in decimal years
\param double *dec   :out: declination in degrees
\param double *dip   :out: dip in degrees
\param double *ti    :out: total intensity in nT
\param double *gv    :out: geomagnetic grid variation
\param double *adec  :out: Annual declination in degrees
\param double *adip  :out: Annual dip in degrees
\param double *ati   :out: Annual total intensity in nT
\param double *x     :out: X Component of the magnetic field
\param double *y     :out: Y Component of the magnetic field
\param double *z     :out: Z Component of the magnetic field
\param double *h     :out: h Component of the magnetic field
\param double *ax    :out: Annual X Component of the magnetic field
\param double *ay    :out: Annual Y Component of the magnetic field
\param double *az    :out: Annual Z Component of the magnetic field
\param double *ah    :out: Annual H Component of the magnetic field
\retval GEO_OK on success
\retval GEO_ERROR on error

{
  */
int DLL_CALLCONV geoMag( double alt,    double glat,  double glon,  double time,
double *dec,  double *dip,  double *ti,   double *gv,
double *adec, double *adip, double *ati,
double *x,    double *y,    double *z,    double *h,
double *ax,   double *ay,   double *az,   double *ah)
{
    double time1;
    double dec2, dip2, ti2, rdec, rdip, rdec2, rdip2;
    double x2,y2,z2,h2;
    double c_rdip,c_rdip2;
    //    double rTd=0.017453292;

    geoMagInit();

    geomg1(alt,glat,glon,time,dec,dip,ti,gv);
    time1 = time + 1.0; // add a year to time
    geomg1(alt,glat,glon,time1,&dec2,&dip2,&ti2,gv);



    /*COMPUTE X, Y, Z, AND H COMPONENTS OF THE MAGNETIC FIELD*/
    rdec = *dec  * DEG_TO_RAD;
    rdip = *dip  * DEG_TO_RAD;
    rdec2 = dec2 * DEG_TO_RAD;
    rdip2 = dip2 * DEG_TO_RAD;
    c_rdip  = cos(rdip);
    c_rdip2 = cos(rdip2);
    /*
      *x= *ti * (cos(rdec) * cos(rdip));
      *y= *ti * (cos(rdip) * sin(rdec));
      *z= *ti * (sin(rdip));
      *h= *ti * (cos(rdip));

      x2=ti2*(cos(rdec2) * cos(rdip2));
      y2=ti2*(cos(rdip2) * sin(rdec2));
      z2=ti2*(sin(rdip2));
      h2=ti2*(cos(rdip2));
*/
    *x= *ti * (cos(rdec) * c_rdip);
    *y= *ti * (c_rdip    * sin(rdec));
    *z= *ti * (sin(rdip));
    *h= *ti * (c_rdip);

    x2=ti2*(cos(rdec2) * c_rdip2);
    y2=ti2*(c_rdip2    * sin(rdec2));
    z2=ti2*(sin(rdip2));
    h2=ti2*(c_rdip2);


    /*  COMPUTE ANNUAL CHANGE FOR TOTAL INTENSITY  */
    *ati = ti2 - *ti;

    /*  COMPUTE ANNUAL CHANGE FOR DIP & DEC (in minutes) */
    *adip = (dip2 - *dip) * DEG_TO_MIN;
    *adec = (dec2 - *dec) * DEG_TO_MIN;


    /*  COMPUTE ANNUAL CHANGE FOR X, Y, Z, AND H */
    *ax  = x2 - *x;
    *ay  = y2 - *y;
    *az  = z2 - *z;
    *ah  = h2 - *h;

    return(GEO_OK);
}





/*************************************************************************/

int DLL_CALLCONV geomg1( double  alt, double glat, double glon, double time,
double *dec, double *dip, double *ti,  double *gv)
{

    /*!
This is documentation that came with the WMM. It is included here for completeness.
\verbatim
//       MAXDEG - MAXIMUM DEGREE OF SPHERICAL HARMONIC MODEL    (INPUT)
//       TIME   - COMPUTATION TIME (YRS)                        (INPUT)
//                (EG. 1 JULY 1995 = 1995.500)
//       ALT    - GEODETIC ALTITUDE (M)                        (INPUT)
//       GLAT   - GEODETIC LATITUDE (DEG.)                      (INPUT)
//       GLON   - GEODETIC LONGITUDE (DEG.)                     (INPUT)
//       EPOCH  - BASE TIME OF GEOMAGNETIC MODEL (YRS)
//       P(N,M) - ASSOCIATED LEGENDRE POLYNOMIALS (UNNORMALIZED)
//       DEC    - GEOMAGNETIC DECLINATION (DEG.)                (OUTPUT)
//                  EAST=POSITIVE ANGLES
//                  WEST=NEGATIVE ANGLES
//       DIP    - GEOMAGNETIC INCLINATION (DEG.)                (OUTPUT)
//                  DOWN=POSITIVE ANGLES
//                    UP=NEGATIVE ANGLES
//       TI     - GEOMAGNETIC TOTAL INTENSITY (NT)              (OUTPUT)
//       GV     - GEOMAGNETIC GRID VARIATION (DEG.)             (OUTPUT)
//                REFERENCED TO GRID NORTH
//                GRID NORTH REFERENCED TO 0 MERIDIAN
//                OF A POLAR STEREOGRAPHIC PROJECTION
//                (ARCTIC/ANTARCTIC ONLY)
//       A      - SEMIMAJOR AXIS OF WGS-84 ELLIPSOID (KM)
//       B      - SEMIMINOR AXIS OF WGS-84 ELLIPSOID (KM)
//       RE     - MEAN RADIUS OF IAU-66 ELLIPSOID (KM)
//       a2,             // a * a
//	 b2,             // b * b
//	 c2,             // a2 - b2
//	 a4,             // a2 * a2
//	 b4,             // b2 * b2
//	 c4,             // a4 - b4
//	 ct,             //C       CT     - COSINE OF (SPHERICAL COORD. LATITUDE)
//	 st,             //C       ST     - SINE OF (SPHERICAL COORD. LATITUDE)
//	 r2,
//	 r,              //C       R      - SPHERICAL COORDINATE RADIAL POSITION (KM)
//	 ca,             //C       CA     - COSINE OF SPHERICAL TO GEODETIC VECTOR ROTATION ANGLE
//	 sa,             //C       SA     - SINE OF SPHERICAL TO GEODETIC VECTOR ROTATION ANGLE
//	 br,             //C       BR     - RADIAL COMPONENT OF GEOMAGNETIC FIELD (NT)
//	 bt,             //C       BT     - THETA COMPONENT OF GEOMAGNETIC FIELD (NT)
//	 bp,             //C       BP     - PHI COMPONENT OF GEOMAGNETIC FIELD (NT)
//	 bx,             //C       BX     - NORTH GEOMAGNETIC COMPONENT (NT)
//	 by,             //C       BY     - EAST GEOMAGNETIC COMPONENT (NT)
//	 bz,             //C       BZ     - VERTICALLY DOWN GEOMAGNETIC COMPONENT (NT)
//       bh;             //C       BH     - HORIZONTAL GEOMAGNETIC COMPONENT (NT)
\endverbatim
*/

    int m,n;
    double temp1, temp2,d,ar,aor,dt,bpp,
    par, parp,r,r2,sa,ca,st,ct,br,bt,bp,bx,by,bz,bh;

    double		 rlon,rlat,
    srlon, srlat, crlon, crlat,
    srlat2, crlat2,
    a,              //       A      - SEMIMAJOR AXIS OF WGS-84 ELLIPSOID (KM)
    b,              //       B      - SEMIMINOR AXIS OF WGS-84 ELLIPSOID (KM)
    re,             //       RE     - MEAN RADIUS OF IAU-66 ELLIPSOID (KM)
    a2,             //   a * a
    b2,             //   b * b
    c2,             //   a2 - b2
    a4,             //   a2 * a2
    b4,             //   b2 * b2
    c4,             //   a4 - b4

    q, q1, q2;

    a = ellips[GEO_DATUM_WE].a; //6378137.0;
    b = GEO_B(ellips[GEO_DATUM_WE].a, (ellips[GEO_DATUM_WE].f1)); //6356752.3142; //wgs-84
    re = 6371200.0;
    a2 = a*a;
    b2 = b*b;
    c2 = a2-b2;
    a4 = a2*a2;
    b4 = b2*b2;
    c4 = a4 - b4;


    dt = time - epoch;
    //     if (dt < 0.0 || dt > 5.0)
    //			printf("\n WARNING geoMag - TIME EXTENDS BEYOND MODEL 5-YEAR LIFE SPAN (dt=%f, time=%f)\n",dt,time);

    rlon   = glon*DEG_TO_RAD;
    rlat   = glat*DEG_TO_RAD;
    srlon  = sin(rlon);
    srlat  = sin(rlat);
    crlon  = cos(rlon);
    crlat  = cos(rlat);
    srlat2 = srlat*srlat;
    crlat2 = crlat*crlat;
    sp[1]  = srlon;
    cp[1]  = crlon;

    /* CONVERT FROM GEODETIC COORDS. TO SPHERICAL COORDS. */

    q = sqrt(a2-c2*srlat2);
    q1 = alt*q;
    q2 = ((q1+a2)/(q1+b2))*((q1+a2)/(q1+b2));
    ct = srlat/sqrt(q2*crlat2+srlat2);
    st = sqrt(1.0-(ct*ct));
    r2 = (alt*alt)+2.0*q1+(a4-c4*srlat2)/(q*q);
    r = sqrt(r2);
    d = sqrt(a2*crlat2+b2*srlat2);
    ca = (alt+d)/r;
    sa = c2*crlat*srlat/(r*d);

    for (m=2; m<=maxord; m++)
    {
        sp[m] = sp[1]*cp[m-1]+cp[1]*sp[m-1];
        cp[m] = cp[1]*cp[m-1]-sp[1]*sp[m-1];
    }

    aor = re/r;
    ar = aor*aor;
    br = bt = bp = bpp = 0.0;
    for (n=1; n<=maxord; n++)
    {
        ar = ar*aor;
        for (m=0;m<=n;m++)
        {
            /*
        COMPUTE UNNORMALIZED ASSOCIATED LEGENDRE POLYNOMIALS
        AND DERIVATIVES VIA RECURSION RELATIONS
     */
            if (n == m)
            {
                p[m][n]  = st * p[m-1][n-1];
                dp[m][n] = st * dp[m-1][n-1] + ct * p[m-1][n-1];
                goto S50;
            }
            if (n == 1 && m == 0)
            {
                p[m][n]  = ct * p[m][n-1];
                dp[m][n] = ct * dp[m][n-1] - st * p[m][n-1];
                goto S50;
            }
            if (n > 1 && n != m)
            {
                if (m > n-2) p[m][n-2]  = 0.0;
                if (m > n-2) dp[m][n-2] = 0.0;
                p[m][n]  = ct * p[m][n-1]  - k[m][n] * p[m][n-2];
                dp[m][n] = ct * dp[m][n-1] - st * p[m][n-1] - k[m][n] * dp[m][n-2];
            }
S50:
            //	TIME ADJUST THE GAUSS COEFFICIENTS
            tc[m][n] = c[m][n]+dt*cd[m][n];
            if (m != 0) tc[n][m-1] = c[n][m-1]+dt*cd[n][m-1];

            // ACCUMULATE TERMS OF THE SPHERICAL HARMONIC EXPANSIONS
            par = ar * p[m][n];
            if (m == 0)
            {
                temp1 = tc[m][n]*cp[m];
                temp2 = tc[m][n]*sp[m];
            }
            else
            {
                temp1 = tc[m][n]*cp[m]+tc[n][m-1]*sp[m];
                temp2 = tc[m][n]*sp[m]-tc[n][m-1]*cp[m];
            }
            bt = bt-ar*temp1*dp[m][n];
            bp += (fm[m]*temp2*par);
            br += (fn[n]*temp1*par);

            //	SPECIAL CASE:  NORTH/SOUTH GEOGRAPHIC POLES
            if (st == 0.0 && m == 1)
            {
                if (n == 1) pp[n] = pp[n-1];
                else pp[n] = ct*pp[n-1]-k[m][n]*pp[n-2];
                parp = ar*pp[n];
                bpp += (fm[m]*temp2*parp);
            }
        }
    }
    if (st == 0.0) bp = bpp;
    else bp /= st;
    /*
    ROTATE MAGNETIC VECTOR COMPONENTS FROM SPHERICAL TO
    GEODETIC COORDINATES
*/
    bx = -bt*ca-br*sa;
    by = bp;
    bz = bt*sa-br*ca;
    /*
**    COMPUTE DECLINATION (DEC), INCLINATION (DIP) AND
**    TOTAL INTENSITY (TI)
*/
    bh   = sqrt((bx*bx)+(by*by));
    *ti  = sqrt((bh*bh)+(bz*bz));
    *dec = atan2(by,bx)*RAD_TO_DEG;
    *dip = atan2(bz,bh)*RAD_TO_DEG;

    /*
**    COMPUTE MAGNETIC GRID VARIATION IF THE CURRENT
**    GEODETIC POSITION IS IN THE ARCTIC OR ANTARCTIC
**    (I.E. GLAT > +55 DEGREES OR GLAT < -55 DEGREES)
**
**    OTHERWISE, SET MAGNETIC GRID VARIATION TO -999.0
*/
    *gv = -999.0;
    if (fabs(glat) >= 55.)
    {
        if (glat > 0.0 && glon >= 0.0)  *gv = *dec-glon;
        if (glat > 0.0 && glon < 0.0)   *gv = *dec+fabs(glon);
        if (glat < 0.0 && glon >= 0.0)  *gv = *dec+glon;
        if (glat < 0.0 && glon < 0.0)   *gv = *dec-fabs(glon);
        if (*gv > +HALF_CIRCLE) *gv -= CIRCLE;
        if (*gv < -HALF_CIRCLE) *gv += CIRCLE;
    }

    return (GEO_OK);

}

/*************************************************************************/
// Short GeoMag routine
/*! \brief This routine computes magnetic declination

\param double lat   : in: latitude in decimal degrees
\param double lon   : in: longitude in decimal degrees
\param double hgt   : in: altitude in meters
\param int    month : in: Month of the year
\param int    day   : in: Day of the month
\param int    year  : in: Year
\param double dec   :out: declination in degrees
*/
int DLL_CALLCONV geoMagGetDec(double lat, double lon, double hgt,
int month, int day, int year, double *dec)
{
    /* Days in each month 1-12 */
    int jday[] = {0,0,31,59,90,120,151,180,211,242,272,303,333 };
    double time,dip,ti,gv;

    /* Determin decimal years from date */
    time = ((jday[month]+day) / 365.0) + year;

    /* Determin declination */
    geoMagInit();
    geomg1(hgt,lat,lon,time,dec,&dip,&ti,&gv);

    return (GEO_OK);

}

/*************************************************************************/
// Short GeoMag routine
/*! \brief This routine computes and returns magnetic declination for a specific date

\param double lat   : in: latitude in decimal degrees
\param double lon   : in: longitude in decimal degrees
\param double hgt   : in: altitude in meters
\param int    month : in: Month of the year
\param int    day   : in: Day of the month
\param int    year  : in: Year
\retval double dec   :out: declination in degrees
*/
double DLL_CALLCONV geoMagGetDecRet(double lat, double lon, double hgt,
int month, int day, int year)
{
    /* Days in each month 1-12 */
    double dec;
    int jday[] = {0,0,31,59,90,120,151,180,211,242,272,303,333 };
    double time,dip,ti,gv;

    /* Determin decimal years from date */
    time = ((jday[month]+day) / 365.0) + year;

    /* Determin declination */
    geoMagInit();
    geomg1(hgt,lat,lon,time,&dec,&dip,&ti,&gv);

    return (dec);

}
/*************************************************************************/
// Short GeoMag routine
/*! \brief This routine computes magnetic declination for now time

\param double lat   : in: latitude in decimal degrees
\param double lon   : in: longitude in decimal degrees
\param double hgt   : in: altitude in meters
\retval double dec   :out: declination in degrees
*/
double DLL_CALLCONV geoMagGetDecNow(double lat, double lon, double hgt)
{
    struct tm *newtime;
    time_t aclock;
    double dec;

    /* Compute the Magnetic Declination */
    time( &aclock );                 /* Get time in seconds */
    newtime = localtime( &aclock );  /* Convert time to struct tm form */

    if(newtime->tm_year < 200 ) newtime->tm_year += 1900;

    //   printf("%d / %d / %d \n",newtime->tm_mon+1, newtime->tm_mday, newtime->tm_year);
    geoMagGetDec(lat,lon,hgt,
    newtime->tm_mon+1, newtime->tm_mday, newtime->tm_year, &dec);

    return (dec);

}

/* Fill the geo struct with the magnetic declination using the computer's time/date */
int DLL_CALLCONV geoMagFillDec(GEO_LOCATION *l, double *dec)
{
    struct tm *newtime;
    time_t aclock;

    /* Compute the Magnetic Declination */
    time( &aclock );                 /* Get time in seconds */
    newtime = localtime( &aclock );  /* Convert time to struct tm form */

    if(newtime->tm_year < 200 ) newtime->tm_year += 1900;

    //   printf("%d / %d / %d \n",newtime->tm_mon+1, newtime->tm_mday, newtime->tm_year);
    geoMagGetDec(l->lat,l->lon,l->hgt,
    newtime->tm_mon+1, newtime->tm_mday, newtime->tm_year, dec);
    l->Declination = *dec;

    return (GEO_OK);
}

// Main test routine
#ifdef GEOMAG_MAIN
void main(void)
{
    int i;
    double dlat, dlon;
    double ati, adec, adip;
    double alt, gv;
    double time, dec, dip, ti;
    double x,y,z,h;
    double ax,ay,az,ah;


    dlat =   40.0;
    dlon = -105.0;
    alt  = 1600.0;
    time = EPOCH;

    // use the geoMag routine to get all of the specific values
    geoMag(alt,dlat,dlon,time,&dec,&dip,&ti,&gv,&adec,&adip,&ati,
    &x, &y, &z, &h, &ax, &ay, &az, &ah);


    printf("\nLATITUDE: %7.2f Deg",dlat);
    printf(  "  LONGITUDE: %7.2f Deg",dlon);
    printf(  "  ALTITUDE: %.2f M",alt);
    printf("\n DATE : %5.1f\n",time);
    printf("\n\t\t\t      OUTPUT\n\t\t\t      ------");
    printf("\n\nMAIN FIELD COMPONENTS\t\t\t   ANNUAL CHANGE");
    printf("\n---------------------\t\t\t   -------------\n");
    printf("\n TI          = %-7.0f nT\t\t   TI  = %-6.0f  nT/yr",ti,ati);
    printf("\n HI          = %-7.0f nT\t\t   HI  = %-6.0f  nT/yr",h,ah);
    printf("\n X           = %-7.0f nT\t\t   X   = %-6.0f  nT/yr ",x,ax);
    printf("\n Y           = %-7.0f nT\t\t   Y   = %-6.0f  nT/yr ",y,ay);
    printf("\n Z           = %-7.0f nT\t\t   Z   = %-6.0f  nT/yr ",z,az);
    printf("\n DEC         = %-7.2f DEG\t\t   DEC = %-6.2f MIN/yr [%-6.2f deg/yr]  ",dec,adec,adec/60.0);
    printf("\n DIP         = %-7.2f DEG\t\t   DIP = %-6.2f  MIN/yr\n",dip,adip);


    // Print the monthly declination values
    printf("----------------------------------------------------------\n");
    printf(" Yearly Declination Values \n");
    printf("----------------------------------------------------------\n");
    for(i=0;i<=4;i++)
    {
        geoMagGetDec(dlat,dlon,alt,1,1,2000+i, &dec);
        printf("%04d : %f deg\n",i+2000,dec);
    }

    // Print the simple declination values for a location and time
    printf("\n\n----------------------------------------------------------\n");
    printf(" Lat, Lon, Hgt, and time (MM/DD/YY) declination example \n");
    printf("----------------------------------------------------------\n");

    geoMagGetDec(dlat, dlon, alt, 6, 1, 2000, &dec);
    printf(" Declination for %7.1f latitude, %5.1f longitude, %5.1f altitude \n is %4.2f degrees on 6/1/2000\n",
    dlat, dlon, alt, dec);

}

#endif
/*************************************************************************/
