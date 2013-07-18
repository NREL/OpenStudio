/**********************************************************************
 *  Copyright (c) 2013, The Pennsylvania State University.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/
#ifndef PRJOBJECTS_H
#define PRJOBJECTS_H

/*
 *  Section                Title                      Status
 *  -------   --------------------------------   ----------------
 *     1                Run Controls                 Supported
 *     2          Species and Contaminants           Supported
 *     3             Level and Icon Data             Supported
 *     4               Day Schedules                 Supported
 *     5               Week Schedules                Supported
 *     6           Wind Pressure Profiles            Supported
 *     7              Kinetic Reactions            Not supported
 *    8a               Filter Elements                 Broken
 *    8b                   Filters                   Supported
 *     9            Source/Sink Elements         Partially tested
 *    10              Airflow Elements           Partially tested
 *    11                Duct Elements              Not supported
 *    12a          Control Super Elements          Not supported
 *    12b               Control Nodes              Not supported
 *    13          Simple Air Handling System         Supported
 *    14                    Zones                    Supported
 *    15         Initial Zone Concentrations         Supported
 *    16                Airflow Paths                Supported
 *    17               Duct Junctions              Not supported
 *    18      Initial Junction Concentrations      Not supported
 *    19                Duct Segments              Not supported
 *    20                Source/Sinks                 Supported
 *    21             Occupancy Schedules           Not supported
 *    22                  Exposures                Not supported
 *    23                 Annotations               Not supported
 */

#include "PrjReader.hpp"

#include <QVector>

#define RX_IS_STRING
#define RX QString
#define RX_C(v) v
#define RX_I(v) QString(#v)

CONTAMNAMESPACESTART
namespace prj
{

// Section 1: Project, Weather, Simulation, and Output Controls

class Weather
{
public:
    Weather();
    void read(Reader *input);
    QString write();
    RX Tambt;  // ambient temperature [K] (R4)
    RX barpres;  // barometric pressure [Pa] NOT corrected to sea level (R4)
    RX windspd;  // wind speed [m/s] (R4)
    RX winddir;  // wind direction: 0 = N, 90 = E, 180 = S, ...; (R4)
    RX relhum;  // relative humidity: 0.0 to 1.0 (R4)
    int daytyp;  // type of day (1-12) (I2)
    int uTa;  // units for Tambt (I2) {W}
    int ubP;  // units for barpres (I2) {W}
    int uws;  // units for windspd (I2) {W}
    int uwd;  // units for winddir (I2) {W}
};

class RunControl
{
public:
    RunControl();
    void read(Reader *input);
    QString write();
    QString name;  /* program name "ContamW" (I1) */
    QString version;  /* program version "3.1" (I1) */
    int echo;  /* (0/1) echo input files to the log file (I2) */
    QString prjdesc;  /* (unused) project description (I1) {W} */
    int skheight;  /* total SketchPad height [cells] (I2) {W} */
    int skwidth;  /* total SketchPad width [cells] (I2) {W} */
    int def_units;  /* default units: 0 = SI, 1 = US (I2) {W} */
    int def_flows;  /* default flows: 0 = mass, 1 = volume (I2) {W} */
    RX def_T;  /* default temperature for zones [K] (R4) {W} */
    int udefT;  /* units for temperature (I2) {W} */
    RX rel_N;  /* angle to true north [degrees] (R4) {W} */
    RX wind_H;  /* elevation for reference wind speed [m] (R4) {W} */
    int uwH;  /* units for _wind_H (I2) {W} */
    RX wind_Ao;  /* local terrain constant (R4) {W} */
    RX wind_a;  /* velocity profile exponent (R4) {W} */
    RX scale;  /* cell scaling factor (R4) */
    int uScale;  /* units of scaling factor: 0 = m, 1 = ft (I2) */
    int orgRow;  /* row of origin (I2) */
    int orgCol;  /* column of origin (I2) */
    int invYaxis;  /* 0 = positive above origin, 1 = negative above origin (I2) */
    int showGeom;  /* 0/1 = show pseudo-geometry info in status bar (I2) */
    Weather ssWeather;
    Weather wptWeather;
    QString WTHpath;  /* full name of weather file (I1) */
    QString CTMpath;  /* full name of contaminant file (I1) */
    QString CVFpath;  /* full name of continuous values file (I1) */
    QString DVFpath;  /* full name of discrete values file (I1) */
    QString WPCfile;  /* full name of WPC file (I1) */
    QString EWCfile;  /* full name of EWC data source file (I1) {W} */
    QString WPCdesc;  /* WPC description (I1) {W} */
    RX X0;  /* X-value of ContamW origin in EWC coordinates [m] (R4) {W} */
    RX Y0;  /* Y-value of ContamW origin in EWC coordinates [m] (R4) {W} */
    RX Z0;  /* Z-value of ContamW origin in EWC coordinates [m] (R4) {W} */
    RX angle;  /* Rotation of ContamW relative to EWC coordinates (R4) {W} */
    int u_XYZ;  /* units of coordinates (I2) {W} */
    RX epsPath;  /* tolerance for matching path locations [-] (R4) {W} */
    RX epsSpcs;  /* tolerance for matching species [-] (R4) {W} */
    QString tShift;  /* time shift of EWC data {W} [s] (hh:mm:ss) {W} */
    QString dStart;  /* date WPC data starts (I1) {W} */
    QString dEnd;  /* date WPC data ends (I1) {W} */
    int useWPCwp;  /* if true, use WPC file wind pressures (I2) */
    int useWPCmf;  /* if true, use WPC file mass fractions (I2) */
    int wpctrig;  /* 0/1 = trigger airflow calculation by WPC data (I2) */
    RX latd;  /* latitude (degrees: north +, south -) (R4) */
    RX lgtd;  /* longitude (degrees: east +, west -) (R4) */
    RX Tznr;  /* time zone (Greenwich = 0, Eastern = -5, etc.) (R4) */
    RX altd;  /* elevation above sea level [m] (R4) */
    RX Tgrnd;  /* ground temperature [K] (R4) */
    int utg;  /* units for ground temperatures (I2) */
    int u_a;  /* units for elevation (I2) */
    int sim_af;  /* airflow simulation: 0 = steady, 1 = dynamic (I2) */
    int afcalc;  /* N-R method for non-linear eqns: 0 = SUR, 1 = STR (I2) */
    int afmaxi;  /* maximum number of N-R iterations (I2) */
    RX afrcnvg;  /* relative airflow convergence factor (R4) */
    RX afacnvg;  /* absolute airflow convergence factor [1/s] (R4) */
    RX afrelax;  /* flow under-relaxation coefficient (for SUR) (R4) */
    int uac2;  /* units for afacnvg (I2) */
    RX Pres;  /* pressure test pressure (R4) {Contam 2.4} */
    int uPres;  /* units of Pres (I2) {Contam 2.4} */
    int afslae;  /* method for linear equations: 0 = SKY, 1 = PCG (I2) */
    int afrseq;  /* if true, resequence the linear equations (I2) */
    int aflmaxi;  /* maximum number of iterations (PCG) (I2) */
    RX aflcnvg;  /* relative convergence factor for (PCG) (R4) */
    int aflinit;  /* if true, do linear airflow initialization (I2) */
    int Tadj;  /* if true, use temperature adjustment (I2) */
    int sim_mf;  /* mass fraction (contaminant) simulation: 0 = none, 1 = steady, 2 = transient, 3 = cyclic (I2) */
    int ccmaxi;  /* simulation: maximum number of cyclic iterations (I2) */
    RX ccrcnvg;  /* relative convergence factor (R4) */
    RX ccacnvg;  /* absolute convergence factor [kg/kg] (R4) */
    RX ccrelax;  /* (unused) over-relaxation coefficient (R4) */
    int uccc;  /* units for ccacnvg (I2) */
    int mfnmthd;  /* simulation: 0 = SKY, 1 = BCG, 2 = SOR, 3 = LU (I2) */
    int mfnrseq;  /* if true, resequence the linear equations (I2) */
    int mfnmaxi;  /* maximum iterations (I2) */
    RX mfnrcnvg;  /* desired relative convergence (R4) */
    RX mfnacnvg;  /* desired absolute convergence (R4) */
    RX mfnrelax;  /* relaxation coefficient (R4) */
    RX mfngamma;  /* trapezoidal integration factor (R4) */
    int uccn;  /* units for mfnacnvg (I2) */
    int mftmthd;  /* 0 = SKY, 1 = BCG, 2 = SOR, 3 = LU (I2) */
    int mftrseq;  /* if true, resequence the linear equations (I2) */
    int mftmaxi;  /* maximum iterations (I2) */
    RX mftrcnvg;  /* desired relative convergence (R4) */
    RX mftacnvg;  /* desired absolute convergence (R4) */
    RX mftrelax;  /* relaxation coefficient (R4) */
    RX mftgamma;  /* trapezoidal integration factor (R4) */
    int ucct;  /* units for mftacnvg (I2) */
    int mfvmthd;  /* 0 = SKY, 2 = SOR, 3 = LU (1 n/a) (I2) */
    int mfvrseq;  /* if true, resequence the linear equations (I2) */
    int mfvmaxi;  /* maximum iterations (I2) */
    RX mfvrcnvg;  /* desired relative convergence (R4) */
    RX mfvacnvg;  /* desired absolute convergence (R4) */
    RX mfvrelax;  /* relaxation coefficient (R4) */
    int uccv;  /* units for mfvacnvg (I2) */
    int mf_solver;  /* mass fraction integration method: {3.1} 0=trapezoid, 1=STS, 2=CVODE (I2) */
    int sim_1dz;  /* if true, use 1D zones (I2) */
    int sim_1dd;  /* if true, use 1D ducts (I2) */
    RX celldx;  /* default length of duct cells for C-D model [m] (R4) */
    int sim_vjt;  /* if true, compute variable junction temperatures (I2) */
    int udx;  /* units of celldx (I2) */
    int cvode_mth;  /* 0 = am, 1 = bdf (I2) */
    RX cvode_rcnvg;  /* relative convergence factor(R4) */
    RX cvode_acnvg;  /* absolute convergence factor(R4) */
    RX cvode_dtmax;  /* maximum time step (R4) */
    int tsdens;  /* (0/1) vary density during time step (I2) */
    RX tsrelax;  /* (inactive) under-relaxation factor for calculating dM/dt (R4) */
    int tsmaxi;  /* maximum number of iterations for density changes (I2) */
    int cnvgSS;  /* (0/1) converge density in steady state init (I1) {2.4b} */
    int densZP;  /* (0/1) density = f(zone pressure) (I1) {2.4b, 3.1} */
    int stackD;  /* (0/1) density varies hydrostatically (I1) {2.4b, 3.1} */
    int dodMdt;  /* (0/1) include dM/dt in airflow calc (I1) {2.4b, 3.1} */
    QString date_st;  /* day-of-year to start steady simulation (mmmdd) */
    QString time_st;  /* time-of-day to start steady simulation (hh:mm:ss) */
    QString date_0;  /* day-of-year to start transient simulation (mmmdd) */
    QString time_0;  /* time-of-day to start transient simulation (hh:mm:ss) */
    QString date_1;  /* day-of-year to end transient simulation (mmmdd) */
    QString time_1;  /* time-of-day to end transient simulation (hh:mm:ss) */
    QString time_step;  /* simulation time step [s] (hh:mm:ss) */
    QString time_list;  /* simulation output (results) time step [s] (hh:mm:ss) */
    QString time_scrn;  /* simulation status time step [s] (up to 1 day) (hh:mm:ss) */
    int restart;  /* use restart file (I2) */
    QString rstdate;  /* restart date (mmmdd) */
    QString rsttime;  /* restart time (hh:mm:ss) */
    int list;  /* data dump parameter (I2) > 0 dump matrix analysis, = 2 dump SIM file output, > 2 dump lognotes. */
    int doDlg;  /* (0/1) ContamX will display dialog box (I1) */
    int pfsave;  /* (0/1) save path flow results to SIM file (I1) */
    int zfsave;  /* (0/1) save zone flow results to SIM file (I1) */
    int zcsave;  /* (0/1) save mass fraction results to SIM file (I1) */
    int achvol;  /* (0/1) ACH based on true volumes instead of std volumes (I1) */
    int achsave;  /* (0/1) save building air exchange rate transient data (I1) */
    int abwsave;  /* (0/1) save air exchange rate box-whisker data (I1) */
    int cbwsave;  /* (0/1) save contaminant box-whisker data (I1) */
    int expsave;  /* (0/1) save exposure transient data (I1) */
    int ebwsave;  /* (0/1) save exposure box-whisker data (I1) */
    int zaasave;  /* (0/1) save zones age-of-air transient data (I1) */
    int zbwsave;  /* (0/1) save zones age-of-air box-whisker data (I1) */
    int rzfsave;  /* (0/1) save binary zone flow file (I1) */
    int rzmsave;  /* (0/1) save binary avg zone mass fraction file (I1) */
    int rz1save;  /* (0/1) save binary 1D zone cell mass fraction file (I1) */
    int csmsave;  /* (0/1) save text contaminant summary file (I1) */
    int srfsave;  /* (0/1) save text surface result file (I1) */
    int logsave;  /* (0/1) save text controls log file (I1) */
    int save[16];  /* (unused by CONTAM; subject to change without notice) (I1) */
    //int nrvals;  /* number of real values in the vector (I2) */
    QVector<RX> rvals;  /* real values (R4) */
    int BldgFlowZ;  /* output building airflow test (zones) (IX) */
    int BldgFlowD;  /* output building airflow test (ducts) (IX) */
    int BldgFlowC;  /* output building airflow test (classified flows) (IX) */
    int cfd_ctype;  /* (0=no cfd, 1=post, 2=quasi, 3=dynamic) cfd coupling method (I2) */
    RX cfd_convcpl;  /* convergence factor for dynamic coupling (R4) */
    int cfd_var;  /* (0/1) use .var file (I2) */
    int cfd_zref;  /* currently not used (I2) */
    int cfd_imax;  /* max number of dynamic coupling iterations (I2) */
    int cfd_dtcmo;  /* number of iterations between outputs to .cmo file (I2) */
};

// Section 2: Species and Contaminants

class Species
{
public:
    Species();
    void read(Reader *input);
    QString write();
    int nr;  // species number (IX), in order from 1 to _nspcs
    int sflag;  // 1 = simulated, 0 = unsimulated species (I2) {W}
    int ntflag;  // 1 = non-trace, 0 = trace species (I2) {W}
    RX molwt;  // molar mass [kg/kmol] - gas (R4)
    RX mdiam;  // mean diameter - particle [m] (R4)
    RX edens;  // effective density - particle [kg/m^3] (R4)
    RX decay;  // decay constant [1/s] (R4) {W}
    RX Dm;  // molecular diffusion coefficient [m2/s] (R4)
    RX ccdef;  // default concentration [kg/kg air] (R4)
    RX Cp;  // (unused) specific heat at constant pressure [J/kgK] (R4)
    int ucc;  // units to display concentration (I2) {W}
    int umd;  // units to display mean diameter (I2) {W}
    int ued;  // units to display effective density (I2) {W}
    int udm;  // units to display diffusion coefficient (I2) {W}
    int ucp;  // units to display specific heat (I2) {W}
    QString name;  // species name (CS)
    QString desc;  // species description (CS) {W}
};

// Section 3: Level and Icon Data

class Icon
{
public:
    Icon();
    void read(Reader *input);
    QString write();
    bool isWall();
    uint bits();
    int icon;  /* icon type - see 'special symbols' in contam.h (I2) {W} */
    int row;  /* row position on the SketchPad (I2) {W} */
    int col;  /* column position on the SketchPad (I2) {W} */
    int nr;  /* zone, path, duct, etc., number (I2) {W} */
};

class Level
{
public:
    Level();
    void read(Reader *input);
    QString write();
    int nr;  // level number (IX), in order from 1 to nlev
    RX refht;  // reference elevation of level [m] (R4)
    RX delht;  // delta elevation to next level [m] (R4) {W}
    //int nicon;  // number of icons on this level (IX)
    int u_rfht;  // units of reference elevation (I2) {W}
    int u_dlht;  // units of delta elevation (I2) {W}
    QString name;  // level name (CS)
    QVector<Icon> icons; /* Icons on this level */
};

// Section 4: Day Schedules

class DaySchedule
{
public:
    DaySchedule();
    void read(Reader *input);
    QString write();
    int nr;  // schedule number (IX); in order from 1 to _ndsch
    //int npts;  // number of data points (I2)
    int shape;  // 0 = rectangular; 1 = trapezoidal (I2)
    int utyp;  // type of units (I2) {W}
    int ucnv;  // units conversion (I2) {W}
    QString name;  // schedule name (CS) {W}
    QString desc;  // schedule description (CS) {W} may be blank
    QVector<QString> time;  // time-of-day [s] (hh:mm:ss converted to I4)
    QVector<RX> ctrl;  // corresponding control value (R4) [-]
};

// Section 5: Week Schedules

class WeekSchedule
{
public:
    WeekSchedule();
    void read(Reader *input);
    QString write();
    int nr;  // schedule number (IX); in order from 1 to _nwsch
    int utyp;  // type of units (I2) {W}
    int ucnv;  // units conversion (I2) {W}
    QString name;  // schedule name (CS) {W}
    QString desc;  // schedule description (CS) {W} may be blank
    int j[12];  // 12 day schedule indices (IX) - converted to pointers
};

// Section 6: Wind Pressure Profiles

class WindPressureProfile
{
public:
    WindPressureProfile();
    void read(Reader *input);
    QString write();
    int nr;  // profile number (IX); in order from 1 to _nwpf
    //int npts;  // number of data points (I2)
    int type;  // 1 = linear; 2 = cubic spline; 3 = trigonometric (I2)
    QString name;  // schedule name (CS) {W}
    QString desc;  // profile description (CS) {W} may be blank
    QVector<RX> azm;  // wind azimuth value {R4} [degrees]
    QVector<RX> coef;  // normalized wind pressure coefficients {R4} [-]
};

// Section 7: Kinetic Reactions - Not supported
// Section 8a: Filter Elements

class FilterElement
{
public:
    virtual QString write()=0;
    virtual ~FilterElement(){}
    enum Type {FLE_CEF,FLE_PF0,FLE_GF0,FLE_SPF};
    int nr;  // element number (IX); in order from 1 to _nflte
    QString ftype;  // element data type (string ? I2)
    RX area;  // face area of filter [m] (R4)
    RX depth;  // depth of filter [m] (R4)
    RX dens;  // density of filter material [kg/m^3] (R4)
    int ual;  // units of area and depth (I2) {W}
    int ud;  // units of density (I2) {W}
    QString name;  // element name (CS) {W}
    QString desc;  // filter description (I1) {W} may be blank
    Type type;
    static FilterElement* readElement(Reader *input);
};

class Fle_Cef : public FilterElement
{
public:
    Fle_Cef(int nr=0,QString ftype=QString(),RX area=RX_I(0),RX depth=RX_I(0),RX dens=RX_I(0),
            int ual=0,int ud=0,QString name=QString(),QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    //int nspcs;  // number of species (IX)
    QVector<QString> spcs;  // species name (I1)
    QVector<RX> eff;  // filter efficiency [-] (R4)
};

class Fle_Pf0 : public FilterElement
{
public:
    Fle_Pf0(int nr=0,QString ftype=QString(),RX area=RX_I(0),RX depth=RX_I(0),RX dens=RX_I(0),
            int ual=0,int ud=0,QString name=QString(),QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    //int npts;  // number of data points (I2)
    int usz;  // units of particle size (I2) {W}
    QVector<RX> size;  // particle size [m] (R4)
    QVector<RX> eff;  // filter efficiency [-](R4)
};

class Fle_Gf0 : public FilterElement
{
public:
    Fle_Gf0(int nr=0,QString ftype=QString(),RX area=RX_I(0),RX depth=RX_I(0),RX dens=RX_I(0),
            int ual=0,int ud=0,QString name=QString(),QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    // int nspcs;  // number of species (IX)
    QVector<QString> spcs;  // species name (CS)
    QVector<RX> bthru;  // break through eff (R4) {Contam 2.4}
    // int npts;  // number of data points (I2)
    QVector<QVector<RX> > load;  // relative total loading [-] (R4)
    QVector<QVector<RX> > eff;  // filter efficiency for this species[-] (R4)
};

class Fle_Spf : public FilterElement
{
public:
    Fle_Spf(int nr=0,QString ftype=QString(),RX area=RX_I(0),RX depth=RX_I(0),RX dens=RX_I(0),
            int ual=0,int ud=0,QString name=QString(),QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    //int nselmt;  // number of sub-elements(I2)
    QVector<int> elmt;  // sub-element numbers (IX) – converted to pointers
};

// Section 8b: Filters

class Filter
{
public:
    Filter();
    void read(Reader *input);
    QString write();
    int nr;  // filter number (IX); in order from 1 to _nfilt
    int fe;  // filter element number (IX)
    //int nsub;  // number of sub-elements, 1 if not super filter (IX)
    QVector<RX> load;  // initial relative loading [-] (R4)
    //int nsload;  // number of species loadings (I2) [0]
    //QVector<QString> spcs;  // species name (I1)
    //QVector<RX> load;  // initial relative loading [-] (R4)
};

// Section 9: Source/Sink Elements

class SourceSinkElement
{
public:
    virtual QString write()=0;
    virtual ~SourceSinkElement(){}
    enum Type {CSE_CCF,CSE_PRS,CSE_CUT,CSE_EDS,CSE_BLS,CSE_BRS,CSE_DVS,CSE_DRS,CSE_DVR,CSE_PLM,CSE_PKM,CSE_SUP,UNKNOWN};
    int nr;  // element number (IX); in order from 1 to _ncse
    QString spcs;  // species name (CS)
    QString ctype;  // element data type (string ? I2)
    QString name;  // element name (CS) {W}
    QString desc;  // element description (CS) {W} may be blank
    Type type;
    static Type convertType(QString string);
    static SourceSinkElement* readElement(Reader *input);
};

class Cse_Ccf : public SourceSinkElement
{
public:
    Cse_Ccf(int nr=0,QString spcs=QString(),QString ctype=0,QString name=QString(),QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX G;  // generation rate [kg/s] (R4)
    RX D;  // deposition rate [kg/s] (R4)
    int u_G;  // units of generation (I2) {W}
    int u_D;  // units of deposition (I2) {W}
};

class Cse_Prs : public SourceSinkElement
{
public:
    Cse_Prs(int nr=0,QString spcs=QString(),QString ctype=0,QString name=QString(),QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX G;  // generation rate [kg/s] (R4)
    RX x;  // pressure exponent [-] (R4)
    int u_G;  // units of generation (I2) {W}
};

class Cse_Cut : public SourceSinkElement
{
public:
    Cse_Cut(int nr=0,QString spcs=QString(),QString ctype=0,QString name=QString(),QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX G;  // generation rate [kg/s] (R4)
    RX Co;  // cutoff concentration [kg/kg] (R4)
    int u_G;  // units of generation (I2) {W}
    int u_C;  // units of concentration (I2) {W}
};

class Cse_Eds : public SourceSinkElement
{
public:
    Cse_Eds(int nr=0,QString spcs=QString(),QString ctype=0,QString name=QString(),QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX G0;  // initial generation rate [kg/s] (R4)
    RX k;  // decay constant [1/s] (R4)
    int u_G;  // units of generation (I2) {W}
    int u_k;  // units of time (I2) {W}
};

class Cse_Bls : public SourceSinkElement
{
public:
    Cse_Bls(int nr=0,QString spcs=QString(),QString ctype=0,QString name=QString(),QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX hm;  // average film mass transfer coefficient [m/s] (R4)
    RX rho;  // film density of air [kg/m^3] (R4)
    RX Kp;  // partition coefficient (R4)
    RX M;  // adsorbent mass/unit area [kg/m^2] (R4)
    int u_h;  // units of hm (I2) {W}
    int u_r;  // units of rho (I2) {W}
    int u_M;  // units of M (I2) {W}
};

class Cse_Brs : public SourceSinkElement
{
public:
    Cse_Brs(int nr=0,QString spcs=QString(),QString ctype=0,QString name=QString(),QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX M;  // mass added to zone in one time step [kg] (R4)
    int u_M;  // units of mass (I2) {W}
};

class Cse_Dvs : public SourceSinkElement
{
public:
    Cse_Dvs(int nr=0,QString spcs=QString(),QString ctype=0,QString name=QString(),QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX dV;  // deposition velocity [m/s] (R4)
    RX dA;  // deposition area [m^2] (R4)
    int u_V;  // units of velocity (I2) {W}
    int u_A;  // units of area (I2) {W}
};

class Cse_Drs : public SourceSinkElement
{
public:
    Cse_Drs(int nr=0,QString spcs=QString(),QString ctype=0,QString name=QString(),QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX kd;  // deposition rate [1/s] (R4)
    int u_k;  // units of deposition rate (I2) {W}
};

class Cse_Dvr : public SourceSinkElement
{
public:
    Cse_Dvr(int nr=0,QString spcs=QString(),QString ctype=0,QString name=QString(),QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX dV;  // deposition velocity [m/s] (R4)
    RX R;  // resuspension rate [1/s] (R4)
    RX dA;  // deposition surface area [m2] (R4)
    RX rA;  // resuspension surface area [m2] (R4)
    int u_V;  // units of deposition velocity (I2) {W}
    int u_R;  // units of resuspension rate (I2) {W}
    int u_dA;  // units of deposition areaa (I2) {W}
    int u_dR;  // units of resuspension area (I2) {W}
};

class Cse_Plm : public SourceSinkElement
{
public:
    Cse_Plm(int nr=0,QString spcs=QString(),QString ctype=0,QString name=QString(),QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX a;  // coefficient(R4) [kg/s]
    RX b;  // exponent (R4)
    RX tp;  // PL model time [s] (R4)
    int u_a;  // units of initial emission factor (I2) {W}
    int u_tp;  // units of PL model time (I2) {W}
};

class Cse_Pkm : public SourceSinkElement
{
public:
    Cse_Pkm(int nr=0,QString spcs=QString(),QString ctype=0,QString name=QString(),QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX a;  // peak emission rate(R4) [kg/s]
    RX b;  // fitting parameter (R4)
    RX tp;  // time to peak [s] (R4)
    int u_a;  // units of peak emission rate (I2) {W}
    int u_tp;  // units of peak time (I2) {W}
};

class Cse_Sup : public SourceSinkElement
{
public:
    Cse_Sup(int nr=0,QString spcs=QString(),QString ctype=0,QString name=QString(),QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    //int n;  // number of sub-elements (IX)
    QVector<int> elmt;  // sub-element numbers (IX) - converted to pointers
};


// Section 10: Airflow Elements

class AirflowElement
{
public:
    virtual QString write()=0;
    virtual ~AirflowElement(){}
    enum Type {PLR_ORF,PLR_LEAK,PLR_CONN,PLR_QCN,PLR_FCN,PLR_TEST1,PLR_TEST2,PLR_CRACK,PLR_STAIR,PLR_SHAFT,
               PLR_BDQ,PLR_BDF,QFR_QAB,QFR_FAB,QFR_CRACK,QFR_TEST2,AFE_DOR,DR_PL2,AFE_CMF,AFE_CVF,AFE_FAN,AFE_CSF,AFE_SUP,UNKNOWN};

    int nr;  // element number (IX); in order from 1 to _nafe
    int icon;  // icon used to represent flow path (I2) {W}
    QString dtype;  // element data type (CS) element type names are stored in _afe_dnames in dtype order.
    Type type;
    QString name;  // element name (CS) {W}
    QString desc;  // element description (CS) {W} may be blank

    static Type convertType(QString string);
    static AirflowElement *readElement(Reader *input);
};

class Plr_Orf : public AirflowElement
{
public:
    Plr_Orf(int nr=0,int icon=0,QString dtype=QString(),QString name=QString(),
            QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX lam;  // laminar flow coefficient (R4)
    RX turb;  // turbulent flow coefficient (R4)
    RX expt;  // pressure exponent (R4)
    RX area;  // actual area [m^2] (R4) {X}
    RX dia;  // hydraulic diameter [m] (R4) {X}
    RX coef;  // flow coefficient (R4) {X}
    RX Re;  // laminar/turbulet transition Reynolds number [-](R4) {X}
    int u_A;  // units of area (I2) {X}
    int u_D;  // units of diameter (I2) {X}
};

class Plr_Leak : public AirflowElement
{
public:
    Plr_Leak(int nr=0,int icon=0,QString dtype=QString(),QString name=QString(),
             QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX lam;  // laminar flow coefficient (R4)
    RX turb;  // turbulent flow coefficient (R4)
    RX expt;  // pressure exponent (R4)
    RX coef;  // flow coefficient (R4) {W}
    RX pres;  // reference pressure drop [Pa] (R4) {W}
    RX area1;  // leakage area per item [m^2] (R4) {W}
    RX area2;  // leakage area per unit length [m^2/m] (R4) {W}
    RX area3;  // leakage area per unit area [m^2/m^2] (R4) {W}
    int u_A1;  // units of area1 [m^2] (I2) {W}
    int u_A2;  // units of area2 [m^2/m] (I2) {W}
    int u_A3;  // units of area3 [m^2/m^2] (I2) {W}
    int u_dP;  // units of pressure (I2) {W}
};

class Plr_Conn : public AirflowElement
{
public:
    Plr_Conn(int nr=0,int icon=0,QString dtype=QString(),QString name=QString(),
             QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX lam;  // laminar flow coefficient (R4)
    RX turb;  // turbulent flow coefficient (R4)
    RX expt;  // pressure exponent - 0.5 (R4)
    RX area;  // actual area [m^2] (R4) {W}
    RX coef;  // flow coefficient (R4) {W}
    int u_A;  // units of area (I2) {W}
};

class Plr_Qcn : public AirflowElement
{
public:
    Plr_Qcn(int nr=0,int icon=0,QString dtype=QString(),QString name=QString(),
            QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX lam;  // laminar flow coefficient (R4)
    RX turb;  // turbulent flow coefficient (R4)
    RX expt;  // pressure exponent (R4)
};

class Plr_Fcn : public AirflowElement
{
public:
    Plr_Fcn(int nr=0,int icon=0,QString dtype=QString(),QString name=QString(),
            QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX lam;  // laminar flow coefficient (R4)
    RX turb;  // turbulent flow coefficient (R4)
    RX expt;  // pressure exponent (R4)
};

class Plr_Test1 : public AirflowElement
{
public:
    Plr_Test1(int nr=0,int icon=0,QString dtype=QString(),QString name=QString(),
              QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX lam;  // laminar flow coefficient (R4)
    RX turb;  // turbulent flow coefficient (R4)
    RX expt;  // pressure exponent (R4)
    RX dP;  // pressure drop [Pa] (R4) {W}
    RX Flow;  // flow rate [kg/s] (R4) {W}
    int u_P;  // units of pressure drop (I2) {W}
    int u_F;  // units of flow (I2) {W}
};

class Plr_Test2 : public AirflowElement
{
public:
    Plr_Test2(int nr=0,int icon=0,QString dtype=QString(),QString name=QString(),
              QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX lam;  // laminar flow coefficient (R4)
    RX turb;  // turbulent flow coefficient (R4)
    RX expt;  // pressure exponent (R4)
    RX dP1;  // point 1 pressure drop [Pa] (R4) {W}
    RX F1;  // point 1 flow rate [kg/s] (R4) {W}
    RX dP2;  // point 2 pressure drop [Pa] (R4) {W}
    RX F2;  // point 2 flow rate [kg/s] (R4) {W}
    int u_P1;  // units of pressure drop (I2) {W}
    int u_F1;  // units of flow (I2) {W}
    int u_P2;  // units of pressure drop (I2) {W}
    int u_F2;  // units of flow (I2) {W}
};

class Plr_Crack : public AirflowElement
{
public:
    Plr_Crack(int nr=0,int icon=0,QString dtype=QString(),QString name=QString(),
              QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX lam;  // laminar flow coefficient (R4)
    RX turb;  // turbulent flow coefficient (R4)
    RX expt;  // pressure exponent (R4)
    RX length;  // crack length [m] (R4) {W}
    RX width;  // crack width [m] (R4) {W}
    int u_L;  // units of length (I2) {W}
    int u_W;  // units of width (I2) {W}
};

class Plr_Stair : public AirflowElement
{
public:
    Plr_Stair(int nr=0,int icon=0,QString dtype=QString(),QString name=QString(),
              QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX lam;  // laminar flow coefficient (R4)
    RX turb;  // turbulent flow coefficient (R4)
    RX expt;  // pressure exponent (R4)
    RX Ht;  // distance between levels [m] (R4) {W}
    RX Area;  // cross-sectional area [m^2] (R4) {W}
    RX peo;  // density of people [pers/m^2] (R4) {W}
    int tread;  // 1 = open tread 0 = closed (IX) {W}
    int u_A;  // units of area (I2) {W}
    int u_D;  // units of distance (I2) {W}
};

class Plr_Shaft : public AirflowElement
{
public:
    Plr_Shaft(int nr=0,int icon=0,QString dtype=QString(),QString name=QString(),
              QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX lam;  // laminar flow coefficient (R4)
    RX turb;  // turbulent flow coefficient (R4)
    RX expt;  // pressure exponent (R4)
    RX Ht;  // distance between levels [m] (R4) {W}
    RX area;  // cross-sectional area [m^2] (R4) {W}
    RX perim;  // perimeter [m] (R4) {W}
    RX rough;  // roughness [m] (R4) {W}
    int u_A;  // units of area (I2) {W}
    int u_D;  // units of distance (I2) {W}
    int u_P;  // units of perimeter (I2) {W}
    int u_R;  // units of roughness (I2) {W}
};

class Plr_Bdq : public AirflowElement
{
public:
    Plr_Bdq(int nr=0,int icon=0,QString dtype=QString(),QString name=QString(),
            QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX lam;  // laminar flow coefficient {R4}
    RX Cp;  // turbulent flow coefficient ( dP > 0 ) {R4}
    RX xp;  // pressure exponent ( dP > 0 ) {R4}
    RX Cn;  // turbulent flow coefficient ( dP < 0 ) {R4}
    RX xn;  // pressure exponent ( dP < 0 ) {R4}
};

class Plr_Bdf : public AirflowElement
{
public:
    Plr_Bdf(int nr=0,int icon=0,QString dtype=QString(),QString name=QString(),
            QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX lam;  // laminar flow coefficient {R4}
    RX Cp;  // turbulent flow coefficient ( dP > 0 ) {R4}
    RX xp;  // pressure exponent ( dP > 0 ) {R4}
    RX Cn;  // turbulent flow coefficient ( dP < 0 ) {R4}
    RX xn;  // pressure exponent ( dP < 0 ) {R4}
};

class Qfr_Qab : public AirflowElement
{
public:
    Qfr_Qab(int nr=0,int icon=0,QString dtype=QString(),QString name=QString(),
            QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX a;  // dP = a*Q + b*Q*Q {R4}
    RX b;  // {R4}
};

class Qfr_Fab : public AirflowElement
{
public:
    Qfr_Fab(int nr=0,int icon=0,QString dtype=QString(),QString name=QString(),
            QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX a;  // dP = a*F + b*F*F {R4}
    RX b;  // {R4}
};

class Qfr_Crack : public AirflowElement
{
public:
    Qfr_Crack(int nr=0,int icon=0,QString dtype=QString(),QString name=QString(),
              QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX a;  // dP = a*F + b*F*F {R4}
    RX b;  // {R4}
    RX length;  // crack length [m] {R4}
    RX width;  // crack width [m] {R4}
    RX depth;  // crack depth [m] {R4}
    int nB;  // number of bends (IX)
    int u_L;  // units of length (IX)
    int u_W;  // units of width (IX)
    int u_D;  // units of depth (IX)
};

class Qfr_Test2 : public AirflowElement
{
public:
    Qfr_Test2(int nr=0,int icon=0,QString dtype=QString(),QString name=QString(),
              QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX a;  // dP = a*F + b*F*F {R4}
    RX b;  // {R4}
    RX dP1;  // point 1 pressure drop [Pa] (R4) {W}
    RX F1;  // point 1 flow rate [kg/s] (R4) {W}
    RX dP2;  // point 2 pressure drop [Pa] (R4) {W}
    RX F2;  // point 2 flow rate [kg/s] (R4) {W}
    int u_P1;  // units of pressure drop (I2) {W}
    int u_F1;  // units of flow (I2) {W}
    int u_P2;  // units of pressure drop (I2) {W}
    int u_F2;  // units of flow (I2) {W}
};

class Afe_Dor : public AirflowElement
{
public:
    Afe_Dor(int nr=0,int icon=0,QString dtype=QString(),QString name=QString(),
            QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX lam;  // laminar flow coefficient (R4)
    RX turb;  // turbulent flow coefficient (R4)
    RX expt;  // pressure exponent (R4)
    RX dTmin;  // minimum temperature difference for two-way flow [C] (R4)
    RX ht;  // height of doorway [m] (R4)
    RX wd;  // width of doorway [m] (R4)
    RX cd;  // discharge coefficient (R4)
    int u_T;  // units of temperature (I2) {W}
    int u_H;  // units of height (I2) {W}
    int u_W;  // units of width (I2) {W}
};

class Dr_Pl2 : public AirflowElement
{
public:
    Dr_Pl2(int nr=0,int icon=0,QString dtype=QString(),QString name=QString(),
           QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX lam;  // laminar flow coefficient (R4)
    RX turb;  // turbulent flow coefficient (R4)
    RX expt;  // pressure exponent (R4)
    RX dH;  // distance above | below midpoint [m] {R4}
    RX ht;  // height of doorway [m] (R4) {W in v. 2.0}
    RX wd;  // width of doorway [m] (R4) {W}
    RX cd;  // discharge coefficient [-] (R4) {W}
    int u_H;  // units of height (I2) {W}
    int u_W;  // units of width (I2) {W}
};

class Afe_Cmf : public AirflowElement
{
public:
    Afe_Cmf(int nr=0,int icon=0,QString dtype=QString(),QString name=QString(),
            QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX Flow;  // design flow rate [kg/s] (R4)
    int u_F;  // units of flow (I2) {W}
};

class Afe_Cvf : public AirflowElement
{
public:
    Afe_Cvf(int nr=0,int icon=0,QString dtype=QString(),QString name=QString(),
            QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX Flow;  // design flow rate [m^3/s] (R4)
    int u_F;  // units of flow (I2) {W}
};

class Afe_Fan : public AirflowElement
{
public:
    Afe_Fan(int nr=0,int icon=0,QString dtype=QString(),QString name=QString(),
            QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    RX lam;  // laminar flow coefficient (R4)
    RX turb;  // turbulent flow coefficient (R4)
    RX expt;  // pressure exponent (R4)
    RX rdens;  // reference fluid density [kg/m^3] {R4}
    RX fdf;  // free delivery flow (prise = 0) [kg/s] {R4}
    RX sop;  // shut-off pressure (flow = 0) [Pa] {R4}
    RX off;  // fan is off if (RPM/rated RPM) < off {R4}
    RX fpc[4];  // fan performance polynomial coefficients {R4}
    //int npts;  // number of mesaured data points (I2) {W}
    RX Sarea;  // shut-off orifice area [m^2] {R4} {W}
    int u_Sa;  // units of shut-off area (I2) {W}
    QVector<RX> mF;  // measured flow rates [kg/s] (R4) {W}
    QVector<int> u_mF;  // units of measured flows (I2) {W}
    QVector<RX> dP;  // measured pressure rises [Pa] (R4) {W}
    QVector<int> u_dP;  // units of pressure rises (I2) {W}
    QVector<RX> rP;  // revised pressure rises [Pa] (R4) {W}
    QVector<int> u_rP;  // units of revised pressures (I2) {W}
};

class Afe_Csf : public AirflowElement
{
public:
    Afe_Csf(int nr=0,int icon=0,QString dtype=QString(),QString name=QString(),
            QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    int npts;  // number of data points (I2)
    int u_x;  // units for x (I2)
    int u_y;  // units for y (I2)
    RX x;  // value of independent variable (R4)
    RX y;  // value of dependent variable (R4)
};

class Afe_Sup : public AirflowElement
{
public:
    Afe_Sup(int nr=0,int icon=0,QString dtype=QString(),QString name=QString(),
            QString desc=QString());
    void readDetails(Reader *input);
    QString write();
    int nse;  // number of sub-elements(I2)
    int sched;  // scheduled sub-element number (IX) (only one)
    int u_H;  // units of relative height (I2)
    int nr;  // sub-element number (IX)
    RX relHt;  // relative height of sub-element (R4)
    int filt;  // 1= filtered, 0= not (I2)
};

// Section 11: Duct Elements - Not supported
// Section 12a: Control Super Elements - Not supported
// Section 12b: Control Nodes - Not supported

// Section 13: Simple Air Handling System (AHS)

class Ahs
{
public:
    Ahs();
    void read(Reader *input);
    QString write();
    int nr;  // AHS number (IX); in order from 1 to _nahs
    int zone_r;  // return zone number (IX)
    int zone_s;  // supply zone number (IX)
    int path_r;  // recirculation air path number (IX)
    int path_s;  // outdoor air path number (IX)
    int path_x;  // exhaust path number (IX)
    QString name;  // AHS name (CS)
    QString desc;  // AHS description (CS) {W} may be blank
};

// Section 14: Zones

class Zone
{
public:
    enum Flags {
        // Zone flag bit 1; variable pressure
        VAR_P=0x0001,NVAR_P=0xFFFE,
        // Zone flag bit 2; variable contaminants
        VAR_C=0x0002,NVAR_C=0xFFFD,
        // Zone flag bit 3; variable temperature
        VAR_T=0x0004,NVAR_T=0xFFFB,
        // Zone flag bit 4; system zone: to avoid zone volume check
        SYS_N=0x0008,NSYS_N=0xFFF7,
        // Zone flag bit 5; un|conditioned space: to compute air change rate
        UNCZN =0x0010 , // flags | UNCZN to indicate unconditioned zone
        SETCZN=0xFFEF, // flags & SETCZN to indicate conditioned zone
        // Zone flag bit 6; CFD zone
        CFDZN =0x0020, // flags | CFDZN to set CFD zone / flags & CFDZN to test for CFD zone
        NCFDZN=0xFFDF,   // flags & NCFDZN to unset CFD zone
        FLAG_N=0x003F};   // all zone flag bits, used in PrjRead()
    Zone();
    void read(Reader *input);
    QString write();
    // Custom getters/setters
    void setVariablePressure(bool b);
    bool variablePressure() const;
    void setVariableContaminants(bool b);
    bool variableContaminants() const;
    void setSystem(bool b);
    bool system() const;

    int nr;  // zone number (IX); in order from 1 to _nzone
    unsigned int flags;  // zone flags - bits defined in contam.h (U2)
    int ps;  // week schedule index (IX); converted to pointer
    int pc;  // control node index (IX); converted to pointer
    int pk;  // kinetic reaction index (IX); converted to pointer
    int pl;  // building level index (IX); converted to pointer
    RX relHt;  // zone height [m] (R4)
    RX Vol;  // zone volume [m^3] (R4)
    RX T0;  // initial zone temperature [K] (R4)
    RX P0;  // initial zone pressure [Pa] (R4)
    QString name;  // zone name (I1) {W}
    int color;  // zone fill color (I2) {W} {Contam 2.4}
    int u_Ht;  // units of height (I2) {W}
    int u_V;  // units of volume (I2) {W}
    int u_T;  // units of temperature (I2) {W}
    int u_P;  // units of pressure (I2) {W}
    int cdaxis;  // conv/diff axis (0=no cd, 1-4 => cd axis direction) (I2)
    int cfd;  // cfd zone (0=no, 1=yes) (I2)
    QString cfdname;  // cfd zone id (I1)
    // QString "1D:";  // string indicating 1D data follows
    RX X1;  // X coordinate of one end of cdaxis
    RX Y1;  // Y coordinate of one end of cdaxis
    RX H1;  // Relative Height of other end of cdaxis
    RX X2;  // X coordinate of other end of cdaxis
    RX Y2;  // Y coordinate of other end of cdaxis
    RX H2;  // Relative Height of other end of cdaxis
    RX celldx;  // length of c/d cell [m] (R4)
    RX axialD;  // axial diffusion coeff [m^2/s] (R4)
    int u_aD;  // units of axial diffusion (I2)
    int u_L;  // units of c/d axis limits (I2)
    QVector<RX> ic;
};

class Path
{
public:
    enum Flags {
        WIND=0x0001, // Path is subject to wind pressure
        WPC_P=0x0002, // Path uses WPC file pressure
        WPC_C=0x0004, // Path uses WPC file contaminants
        WPC_F=0x0006, // Path uses WPC pressure or contaminants
        AHS_S=0x0008, // Path is a system (supply or return) flow path
        AHS_R=0x0010, // Path is a recirculation flow path
        AHS_O=0x0020, // Path is an outside air flow path
        AHS_X=0x0040, // Path is an exhaust flow path
        AHS_P=0x0078, // Path is any AHS path
        LIM_P=0x0080, // Path is pressure limited
        LIM_F=0x0100, // Path is flow limited
        FAN_F=0x0200}; // Path is a constant flow fan element
    Path();
    void read(Reader *input);
    QString write();
    // Custom getters/setters
    void setWindPressure(bool b);
    bool windPressure();
    void setSystem(bool b);
    bool system();
    void setRecirculation(bool b);
    bool recirculation();
    void setOutsideAir(bool b);
    bool outsideAir();
    void setExhaust(bool b);
    bool exhaust();

    int nr;  // path number (IX); in order from 1 to _npath
    int flags;  // airflow path flag values (I2)
    int pzn;  // zone N index (IX); converted to pointer
    int pzm;  // zone M index (IX); converted to pointer
    int pe;  // flow element index (IX); converted to pointer
    int pf;  // filter index (IX); converted to pointer
    int pw;  // wind coefficients index (IX); converted to pointer
    int pa;  // AHS index (IX); converted to pointer
    int ps;  // schedule index (IX); converted to pointer
    int pc;  // control node index (IX); converted to pointer
    int pld;  // level index (IX); converted to pointer
    RX X;  // X-coordinate of envelope path [m] (R4) {Contam 2.1}
    RX Y;  // Y-coordinate of envelope path [m] (R4) {Contam 2.1}
    RX relHt;  // height relative to current level [m] (R4)
    RX mult;  // element multiplier (R4)
    RX wPset;  // constant wind pressure [Pa] (pw==NULL) (R4)
    RX wPmod;  // wind speed(?) modifier (pw!=NULL) (R4)
    RX wazm;  // wall azimuth angle (pw!=NULL) (R4)
    RX Fahs;  // AHS path flow rate [kg/s] (pw==NULL) (R4)
    RX Xmax;  // flow or pressure limit - maximum (R4) {W}
    RX Xmin;  // flow or pressure limit - minimum (R4) {W}
    unsigned int icon;  // icon used to represent flow path (U1) {W}
    unsigned int dir;  // positive flow direction on sketchpad (U1) {W}
    int u_Ht;  // units of height (I2) {W}
    int u_XY;  // units of X and Y (I2) {W}
    int u_dP;  // units of pressure difference (I2) {W}
    int u_F;  // units of flow (I2) {W}
    int cfd;  // cfd path (0=no, 1=yes) (I2) {CONTAM 3.0}
    QString name;  // cfd path id (CS)
    int cfd_ptype;  // boundary condition type (0=mass flow, 1=pressure) (I2)
    int cfd_btype;  // pressure bc type (if ptype = 1, 0=linear, 1=stagnation pressure) (I2)
    int cfd_capp;  // coupling approach (1=pressure-pressure) (I2)
};

class SourceSink
{
public:
    SourceSink();
    void read(Reader *input);
    QString write();
    int nr;  // source/sink number (IX); in order from 1 to _ncss
    int pz;  // zone index (IX); converted to pointer
    int pe;  // source/sink element index (IX); converted to pointer
    int ps;  // schedule index (IX); converted to pointer
    int pc;  // control node index (IX); converted to pointer
    RX mult;  // multiplier (R4)
    RX CC0;  // initial mass fraction (some types) [kg/kg] (R4)
    RX Xmin;  // minimum x coordinate of source/sink location [m] (R4)
    RX Ymin;  // minimum y coordinate of source/sink location [m] (R4)
    RX Hmin;  // minimum z coordinate relative to level [m] (R4)
    RX Xmax;  // maximum x coordinate of source/sink location [m] (R4)
    RX Ymax;  // maximum y coordinate of source/sink location [m] (R4)
    RX Hmax;  // maximum z coordinate relative to level [m] (R4)
    int u_XYZ;  // units of location coordinates (I2)
    int cfd;  // cfd path (0=no, 1=yes) (I2) {CONTAM 3.0}
    QString cfdname;  // cfd path id (CS)
};

}
CONTAMNAMESPACEEND

#endif
