/**********************************************************************
*  Copyright (c) 2013, Alliance for Sustainable Energy.
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
#ifndef PRJPRIVATE_H
#define PRJPRIVATE_H

#include "PrjDefines.hpp"
#include "PrjReader.hpp"
#include "PrjSubobjects.hpp"
#include <QSharedData>

namespace openstudio {
namespace contam {

class ZonePrivate : public QSharedData
{
public:
  ZonePrivate(int nr=0,unsigned int flags=0,int ps=0,int pc=0,int pk=0,int pl=0,RX relHt=RX_INIT(0),RX Vol=RX_INIT(0),RX T0=RX_INIT(0),RX P0=RX_INIT(0),std::string name=std::string(),int color=0,int u_Ht=0,int u_V=0,int u_T=0,int u_P=0,int cdaxis=0,int cfd=0,std::string cfdname=std::string(),RX X1=RX_INIT(0),RX Y1=RX_INIT(0),RX H1=RX_INIT(0),RX X2=RX_INIT(0),RX Y2=RX_INIT(0),RX H2=RX_INIT(0),RX celldx=RX_INIT(0),RX axialD=RX_INIT(0),int u_aD=0,int u_L=0);
  void read(Reader &input);
  std::string write();

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
  std::string name;  // zone name (CS) {W}
  int color;  // zone fill color (I2) {W} {Contam 2.4}
  int u_Ht;  // units of height (I2) {W}
  int u_V;  // units of volume (I2) {W}
  int u_T;  // units of temperature (I2) {W}
  int u_P;  // units of pressure (I2) {W}
  int cdaxis;  // conv/diff axis (0=no cd, 1-4 => cd axis direction) (I2)
  int cfd;  // cfd zone (0=no, 1=yes) (I2)
  std::string cfdname;  // cfd zone id (CS)
  RX X1;  // X coordinate of one end of cdaxis (RX)
  RX Y1;  // Y coordinate of one end of cdaxis (RX)
  RX H1;  // Relative Height of one end of cdaxis (RX)
  RX X2;  // X coordinate of other end of cdaxis (RX)
  RX Y2;  // Y coordinate of other end of cdaxis (RX)
  RX H2;  // Relative Height of other end of cdaxis (RX)
  RX celldx;  // length of c/d cell [m] (R4)
  RX axialD;  // axial diffusion coeff [m^2/s] (R4)
  int u_aD;  // units of axial diffusion (I2)
  int u_L;  // units of c/d axis limits (I2)

  std::vector<RX> ic;

};

class SpeciesPrivate : public QSharedData
{
public:
  SpeciesPrivate(int nr=0,int sflag=0,int ntflag=0,RX molwt=RX_INIT(0),RX mdiam=RX_INIT(0),RX edens=RX_INIT(0),RX decay=RX_INIT(0),RX Dm=RX_INIT(0),RX ccdef=RX_INIT(0),RX Cp=RX_INIT(0),int ucc=0,int umd=0,int ued=0,int udm=0,int ucp=0,std::string name=std::string(),std::string desc=std::string());
  void read(Reader &reader);
  std::string write();
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
  std::string name;  // species name (CS)
  std::string desc;  // species description (CS) {W}
};

class AhsPrivate : public QSharedData
{
public:
  AhsPrivate(int nr=0,int zone_r=0,int zone_s=0,int path_r=0,int path_s=0,int path_x=0,std::string name=std::string(),std::string desc=std::string());
  void read(Reader &reader);
  std::string write();
  int nr;  // AHS number (IX); in order from 1 to _nahs
  int zone_r;  // return zone number (IX)
  int zone_s;  // supply zone number (IX)
  int path_r;  // recircultaion air path number (IX)
  int path_s;  // outdoor air path number (IX)
  int path_x;  // exhaust path number (IX)
  std::string name;  // AHS name (CS)
  std::string desc;  // AHS description (CS) {W} may be blank
};

class PathPrivate : public QSharedData
{
public:
  PathPrivate(int nr=0,int flags=0,int pzn=0,int pzm=0,int pe=0,int pf=0,int pw=0,int pa=0,int ps=0,int pc=0,int pld=0,RX X=RX_INIT(0),RX Y=RX_INIT(0),RX relHt=RX_INIT(0),RX mult=RX_INIT(0),RX wPset=RX_INIT(0),RX wPmod=RX_INIT(0),RX wazm=RX_INIT(0),RX Fahs=RX_INIT(0),RX Xmax=RX_INIT(0),RX Xmin=RX_INIT(0),unsigned int icon=0,unsigned int dir=0,int u_Ht=0,int u_XY=0,int u_dP=0,int u_F=0,int cfd=0,std::string cfd_name=std::string(),int cfd_ptype=0,int cfd_btype=0,int cfd_capp=0);
  void read(Reader &reader);
  std::string write();
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
  std::string cfd_name;  // cfd path id (CS)
  int cfd_ptype;  // boundary condition type (0=mass flow, 1=pressure) (I2)
  int cfd_btype;  // pressure bc type (if ptype = 1, 0=linear, 1=stagnation pressure) (I2)
  int cfd_capp;  // coupling approach (1=pressure-pressure) (I2)
};

class RunControlPrivate : public QSharedData
{
public:
  RunControlPrivate(std::string name=std::string(),std::string version=std::string(),int echo=0,std::string prjdesc=std::string(),int skheight=0,int skwidth=0,int def_units=0,int def_flows=0,RX def_T=RX_INIT(0),int udefT=0,RX rel_N=RX_INIT(0),RX wind_H=RX_INIT(0),int uwH=0,RX wind_Ao=RX_INIT(0),RX wind_a=RX_INIT(0),RX scale=RX_INIT(0),int uScale=0,int orgRow=0,int orgCol=0,int invYaxis=0,int showGeom=0,WeatherData ssWeather=WeatherData(),WeatherData wptWeather=WeatherData(),std::string WTHpath=std::string(),std::string CTMpath=std::string(),std::string CVFpath=std::string(),std::string DVFpath=std::string(),std::string WPCfile=std::string(),std::string EWCfile=std::string(),std::string WPCdesc=std::string(),RX X0=RX_INIT(0),RX Y0=RX_INIT(0),RX Z0=RX_INIT(0),RX angle=RX_INIT(0),int u_XYZ=0,RX epsPath=RX_INIT(0),RX epsSpcs=RX_INIT(0),std::string tShift=std::string(),std::string dStart=std::string(),std::string dEnd=std::string(),int useWPCwp=0,int useWPCmf=0,int wpctrig=0,RX latd=RX_INIT(0),RX lgtd=RX_INIT(0),RX Tznr=RX_INIT(0),RX altd=RX_INIT(0),RX Tgrnd=RX_INIT(0),int utg=0,int u_a=0,int sim_af=0,int afcalc=0,int afmaxi=0,RX afrcnvg=RX_INIT(0),RX afacnvg=RX_INIT(0),RX afrelax=RX_INIT(0),int uac2=0,RX Pres=RX_INIT(0),int uPres=0,int afslae=0,int afrseq=0,int aflmaxi=0,RX aflcnvg=RX_INIT(0),int aflinit=0,int Tadj=0,int sim_mf=0,int ccmaxi=0,RX ccrcnvg=RX_INIT(0),RX ccacnvg=RX_INIT(0),RX ccrelax=RX_INIT(0),int uccc=0,int mfnmthd=0,int mfnrseq=0,int mfnmaxi=0,RX mfnrcnvg=RX_INIT(0),RX mfnacnvg=RX_INIT(0),RX mfnrelax=RX_INIT(0),RX mfngamma=RX_INIT(0),int uccn=0,int mftmthd=0,int mftrseq=0,int mftmaxi=0,RX mftrcnvg=RX_INIT(0),RX mftacnvg=RX_INIT(0),RX mftrelax=RX_INIT(0),RX mftgamma=RX_INIT(0),int ucct=0,int mfvmthd=0,int mfvrseq=0,int mfvmaxi=0,RX mfvrcnvg=RX_INIT(0),RX mfvacnvg=RX_INIT(0),RX mfvrelax=RX_INIT(0),int uccv=0,int mf_solver=0,int sim_1dz=0,int sim_1dd=0,RX celldx=RX_INIT(0),int sim_vjt=0,int udx=0,int cvode_mth=0,RX cvode_rcnvg=RX_INIT(0),RX cvode_acnvg=RX_INIT(0),RX cvode_dtmax=RX_INIT(0),int tsdens=0,RX tsrelax=RX_INIT(0),int tsmaxi=0,int cnvgSS=0,int densZP=0,int stackD=0,int dodMdt=0,std::string date_st=std::string(),std::string time_st=std::string(),std::string date_0=std::string(),std::string time_0=std::string(),std::string date_1=std::string(),std::string time_1=std::string(),std::string time_step=std::string(),std::string time_list=std::string(),std::string time_scrn=std::string(),int restart=0,std::string rstdate=std::string(),std::string rsttime=std::string(),int list=0,int doDlg=0,int pfsave=0,int zfsave=0,int zcsave=0,int achvol=0,int achsave=0,int abwsave=0,int cbwsave=0,int expsave=0,int ebwsave=0,int zaasave=0,int zbwsave=0,int rzfsave=0,int rzmsave=0,int rz1save=0,int csmsave=0,int srfsave=0,int logsave=0,std::vector<int> save=std::vector<int>(),std::vector<RX> rvals=std::vector<RX>(),int BldgFlowZ=0,int BldgFlowD=0,int BldgFlowC=0,int cfd_ctype=0,RX cfd_convcpl=RX_INIT(0),int cfd_var=0,int cfd_zref=0,int cfd_imax=0,int cfd_dtcmo=0);
  void read(Reader &reader);
  std::string write();
  std::string name;  // program name "ContamW" (CS)
  std::string version;  // program version "3.1" (CS)
  int echo;  // (0/1) echo input files to the log file (I2)
  std::string prjdesc;  // (unused) project description (CS) {W}
  int skheight;  // total SketchPad height [cells] (I2) {W}
  int skwidth;  // total SketchPad width [cells] (I2) {W}
  int def_units;  // default units: 0 = SI, 1 = US (I2) {W}
  int def_flows;  // default flows: 0 = mass, 1 = volume (I2) {W}
  RX def_T;  // default temperature for zones [K] (R4) {W}
  int udefT;  // units for temperature (I2) {W}
  RX rel_N;  // angle to true north [degrees] (R4) {W}
  RX wind_H;  // elevation for reference wind speed [m] (R4) {W}
  int uwH;  // units for _wind_H (I2) {W}
  RX wind_Ao;  // local terrain constant (R4) {W}
  RX wind_a;  // velocity profile exponent (R4) {W}
  RX scale;  // cell scaling factor (R4)
  int uScale;  // units of scaling factor: 0 = m, 1 = ft (I2)
  int orgRow;  // row of origin (I2)
  int orgCol;  // column of origin (I2)
  int invYaxis;  // 0 = positive above origin, 1 = negative above origin (I2)
  int showGeom;  // 0/1 = show pseudo-geometry info in status bar (I2)
  WeatherData ssWeather;  // weather data for steady-state simulation (WeatherData)
  WeatherData wptWeather;  // weather data the wind pressure test (WeatherData)
  std::string WTHpath;  // full name of weather file (CS)
  std::string CTMpath;  // full name of contaminant file (CS)
  std::string CVFpath;  // full name of continuous values file (CS)
  std::string DVFpath;  // full name of discrete values file (CS)
  std::string WPCfile;  // full name of WPC file (CS)
  std::string EWCfile;  // full name of EWC data source file (CS) {W}
  std::string WPCdesc;  // WPC description (CS) {W}
  RX X0;  // X-value of ContamW origin in EWC coordinates [m] (R4) {W}
  RX Y0;  // Y-value of ContamW origin in EWC coordinates [m] (R4) {W}
  RX Z0;  // Z-value of ContamW origin in EWC coordinates [m] (R4) {W}
  RX angle;  // Rotation of ContamW relative to EWC coordinates (R4) {W}
  int u_XYZ;  // units of coordinates (I2) {W}
  RX epsPath;  // tolerance for matching path locations [-] (R4) {W}
  RX epsSpcs;  // tolerance for matching species [-] (R4) {W}
  std::string tShift;  // time shift of EWC data {W} [s] (hh:mm:ss) {W}
  std::string dStart;  // date WPC data starts (mm/dd) {W}
  std::string dEnd;  // date WPC data ends (mm/dd) {W}
  int useWPCwp;  // if true, use WPC file wind pressures (I2)
  int useWPCmf;  // if true, use WPC file mass fractions (I2)
  int wpctrig;  // 0/1 = trigger airflow calculation by WPC data (I2)
  RX latd;  // latitude (degrees: north +, south -) (R4)
  RX lgtd;  // longitude (degrees: east +, west -) (R4)
  RX Tznr;  // time zone (Greenwich = 0, Eastern = -5, etc.) (R4)
  RX altd;  // elevation above sea level [m] (R4)
  RX Tgrnd;  // ground temperature [K] (R4)
  int utg;  // units for ground temperatures (I2)
  int u_a;  // units for elevation (I2)
  int sim_af;  // airflow simulation: 0 = steady, 1 = dynamic (I2)
  int afcalc;  // N-R method for non-linear eqns: 0 = SUR, 1 = STR (I2)
  int afmaxi;  // maximum number of N-R iterations (I2)
  RX afrcnvg;  // relative airflow convergence factor (R4)
  RX afacnvg;  // absolute airflow convergence factor [1/s] (R4)
  RX afrelax;  // flow under-relaxation coefficient (for SUR) (R4)
  int uac2;  // units for afacnvg (I2)
  RX Pres;  // pressure test pressure (R4) {Contam 2.4}
  int uPres;  // units of Pres (I2) {Contam 2.4}
  int afslae;  // method for linear equations: 0 = SKY, 1 = PCG (I2)
  int afrseq;  // if true, resequence the linear equations (I2)
  int aflmaxi;  // maximum number of iterations (PCG) (I2)
  RX aflcnvg;  // relative convergence factor for (PCG) (R4)
  int aflinit;  // if true, do linear airflow initialization (I2)
  int Tadj;  // if true, use temperature adjustment (I2)
  int sim_mf;  // mass fraction (contaminant) simulation: 0 = none, 1 = steady, 2 = transient, 3 = cyclic (I2)
  int ccmaxi;  // simulation: maximum number of cyclic iterations (I2)
  RX ccrcnvg;  // relative convergence factor (R4)
  RX ccacnvg;  // absolute convergence factor [kg/kg] (R4)
  RX ccrelax;  // (unused) over-relaxation coefficient (R4)
  int uccc;  // units for ccacnvg (I2)
  int mfnmthd;  // simulation: 0 = SKY, 1 = BCG, 2 = SOR, 3 = LU (I2)
  int mfnrseq;  // if true, resequence the linear equations (I2)
  int mfnmaxi;  // maximum iterations (I2)
  RX mfnrcnvg;  // desired relative convergence (R4)
  RX mfnacnvg;  // desired absolute convergence (R4)
  RX mfnrelax;  // relaxation coefficient (R4)
  RX mfngamma;  // trapezoidal integration factor (R4)
  int uccn;  // units for mfnacnvg (I2)
  int mftmthd;  // 0 = SKY, 1 = BCG, 2 = SOR, 3 = LU (I2)
  int mftrseq;  // if true, resequence the linear equations (I2)
  int mftmaxi;  // maximum iterations (I2)
  RX mftrcnvg;  // desired relative convergence (R4)
  RX mftacnvg;  // desired absolute convergence (R4)
  RX mftrelax;  // relaxation coefficient (R4)
  RX mftgamma;  // trapezoidal integration factor (R4)
  int ucct;  // units for mftacnvg (I2)
  int mfvmthd;  // 0 = SKY, 2 = SOR, 3 = LU (1 n/a) (I2)
  int mfvrseq;  // if true, resequence the linear equations (I2)
  int mfvmaxi;  // maximum iterations (I2)
  RX mfvrcnvg;  // desired relative convergence (R4)
  RX mfvacnvg;  // desired absolute convergence (R4)
  RX mfvrelax;  // relaxation coefficient (R4)
  int uccv;  // units for mfvacnvg (I2)
  int mf_solver;  // mass fraction integration method: {3.1} 0=trapezoid, 1=STS, 2=CVODE (I2)
  int sim_1dz;  // if true, use 1D zones (I2)
  int sim_1dd;  // if true, use 1D ducts (I2)
  RX celldx;  // default length of duct cells for C-D model [m] (R4)
  int sim_vjt;  // if true, compute variable junction temperatures (I2)
  int udx;  // units of celldx (I2)
  int cvode_mth;  // 0 = am, 1 = bdf (I2)
  RX cvode_rcnvg;  // relative convergence factor(R4)
  RX cvode_acnvg;  // absolute convergence factor(R4)
  RX cvode_dtmax;  // maximum time step (R4)
  int tsdens;  // (0/1) vary density during time step (I2)
  RX tsrelax;  // (inactive) under-relaxation factor for calculating dM/dt (R4)
  int tsmaxi;  // maximum number of iterations for density changes (I2)
  int cnvgSS;  // (0/1) converge density in steady state init (I1) {2.4b}
  int densZP;  // (0/1) density = f(zone pressure) (I1) {2.4b, 3.1}
  int stackD;  // (0/1) density varies hydrostatically (I1) {2.4b, 3.1}
  int dodMdt;  // (0/1) include dM/dt in airflow calc (I1) {2.4b, 3.1}
  std::string date_st;  // day-of-year to start steady simulation (mmmdd)
  std::string time_st;  // time-of-day to start steady simulation (hh:mm:ss)
  std::string date_0;  // day-of-year to start transient simulation (mmmdd)
  std::string time_0;  // time-of-day to start transient simulation (hh:mm:ss)
  std::string date_1;  // day-of-year to end transient simulation (mmmdd)
  std::string time_1;  // time-of-day to end transient simulation (hh:mm:ss)
  std::string time_step;  // simulation time step [s] (hh:mm:ss)
  std::string time_list;  // simulation output (results) time step [s] (hh:mm:ss)
  std::string time_scrn;  // simulation status time step [s] (up to 1 day) (hh:mm:ss)
  int restart;  // use restart file (I2)
  std::string rstdate;  // restart date (mmmdd)
  std::string rsttime;  // restart time (hh:mm:ss)
  int list;  // data dump parameter (I2) > 0 dump matrix analysis, = 2 dump SIM file output, > 2 dump lognotes.
  int doDlg;  // (0/1) ContamX will display dialog box (I1)
  int pfsave;  // (0/1) save path flow results to SIM file (I1)
  int zfsave;  // (0/1) save zone flow results to SIM file (I1)
  int zcsave;  // (0/1) save mass fraction results to SIM file (I1)
  int achvol;  // (0/1) ACH based on true volumes instead of std volumes (I1)
  int achsave;  // (0/1) save building air exchange rate transient data (I1)
  int abwsave;  // (0/1) save air exchange rate box-whisker data (I1)
  int cbwsave;  // (0/1) save contaminant box-whisker data (I1)
  int expsave;  // (0/1) save exposure transient data (I1)
  int ebwsave;  // (0/1) save exposure box-whisker data (I1)
  int zaasave;  // (0/1) save zones age-of-air transient data (I1)
  int zbwsave;  // (0/1) save zones age-of-air box-whisker data (I1)
  int rzfsave;  // (0/1) save binary zone flow file (I1)
  int rzmsave;  // (0/1) save binary avg zone mass fraction file (I1)
  int rz1save;  // (0/1) save binary 1D zone cell mass fraction file (I1)
  int csmsave;  // (0/1) save text contaminant summary file (I1)
  int srfsave;  // (0/1) save text surface result file (I1)
  int logsave;  // (0/1) save text controls log file (I1)
  std::vector<int> save;  // (unused by CONTAM; subject to change without notice) (I1)
  std::vector<RX> rvals;  // real values (R4)
  int BldgFlowZ;  // output building airflow test (zones) (IX)
  int BldgFlowD;  // output building airflow test (ducts) (IX)
  int BldgFlowC;  // output building airflow test (classified flows) (IX)
  int cfd_ctype;  // (0=no cfd, 1=post, 2=quasi, 3=dynamic) cfd coupling method (I2)
  RX cfd_convcpl;  // convergence factor for dynamic coupling (R4)
  int cfd_var;  // (0/1) use .var file (I2)
  int cfd_zref;  // currently not used (I2)
  int cfd_imax;  // max number of dynamic coupling iterations (I2)
  int cfd_dtcmo;  // number of iterations between outputs to .cmo file (I2)
};

class LevelPrivate : public QSharedData
{
public:
  LevelPrivate(int nr=0,RX refht=RX_INIT(0),RX delht=RX_INIT(0),int u_rfht=0,int u_dlht=0,std::string name=std::string(),std::vector<Icon> icons=std::vector<Icon>());
  void read(Reader &reader);
  std::string write();
  int nr;  // level number (IX), in order from 1 to nlev
  RX refht;  // reference elevation of level [m] (R4)
  RX delht;  // delta elevation to next level [m] (R4) {W}
  int u_rfht;  // units of reference elevation (I2) {W}
  int u_dlht;  // units of delta elevation (I2) {W}
  std::string name;  // level name (CS)
  std::vector<Icon> icons;  // level icons (Icon)
};

class DaySchedulePrivate : public QSharedData
{
public:
  DaySchedulePrivate(int nr=0,int shape=0,int utyp=0,int ucnv=0,std::string name=std::string(),std::string desc=std::string(),std::vector<SchedulePoint> points=std::vector<SchedulePoint>());
  void read(Reader &reader);
  std::string write();
  int nr;  // schedule number (IX); in order from 1 to _ndsch
  int shape;  // 0 = rectangular; 1 = trapezoidal (I2)
  int utyp;  // type of units (I2) {W}
  int ucnv;  // units conversion (I2) {W}
  std::string name;  // schedule name (CS) {W}
  std::string desc;  // schedule description (CS) {W} may be blank
  std::vector<SchedulePoint> points;  // control points (SchedulePoint)
};

class WeekSchedulePrivate : public QSharedData
{
public:
  WeekSchedulePrivate(int nr=0,int utyp=0,int ucnv=0,std::string name=std::string(),std::string desc=std::string(),std::vector<int> j=std::vector<int>());
  void read(Reader &reader);
  std::string write();
  int nr;  // schedule number (IX); in order from 1 to _nwsch
  int utyp;  // type of units (I2) {W}
  int ucnv;  // units conversion (I2) {W}
  std::string name;  // schedule name (CS) {W}
  std::string desc;  // schedule description (CS) {W} may be blank
  std::vector<int> j;  // 12 day schedule indices (IX) - converted to pointers
};

class WindPressureProfilePrivate : public QSharedData
{
public:
  WindPressureProfilePrivate(int nr=0,int type=0,std::string name=std::string(),std::string desc=std::string(),std::vector<PressureCoefficientPoint> coeffs=std::vector<PressureCoefficientPoint>());
  void read(Reader &reader);
  std::string write();
  int nr;  // profile number (IX); in order from 1 to _nwpf
  int type;  // 1 = linear; 2 = cubic spline; 3 = trigonometric (I2)
  std::string name;  // schedule name (CS) {W}
  std::string desc;  // profile description (CS) {W} may be blank
  std::vector<PressureCoefficientPoint> coeffs;  // wind pressure coefficient objects (PressureCoefficientPoint)
};

class CtrlDatPrivate : public QSharedData
{
public:
  CtrlDatPrivate(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string());
  void read(Reader &reader);
  std::string write(std::string datatype);

  int nr;  // node (SketchPad) number (IX); in order from 1 to _nctrl
  //std::string dataType;  // node data type (CS)
  int seqnr;  // computation sequence number (IX); set in ContamW
  unsigned int flags;  // flags for offset & scale, time constant, and 1D sensor (U2)
  int inreq;  // number of required inputs (I2) {W}
  int n1;  // SketchPad number of input node #1 (IX)
  int n2;  // SketchPad number of input node #2 (IX)
  std::string name;  // element name (CS) {W}
  std::string desc;  // control node description (CS) {W} may be blank
};

class SnsDatPrivate : public QSharedData
{
public:
  SnsDatPrivate(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),RX offset=RX_INIT(0),RX scale=RX_INIT(0),RX tau=RX_INIT(0),RX oldsig=RX_INIT(0),int source=0,int type=0,int measure=0,RX X=RX_INIT(0),RX Y=RX_INIT(0),RX relHt=RX_INIT(0),std::string units=std::string(),std::string species=std::string());
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // node (SketchPad) number (IX); in order from 1 to _nctrl
  //std::string dataType;  // node data type (CS)
  int seqnr;  // computation sequence number (IX); set in ContamW
  unsigned int flags;  // flags for offset & scale, time constant, and 1D sensor (U2)
  int inreq;  // number of required inputs (I2) {W}
  int n1;  // SketchPad number of input node #1 (IX)
  int n2;  // SketchPad number of input node #2 (IX)
  std::string name;  // element name (CS) {W}
  std::string desc;  // control node description (CS) {W} may be blank
  RX offset;  // offset value (R4)
  RX scale;  // scale value (R4)
  RX tau;  // time constant (R4)
  RX oldsig;  // signal at last time step - for restart (R4)
  int source;  // index of source (source not defined at read time) (IX)
  int type;  // type of source: 1=zone, 2=path, 3=junction, 4=duct, 5=exp, 6=term (I2)
  int measure;  // 0=contaminant, 1=temperature, 2=flow rate, 3=dP, 4=Pgage, 5=zone occupancy (I2)
  RX X;  // X-coordinate of sensor [m] (R4)
  RX Y;  // Y-coordinate of sensor [m] (R4)
  RX relHt;  // relative height of sensor [m] (R4)
  std::string units;  // units of coordinates {W} (CS)
  std::string species;  // species name [CS]; convert to pointer
};

class SchDatPrivate : public QSharedData
{
public:
  SchDatPrivate(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),int ps=0);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // node (SketchPad) number (IX); in order from 1 to _nctrl
  //std::string dataType;  // node data type (CS)
  int seqnr;  // computation sequence number (IX); set in ContamW
  unsigned int flags;  // flags for offset & scale, time constant, and 1D sensor (U2)
  int inreq;  // number of required inputs (I2) {W}
  int n1;  // SketchPad number of input node #1 (IX)
  int n2;  // SketchPad number of input node #2 (IX)
  std::string name;  // element name (CS) {W}
  std::string desc;  // control node description (CS) {W} may be blank
  int ps;  // week schedule index (I2); converted to pointer
};

class SetDatPrivate : public QSharedData
{
public:
  SetDatPrivate(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),RX value=RX_INIT(0));
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // node (SketchPad) number (IX); in order from 1 to _nctrl
  //std::string dataType;  // node data type (CS)
  int seqnr;  // computation sequence number (IX); set in ContamW
  unsigned int flags;  // flags for offset & scale, time constant, and 1D sensor (U2)
  int inreq;  // number of required inputs (I2) {W}
  int n1;  // SketchPad number of input node #1 (IX)
  int n2;  // SketchPad number of input node #2 (IX)
  std::string name;  // element name (CS) {W}
  std::string desc;  // control node description (CS) {W} may be blank
  RX value;  // constant value (R4)
};

class CdvDatPrivate : public QSharedData
{
public:
  CdvDatPrivate(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),std::string valuename=std::string());
  void read(Reader &reader);
  std::string write(std::string datatype);
  void readDetails(Reader &input);
  int nr;  // node (SketchPad) number (IX); in order from 1 to _nctrl
  //std::string dataType;  // node data type (CS)
  int seqnr;  // computation sequence number (IX); set in ContamW
  unsigned int flags;  // flags for offset & scale, time constant, and 1D sensor (U2)
  int inreq;  // number of required inputs (I2) {W}
  int n1;  // SketchPad number of input node #1 (IX)
  int n2;  // SketchPad number of input node #2 (IX)
  std::string name;  // element name (CS) {W}
  std::string desc;  // control node description (CS) {W} may be blank
  std::string valuename;  // name of the value read from the Discrete or Continuous Values file (CS)
};

class LogDatPrivate : public QSharedData
{
public:
  LogDatPrivate(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),RX offset=RX_INIT(0),RX scale=RX_INIT(0),int udef=0,std::string header=std::string(),std::string units=std::string());
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // node (SketchPad) number (IX); in order from 1 to _nctrl
  //std::string dataType;  // node data type (CS)
  int seqnr;  // computation sequence number (IX); set in ContamW
  unsigned int flags;  // flags for offset & scale, time constant, and 1D sensor (U2)
  int inreq;  // number of required inputs (I2) {W}
  int n1;  // SketchPad number of input node #1 (IX)
  int n2;  // SketchPad number of input node #2 (IX)
  std::string name;  // element name (CS) {W}
  std::string desc;  // control node description (CS) {W} may be blank
  RX offset;  // offset value (R4)
  RX scale;  // scale value (R4)
  int udef;  // true if using default units (I2) {W}
  std::string header;  // header string (CS)
  std::string units;  // units string (CS)
};

class ModDatPrivate : public QSharedData
{
public:
  ModDatPrivate(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),RX offset=RX_INIT(0),RX scale=RX_INIT(0));
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // node (SketchPad) number (IX); in order from 1 to _nctrl
  //std::string dataType;  // node data type (CS)
  int seqnr;  // computation sequence number (IX); set in ContamW
  unsigned int flags;  // flags for offset & scale, time constant, and 1D sensor (U2)
  int inreq;  // number of required inputs (I2) {W}
  int n1;  // SketchPad number of input node #1 (IX)
  int n2;  // SketchPad number of input node #2 (IX)
  std::string name;  // element name (CS) {W}
  std::string desc;  // control node description (CS) {W} may be blank
  RX offset;  // offset value (R4)
  RX scale;  // scale value (R4)
};

class HysDatPrivate : public QSharedData
{
public:
  HysDatPrivate(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),RX slack=RX_INIT(0),RX slope=RX_INIT(0),RX oldsig=RX_INIT(0));
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // node (SketchPad) number (IX); in order from 1 to _nctrl
  //std::string dataType;  // node data type (CS)
  int seqnr;  // computation sequence number (IX); set in ContamW
  unsigned int flags;  // flags for offset & scale, time constant, and 1D sensor (U2)
  int inreq;  // number of required inputs (I2) {W}
  int n1;  // SketchPad number of input node #1 (IX)
  int n2;  // SketchPad number of input node #2 (IX)
  std::string name;  // element name (CS) {W}
  std::string desc;  // control node description (CS) {W} may be blank
  RX slack;  // hysteresis parameter (R4)
  RX slope;  // 1.0 / (1.0 - slack) (R4)
  RX oldsig;  // prior output signal (R4)
};

class DlsDatPrivate : public QSharedData
{
public:
  DlsDatPrivate(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),int dsincr=0,int dsdecr=0);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // node (SketchPad) number (IX); in order from 1 to _nctrl
  //std::string dataType;  // node data type (CS)
  int seqnr;  // computation sequence number (IX); set in ContamW
  unsigned int flags;  // flags for offset & scale, time constant, and 1D sensor (U2)
  int inreq;  // number of required inputs (I2) {W}
  int n1;  // SketchPad number of input node #1 (IX)
  int n2;  // SketchPad number of input node #2 (IX)
  std::string name;  // element name (CS) {W}
  std::string desc;  // control node description (CS) {W} may be blank
  int dsincr;  // day schedule number for increasing signal (I2)
  int dsdecr;  // day schedule number for decreasing signal (I2)
};

class DlxDatPrivate : public QSharedData
{
public:
  DlxDatPrivate(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),int tauincr=0,int taudecr=0);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // node (SketchPad) number (IX); in order from 1 to _nctrl
  //std::string dataType;  // node data type (CS)
  int seqnr;  // computation sequence number (IX); set in ContamW
  unsigned int flags;  // flags for offset & scale, time constant, and 1D sensor (U2)
  int inreq;  // number of required inputs (I2) {W}
  int n1;  // SketchPad number of input node #1 (IX)
  int n2;  // SketchPad number of input node #2 (IX)
  std::string name;  // element name (CS) {W}
  std::string desc;  // control node description (CS) {W} may be blank
  int tauincr;  // time constant for increasing signal [s] (I4)
  int taudecr;  // time constant for decreasing signal [s] (I4)
};

class RavDatPrivate : public QSharedData
{
public:
  RavDatPrivate(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),int tspan=0);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // node (SketchPad) number (IX); in order from 1 to _nctrl
  //std::string dataType;  // node data type (CS)
  int seqnr;  // computation sequence number (IX); set in ContamW
  unsigned int flags;  // flags for offset & scale, time constant, and 1D sensor (U2)
  int inreq;  // number of required inputs (I2) {W}
  int n1;  // SketchPad number of input node #1 (IX)
  int n2;  // SketchPad number of input node #2 (IX)
  std::string name;  // element name (CS) {W}
  std::string desc;  // control node description (CS) {W} may be blank
  int tspan;  // time span for the running average [s] (I4)
};

class SumAvgPrivate : public QSharedData
{
public:
  SumAvgPrivate(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),std::vector<int> pc=std::vector<int>());
  void read(Reader &reader);
  std::string write(std::string datatype);
  void readDetails(Reader &input);
  int nr;  // node (SketchPad) number (IX); in order from 1 to _nctrl
  //std::string dataType;  // node data type (CS)
  int seqnr;  // computation sequence number (IX); set in ContamW
  unsigned int flags;  // flags for offset & scale, time constant, and 1D sensor (U2)
  int inreq;  // number of required inputs (I2) {W}
  int n1;  // SketchPad number of input node #1 (IX)
  int n2;  // SketchPad number of input node #2 (IX)
  std::string name;  // element name (CS) {W}
  std::string desc;  // control node description (CS) {W} may be blank
  std::vector<int> pc;  // indices of npcs control nodes (IX)
};

class BanDatPrivate : public QSharedData
{
public:
  BanDatPrivate(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),RX band=RX_INIT(0));
  void read(Reader &reader);
  std::string write(std::string datatype);
  void readDetails(Reader &input);
  int nr;  // node (SketchPad) number (IX); in order from 1 to _nctrl
  //std::string dataType;  // node data type (CS)
  int seqnr;  // computation sequence number (IX); set in ContamW
  unsigned int flags;  // flags for offset & scale, time constant, and 1D sensor (U2)
  int inreq;  // number of required inputs (I2) {W}
  int n1;  // SketchPad number of input node #1 (IX)
  int n2;  // SketchPad number of input node #2 (IX)
  std::string name;  // element name (CS) {W}
  std::string desc;  // control node description (CS) {W} may be blank
  RX band;  // width of band (R4)
};

class PcDatPrivate : public QSharedData
{
public:
  PcDatPrivate(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),RX kp=RX_INIT(0));
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // node (SketchPad) number (IX); in order from 1 to _nctrl
  //std::string dataType;  // node data type (CS)
  int seqnr;  // computation sequence number (IX); set in ContamW
  unsigned int flags;  // flags for offset & scale, time constant, and 1D sensor (U2)
  int inreq;  // number of required inputs (I2) {W}
  int n1;  // SketchPad number of input node #1 (IX)
  int n2;  // SketchPad number of input node #2 (IX)
  std::string name;  // element name (CS) {W}
  std::string desc;  // control node description (CS) {W} may be blank
  RX kp;  // proportional gain factor (R4)
};

class PicDatPrivate : public QSharedData
{
public:
  PicDatPrivate(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),RX kp=RX_INIT(0),RX ki=RX_INIT(0),RX oldsig=RX_INIT(0),RX olderr=RX_INIT(0));
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // node (SketchPad) number (IX); in order from 1 to _nctrl
  //std::string dataType;  // node data type (CS)
  int seqnr;  // computation sequence number (IX); set in ContamW
  unsigned int flags;  // flags for offset & scale, time constant, and 1D sensor (U2)
  int inreq;  // number of required inputs (I2) {W}
  int n1;  // SketchPad number of input node #1 (IX)
  int n2;  // SketchPad number of input node #2 (IX)
  std::string name;  // element name (CS) {W}
  std::string desc;  // control node description (CS) {W} may be blank
  RX kp;  // proportional gain factor (R4)
  RX ki;  // integral gain factor (R4)
  RX oldsig;  // prior output signal - for restart (R4)
  RX olderr;  // prior error value - for restart (R4)
};

class SupDatPrivate : public QSharedData
{
public:
  SupDatPrivate(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),int def=0,int se=0,int in=0,int out=0);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // node (SketchPad) number (IX); in order from 1 to _nctrl
  //std::string dataType;  // node data type (CS)
  int seqnr;  // computation sequence number (IX); set in ContamW
  unsigned int flags;  // flags for offset & scale, time constant, and 1D sensor (U2)
  int inreq;  // number of required inputs (I2) {W}
  int n1;  // SketchPad number of input node #1 (IX)
  int n2;  // SketchPad number of input node #2 (IX)
  std::string name;  // element name (CS) {W}
  std::string desc;  // control node description (CS) {W} may be blank
  int def;  // 0:super element undefined 1:defined (I2)
  int se;  // super element number (IX)
  int in;  // input sub-node number (IX)
  int out;  // output sub-node number (IX)
};

class PlrOrfPrivate : public QSharedData
{
public:
  PlrOrfPrivate(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),RX expt=RX_INIT(0),RX area=RX_INIT(0),RX dia=RX_INIT(0),RX coef=RX_INIT(0),RX Re=RX_INIT(0),int u_A=0,int u_D=0);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // element number (IX); in order from 1 to _nafe
  int icon;  // icon used to represent flow path (I2) {W}
  //std::string dataType;  // element data type (CS) element type names are stored in _afe_dnames in dtype order.
  std::string name;  // element name (CS) {W}
  std::string desc;  // element description (CS) {W} may be blank
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

class PlrLeakPrivate : public QSharedData
{
public:
  PlrLeakPrivate(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),RX expt=RX_INIT(0),RX coef=RX_INIT(0),RX pres=RX_INIT(0),RX area1=RX_INIT(0),RX area2=RX_INIT(0),RX area3=RX_INIT(0),int u_A1=0,int u_A2=0,int u_A3=0,int u_dP=0);
  void read(Reader &reader);
  std::string write(std::string datatype);
  void readDetails(Reader &input);
  int nr;  // element number (IX); in order from 1 to _nafe
  int icon;  // icon used to represent flow path (I2) {W}
  //std::string dataType;  // element data type (CS) element type names are stored in _afe_dnames in dtype order.
  std::string name;  // element name (CS) {W}
  std::string desc;  // element description (CS) {W} may be blank
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

class PlrConnPrivate : public QSharedData
{
public:
  PlrConnPrivate(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),RX expt=RX_INIT(0),RX area=RX_INIT(0),RX coef=RX_INIT(0),int u_A=0);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // element number (IX); in order from 1 to _nafe
  int icon;  // icon used to represent flow path (I2) {W}
  // std::string dataType;  // element data type (CS) element type names are stored in _afe_dnames in dtype order.
  std::string name;  // element name (CS) {W}
  std::string desc;  // element description (CS) {W} may be blank
  RX lam;  // laminar flow coefficient (R4)
  RX turb;  // turbulent flow coefficient (R4)
  RX expt;  // pressure exponent - 0.5 (R4)
  RX area;  // actual area [m^2] (R4) {W}
  RX coef;  // flow coefficient (R4) {W}
  int u_A;  // units of area (I2) {W}
};

class PlrQcnPrivate : public QSharedData
{
public:
  PlrQcnPrivate(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),RX expt=RX_INIT(0));
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // element number (IX); in order from 1 to _nafe
  int icon;  // icon used to represent flow path (I2) {W}
  // std::string dataType;  // element data type (CS) element type names are stored in _afe_dnames in dtype order.
  std::string name;  // element name (CS) {W}
  std::string desc;  // element description (CS) {W} may be blank
  RX lam;  // laminar flow coefficient (R4)
  RX turb;  // turbulent flow coefficient (R4)
  RX expt;  // pressure exponent (R4)
};

class PlrFcnPrivate : public QSharedData
{
public:
  PlrFcnPrivate(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),RX expt=RX_INIT(0));
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // element number (IX); in order from 1 to _nafe
  int icon;  // icon used to represent flow path (I2) {W}
  // std::string dataType;  // element data type (CS) element type names are stored in _afe_dnames in dtype order.
  std::string name;  // element name (CS) {W}
  std::string desc;  // element description (CS) {W} may be blank
  RX lam;  // laminar flow coefficient (R4)
  RX turb;  // turbulent flow coefficient (R4)
  RX expt;  // pressure exponent (R4)
};

class PlrTest1Private : public QSharedData
{
public:
  PlrTest1Private(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),RX expt=RX_INIT(0),RX dP=RX_INIT(0),RX Flow=RX_INIT(0),int u_P=0,int u_F=0);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // element number (IX); in order from 1 to _nafe
  int icon;  // icon used to represent flow path (I2) {W}
  // std::string dataType;  // element data type (CS) element type names are stored in _afe_dnames in dtype order.
  std::string name;  // element name (CS) {W}
  std::string desc;  // element description (CS) {W} may be blank
  RX lam;  // laminar flow coefficient (R4)
  RX turb;  // turbulent flow coefficient (R4)
  RX expt;  // pressure exponent (R4)
  RX dP;  // pressure drop [Pa] (R4) {W}
  RX Flow;  // flow rate [kg/s] (R4) {W}
  int u_P;  // units of pressure drop (I2) {W}
  int u_F;  // units of flow (I2) {W}
};

class PlrTest2Private : public QSharedData
{
public:
  PlrTest2Private(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),RX expt=RX_INIT(0),RX dP1=RX_INIT(0),RX F1=RX_INIT(0),RX dP2=RX_INIT(0),RX F2=RX_INIT(0),int u_P1=0,int u_F1=0,int u_P2=0,int u_F2=0);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // element number (IX); in order from 1 to _nafe
  int icon;  // icon used to represent flow path (I2) {W}
  // std::string dataType;  // element data type (CS) element type names are stored in _afe_dnames in dtype order.
  std::string name;  // element name (CS) {W}
  std::string desc;  // element description (CS) {W} may be blank
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

class PlrCrackPrivate : public QSharedData
{
public:
  PlrCrackPrivate(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),RX expt=RX_INIT(0),RX length=RX_INIT(0),RX width=RX_INIT(0),int u_L=0,int u_W=0);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // element number (IX); in order from 1 to _nafe
  int icon;  // icon used to represent flow path (I2) {W}
  // std::string dataType;  // element data type (CS) element type names are stored in _afe_dnames in dtype order.
  std::string name;  // element name (CS) {W}
  std::string desc;  // element description (CS) {W} may be blank
  RX lam;  // laminar flow coefficient (R4)
  RX turb;  // turbulent flow coefficient (R4)
  RX expt;  // pressure exponent (R4)
  RX length;  // crack length [m] (R4) {W}
  RX width;  // crack width [m] (R4) {W}
  int u_L;  // units of length (I2) {W}
  int u_W;  // units of width (I2) {W}
};

class PlrStairPrivate : public QSharedData
{
public:
  PlrStairPrivate(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),RX expt=RX_INIT(0),RX Ht=RX_INIT(0),RX Area=RX_INIT(0),RX peo=RX_INIT(0),int tread=0,int u_A=0,int u_D=0);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // element number (IX); in order from 1 to _nafe
  int icon;  // icon used to represent flow path (I2) {W}
  // std::string dataType;  // element data type (CS) element type names are stored in _afe_dnames in dtype order.
  std::string name;  // element name (CS) {W}
  std::string desc;  // element description (CS) {W} may be blank
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

class PlrShaftPrivate : public QSharedData
{
public:
  PlrShaftPrivate(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),RX expt=RX_INIT(0),RX Ht=RX_INIT(0),RX area=RX_INIT(0),RX perim=RX_INIT(0),RX rough=RX_INIT(0),int u_A=0,int u_D=0,int u_P=0,int u_R=0);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // element number (IX); in order from 1 to _nafe
  int icon;  // icon used to represent flow path (I2) {W}
  // std::string dataType;  // element data type (CS) element type names are stored in _afe_dnames in dtype order.
  std::string name;  // element name (CS) {W}
  std::string desc;  // element description (CS) {W} may be blank
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

class PlrBdqPrivate : public QSharedData
{
public:
  PlrBdqPrivate(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX Cp=RX_INIT(0),RX xp=RX_INIT(0),RX Cn=RX_INIT(0),RX xn=RX_INIT(0));
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // element number (IX); in order from 1 to _nafe
  int icon;  // icon used to represent flow path (I2) {W}
  // std::string dataType;  // element data type (CS) element type names are stored in _afe_dnames in dtype order.
  std::string name;  // element name (CS) {W}
  std::string desc;  // element description (CS) {W} may be blank
  RX lam;  // laminar flow coefficient {R4}
  RX Cp;  // turbulent flow coefficient ( dP > 0 ) {R4}
  RX xp;  // pressure exponent ( dP > 0 ) {R4}
  RX Cn;  // turbulent flow coefficient ( dP < 0 ) {R4}
  RX xn;  // pressure exponent ( dP < 0 ) {R4}
};

class PlrBdfPrivate : public QSharedData
{
public:
  PlrBdfPrivate(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX Cp=RX_INIT(0),RX xp=RX_INIT(0),RX Cn=RX_INIT(0),RX xn=RX_INIT(0));
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // element number (IX); in order from 1 to _nafe
  int icon;  // icon used to represent flow path (I2) {W}
  // std::string dataType;  // element data type (CS) element type names are stored in _afe_dnames in dtype order.
  std::string name;  // element name (CS) {W}
  std::string desc;  // element description (CS) {W} may be blank
  RX lam;  // laminar flow coefficient {R4}
  RX Cp;  // turbulent flow coefficient ( dP > 0 ) {R4}
  RX xp;  // pressure exponent ( dP > 0 ) {R4}
  RX Cn;  // turbulent flow coefficient ( dP < 0 ) {R4}
  RX xn;  // pressure exponent ( dP < 0 ) {R4}
};

class QfrQabPrivate : public QSharedData
{
public:
  QfrQabPrivate(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX a=RX_INIT(0),RX b=RX_INIT(0));
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // element number (IX); in order from 1 to _nafe
  int icon;  // icon used to represent flow path (I2) {W}
  // std::string dataType;  // element data type (CS) element type names are stored in _afe_dnames in dtype order.
  std::string name;  // element name (CS) {W}
  std::string desc;  // element description (CS) {W} may be blank
  RX a;  // dP = a*Q + b*Q*Q {R4}
  RX b;  // {R4}
};

class QfrFabPrivate : public QSharedData
{
public:
  QfrFabPrivate(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX a=RX_INIT(0),RX b=RX_INIT(0));
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // element number (IX); in order from 1 to _nafe
  int icon;  // icon used to represent flow path (I2) {W}
  // std::string dataType;  // element data type (CS) element type names are stored in _afe_dnames in dtype order.
  std::string name;  // element name (CS) {W}
  std::string desc;  // element description (CS) {W} may be blank
  RX a;  // dP = a*F + b*F*F {R4}
  RX b;  // {R4}
};

class QfrCrackPrivate : public QSharedData
{
public:
  QfrCrackPrivate(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX a=RX_INIT(0),RX b=RX_INIT(0),RX length=RX_INIT(0),RX width=RX_INIT(0),RX depth=RX_INIT(0),int nB=0,int u_L=0,int u_W=0,int u_D=0);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // element number (IX); in order from 1 to _nafe
  int icon;  // icon used to represent flow path (I2) {W}
  // std::string dataType;  // element data type (CS) element type names are stored in _afe_dnames in dtype order.
  std::string name;  // element name (CS) {W}
  std::string desc;  // element description (CS) {W} may be blank
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

class QfrTest2Private : public QSharedData
{
public:
  QfrTest2Private(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX a=RX_INIT(0),RX b=RX_INIT(0),RX dP1=RX_INIT(0),RX F1=RX_INIT(0),RX dP2=RX_INIT(0),RX F2=RX_INIT(0),int u_P1=0,int u_F1=0,int u_P2=0,int u_F2=0);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // element number (IX); in order from 1 to _nafe
  int icon;  // icon used to represent flow path (I2) {W}
  // std::string dataType;  // element data type (CS) element type names are stored in _afe_dnames in dtype order.
  std::string name;  // element name (CS) {W}
  std::string desc;  // element description (CS) {W} may be blank
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

class AfeDorPrivate : public QSharedData
{
public:
  AfeDorPrivate(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),RX expt=RX_INIT(0),RX dTmin=RX_INIT(0),RX ht=RX_INIT(0),RX wd=RX_INIT(0),RX cd=RX_INIT(0),int u_T=0,int u_H=0,int u_W=0);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // element number (IX); in order from 1 to _nafe
  int icon;  // icon used to represent flow path (I2) {W}
  // std::string dataType;  // element data type (CS) element type names are stored in _afe_dnames in dtype order.
  std::string name;  // element name (CS) {W}
  std::string desc;  // element description (CS) {W} may be blank
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

class DrPl2Private : public QSharedData
{
public:
  DrPl2Private(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),RX expt=RX_INIT(0),RX dH=RX_INIT(0),RX ht=RX_INIT(0),RX wd=RX_INIT(0),RX cd=RX_INIT(0),int u_H=0,int u_W=0);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // element number (IX); in order from 1 to _nafe
  int icon;  // icon used to represent flow path (I2) {W}
  // std::string dataType;  // element data type (CS) element type names are stored in _afe_dnames in dtype order.
  std::string name;  // element name (CS) {W}
  std::string desc;  // element description (CS) {W} may be blank
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

class AfeCmfPrivate : public QSharedData
{
public:
  AfeCmfPrivate(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX Flow=RX_INIT(0),int u_F=0);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // element number (IX); in order from 1 to _nafe
  int icon;  // icon used to represent flow path (I2) {W}
  // std::string dataType;  // element data type (CS) element type names are stored in _afe_dnames in dtype order.
  std::string name;  // element name (CS) {W}
  std::string desc;  // element description (CS) {W} may be blank
  RX Flow;  // design flow rate [kg/s] (R4)
  int u_F;  // units of flow (I2) {W}
};

class AfeCvfPrivate : public QSharedData
{
public:
  AfeCvfPrivate(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX Flow=RX_INIT(0),int u_F=0);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // element number (IX); in order from 1 to _nafe
  int icon;  // icon used to represent flow path (I2) {W}
  // std::string dataType;  // element data type (CS) element type names are stored in _afe_dnames in dtype order.
  std::string name;  // element name (CS) {W}
  std::string desc;  // element description (CS) {W} may be blank
  RX Flow;  // design flow rate [m^3/s] (R4)
  int u_F;  // units of flow (I2) {W}
};

class AfeFanPrivate : public QSharedData
{
public:
  AfeFanPrivate(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),RX expt=RX_INIT(0),RX rdens=RX_INIT(0),RX fdf=RX_INIT(0),RX sop=RX_INIT(0),RX off=RX_INIT(0),std::vector<RX> fpc=std::vector<RX>(),RX Sarea=RX_INIT(0),int u_Sa=0,std::vector<FanDataPoint> data=std::vector<FanDataPoint>());
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // element number (IX); in order from 1 to _nafe
  int icon;  // icon used to represent flow path (I2) {W}
  // std::string dataType;  // element data type (CS) element type names are stored in _afe_dnames in dtype order.
  std::string name;  // element name (CS) {W}
  std::string desc;  // element description (CS) {W} may be blank
  RX lam;  // laminar flow coefficient (R4)
  RX turb;  // turbulent flow coefficient (R4)
  RX expt;  // pressure exponent (R4)
  RX rdens;  // reference fluid density [kg/m^3] {R4}
  RX fdf;  // free delivery flow (prise = 0) [kg/s] {R4}
  RX sop;  // shut-off pressure (flow = 0) [Pa] {R4}
  RX off;  // fan is off if (RPM/rated RPM) < off {R4}
  std::vector<RX> fpc;  // fan performance polynomial coefficients {R4}
  RX Sarea;  // shut-off orifice area [m^2] {R4} {W}
  int u_Sa;  // units of shut-off area (I2) {W}
  std::vector<FanDataPoint> data;  // measured fan data points (FanDataPoint)
};

class AfeCsfPrivate : public QSharedData
{
public:
  AfeCsfPrivate(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),int u_x=0,int u_y=0,std::vector<DataPoint> data=std::vector<DataPoint>());
  void read(Reader &reader);
  std::string write(std::string datatype);
  void readDetails(Reader &input);
  int nr;  // element number (IX); in order from 1 to _nafe
  int icon;  // icon used to represent flow path (I2) {W}
  // std::string dataType;  // element data type (CS) element type names are stored in _afe_dnames in dtype order.
  std::string name;  // element name (CS) {W}
  std::string desc;  // element description (CS) {W} may be blank
  int u_x;  // units for x (I2)
  int u_y;  // units for y (I2)
  std::vector<DataPoint> data;  // data points (DataPoint)
};

class AfeSupPrivate : public QSharedData
{
public:
  AfeSupPrivate(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),int sched=0,int u_H=0,std::vector<AirflowSubelementData> subelements=std::vector<AirflowSubelementData>());
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);
  int nr;  // element number (IX); in order from 1 to _nafe
  int icon;  // icon used to represent flow path (I2) {W}
  // std::string dataType;  // element data type (CS) element type names are stored in _afe_dnames in dtype order.
  std::string name;  // element name (CS) {W}
  std::string desc;  // element description (CS) {W} may be blank
  int sched;  // scheduled sub-element number (IX) (only one)
  int u_H;  // units of relative height (I2)
  std::vector<AirflowSubelementData> subelements;  // Subelement data (AirflowSubelementData)
};

} // contam
} // openstudio

#endif
