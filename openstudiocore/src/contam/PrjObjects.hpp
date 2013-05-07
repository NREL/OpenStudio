#ifndef PRJOBJECTS_H
#define PRJOBJECTS_H

#include "PrjReader.hpp"

#define RX_IS_STRING
#define RX QString
#define RX_C(v) v
#define RX_INIT QString("0.0")

namespace openstudio {
namespace contam {
namespace prj {

class Weather
{
public:
    void read(Reader *input);
    QString write();
    RX Tambt;  /* ambient temperature [K] (R4) */
    RX barpres;  /* barometric pressure [Pa] NOT corrected to sea level (R4) */
    RX windspd;  /* wind speed [m/s] (R4) */
    RX winddir;  /* wind direction: 0 = N, 90 = E, 180 = S, ...; (R4) */
    RX relhum;  /* relative humidity: 0.0 to 1.0 (R4) */
    int daytyp;  /* type of day (1-12) (I2) */
    int uTa;  /* units for Tambt (I2) {W} */
    int ubP;  /* units for barpres (I2) {W} */
    int uws;  /* units for windspd (I2) {W} */
    int uwd;  /* units for winddir (I2) {W} */
};

class RunControl
{
public:
    RunControl(){rvals=NULL;}
    ~RunControl(){if(rvals)delete rvals;}
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
    QString cnvgSS;  /* (0/1) converge density in steady state init (I1) {2.4b} */
    QString densZP;  /* (0/1) density = f(zone pressure) (I1) {2.4b, 3.1} */
    QString stackD;  /* (0/1) density varies hydrostatically (I1) {2.4b, 3.1} */
    QString dodMdt;  /* (0/1) include dM/dt in airflow calc (I1) {2.4b, 3.1} */
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
    int nrvals;  /* number of real values in the vector (I2) */
    RX *rvals;  /* real values (R4) */
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

class Zone
{
public:
    Zone();
    void read(Reader *input);
    QString write();
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
};

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
    ~Level();
    void read(Reader *input);
    QString write();
    int nr;  // level number (IX), in order from 1 to nlev
    RX refht;  // reference elevation of level [m] (R4)
    RX delht;  // delta elevation to next level [m] (R4) {W}
    int nicon;  // number of icons on this level (IX)
    int u_rfht;  // units of reference elevation (I2) {W}
    int u_dlht;  // units of delta elevation (I2) {W}
    QString name;  // level name (CS)
    Icon *icons; /* Icons on this level */
};

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

class Path
{
public:
    Path();
    void read(Reader *input);
    QString write();
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

}
}
}

#endif // PRJOBJECTS_H
