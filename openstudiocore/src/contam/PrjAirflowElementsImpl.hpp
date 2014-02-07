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
#ifndef PRJAIRFLOWELEMENTSIMPL_H
#define PRJAIRFLOWELEMENTSIMPL_H

#include "PrjDefines.hpp"
#include "PrjReader.hpp"
#include "PrjSubobjects.hpp"
#include <QSharedData>

namespace openstudio {
namespace contam {

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
