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
#include "PrjAirflowElementsImpl.hpp"

namespace openstudio {
namespace contam {

PlrOrfPrivate::PlrOrfPrivate(int nr,int icon,std::string name,std::string desc,RX lam,RX turb,RX expt,RX area,RX dia,RX coef,RX Re,int u_A,int u_D):nr(nr),icon(icon),name(name),desc(desc),lam(lam),turb(turb),expt(expt),area(area),dia(dia),coef(coef),Re(Re),u_A(u_A),u_D(u_D)
{}

void PlrOrfPrivate::read(Reader &input)
{
  nr = input.read<int>(FILELINE);
  icon = input.read<int>(FILELINE);
  std::string dataType = input.readString(FILELINE);
  name = input.readString(FILELINE);
  desc = input.readLine(FILELINE);
  lam = input.readNumber<RX>(FILELINE);
  turb = input.readNumber<RX>(FILELINE);
  expt = input.readNumber<RX>(FILELINE);
  area = input.readNumber<RX>(FILELINE);
  dia = input.readNumber<RX>(FILELINE);
  coef = input.readNumber<RX>(FILELINE);
  Re = input.readNumber<RX>(FILELINE);
  u_A = input.read<int>(FILELINE);
  u_D = input.read<int>(FILELINE);
}

std::string PlrOrfPrivate::write()
{
  std::string string;
  string += openstudio::toString(nr) + ' ' + openstudio::toString(icon) + " plr_orfc " + name + '\n';
  string += desc + '\n';
  string += openstudio::toString(lam) + ' ' + openstudio::toString(turb) + ' ' + openstudio::toString(expt) + ' ' + openstudio::toString(area) + ' ' + openstudio::toString(dia) + ' ' + openstudio::toString(coef) + ' ' + openstudio::toString(Re) + ' ' + openstudio::toString(u_A) + ' ' + openstudio::toString(u_D) + '\n';
  return string;
}

void PlrOrfPrivate::readDetails(Reader &input)
{
  lam = input.readNumber<RX>(FILELINE);
  turb = input.readNumber<RX>(FILELINE);
  expt = input.readNumber<RX>(FILELINE);
  area = input.readNumber<RX>(FILELINE);
  dia = input.readNumber<RX>(FILELINE);
  coef = input.readNumber<RX>(FILELINE);
  Re = input.readNumber<RX>(FILELINE);
  u_A = input.read<int>(FILELINE);
  u_D = input.read<int>(FILELINE);
}

PlrLeakPrivate::PlrLeakPrivate(int nr,int icon,std::string name,std::string desc,RX lam,RX turb,RX expt,RX coef,RX pres,RX area1,RX area2,RX area3,int u_A1,int u_A2,int u_A3,int u_dP):nr(nr),icon(icon),name(name),desc(desc),lam(lam),turb(turb),expt(expt),coef(coef),pres(pres),area1(area1),area2(area2),area3(area3),u_A1(u_A1),u_A2(u_A2),u_A3(u_A3),u_dP(u_dP)
{}

void PlrLeakPrivate::read(Reader &input)
{
  nr = input.read<int>(FILELINE);
  icon = input.read<int>(FILELINE);
  std::string dataType = input.readString(FILELINE);
  name = input.readString(FILELINE);
  desc = input.readLine(FILELINE);
  lam = input.readNumber<RX>(FILELINE);
  turb = input.readNumber<RX>(FILELINE);
  expt = input.readNumber<RX>(FILELINE);
  coef = input.readNumber<RX>(FILELINE);
  pres = input.readNumber<RX>(FILELINE);
  area1 = input.readNumber<RX>(FILELINE);
  area2 = input.readNumber<RX>(FILELINE);
  area3 = input.readNumber<RX>(FILELINE);
  u_A1 = input.read<int>(FILELINE);
  u_A2 = input.read<int>(FILELINE);
  u_A3 = input.read<int>(FILELINE);
  u_dP = input.read<int>(FILELINE);
}

std::string PlrLeakPrivate::write(std::string datatype)
{
  std::string string;
  string += openstudio::toString(nr) + ' ' + openstudio::toString(icon) + ' ' + datatype + ' ' + name + '\n';
  string += desc + '\n';
  string += openstudio::toString(lam) + ' ' + openstudio::toString(turb) + ' ' + openstudio::toString(expt) + ' ' + openstudio::toString(coef) + ' ' + openstudio::toString(pres) + ' ' + openstudio::toString(area1) + ' ' + openstudio::toString(area2) + ' ' + openstudio::toString(area3) + ' ' + openstudio::toString(u_A1) + ' ' + openstudio::toString(u_A2) + ' ' + openstudio::toString(u_A3) + ' ' + openstudio::toString(u_dP) + '\n';
  return string;
}

void PlrLeakPrivate::readDetails(Reader &input)
{
  lam = input.readNumber<RX>(FILELINE);
  turb = input.readNumber<RX>(FILELINE);
  expt = input.readNumber<RX>(FILELINE);
  coef = input.readNumber<RX>(FILELINE);
  pres = input.readNumber<RX>(FILELINE);
  area1 = input.readNumber<RX>(FILELINE);
  area2 = input.readNumber<RX>(FILELINE);
  area3 = input.readNumber<RX>(FILELINE);
  u_A1 = input.read<int>(FILELINE);
  u_A2 = input.read<int>(FILELINE);
  u_A3 = input.read<int>(FILELINE);
  u_dP = input.read<int>(FILELINE);
}

PlrConnPrivate::PlrConnPrivate(int nr,int icon,std::string name,std::string desc,RX lam,RX turb,RX expt,RX area,RX coef,int u_A):nr(nr),icon(icon),name(name),desc(desc),lam(lam),turb(turb),expt(expt),area(area),coef(coef),u_A(u_A)
{}

void PlrConnPrivate::read(Reader &input)
{
  nr = input.read<int>(FILELINE);
  icon = input.read<int>(FILELINE);
  std::string dataType = input.readString(FILELINE);
  name = input.readString(FILELINE);
  desc = input.readLine(FILELINE);
  lam = input.readNumber<RX>(FILELINE);
  turb = input.readNumber<RX>(FILELINE);
  expt = input.readNumber<RX>(FILELINE);
  area = input.readNumber<RX>(FILELINE);
  coef = input.readNumber<RX>(FILELINE);
  u_A = input.read<int>(FILELINE);
}

std::string PlrConnPrivate::write()
{
  std::string string;
  string += openstudio::toString(nr) + ' ' + openstudio::toString(icon) + " plr_conn " + name + '\n';
  string += desc + '\n';
  string += openstudio::toString(lam) + ' ' + openstudio::toString(turb) + ' ' + openstudio::toString(expt) + ' ' + openstudio::toString(area) + ' ' + openstudio::toString(coef) + ' ' + openstudio::toString(u_A) + '\n';
  return string;
}

void PlrConnPrivate::readDetails(Reader &input)
{
  lam = input.readNumber<RX>(FILELINE);
  turb = input.readNumber<RX>(FILELINE);
  expt = input.readNumber<RX>(FILELINE);
  area = input.readNumber<RX>(FILELINE);
  coef = input.readNumber<RX>(FILELINE);
  u_A = input.read<int>(FILELINE);
}

PlrQcnPrivate::PlrQcnPrivate(int nr,int icon,std::string name,std::string desc,RX lam,RX turb,RX expt):nr(nr),icon(icon),name(name),desc(desc),lam(lam),turb(turb),expt(expt)
{}

void PlrQcnPrivate::read(Reader &input)
{
  nr = input.read<int>(FILELINE);
  icon = input.read<int>(FILELINE);
  std::string dataType = input.readString(FILELINE);
  name = input.readString(FILELINE);
  desc = input.readLine(FILELINE);
  lam = input.readNumber<RX>(FILELINE);
  turb = input.readNumber<RX>(FILELINE);
  expt = input.readNumber<RX>(FILELINE);
}

std::string PlrQcnPrivate::write()
{
  std::string string;
  string += openstudio::toString(nr) + ' ' + openstudio::toString(icon) + " plr_qcn " + name + '\n';
  string += desc + '\n';
  string += openstudio::toString(lam) + ' ' + openstudio::toString(turb) + ' ' + openstudio::toString(expt) + '\n';
  return string;
}

void PlrQcnPrivate::readDetails(Reader &input)
{
  lam = input.readNumber<RX>(FILELINE);
  turb = input.readNumber<RX>(FILELINE);
  expt = input.readNumber<RX>(FILELINE);
}

PlrFcnPrivate::PlrFcnPrivate(int nr,int icon,std::string name,std::string desc,RX lam,RX turb,RX expt):nr(nr),icon(icon),name(name),desc(desc),lam(lam),turb(turb),expt(expt)
{}

void PlrFcnPrivate::read(Reader &input)
{
  nr = input.read<int>(FILELINE);
  icon = input.read<int>(FILELINE);
  std::string dataType = input.readString(FILELINE);
  name = input.readString(FILELINE);
  desc = input.readLine(FILELINE);
  lam = input.readNumber<RX>(FILELINE);
  turb = input.readNumber<RX>(FILELINE);
  expt = input.readNumber<RX>(FILELINE);
}

std::string PlrFcnPrivate::write()
{
  std::string string;
  string += openstudio::toString(nr) + ' ' + openstudio::toString(icon) + " plr_fcn " + name + '\n';
  string += desc + '\n';
  string += openstudio::toString(lam) + ' ' + openstudio::toString(turb) + ' ' + openstudio::toString(expt) + '\n';
  return string;
}

void PlrFcnPrivate::readDetails(Reader &input)
{
  lam = input.readNumber<RX>(FILELINE);
  turb = input.readNumber<RX>(FILELINE);
  expt = input.readNumber<RX>(FILELINE);
}

PlrTest1Private::PlrTest1Private(int nr,int icon,std::string name,std::string desc,RX lam,RX turb,RX expt,RX dP,RX Flow,int u_P,int u_F):nr(nr),icon(icon),name(name),desc(desc),lam(lam),turb(turb),expt(expt),dP(dP),Flow(Flow),u_P(u_P),u_F(u_F)
{}

void PlrTest1Private::read(Reader &input)
{
  nr = input.read<int>(FILELINE);
  icon = input.read<int>(FILELINE);
  std::string dataType = input.readString(FILELINE);
  name = input.readString(FILELINE);
  desc = input.readLine(FILELINE);
  lam = input.readNumber<RX>(FILELINE);
  turb = input.readNumber<RX>(FILELINE);
  expt = input.readNumber<RX>(FILELINE);
  dP = input.readNumber<RX>(FILELINE);
  Flow = input.readNumber<RX>(FILELINE);
  u_P = input.read<int>(FILELINE);
  u_F = input.read<int>(FILELINE);
}

std::string PlrTest1Private::write()
{
  std::string string;
  string += openstudio::toString(nr) + ' ' + openstudio::toString(icon) + " plr_test1 " + name + '\n';
  string += desc + '\n';
  string += openstudio::toString(lam) + ' ' + openstudio::toString(turb) + ' ' + openstudio::toString(expt) + ' ' + openstudio::toString(dP) + ' ' + openstudio::toString(Flow) + ' ' + openstudio::toString(u_P) + ' ' + openstudio::toString(u_F) + '\n';
  return string;
}

void PlrTest1Private::readDetails(Reader &input)
{
  lam = input.readNumber<RX>(FILELINE);
  turb = input.readNumber<RX>(FILELINE);
  expt = input.readNumber<RX>(FILELINE);
  dP = input.readNumber<RX>(FILELINE);
  Flow = input.readNumber<RX>(FILELINE);
  u_P = input.read<int>(FILELINE);
  u_F = input.read<int>(FILELINE);
}

PlrTest2Private::PlrTest2Private(int nr,int icon,std::string name,std::string desc,RX lam,RX turb,RX expt,RX dP1,RX F1,RX dP2,RX F2,int u_P1,int u_F1,int u_P2,int u_F2):nr(nr),icon(icon),name(name),desc(desc),lam(lam),turb(turb),expt(expt),dP1(dP1),F1(F1),dP2(dP2),F2(F2),u_P1(u_P1),u_F1(u_F1),u_P2(u_P2),u_F2(u_F2)
{}

void PlrTest2Private::read(Reader &input)
{
  nr = input.read<int>(FILELINE);
  icon = input.read<int>(FILELINE);
  std::string dataType = input.readString(FILELINE);
  name = input.readString(FILELINE);
  desc = input.readLine(FILELINE);
  lam = input.readNumber<RX>(FILELINE);
  turb = input.readNumber<RX>(FILELINE);
  expt = input.readNumber<RX>(FILELINE);
  dP1 = input.readNumber<RX>(FILELINE);
  F1 = input.readNumber<RX>(FILELINE);
  dP2 = input.readNumber<RX>(FILELINE);
  F2 = input.readNumber<RX>(FILELINE);
  u_P1 = input.read<int>(FILELINE);
  u_F1 = input.read<int>(FILELINE);
  u_P2 = input.read<int>(FILELINE);
  u_F2 = input.read<int>(FILELINE);
}

std::string PlrTest2Private::write()
{
  std::string string;
  string += openstudio::toString(nr) + ' ' + openstudio::toString(icon) + " plr_test2 " + name + '\n';
  string += desc + '\n';
  string += openstudio::toString(lam) + ' ' + openstudio::toString(turb) + ' ' + openstudio::toString(expt) + ' ' + openstudio::toString(dP1) + ' ' + openstudio::toString(F1) + ' ' + openstudio::toString(dP2) + ' ' + openstudio::toString(F2) + ' ' + openstudio::toString(u_P1) + ' ' + openstudio::toString(u_F1) + ' ' + openstudio::toString(u_P2) + ' ' + openstudio::toString(u_F2) + '\n';
  return string;
}

void PlrTest2Private::readDetails(Reader &input)
{
  lam = input.readNumber<RX>(FILELINE);
  turb = input.readNumber<RX>(FILELINE);
  expt = input.readNumber<RX>(FILELINE);
  dP1 = input.readNumber<RX>(FILELINE);
  F1 = input.readNumber<RX>(FILELINE);
  dP2 = input.readNumber<RX>(FILELINE);
  F2 = input.readNumber<RX>(FILELINE);
  u_P1 = input.read<int>(FILELINE);
  u_F1 = input.read<int>(FILELINE);
  u_P2 = input.read<int>(FILELINE);
  u_F2 = input.read<int>(FILELINE);
}

PlrCrackPrivate::PlrCrackPrivate(int nr,int icon,std::string name,std::string desc,RX lam,RX turb,RX expt,RX length,RX width,int u_L,int u_W):nr(nr),icon(icon),name(name),desc(desc),lam(lam),turb(turb),expt(expt),length(length),width(width),u_L(u_L),u_W(u_W)
{}

void PlrCrackPrivate::read(Reader &input)
{
  nr = input.read<int>(FILELINE);
  icon = input.read<int>(FILELINE);
  std::string dataType = input.readString(FILELINE);
  name = input.readString(FILELINE);
  desc = input.readLine(FILELINE);
  lam = input.readNumber<RX>(FILELINE);
  turb = input.readNumber<RX>(FILELINE);
  expt = input.readNumber<RX>(FILELINE);
  length = input.readNumber<RX>(FILELINE);
  width = input.readNumber<RX>(FILELINE);
  u_L = input.read<int>(FILELINE);
  u_W = input.read<int>(FILELINE);
}

std::string PlrCrackPrivate::write()
{
  std::string string;
  string += openstudio::toString(nr) + ' ' + openstudio::toString(icon) + " plr_crack " + name + '\n';
  string += desc + '\n';
  string += openstudio::toString(lam) + ' ' + openstudio::toString(turb) + ' ' + openstudio::toString(expt) + ' ' + openstudio::toString(length) + ' ' + openstudio::toString(width) + ' ' + openstudio::toString(u_L) + ' ' + openstudio::toString(u_W) + '\n';
  return string;
}

void PlrCrackPrivate::readDetails(Reader &input)
{
  lam = input.readNumber<RX>(FILELINE);
  turb = input.readNumber<RX>(FILELINE);
  expt = input.readNumber<RX>(FILELINE);
  length = input.readNumber<RX>(FILELINE);
  width = input.readNumber<RX>(FILELINE);
  u_L = input.read<int>(FILELINE);
  u_W = input.read<int>(FILELINE);
}

PlrStairPrivate::PlrStairPrivate(int nr,int icon,std::string name,std::string desc,RX lam,RX turb,RX expt,RX Ht,RX Area,RX peo,int tread,int u_A,int u_D):nr(nr),icon(icon),name(name),desc(desc),lam(lam),turb(turb),expt(expt),Ht(Ht),Area(Area),peo(peo),tread(tread),u_A(u_A),u_D(u_D)
{}

void PlrStairPrivate::read(Reader &input)
{
  nr = input.read<int>(FILELINE);
  icon = input.read<int>(FILELINE);
  std::string dataType = input.readString(FILELINE);
  name = input.readString(FILELINE);
  desc = input.readLine(FILELINE);
  lam = input.readNumber<RX>(FILELINE);
  turb = input.readNumber<RX>(FILELINE);
  expt = input.readNumber<RX>(FILELINE);
  Ht = input.readNumber<RX>(FILELINE);
  Area = input.readNumber<RX>(FILELINE);
  peo = input.readNumber<RX>(FILELINE);
  tread = input.read<int>(FILELINE);
  u_A = input.read<int>(FILELINE);
  u_D = input.read<int>(FILELINE);
}

std::string PlrStairPrivate::write()
{
  std::string string;
  string += openstudio::toString(nr) + ' ' + openstudio::toString(icon) + " plr_stair " + name + '\n';
  string += desc + '\n';
  string += openstudio::toString(lam) + ' ' + openstudio::toString(turb) + ' ' + openstudio::toString(expt) + ' ' + openstudio::toString(Ht) + ' ' + openstudio::toString(Area) + ' ' + openstudio::toString(peo) + ' ' + openstudio::toString(tread) + ' ' + openstudio::toString(u_A) + ' ' + openstudio::toString(u_D) + '\n';
  return string;
}

void PlrStairPrivate::readDetails(Reader &input)
{
  lam = input.readNumber<RX>(FILELINE);
  turb = input.readNumber<RX>(FILELINE);
  expt = input.readNumber<RX>(FILELINE);
  Ht = input.readNumber<RX>(FILELINE);
  Area = input.readNumber<RX>(FILELINE);
  peo = input.readNumber<RX>(FILELINE);
  tread = input.read<int>(FILELINE);
  u_A = input.read<int>(FILELINE);
  u_D = input.read<int>(FILELINE);
}

PlrShaftPrivate::PlrShaftPrivate(int nr,int icon,std::string name,std::string desc,RX lam,RX turb,RX expt,RX Ht,RX area,RX perim,RX rough,int u_A,int u_D,int u_P,int u_R):nr(nr),icon(icon),name(name),desc(desc),lam(lam),turb(turb),expt(expt),Ht(Ht),area(area),perim(perim),rough(rough),u_A(u_A),u_D(u_D),u_P(u_P),u_R(u_R)
{}

void PlrShaftPrivate::read(Reader &input)
{
  nr = input.read<int>(FILELINE);
  icon = input.read<int>(FILELINE);
  std::string dataType = input.readString(FILELINE);
  name = input.readString(FILELINE);
  desc = input.readLine(FILELINE);
  lam = input.readNumber<RX>(FILELINE);
  turb = input.readNumber<RX>(FILELINE);
  expt = input.readNumber<RX>(FILELINE);
  Ht = input.readNumber<RX>(FILELINE);
  area = input.readNumber<RX>(FILELINE);
  perim = input.readNumber<RX>(FILELINE);
  rough = input.readNumber<RX>(FILELINE);
  u_A = input.read<int>(FILELINE);
  u_D = input.read<int>(FILELINE);
  u_P = input.read<int>(FILELINE);
  u_R = input.read<int>(FILELINE);
}

std::string PlrShaftPrivate::write()
{
  std::string string;
  string += openstudio::toString(nr) + ' ' + openstudio::toString(icon) + " plr_shaft " + name + '\n';
  string += desc + '\n';
  string += openstudio::toString(lam) + ' ' + openstudio::toString(turb) + ' ' + openstudio::toString(expt) + ' ' + openstudio::toString(Ht) + ' ' + openstudio::toString(area) + ' ' + openstudio::toString(perim) + ' ' + openstudio::toString(rough) + ' ' + openstudio::toString(u_A) + ' ' + openstudio::toString(u_D) + ' ' + openstudio::toString(u_P) + ' ' + openstudio::toString(u_R) + '\n';
  return string;
}

void PlrShaftPrivate::readDetails(Reader &input)
{
  lam = input.readNumber<RX>(FILELINE);
  turb = input.readNumber<RX>(FILELINE);
  expt = input.readNumber<RX>(FILELINE);
  Ht = input.readNumber<RX>(FILELINE);
  area = input.readNumber<RX>(FILELINE);
  perim = input.readNumber<RX>(FILELINE);
  rough = input.readNumber<RX>(FILELINE);
  u_A = input.read<int>(FILELINE);
  u_D = input.read<int>(FILELINE);
  u_P = input.read<int>(FILELINE);
  u_R = input.read<int>(FILELINE);
}

PlrBdqPrivate::PlrBdqPrivate(int nr,int icon,std::string name,std::string desc,RX lam,RX Cp,RX xp,RX Cn,RX xn):nr(nr),icon(icon),name(name),desc(desc),lam(lam),Cp(Cp),xp(xp),Cn(Cn),xn(xn)
{}

void PlrBdqPrivate::read(Reader &input)
{
  nr = input.read<int>(FILELINE);
  icon = input.read<int>(FILELINE);
  std::string dataType = input.readString(FILELINE);
  name = input.readString(FILELINE);
  desc = input.readLine(FILELINE);
  lam = input.readNumber<RX>(FILELINE);
  Cp = input.readNumber<RX>(FILELINE);
  xp = input.readNumber<RX>(FILELINE);
  Cn = input.readNumber<RX>(FILELINE);
  xn = input.readNumber<RX>(FILELINE);
}

std::string PlrBdqPrivate::write()
{
  std::string string;
  string += openstudio::toString(nr) + ' ' + openstudio::toString(icon) + " plr_bdq " + name + '\n';
  string += desc + '\n';
  string += openstudio::toString(lam) + ' ' + openstudio::toString(Cp) + ' ' + openstudio::toString(xp) + ' ' + openstudio::toString(Cn) + ' ' + openstudio::toString(xn) + '\n';
  return string;
}

void PlrBdqPrivate::readDetails(Reader &input)
{
  lam = input.readNumber<RX>(FILELINE);
  Cp = input.readNumber<RX>(FILELINE);
  xp = input.readNumber<RX>(FILELINE);
  Cn = input.readNumber<RX>(FILELINE);
  xn = input.readNumber<RX>(FILELINE);
}

PlrBdfPrivate::PlrBdfPrivate(int nr,int icon,std::string name,std::string desc,RX lam,RX Cp,RX xp,RX Cn,RX xn):nr(nr),icon(icon),name(name),desc(desc),lam(lam),Cp(Cp),xp(xp),Cn(Cn),xn(xn)
{}

void PlrBdfPrivate::read(Reader &input)
{
  nr = input.read<int>(FILELINE);
  icon = input.read<int>(FILELINE);
  std::string dataType = input.readString(FILELINE);
  name = input.readString(FILELINE);
  desc = input.readLine(FILELINE);
  lam = input.readNumber<RX>(FILELINE);
  Cp = input.readNumber<RX>(FILELINE);
  xp = input.readNumber<RX>(FILELINE);
  Cn = input.readNumber<RX>(FILELINE);
  xn = input.readNumber<RX>(FILELINE);
}

std::string PlrBdfPrivate::write()
{
  std::string string;
  string += openstudio::toString(nr) + ' ' + openstudio::toString(icon) + " plr_bdf " + name + '\n';
  string += desc + '\n';
  string += openstudio::toString(lam) + ' ' + openstudio::toString(Cp) + ' ' + openstudio::toString(xp) + ' ' + openstudio::toString(Cn) + ' ' + openstudio::toString(xn) + '\n';
  return string;
}

void PlrBdfPrivate::readDetails(Reader &input)
{
  lam = input.readNumber<RX>(FILELINE);
  Cp = input.readNumber<RX>(FILELINE);
  xp = input.readNumber<RX>(FILELINE);
  Cn = input.readNumber<RX>(FILELINE);
  xn = input.readNumber<RX>(FILELINE);
}

QfrQabPrivate::QfrQabPrivate(int nr,int icon,std::string name,std::string desc,RX a,RX b):nr(nr),icon(icon),name(name),desc(desc),a(a),b(b)
{}

void QfrQabPrivate::read(Reader &input)
{
  nr = input.read<int>(FILELINE);
  icon = input.read<int>(FILELINE);
  std::string dataType = input.readString(FILELINE);
  name = input.readString(FILELINE);
  desc = input.readLine(FILELINE);
  a = input.readNumber<RX>(FILELINE);
  b = input.readNumber<RX>(FILELINE);
}

std::string QfrQabPrivate::write()
{
  std::string string;
  string += openstudio::toString(nr) + ' ' + openstudio::toString(icon) + " qfr_qab " + name + '\n';
  string += desc + '\n';
  string += openstudio::toString(a) + ' ' + openstudio::toString(b) + '\n';
  return string;
}

void QfrQabPrivate::readDetails(Reader &input)
{
  a = input.readNumber<RX>(FILELINE);
  b = input.readNumber<RX>(FILELINE);
}

QfrFabPrivate::QfrFabPrivate(int nr,int icon,std::string name,std::string desc,RX a,RX b):nr(nr),icon(icon),name(name),desc(desc),a(a),b(b)
{}

void QfrFabPrivate::read(Reader &input)
{
  nr = input.read<int>(FILELINE);
  icon = input.read<int>(FILELINE);
  std::string dataType = input.readString(FILELINE);
  name = input.readString(FILELINE);
  desc = input.readLine(FILELINE);
  a = input.readNumber<RX>(FILELINE);
  b = input.readNumber<RX>(FILELINE);
}

std::string QfrFabPrivate::write()
{
  std::string string;
  string += openstudio::toString(nr) + ' ' + openstudio::toString(icon) + " qfr_fab " + name + '\n';
  string += desc + '\n';
  string += openstudio::toString(a) + ' ' + openstudio::toString(b) + '\n';
  return string;
}

void QfrFabPrivate::readDetails(Reader &input)
{
  a = input.readNumber<RX>(FILELINE);
  b = input.readNumber<RX>(FILELINE);
}

QfrCrackPrivate::QfrCrackPrivate(int nr,int icon,std::string name,std::string desc,RX a,RX b,RX length,RX width,RX depth,int nB,int u_L,int u_W,int u_D):nr(nr),icon(icon),name(name),desc(desc),a(a),b(b),length(length),width(width),depth(depth),nB(nB),u_L(u_L),u_W(u_W),u_D(u_D)
{}

void QfrCrackPrivate::read(Reader &input)
{
  nr = input.read<int>(FILELINE);
  icon = input.read<int>(FILELINE);
  std::string dataType = input.readString(FILELINE);
  name = input.readString(FILELINE);
  desc = input.readLine(FILELINE);
  a = input.readNumber<RX>(FILELINE);
  b = input.readNumber<RX>(FILELINE);
  length = input.readNumber<RX>(FILELINE);
  width = input.readNumber<RX>(FILELINE);
  depth = input.readNumber<RX>(FILELINE);
  nB = input.read<int>(FILELINE);
  u_L = input.read<int>(FILELINE);
  u_W = input.read<int>(FILELINE);
  u_D = input.read<int>(FILELINE);
}

std::string QfrCrackPrivate::write()
{
  std::string string;
  string += openstudio::toString(nr) + ' ' + openstudio::toString(icon) + " qfr_crack " + name + '\n';
  string += desc + '\n';
  string += openstudio::toString(a) + ' ' + openstudio::toString(b) + ' ' + openstudio::toString(length) + ' ' + openstudio::toString(width) + ' ' + openstudio::toString(depth) + ' ' + openstudio::toString(nB) + ' ' + openstudio::toString(u_L) + ' ' + openstudio::toString(u_W) + ' ' + openstudio::toString(u_D) + '\n';
  return string;
}

void QfrCrackPrivate::readDetails(Reader &input)
{
  a = input.readNumber<RX>(FILELINE);
  b = input.readNumber<RX>(FILELINE);
  length = input.readNumber<RX>(FILELINE);
  width = input.readNumber<RX>(FILELINE);
  depth = input.readNumber<RX>(FILELINE);
  nB = input.read<int>(FILELINE);
  u_L = input.read<int>(FILELINE);
  u_W = input.read<int>(FILELINE);
  u_D = input.read<int>(FILELINE);
}

QfrTest2Private::QfrTest2Private(int nr,int icon,std::string name,std::string desc,RX a,RX b,RX dP1,RX F1,RX dP2,RX F2,int u_P1,int u_F1,int u_P2,int u_F2):nr(nr),icon(icon),name(name),desc(desc),a(a),b(b),dP1(dP1),F1(F1),dP2(dP2),F2(F2),u_P1(u_P1),u_F1(u_F1),u_P2(u_P2),u_F2(u_F2)
{}

void QfrTest2Private::read(Reader &input)
{
  nr = input.read<int>(FILELINE);
  icon = input.read<int>(FILELINE);
  // dataType = input.readString(FILELINE);
  name = input.readString(FILELINE);
  desc = input.readLine(FILELINE);
  a = input.readNumber<RX>(FILELINE);
  b = input.readNumber<RX>(FILELINE);
  dP1 = input.readNumber<RX>(FILELINE);
  F1 = input.readNumber<RX>(FILELINE);
  dP2 = input.readNumber<RX>(FILELINE);
  F2 = input.readNumber<RX>(FILELINE);
  u_P1 = input.read<int>(FILELINE);
  u_F1 = input.read<int>(FILELINE);
  u_P2 = input.read<int>(FILELINE);
  u_F2 = input.read<int>(FILELINE);
}

std::string QfrTest2Private::write()
{
  std::string string;
  string += openstudio::toString(nr) + ' ' + openstudio::toString(icon) + " qfr_test2 " + name + '\n';
  string += desc + '\n';
  string += openstudio::toString(a) + ' ' + openstudio::toString(b) + ' ' + openstudio::toString(dP1) + ' ' + openstudio::toString(F1) + ' ' + openstudio::toString(dP2) + ' ' + openstudio::toString(F2) + ' ' + openstudio::toString(u_P1) + ' ' + openstudio::toString(u_F1) + ' ' + openstudio::toString(u_P2) + ' ' + openstudio::toString(u_F2) + '\n';
  return string;
}

void QfrTest2Private::readDetails(Reader &input)
{
  a = input.readNumber<RX>(FILELINE);
  b = input.readNumber<RX>(FILELINE);
  dP1 = input.readNumber<RX>(FILELINE);
  F1 = input.readNumber<RX>(FILELINE);
  dP2 = input.readNumber<RX>(FILELINE);
  F2 = input.readNumber<RX>(FILELINE);
  u_P1 = input.read<int>(FILELINE);
  u_F1 = input.read<int>(FILELINE);
  u_P2 = input.read<int>(FILELINE);
  u_F2 = input.read<int>(FILELINE);
}

AfeDorPrivate::AfeDorPrivate(int nr,int icon,std::string name,std::string desc,RX lam,RX turb,RX expt,RX dTmin,RX ht,RX wd,RX cd,int u_T,int u_H,int u_W):nr(nr),icon(icon),name(name),desc(desc),lam(lam),turb(turb),expt(expt),dTmin(dTmin),ht(ht),wd(wd),cd(cd),u_T(u_T),u_H(u_H),u_W(u_W)
{}

void AfeDorPrivate::read(Reader &input)
{
  nr = input.read<int>(FILELINE);
  icon = input.read<int>(FILELINE);
  std::string dataType = input.readString(FILELINE);
  name = input.readString(FILELINE);
  desc = input.readLine(FILELINE);
  lam = input.readNumber<RX>(FILELINE);
  turb = input.readNumber<RX>(FILELINE);
  expt = input.readNumber<RX>(FILELINE);
  dTmin = input.readNumber<RX>(FILELINE);
  ht = input.readNumber<RX>(FILELINE);
  wd = input.readNumber<RX>(FILELINE);
  cd = input.readNumber<RX>(FILELINE);
  u_T = input.read<int>(FILELINE);
  u_H = input.read<int>(FILELINE);
  u_W = input.read<int>(FILELINE);
}

std::string AfeDorPrivate::write()
{
  std::string string;
  string += openstudio::toString(nr) + ' ' + openstudio::toString(icon) + " dor_door " + name + '\n';
  string += desc + '\n';
  string += openstudio::toString(lam) + ' ' + openstudio::toString(turb) + ' ' + openstudio::toString(expt) + ' ' + openstudio::toString(dTmin) + ' ' + openstudio::toString(ht) + ' ' + openstudio::toString(wd) + ' ' + openstudio::toString(cd) + ' ' + openstudio::toString(u_T) + ' ' + openstudio::toString(u_H) + ' ' + openstudio::toString(u_W) + '\n';
  return string;
}

void AfeDorPrivate::readDetails(Reader &input)
{
  lam = input.readNumber<RX>(FILELINE);
  turb = input.readNumber<RX>(FILELINE);
  expt = input.readNumber<RX>(FILELINE);
  dTmin = input.readNumber<RX>(FILELINE);
  ht = input.readNumber<RX>(FILELINE);
  wd = input.readNumber<RX>(FILELINE);
  cd = input.readNumber<RX>(FILELINE);
  u_T = input.read<int>(FILELINE);
  u_H = input.read<int>(FILELINE);
  u_W = input.read<int>(FILELINE);
}

DrPl2Private::DrPl2Private(int nr,int icon,std::string name,std::string desc,RX lam,RX turb,RX expt,RX dH,RX ht,RX wd,RX cd,int u_H,int u_W):nr(nr),icon(icon),name(name),desc(desc),lam(lam),turb(turb),expt(expt),dH(dH),ht(ht),wd(wd),cd(cd),u_H(u_H),u_W(u_W)
{}

void DrPl2Private::read(Reader &input)
{
  nr = input.read<int>(FILELINE);
  icon = input.read<int>(FILELINE);
  std::string dataType = input.readString(FILELINE);
  name = input.readString(FILELINE);
  desc = input.readLine(FILELINE);
  lam = input.readNumber<RX>(FILELINE);
  turb = input.readNumber<RX>(FILELINE);
  expt = input.readNumber<RX>(FILELINE);
  dH = input.readNumber<RX>(FILELINE);
  ht = input.readNumber<RX>(FILELINE);
  wd = input.readNumber<RX>(FILELINE);
  cd = input.readNumber<RX>(FILELINE);
  u_H = input.read<int>(FILELINE);
  u_W = input.read<int>(FILELINE);
}

std::string DrPl2Private::write()
{
  std::string string;
  string += openstudio::toString(nr) + ' ' + openstudio::toString(icon) + " dor_pl2 " + name + '\n';
  string += desc + '\n';
  string += openstudio::toString(lam) + ' ' + openstudio::toString(turb) + ' ' + openstudio::toString(expt) + ' ' + openstudio::toString(dH) + ' ' + openstudio::toString(ht) + ' ' + openstudio::toString(wd) + ' ' + openstudio::toString(cd) + ' ' + openstudio::toString(u_H) + ' ' + openstudio::toString(u_W) + '\n';
  return string;
}

void DrPl2Private::readDetails(Reader &input)
{
  lam = input.readNumber<RX>(FILELINE);
  turb = input.readNumber<RX>(FILELINE);
  expt = input.readNumber<RX>(FILELINE);
  dH = input.readNumber<RX>(FILELINE);
  ht = input.readNumber<RX>(FILELINE);
  wd = input.readNumber<RX>(FILELINE);
  cd = input.readNumber<RX>(FILELINE);
  u_H = input.read<int>(FILELINE);
  u_W = input.read<int>(FILELINE);
}

AfeCmfPrivate::AfeCmfPrivate(int nr,int icon,std::string name,std::string desc,RX Flow,int u_F):nr(nr),icon(icon),name(name),desc(desc),Flow(Flow),u_F(u_F)
{}

void AfeCmfPrivate::read(Reader &input)
{
  nr = input.read<int>(FILELINE);
  icon = input.read<int>(FILELINE);
  std::string dataType = input.readString(FILELINE);
  name = input.readString(FILELINE);
  desc = input.readLine(FILELINE);
  Flow = input.readNumber<RX>(FILELINE);
  u_F = input.read<int>(FILELINE);
}

std::string AfeCmfPrivate::write()
{
  std::string string;
  string += openstudio::toString(nr) + ' ' + openstudio::toString(icon) + " fan_cmf " + name + '\n';
  string += desc + '\n';
  string += openstudio::toString(Flow) + ' ' + openstudio::toString(u_F) + '\n';
  return string;
}

void AfeCmfPrivate::readDetails(Reader &input)
{
  Flow = input.readNumber<RX>(FILELINE);
  u_F = input.read<int>(FILELINE);
}

AfeCvfPrivate::AfeCvfPrivate(int nr,int icon,std::string name,std::string desc,RX Flow,int u_F):nr(nr),icon(icon),name(name),desc(desc),Flow(Flow),u_F(u_F)
{}

void AfeCvfPrivate::read(Reader &input)
{
  nr = input.read<int>(FILELINE);
  icon = input.read<int>(FILELINE);
  std::string dataType = input.readString(FILELINE);
  name = input.readString(FILELINE);
  desc = input.readLine(FILELINE);
  Flow = input.readNumber<RX>(FILELINE);
  u_F = input.read<int>(FILELINE);
}

std::string AfeCvfPrivate::write()
{
  std::string string;
  string += openstudio::toString(nr) + ' ' + openstudio::toString(icon) + " fan_cvf " + name + '\n';
  string += desc + '\n';
  string += openstudio::toString(Flow) + ' ' + openstudio::toString(u_F) + '\n';
  return string;
}

void AfeCvfPrivate::readDetails(Reader &input)
{
  Flow = input.readNumber<RX>(FILELINE);
  u_F = input.read<int>(FILELINE);
}

AfeFanPrivate::AfeFanPrivate(int nr,int icon,std::string name,std::string desc,RX lam,RX turb,RX expt,RX rdens,RX fdf,RX sop,RX off,std::vector<RX> fpc,RX Sarea,int u_Sa,std::vector<FanDataPoint> data):nr(nr),icon(icon),name(name),desc(desc),lam(lam),turb(turb),expt(expt),rdens(rdens),fdf(fdf),sop(sop),off(off),fpc(fpc),Sarea(Sarea),u_Sa(u_Sa),data(data)
{}

void AfeFanPrivate::read(Reader &input)
{
  nr = input.read<int>(FILELINE);
  icon = input.read<int>(FILELINE);
  std::string dataType = input.readString(FILELINE);
  name = input.readString(FILELINE);
  desc = input.readLine(FILELINE);
  lam = input.readNumber<RX>(FILELINE);
  turb = input.readNumber<RX>(FILELINE);
  expt = input.readNumber<RX>(FILELINE);
  rdens = input.readNumber<RX>(FILELINE);
  fdf = input.readNumber<RX>(FILELINE);
  sop = input.readNumber<RX>(FILELINE);
  off = input.readNumber<RX>(FILELINE);
  for(int i=0;i<4;i++)
    fpc.push_back(input.read<RX>(FILELINE));
  int npts = input.read<int>(FILELINE);
  Sarea = input.readNumber<RX>(FILELINE);
  u_Sa = input.read<int>(FILELINE);
  for(int i=0;i<npts;i++)
  {
    FanDataPoint object;
    object.read(input);
    data.push_back(object);
  }

}

std::string AfeFanPrivate::write()
{
  std::string string;
  string += openstudio::toString(nr) + ' ' + openstudio::toString(icon) + " fan_fan " + name + '\n';
  string += desc + '\n';
  string += openstudio::toString(lam) + ' ' + openstudio::toString(turb) + ' ' + openstudio::toString(expt) + ' ' + openstudio::toString(rdens) + ' ' + openstudio::toString(fdf) + ' ' + openstudio::toString(sop) + ' ' + openstudio::toString(off) + '\n';
  for(int i=0;i<4;i++)
    string += openstudio::toString(fpc[i]) + ' ';
  string += '\n';
  string += openstudio::toString(data.size()) + ' ' + openstudio::toString(Sarea) + ' ' + openstudio::toString(u_Sa) + '\n';
  for(unsigned int i=0;i<data.size();i++)
  {
    string += data[i].write();
  }
  return string;
}

void AfeFanPrivate::readDetails(Reader &input)
{
  lam = input.readNumber<RX>(FILELINE);
  turb = input.readNumber<RX>(FILELINE);
  expt = input.readNumber<RX>(FILELINE);
  rdens = input.readNumber<RX>(FILELINE);
  fdf = input.readNumber<RX>(FILELINE);
  sop = input.readNumber<RX>(FILELINE);
  off = input.readNumber<RX>(FILELINE);
  for(int i=0;i<4;i++)
    fpc.push_back(input.read<RX>(FILELINE));
  int npts = input.read<int>(FILELINE);
  Sarea = input.readNumber<RX>(FILELINE);
  u_Sa = input.read<int>(FILELINE);
  for(int i=0;i<npts;i++)
  {
    FanDataPoint object;
    object.read(input);
    data.push_back(object);
  }

}

AfeCsfPrivate::AfeCsfPrivate(int nr,int icon,std::string name,std::string desc,int u_x,int u_y,std::vector<DataPoint> data):nr(nr),icon(icon),name(name),desc(desc),u_x(u_x),u_y(u_y),data(data)
{}

void AfeCsfPrivate::read(Reader &input)
{
  nr = input.read<int>(FILELINE);
  icon = input.read<int>(FILELINE);
  std::string dataType = input.readString(FILELINE);
  name = input.readString(FILELINE);
  desc = input.readLine(FILELINE);
  int npts = input.read<int>(FILELINE);
  u_x = input.read<int>(FILELINE);
  u_y = input.read<int>(FILELINE);
  for(int i=0;i<npts;i++)
  {
    DataPoint object;
    object.read(input);
    data.push_back(object);
  }

}

std::string AfeCsfPrivate::write(std::string datatype)
{
  std::string string;
  string += openstudio::toString(nr) + ' ' + openstudio::toString(icon) + ' ' + datatype + ' ' + name + '\n';
  string += desc + '\n';
  string += openstudio::toString(data.size()) + ' ' + openstudio::toString(u_x) + ' ' + openstudio::toString(u_y) + '\n';
  for(unsigned int i=0;i<data.size();i++)
  {
    string += data[i].write();
  }
  return string;
}

void AfeCsfPrivate::readDetails(Reader &input)
{
  int npts = input.read<int>(FILELINE);
  u_x = input.read<int>(FILELINE);
  u_y = input.read<int>(FILELINE);
  for(int i=0;i<npts;i++)
  {
    DataPoint object;
    object.read(input);
    data.push_back(object);
  }

}

AfeSupPrivate::AfeSupPrivate(int nr,int icon,std::string name,std::string desc,int sched,int u_H,std::vector<AirflowSubelementData> subelements):nr(nr),icon(icon),name(name),desc(desc),sched(sched),u_H(u_H),subelements(subelements)
{}

void AfeSupPrivate::read(Reader &input)
{
  nr = input.read<int>(FILELINE);
  icon = input.read<int>(FILELINE);
  std::string dataType = input.readString(FILELINE);
  name = input.readString(FILELINE);
  desc = input.readLine(FILELINE);
  int nse = input.read<int>(FILELINE);
  sched = input.read<int>(FILELINE);
  u_H = input.read<int>(FILELINE);
  for(int i=0;i<nse;i++)
  {
    AirflowSubelementData object;
    object.read(input);
    subelements.push_back(object);
  }

}

std::string AfeSupPrivate::write()
{
  std::string string;
  string += openstudio::toString(nr) + ' ' + openstudio::toString(icon) + " sup_afe " + name + '\n';
  string += desc + '\n';
  string += openstudio::toString(subelements.size()) + ' ' + openstudio::toString(sched) + ' ' + openstudio::toString(u_H) + '\n';
  for(unsigned int i=0;i<subelements.size();i++)
  {
    string += subelements[i].write();
  }
  return string;
}

void AfeSupPrivate::readDetails(Reader &input)
{
  int nse = input.read<int>(FILELINE);
  sched = input.read<int>(FILELINE);
  u_H = input.read<int>(FILELINE);
  for(int i=0;i<nse;i++)
  {
    AirflowSubelementData object;
    object.read(input);
    subelements.push_back(object);
  }

}

} // contam
} // openstudio
