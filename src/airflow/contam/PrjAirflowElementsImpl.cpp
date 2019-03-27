/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include <algorithm>

#include "PrjAirflowElementsImpl.hpp"
#include "../../utilities/core/StringHelpers.hpp"

namespace openstudio {
namespace contam {
namespace detail {

void PlrOrfImpl::setDefaults()
{
  m_nr = 0;
  m_icon = 0;
  m_lam = PRJFLOAT("0.0");
  m_turb = PRJFLOAT("0.0");
  m_expt = PRJFLOAT("0.0");
  m_area = PRJFLOAT("0.0");
  m_dia = PRJFLOAT("0.0");
  m_coef = PRJFLOAT("0.0");
  m_Re = PRJFLOAT("0.0");
  m_u_A = 0;
  m_u_D = 0;
}

PlrOrfImpl::PlrOrfImpl()
{
  setDefaults();
}

PlrOrfImpl::PlrOrfImpl(int nr,int icon,std::string name,std::string desc)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
}

PlrOrfImpl::PlrOrfImpl(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double area,
                       double dia,double coef,double Re,int u_A,int u_D)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setLam(lam);
  setTurb(turb);
  setExpt(expt);
  setArea(area);
  setDia(dia);
  setCoef(coef);
  setRe(Re);
  setU_A(u_A);
  setU_D(u_D);
}

PlrOrfImpl::PlrOrfImpl(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string area,
                       std::string dia,std::string coef,std::string Re,int u_A,int u_D)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setLam(lam);
  setTurb(turb);
  setExpt(expt);
  setArea(area);
  setDia(dia);
  setCoef(coef);
  setRe(Re);
  setU_A(u_A);
  setU_D(u_D);
}

void PlrOrfImpl::read(Reader &input)
{
  setNr(input.read<int>());
  setIcon(input.read<int>());
  std::string dataType = input.readString(); // Should really check this
  setName(input.readString());
  setDesc(input.readLine());
  setLam(input.readNumber<std::string>());
  setTurb(input.readNumber<std::string>());
  setExpt(input.readNumber<std::string>());
  setArea(input.readNumber<std::string>());
  setDia(input.readNumber<std::string>());
  setCoef(input.readNumber<std::string>());
  setRe(input.readNumber<std::string>());
  setU_A(input.read<int>());
  setU_D(input.read<int>());
}

std::string PlrOrfImpl::write()
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_icon) + " plr_orfc " + m_name + '\n';
  string += m_desc + '\n';
  string += ANY_TO_STR(m_lam) + ' ' + ANY_TO_STR(m_turb) + ' ' + ANY_TO_STR(m_expt) + ' ' + ANY_TO_STR(m_area)
    + ' ' + ANY_TO_STR(m_dia) + ' ' + ANY_TO_STR(m_coef) + ' ' + ANY_TO_STR(m_Re) + ' '
    + ANY_TO_STR(m_u_A) + ' ' + ANY_TO_STR(m_u_D) + '\n';
  return string;
}

void PlrOrfImpl::readDetails(Reader &input)
{
  setLam(input.readNumber<std::string>());
  setTurb(input.readNumber<std::string>());
  setExpt(input.readNumber<std::string>());
  setArea(input.readNumber<std::string>());
  setDia(input.readNumber<std::string>());
  setCoef(input.readNumber<std::string>());
  setRe(input.readNumber<std::string>());
  setU_A(input.read<int>());
  setU_D(input.read<int>());
}

int PlrOrfImpl::nr() const
{
  return m_nr;
}

void PlrOrfImpl::setNr(const int nr)
{
  m_nr = nr;
}

int PlrOrfImpl::icon() const
{
  return m_icon;
}

void PlrOrfImpl::setIcon(const int icon)
{
  m_icon = icon;
}

std::string PlrOrfImpl::name() const
{
  return m_name;
}

void PlrOrfImpl::setName(const std::string &name)
{
  m_name = name;
}

std::string PlrOrfImpl::desc() const
{
  return m_desc;
}

void PlrOrfImpl::setDesc(const std::string &desc)
{
  m_desc = desc;
}

double PlrOrfImpl::lam() const
{
  return openstudio::string_conversions::to<double>(m_lam);
}

bool PlrOrfImpl::setLam(const double lam)
{
  m_lam = openstudio::string_conversions::number(lam);
  return true;
}

bool PlrOrfImpl::setLam(const std::string &lam)
{
  return assign_if_valid<double>(lam, m_lam);
}

double PlrOrfImpl::turb() const
{
  return openstudio::string_conversions::to<double>(m_turb);
}

bool PlrOrfImpl::setTurb(const double turb)
{
  m_turb = openstudio::string_conversions::number(turb);
  return true;
}

bool PlrOrfImpl::setTurb(const std::string &turb)
{
  return assign_if_valid<double>(turb, m_turb);
}

double PlrOrfImpl::expt() const
{
  return openstudio::string_conversions::to<double>(m_expt);
}

bool PlrOrfImpl::setExpt(const double expt)
{
  m_expt = openstudio::string_conversions::number(expt);
  return true;
}

bool PlrOrfImpl::setExpt(const std::string &expt)
{
  return assign_if_valid<double>(expt, m_expt);
}

double PlrOrfImpl::area() const
{
  return openstudio::string_conversions::to<double>(m_area);
}

bool PlrOrfImpl::setArea(const double area)
{
  m_area = openstudio::string_conversions::number(area);
  return true;
}

bool PlrOrfImpl::setArea(const std::string &area)
{
  return assign_if_valid<double>(area, m_area);
}

double PlrOrfImpl::dia() const
{
  return openstudio::string_conversions::to<double>(m_dia);
}

bool PlrOrfImpl::setDia(const double dia)
{
  m_dia = openstudio::string_conversions::number(dia);
  return true;
}

bool PlrOrfImpl::setDia(const std::string &dia)
{
  return assign_if_valid<double>(dia, m_dia);
}

double PlrOrfImpl::coef() const
{
  return openstudio::string_conversions::to<double>(m_coef);
}

bool PlrOrfImpl::setCoef(const double coef)
{
  m_coef = openstudio::string_conversions::number(coef);
  return true;
}

bool PlrOrfImpl::setCoef(const std::string &coef)
{
  return assign_if_valid<double>(coef, m_coef);
}

double PlrOrfImpl::Re() const
{
  return openstudio::string_conversions::to<double>(m_Re);
}

bool PlrOrfImpl::setRe(const double Re)
{
  m_Re = openstudio::string_conversions::number(Re);
  return true;
}

bool PlrOrfImpl::setRe(const std::string &Re)
{
  return assign_if_valid<double>(Re, m_Re);
}

int PlrOrfImpl::u_A() const
{
  return m_u_A;
}

void PlrOrfImpl::setU_A(const int u_A)
{
  m_u_A = u_A;
}

int PlrOrfImpl::u_D() const
{
  return m_u_D;
}

void PlrOrfImpl::setU_D(const int u_D)
{
  m_u_D = u_D;
}

void PlrLeakImpl::setDefaults()
{
  m_nr = 0;
  m_icon = 0;
  m_lam = PRJFLOAT("0.0");
  m_turb = PRJFLOAT("0.0");
  m_expt = PRJFLOAT("0.0");
  m_coef = PRJFLOAT("0.0");
  m_pres = PRJFLOAT("0.0");
  m_area1 = PRJFLOAT("0.0");
  m_area2 = PRJFLOAT("0.0");
  m_area3 = PRJFLOAT("0.0");
  m_u_A1 = 0;
  m_u_A2 = 0;
  m_u_A3 = 0;
  m_u_dP = 0;
}

PlrLeakImpl::PlrLeakImpl()
{
  setDefaults();
}

PlrLeakImpl::PlrLeakImpl(int nr,int icon,std::string name,std::string desc)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
}

PlrLeakImpl::PlrLeakImpl(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,
                         double coef,double pres,double area1,double area2,double area3,int u_A1,int u_A2,
                         int u_A3,int u_dP)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setLam(lam);
  setTurb(turb);
  setExpt(expt);
  setCoef(coef);
  setPres(pres);
  setArea1(area1);
  setArea2(area2);
  setArea3(area3);
  setU_A1(u_A1);
  setU_A2(u_A2);
  setU_A3(u_A3);
  setU_dP(u_dP);
}

PlrLeakImpl::PlrLeakImpl(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,
                         std::string coef,std::string pres,std::string area1,std::string area2,std::string area3,int u_A1,int u_A2,
                         int u_A3,int u_dP)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setLam(lam);
  setTurb(turb);
  setExpt(expt);
  setCoef(coef);
  setPres(pres);
  setArea1(area1);
  setArea2(area2);
  setArea3(area3);
  setU_A1(u_A1);
  setU_A2(u_A2);
  setU_A3(u_A3);
  setU_dP(u_dP);
}

void PlrLeakImpl::read(Reader &input)
{
  setNr(input.read<int>());
  setIcon(input.read<int>());
  std::string dataType = input.readString(); // Should really check this
  setName(input.readString());
  setDesc(input.readLine());
  setLam(input.readNumber<std::string>());
  setTurb(input.readNumber<std::string>());
  setExpt(input.readNumber<std::string>());
  setCoef(input.readNumber<std::string>());
  setPres(input.readNumber<std::string>());
  setArea1(input.readNumber<std::string>());
  setArea2(input.readNumber<std::string>());
  setArea3(input.readNumber<std::string>());
  setU_A1(input.read<int>());
  setU_A2(input.read<int>());
  setU_A3(input.read<int>());
  setU_dP(input.read<int>());
}

std::string PlrLeakImpl::write(std::string datatype)
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_icon) + ' ' + datatype + ' ' + m_name + '\n';
  string += m_desc + '\n';
  string += ANY_TO_STR(m_lam) + ' ' + ANY_TO_STR(m_turb) + ' ' + ANY_TO_STR(m_expt) + ' '
    + ANY_TO_STR(m_coef) + ' ' + ANY_TO_STR(m_pres) + ' ' + ANY_TO_STR(m_area1) + ' '
    + ANY_TO_STR(m_area2) + ' ' + ANY_TO_STR(m_area3) + ' ' + ANY_TO_STR(m_u_A1) + ' '
    + ANY_TO_STR(m_u_A2) + ' ' + ANY_TO_STR(m_u_A3) + ' ' + ANY_TO_STR(m_u_dP) + '\n';
  return string;
}

void PlrLeakImpl::readDetails(Reader &input)
{
  setLam(input.readNumber<std::string>());
  setTurb(input.readNumber<std::string>());
  setExpt(input.readNumber<std::string>());
  setCoef(input.readNumber<std::string>());
  setPres(input.readNumber<std::string>());
  setArea1(input.readNumber<std::string>());
  setArea2(input.readNumber<std::string>());
  setArea3(input.readNumber<std::string>());
  setU_A1(input.read<int>());
  setU_A2(input.read<int>());
  setU_A3(input.read<int>());
  setU_dP(input.read<int>());
}

int PlrLeakImpl::nr() const
{
  return m_nr;
}

void PlrLeakImpl::setNr(const int nr)
{
  m_nr = nr;
}

int PlrLeakImpl::icon() const
{
  return m_icon;
}

void PlrLeakImpl::setIcon(const int icon)
{
  m_icon = icon;
}

std::string PlrLeakImpl::name() const
{
  return m_name;
}

void PlrLeakImpl::setName(const std::string &name)
{
  m_name = name;
}

std::string PlrLeakImpl::desc() const
{
  return m_desc;
}

void PlrLeakImpl::setDesc(const std::string &desc)
{
  m_desc = desc;
}

double PlrLeakImpl::lam() const
{
  return openstudio::string_conversions::to<double>(m_lam);
}

bool PlrLeakImpl::setLam(const double lam)
{
  m_lam = openstudio::string_conversions::number(lam);
  return true;
}

bool PlrLeakImpl::setLam(const std::string &lam)
{
  return assign_if_valid<double>(lam, m_lam);
}

double PlrLeakImpl::turb() const
{
  return openstudio::string_conversions::to<double>(m_turb);
}

bool PlrLeakImpl::setTurb(const double turb)
{
  m_turb = openstudio::string_conversions::number(turb);
  return true;
}

bool PlrLeakImpl::setTurb(const std::string &turb)
{
  return assign_if_valid<double>(turb, m_turb);
}

double PlrLeakImpl::expt() const
{
  return openstudio::string_conversions::to<double>(m_expt);
}

bool PlrLeakImpl::setExpt(const double expt)
{
  m_expt = openstudio::string_conversions::number(expt);
  return true;
}

bool PlrLeakImpl::setExpt(const std::string &expt)
{
  return assign_if_valid<double>(expt, m_expt);
}

double PlrLeakImpl::coef() const
{
  return openstudio::string_conversions::to<double>(m_coef);
}

bool PlrLeakImpl::setCoef(const double coef)
{
  m_coef = openstudio::string_conversions::number(coef);
  return true;
}

bool PlrLeakImpl::setCoef(const std::string &coef)
{
  return assign_if_valid<double>(coef, m_coef);
}

double PlrLeakImpl::pres() const
{
  return openstudio::string_conversions::to<double>(m_pres);
}

bool PlrLeakImpl::setPres(const double pres)
{
  m_pres = openstudio::string_conversions::number(pres);
  return true;
}

bool PlrLeakImpl::setPres(const std::string &pres)
{
  return assign_if_valid<double>(pres, m_pres);
}

double PlrLeakImpl::area1() const
{
  return openstudio::string_conversions::to<double>(m_area1);
}

bool PlrLeakImpl::setArea1(const double area1)
{
  m_area1 = openstudio::string_conversions::number(area1);
  return true;
}

bool PlrLeakImpl::setArea1(const std::string &area1)
{
  return assign_if_valid<double>(area1, m_area1);
}

double PlrLeakImpl::area2() const
{
  return openstudio::string_conversions::to<double>(m_area2);
}

bool PlrLeakImpl::setArea2(const double area2)
{
  m_area2 = openstudio::string_conversions::number(area2);
  return true;
}

bool PlrLeakImpl::setArea2(const std::string &area2)
{
  return assign_if_valid<double>(area2, m_area2);
}

double PlrLeakImpl::area3() const
{
  return openstudio::string_conversions::to<double>(m_area3);
}

bool PlrLeakImpl::setArea3(const double area3)
{
  m_area3 = openstudio::string_conversions::number(area3);
  return true;
}

bool PlrLeakImpl::setArea3(const std::string &area3)
{
  return assign_if_valid<double>(area3, m_area3);
}

int PlrLeakImpl::u_A1() const
{
  return m_u_A1;
}

void PlrLeakImpl::setU_A1(const int u_A1)
{
  m_u_A1 = u_A1;
}

int PlrLeakImpl::u_A2() const
{
  return m_u_A2;
}

void PlrLeakImpl::setU_A2(const int u_A2)
{
  m_u_A2 = u_A2;
}

int PlrLeakImpl::u_A3() const
{
  return m_u_A3;
}

void PlrLeakImpl::setU_A3(const int u_A3)
{
  m_u_A3 = u_A3;
}

int PlrLeakImpl::u_dP() const
{
  return m_u_dP;
}

void PlrLeakImpl::setU_dP(const int u_dP)
{
  m_u_dP = u_dP;
}

void PlrConnImpl::setDefaults()
{
  m_nr = 0;
  m_icon = 0;
  m_lam = PRJFLOAT("0.0");
  m_turb = PRJFLOAT("0.0");
  m_expt = PRJFLOAT("0.0");
  m_coef = PRJFLOAT("0.0");
  m_area = PRJFLOAT("0.0");
  m_u_A = 0;
}

PlrConnImpl::PlrConnImpl()
{
  setDefaults();
}

PlrConnImpl::PlrConnImpl(int nr,int icon,std::string name,std::string desc)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
}

PlrConnImpl::PlrConnImpl(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,
                         double area,double coef,int u_A)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setLam(lam);
  setTurb(turb);
  setExpt(expt);
  setCoef(coef);
  setArea(area);
  setU_A(u_A);
}

PlrConnImpl::PlrConnImpl(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,
                         std::string area,std::string coef,int u_A)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setLam(lam);
  setTurb(turb);
  setExpt(expt);
  setCoef(coef);
  setArea(area);
  setU_A(u_A);
}

void PlrConnImpl::read(Reader &input)
{
  setNr(input.read<int>());
  setIcon(input.read<int>());
  std::string dataType = input.readString(); // Should really check this
  setName(input.readString());
  setDesc(input.readLine());
  setLam(input.readNumber<std::string>());
  setTurb(input.readNumber<std::string>());
  setExpt(input.readNumber<std::string>());
  setArea(input.readNumber<std::string>());
  setCoef(input.readNumber<std::string>());
  setU_A(input.read<int>());
}

std::string PlrConnImpl::write()
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_icon) + " plr_conn " + m_name + '\n';
  string += m_desc + '\n';
  string += ANY_TO_STR(m_lam) + ' ' + ANY_TO_STR(m_turb) + ' ' + ANY_TO_STR(m_expt) + ' ' + ANY_TO_STR(m_area)
    + ' ' + ANY_TO_STR(m_coef) + ' ' + ANY_TO_STR(m_u_A) + '\n';
  return string;
}

void PlrConnImpl::readDetails(Reader &input)
{
  setLam(input.readNumber<std::string>());
  setTurb(input.readNumber<std::string>());
  setExpt(input.readNumber<std::string>());
  setArea(input.readNumber<std::string>());
  setCoef(input.readNumber<std::string>());
  setU_A(input.read<int>());
}

int PlrConnImpl::nr() const
{
  return m_nr;
}

void PlrConnImpl::setNr(const int nr)
{
  m_nr = nr;
}

int PlrConnImpl::icon() const
{
  return m_icon;
}

void PlrConnImpl::setIcon(const int icon)
{
  m_icon = icon;
}

std::string PlrConnImpl::name() const
{
  return m_name;
}

void PlrConnImpl::setName(const std::string &name)
{
  m_name = name;
}

std::string PlrConnImpl::desc() const
{
  return m_desc;
}

void PlrConnImpl::setDesc(const std::string &desc)
{
  m_desc = desc;
}

double PlrConnImpl::lam() const
{
  return openstudio::string_conversions::to<double>(m_lam);
}

bool PlrConnImpl::setLam(const double lam)
{
  m_lam = openstudio::string_conversions::number(lam);
  return true;
}

bool PlrConnImpl::setLam(const std::string &lam)
{
  return assign_if_valid<double>(lam, m_lam);
}

double PlrConnImpl::turb() const
{
  return openstudio::string_conversions::to<double>(m_turb);
}

bool PlrConnImpl::setTurb(const double turb)
{
  m_turb = openstudio::string_conversions::number(turb);
  return true;
}

bool PlrConnImpl::setTurb(const std::string &turb)
{
  return assign_if_valid<double>(turb, m_turb);
}

double PlrConnImpl::expt() const
{
  return openstudio::string_conversions::to<double>(m_expt);
}

bool PlrConnImpl::setExpt(const double expt)
{
  m_expt = openstudio::string_conversions::number(expt);
  return true;
}

bool PlrConnImpl::setExpt(const std::string &expt)
{
  return assign_if_valid<double>(expt, m_expt);
}

double PlrConnImpl::area() const
{
  return openstudio::string_conversions::to<double>(m_area);
}

bool PlrConnImpl::setArea(const double area)
{
  m_area = openstudio::string_conversions::number(area);
  return true;
}

bool PlrConnImpl::setArea(const std::string &area)
{
  return assign_if_valid<double>(area, m_area);
}

double PlrConnImpl::coef() const
{
  return openstudio::string_conversions::to<double>(m_coef);
}

bool PlrConnImpl::setCoef(const double coef)
{
  m_coef = openstudio::string_conversions::number(coef);
  return true;
}

bool PlrConnImpl::setCoef(const std::string &coef)
{
  return assign_if_valid<double>(coef, m_coef);
}

int PlrConnImpl::u_A() const
{
  return m_u_A;
}

void PlrConnImpl::setU_A(const int u_A)
{
  m_u_A = u_A;
}

void PlrGeneralImpl::setDefaults()
{
  m_nr = 0;
  m_icon = 0;
  m_lam = PRJFLOAT("0.0");
  m_turb = PRJFLOAT("0.0");
  m_expt = PRJFLOAT("0.0");
}

PlrGeneralImpl::PlrGeneralImpl()
{
  setDefaults();
}

PlrGeneralImpl::PlrGeneralImpl(int nr,int icon,std::string name,std::string desc)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
}

PlrGeneralImpl::PlrGeneralImpl(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setLam(lam);
  setTurb(turb);
  setExpt(expt);
}

PlrGeneralImpl::PlrGeneralImpl(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setLam(lam);
  setTurb(turb);
  setExpt(expt);
}

void PlrGeneralImpl::read(Reader &input)
{
  setNr(input.read<int>());
  setIcon(input.read<int>());
  std::string dataType = input.readString(); // Should really check this
  setName(input.readString());
  setDesc(input.readLine());
  setLam(input.readNumber<std::string>());
  setTurb(input.readNumber<std::string>());
  setExpt(input.readNumber<std::string>());
}

std::string PlrGeneralImpl::write(std::string dataType)
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_icon) + ' ' + dataType + ' ' + m_name + '\n';
  string += m_desc + '\n';
  string += ANY_TO_STR(m_lam) + ' ' + ANY_TO_STR(m_turb) + ' ' + ANY_TO_STR(m_expt) + '\n';
  return string;
}

void PlrGeneralImpl::readDetails(Reader &input)
{
  setLam(input.readNumber<std::string>());
  setTurb(input.readNumber<std::string>());
  setExpt(input.readNumber<std::string>());
}

int PlrGeneralImpl::nr() const
{
  return m_nr;
}

void PlrGeneralImpl::setNr(const int nr)
{
  m_nr = nr;
}

int PlrGeneralImpl::icon() const
{
  return m_icon;
}

void PlrGeneralImpl::setIcon(const int icon)
{
  m_icon = icon;
}

std::string PlrGeneralImpl::name() const
{
  return m_name;
}

void PlrGeneralImpl::setName(const std::string &name)
{
  m_name = name;
}

std::string PlrGeneralImpl::desc() const
{
  return m_desc;
}

void PlrGeneralImpl::setDesc(const std::string &desc)
{
  m_desc = desc;
}

double PlrGeneralImpl::lam() const
{
  return openstudio::string_conversions::to<double>(m_lam);
}

bool PlrGeneralImpl::setLam(const double lam)
{
  m_lam = openstudio::string_conversions::number(lam);
  return true;
}

bool PlrGeneralImpl::setLam(const std::string &lam)
{
  return assign_if_valid<double>(lam, m_lam);
}

double PlrGeneralImpl::turb() const
{
  return openstudio::string_conversions::to<double>(m_turb);
}

bool PlrGeneralImpl::setTurb(const double turb)
{
  m_turb = openstudio::string_conversions::number(turb);
  return true;
}

bool PlrGeneralImpl::setTurb(const std::string &turb)
{
  return assign_if_valid<double>(turb, m_turb);
}

double PlrGeneralImpl::expt() const
{
  return openstudio::string_conversions::to<double>(m_expt);
}

bool PlrGeneralImpl::setExpt(const double expt)
{
  m_expt = openstudio::string_conversions::number(expt);
  return true;
}

bool PlrGeneralImpl::setExpt(const std::string &expt)
{
  return assign_if_valid<double>(expt, m_expt);
}

void PlrTest1Impl::setDefaults()
{
  m_nr = 0;
  m_icon = 0;
  m_lam = PRJFLOAT("0.0");
  m_turb = PRJFLOAT("0.0");
  m_expt = PRJFLOAT("0.0");
  m_dP = PRJFLOAT("0.0");
  m_Flow = PRJFLOAT("0.0");
  m_u_P = 0;
  m_u_F = 0;
}

PlrTest1Impl::PlrTest1Impl()
{
  setDefaults();
}

PlrTest1Impl::PlrTest1Impl(int nr,int icon,std::string name,std::string desc)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
}

PlrTest1Impl::PlrTest1Impl(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,
                           double dP,double Flow,int u_P,int u_F)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setLam(lam);
  setTurb(turb);
  setExpt(expt);
  setDP(dP);
  setFlow(Flow);
  setU_P(u_P);
  setU_F(u_F);
}

PlrTest1Impl::PlrTest1Impl(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,
                           std::string dP,std::string Flow,int u_P,int u_F)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setLam(lam);
  setTurb(turb);
  setExpt(expt);
  setDP(dP);
  setFlow(Flow);
  setU_P(u_P);
  setU_F(u_F);
}

void PlrTest1Impl::read(Reader &input)
{
  setNr(input.read<int>());
  setIcon(input.read<int>());
  std::string dataType = input.readString(); // Should really check this
  setName(input.readString());
  setDesc(input.readLine());
  setLam(input.readNumber<std::string>());
  setTurb(input.readNumber<std::string>());
  setExpt(input.readNumber<std::string>());
  setDP(input.readNumber<std::string>());
  setFlow(input.readNumber<std::string>());
  setU_P(input.read<int>());
  setU_F(input.read<int>());
}

std::string PlrTest1Impl::write()
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_icon) + " plr_test1 " + m_name + '\n';
  string += m_desc + '\n';
  string += ANY_TO_STR(m_lam) + ' ' + ANY_TO_STR(m_turb) + ' ' + ANY_TO_STR(m_expt) + ' '
    + ANY_TO_STR(m_dP) + ' ' + ANY_TO_STR(m_Flow) + ' ' + ANY_TO_STR(m_u_P) + ' '
    + ANY_TO_STR(m_u_F) + '\n';
  return string;
}

void PlrTest1Impl::readDetails(Reader &input)
{
  setLam(input.readNumber<std::string>());
  setTurb(input.readNumber<std::string>());
  setExpt(input.readNumber<std::string>());
  setDP(input.readNumber<std::string>());
  setFlow(input.readNumber<std::string>());
  setU_P(input.read<int>());
  setU_F(input.read<int>());
}

int PlrTest1Impl::nr() const
{
  return m_nr;
}

void PlrTest1Impl::setNr(const int nr)
{
  m_nr = nr;
}

int PlrTest1Impl::icon() const
{
  return m_icon;
}

void PlrTest1Impl::setIcon(const int icon)
{
  m_icon = icon;
}

std::string PlrTest1Impl::name() const
{
  return m_name;
}

void PlrTest1Impl::setName(const std::string &name)
{
  m_name = name;
}

std::string PlrTest1Impl::desc() const
{
  return m_desc;
}

void PlrTest1Impl::setDesc(const std::string &desc)
{
  m_desc = desc;
}

double PlrTest1Impl::lam() const
{
  return openstudio::string_conversions::to<double>(m_lam);
}

bool PlrTest1Impl::setLam(const double lam)
{
  m_lam = openstudio::string_conversions::number(lam);
  return true;
}

bool PlrTest1Impl::setLam(const std::string &lam)
{
  return assign_if_valid<double>(lam, m_lam);
}

double PlrTest1Impl::turb() const
{
  return openstudio::string_conversions::to<double>(m_turb);
}

bool PlrTest1Impl::setTurb(const double turb)
{
  m_turb = openstudio::string_conversions::number(turb);
  return true;
}

bool PlrTest1Impl::setTurb(const std::string &turb)
{
  return assign_if_valid<double>(turb, m_turb);
}

double PlrTest1Impl::expt() const
{
  return openstudio::string_conversions::to<double>(m_expt);
}

bool PlrTest1Impl::setExpt(const double expt)
{
  m_expt = openstudio::string_conversions::number(expt);
  return true;
}

bool PlrTest1Impl::setExpt(const std::string &expt)
{
  return assign_if_valid<double>(expt, m_expt);
}

double PlrTest1Impl::dP() const
{
  return openstudio::string_conversions::to<double>(m_dP);
}

bool PlrTest1Impl::setDP(const double dP)
{
  m_dP = openstudio::string_conversions::number(dP);
  return true;
}

bool PlrTest1Impl::setDP(const std::string &dP)
{
  return assign_if_valid<double>(dP, m_dP);
}

double PlrTest1Impl::Flow() const
{
  return openstudio::string_conversions::to<double>(m_Flow);
}

bool PlrTest1Impl::setFlow(const double Flow)
{
  m_Flow = openstudio::string_conversions::number(Flow);
  return true;
}

bool PlrTest1Impl::setFlow(const std::string &Flow)
{
  return assign_if_valid<double>(Flow, m_Flow);
}

int PlrTest1Impl::u_P() const
{
  return m_u_P;
}

void PlrTest1Impl::setU_P(const int u_P)
{
  m_u_P = u_P;
}

int PlrTest1Impl::u_F() const
{
  return m_u_F;
}

void PlrTest1Impl::setU_F(const int u_F)
{
  m_u_F = u_F;
}

void PlrTest2Impl::setDefaults()
{
  m_nr = 0;
  m_icon = 0;
  m_lam = PRJFLOAT("0.0");
  m_turb = PRJFLOAT("0.0");
  m_expt = PRJFLOAT("0.0");
  m_dP1 = PRJFLOAT("0.0");
  m_F1 = PRJFLOAT("0.0");
  m_dP2 = PRJFLOAT("0.0");
  m_F2 = PRJFLOAT("0.0");
  m_u_P1 = 0;
  m_u_F1 = 0;
  m_u_P2 = 0;
  m_u_F2 = 0;
}

PlrTest2Impl::PlrTest2Impl()
{
  setDefaults();
}

PlrTest2Impl::PlrTest2Impl(int nr,int icon,std::string name,std::string desc)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
}

PlrTest2Impl::PlrTest2Impl(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,
                           double dP1,double F1,double dP2,double F2,int u_P1,int u_F1,int u_P2,int u_F2)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setLam(lam);
  setTurb(turb);
  setExpt(expt);
  setDP1(dP1);
  setF1(F1);
  setDP2(dP2);
  setF2(F2);
  setU_P1(u_P1);
  setU_F1(u_F1);
  setU_P2(u_P2);
  setU_F2(u_F2);
}

PlrTest2Impl::PlrTest2Impl(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,
                           std::string dP1,std::string F1,std::string dP2,std::string F2,int u_P1,int u_F1,int u_P2,int u_F2)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setLam(lam);
  setTurb(turb);
  setExpt(expt);
  setDP1(dP1);
  setF1(F1);
  setDP2(dP2);
  setF2(F2);
  setU_P1(u_P1);
  setU_F1(u_F1);
  setU_P2(u_P2);
  setU_F2(u_F2);
}

void PlrTest2Impl::read(Reader &input)
{
  setNr(input.read<int>());
  setIcon(input.read<int>());
  std::string dataType = input.readString(); // Should really check this
  setName(input.readString());
  setDesc(input.readLine());
  setLam(input.readNumber<std::string>());
  setTurb(input.readNumber<std::string>());
  setExpt(input.readNumber<std::string>());
  setDP1(input.readNumber<std::string>());
  setF1(input.readNumber<std::string>());
  setDP2(input.readNumber<std::string>());
  setF2(input.readNumber<std::string>());
  setU_P1(input.read<int>());
  setU_F1(input.read<int>());
  setU_P2(input.read<int>());
  setU_F2(input.read<int>());
}

std::string PlrTest2Impl::write()
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_icon) + " plr_test2 " + m_name + '\n';
  string += m_desc + '\n';
  string += ANY_TO_STR(m_lam) + ' ' + ANY_TO_STR(m_turb) + ' ' + ANY_TO_STR(m_expt) + ' ' + ANY_TO_STR(m_dP1)
    + ' ' + ANY_TO_STR(m_F1) + ' ' + ANY_TO_STR(m_dP2) + ' ' + ANY_TO_STR(m_F2) + ' '
    + ANY_TO_STR(m_u_P1) + ' ' + ANY_TO_STR(m_u_F1) + ' ' + ANY_TO_STR(m_u_P2) + ' ' + ANY_TO_STR(m_u_F2)
    + '\n';
  return string;
}

void PlrTest2Impl::readDetails(Reader &input)
{
  setLam(input.readNumber<std::string>());
  setTurb(input.readNumber<std::string>());
  setExpt(input.readNumber<std::string>());
  setDP1(input.readNumber<std::string>());
  setF1(input.readNumber<std::string>());
  setDP2(input.readNumber<std::string>());
  setF2(input.readNumber<std::string>());
  setU_P1(input.read<int>());
  setU_F1(input.read<int>());
  setU_P2(input.read<int>());
  setU_F2(input.read<int>());
}

int PlrTest2Impl::nr() const
{
  return m_nr;
}

void PlrTest2Impl::setNr(const int nr)
{
  m_nr = nr;
}

int PlrTest2Impl::icon() const
{
  return m_icon;
}

void PlrTest2Impl::setIcon(const int icon)
{
  m_icon = icon;
}

std::string PlrTest2Impl::name() const
{
  return m_name;
}

void PlrTest2Impl::setName(const std::string &name)
{
  m_name = name;
}

std::string PlrTest2Impl::desc() const
{
  return m_desc;
}

void PlrTest2Impl::setDesc(const std::string &desc)
{
  m_desc = desc;
}

double PlrTest2Impl::lam() const
{
  return openstudio::string_conversions::to<double>(m_lam);
}

bool PlrTest2Impl::setLam(const double lam)
{
  m_lam = openstudio::string_conversions::number(lam);
  return true;
}

bool PlrTest2Impl::setLam(const std::string &lam)
{
  return assign_if_valid<double>(lam, m_lam);
}

double PlrTest2Impl::turb() const
{
  return openstudio::string_conversions::to<double>(m_turb);
}

bool PlrTest2Impl::setTurb(const double turb)
{
  m_turb = openstudio::string_conversions::number(turb);
  return true;
}

bool PlrTest2Impl::setTurb(const std::string &turb)
{
  return assign_if_valid<double>(turb, m_turb);
}

double PlrTest2Impl::expt() const
{
  return openstudio::string_conversions::to<double>(m_expt);
}

bool PlrTest2Impl::setExpt(const double expt)
{
  m_expt = openstudio::string_conversions::number(expt);
  return true;
}

bool PlrTest2Impl::setExpt(const std::string &expt)
{
  return assign_if_valid<double>(expt, m_expt);
}

double PlrTest2Impl::dP1() const
{
  return openstudio::string_conversions::to<double>(m_dP1);
}

bool PlrTest2Impl::setDP1(const double dP1)
{
  m_dP1 = openstudio::string_conversions::number(dP1);
  return true;
}

bool PlrTest2Impl::setDP1(const std::string &dP1)
{
  return assign_if_valid<double>(dP1, m_dP1);
}

double PlrTest2Impl::F1() const
{
  return openstudio::string_conversions::to<double>(m_F1);
}

bool PlrTest2Impl::setF1(const double F1)
{
  m_F1 = openstudio::string_conversions::number(F1);
  return true;
}

bool PlrTest2Impl::setF1(const std::string &F1)
{
  return assign_if_valid<double>(F1, m_F1);
}

double PlrTest2Impl::dP2() const
{
  return openstudio::string_conversions::to<double>(m_dP2);
}

bool PlrTest2Impl::setDP2(const double dP2)
{
  m_dP2 = openstudio::string_conversions::number(dP2);
  return true;
}

bool PlrTest2Impl::setDP2(const std::string &dP2)
{
  return assign_if_valid<double>(dP2, m_dP2);
}

double PlrTest2Impl::F2() const
{
  return openstudio::string_conversions::to<double>(m_F2);
}

bool PlrTest2Impl::setF2(const double F2)
{
  m_F2 = openstudio::string_conversions::number(F2);
  return true;
}

bool PlrTest2Impl::setF2(const std::string &F2)
{
  return assign_if_valid<double>(F2, m_F2);
}

int PlrTest2Impl::u_P1() const
{
  return m_u_P1;
}

void PlrTest2Impl::setU_P1(const int u_P1)
{
  m_u_P1 = u_P1;
}

int PlrTest2Impl::u_F1() const
{
  return m_u_F1;
}

void PlrTest2Impl::setU_F1(const int u_F1)
{
  m_u_F1 = u_F1;
}

int PlrTest2Impl::u_P2() const
{
  return m_u_P2;
}

void PlrTest2Impl::setU_P2(const int u_P2)
{
  m_u_P2 = u_P2;
}

int PlrTest2Impl::u_F2() const
{
  return m_u_F2;
}

void PlrTest2Impl::setU_F2(const int u_F2)
{
  m_u_F2 = u_F2;
}

void PlrCrackImpl::setDefaults()
{
  m_nr = 0;
  m_icon = 0;
  m_lam = PRJFLOAT("0.0");
  m_turb = PRJFLOAT("0.0");
  m_expt = PRJFLOAT("0.0");
  m_length = PRJFLOAT("0.0");
  m_width = PRJFLOAT("0.0");
  m_u_L = 0;
  m_u_W = 0;
}

PlrCrackImpl::PlrCrackImpl()
{
  setDefaults();
}

PlrCrackImpl::PlrCrackImpl(int nr,int icon,std::string name,std::string desc)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
}

PlrCrackImpl::PlrCrackImpl(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,
                           double length,double width,int u_L,int u_W)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setLam(lam);
  setTurb(turb);
  setExpt(expt);
  setLength(length);
  setWidth(width);
  setU_L(u_L);
  setU_W(u_W);
}

PlrCrackImpl::PlrCrackImpl(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,
                           std::string length,std::string width,int u_L,int u_W)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setLam(lam);
  setTurb(turb);
  setExpt(expt);
  setLength(length);
  setWidth(width);
  setU_L(u_L);
  setU_W(u_W);
}

void PlrCrackImpl::read(Reader &input)
{
  setNr(input.read<int>());
  setIcon(input.read<int>());
  std::string dataType = input.readString(); // Should really check this
  setName(input.readString());
  setDesc(input.readLine());
  setLam(input.readNumber<std::string>());
  setTurb(input.readNumber<std::string>());
  setExpt(input.readNumber<std::string>());
  setLength(input.readNumber<std::string>());
  setWidth(input.readNumber<std::string>());
  setU_L(input.read<int>());
  setU_W(input.read<int>());
}

std::string PlrCrackImpl::write()
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_icon) + " plr_crack " + m_name + '\n';
  string += m_desc + '\n';
  string += ANY_TO_STR(m_lam) + ' ' + ANY_TO_STR(m_turb) + ' ' + ANY_TO_STR(m_expt) + ' '
    + ANY_TO_STR(m_length) + ' ' + ANY_TO_STR(m_width) + ' ' + ANY_TO_STR(m_u_L) + ' '
    + ANY_TO_STR(m_u_W) + '\n';
  return string;
}

void PlrCrackImpl::readDetails(Reader &input)
{
  setLam(input.readNumber<std::string>());
  setTurb(input.readNumber<std::string>());
  setExpt(input.readNumber<std::string>());
  setLength(input.readNumber<std::string>());
  setWidth(input.readNumber<std::string>());
  setU_L(input.read<int>());
  setU_W(input.read<int>());
}

int PlrCrackImpl::nr() const
{
  return m_nr;
}

void PlrCrackImpl::setNr(const int nr)
{
  m_nr = nr;
}

int PlrCrackImpl::icon() const
{
  return m_icon;
}

void PlrCrackImpl::setIcon(const int icon)
{
  m_icon = icon;
}

std::string PlrCrackImpl::name() const
{
  return m_name;
}

void PlrCrackImpl::setName(const std::string &name)
{
  m_name = name;
}

std::string PlrCrackImpl::desc() const
{
  return m_desc;
}

void PlrCrackImpl::setDesc(const std::string &desc)
{
  m_desc = desc;
}

double PlrCrackImpl::lam() const
{
  return openstudio::string_conversions::to<double>(m_lam);
}

bool PlrCrackImpl::setLam(const double lam)
{
  m_lam = openstudio::string_conversions::number(lam);
  return true;
}

bool PlrCrackImpl::setLam(const std::string &lam)
{
  return assign_if_valid<double>(lam, m_lam);
}

double PlrCrackImpl::turb() const
{
  return openstudio::string_conversions::to<double>(m_turb);
}

bool PlrCrackImpl::setTurb(const double turb)
{
  m_turb = openstudio::string_conversions::number(turb);
  return true;
}

bool PlrCrackImpl::setTurb(const std::string &turb)
{
  return assign_if_valid<double>(turb, m_turb);
}

double PlrCrackImpl::expt() const
{
  return openstudio::string_conversions::to<double>(m_expt);
}

bool PlrCrackImpl::setExpt(const double expt)
{
  m_expt = openstudio::string_conversions::number(expt);
  return true;
}

bool PlrCrackImpl::setExpt(const std::string &expt)
{
  return assign_if_valid<double>(expt, m_expt);
}

double PlrCrackImpl::length() const
{
  return openstudio::string_conversions::to<double>(m_length);
}

bool PlrCrackImpl::setLength(const double length)
{
  m_length = openstudio::string_conversions::number(length);
  return true;
}

bool PlrCrackImpl::setLength(const std::string &length)
{
  return assign_if_valid<double>(length, m_length);
}

double PlrCrackImpl::width() const
{
  return openstudio::string_conversions::to<double>(m_width);
}

bool PlrCrackImpl::setWidth(const double width)
{
  m_width = openstudio::string_conversions::number(width);
  return true;
}

bool PlrCrackImpl::setWidth(const std::string &width)
{
  return assign_if_valid<double>(width, m_width);
}

int PlrCrackImpl::u_L() const
{
  return m_u_L;
}

void PlrCrackImpl::setU_L(const int u_L)
{
  m_u_L = u_L;
}

int PlrCrackImpl::u_W() const
{
  return m_u_W;
}

void PlrCrackImpl::setU_W(const int u_W)
{
  m_u_W = u_W;
}

void PlrStairImpl::setDefaults()
{
  m_nr = 0;
  m_icon = 0;
  m_lam = PRJFLOAT("0.0");
  m_turb = PRJFLOAT("0.0");
  m_expt = PRJFLOAT("0.0");
  m_Ht = PRJFLOAT("0.0");
  m_Area = PRJFLOAT("0.0");
  m_peo = PRJFLOAT("0.0");
  m_tread = 0;
  m_u_A = 0;
  m_u_D = 0;
}

PlrStairImpl::PlrStairImpl()
{
  setDefaults();
}

PlrStairImpl::PlrStairImpl(int nr,int icon,std::string name,std::string desc)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
}

PlrStairImpl::PlrStairImpl(int nr,int icon,std::string name,std::string desc,double lam,double turb,
                           double expt,double Ht,double Area,double peo,int tread,int u_A,
                           int u_D)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setLam(lam);
  setTurb(turb);
  setExpt(expt);
  setHt(Ht);
  setArea(Area);
  setPeople(peo);
  setTread(tread);
  setU_A(u_A);
  setU_D(u_D);
}

PlrStairImpl::PlrStairImpl(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,
                           std::string expt,std::string Ht,std::string Area,std::string peo,int tread,int u_A,
                           int u_D)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setLam(lam);
  setTurb(turb);
  setExpt(expt);
  setHt(Ht);
  setArea(Area);
  setPeople(peo);
  setTread(tread);
  setU_A(u_A);
  setU_D(u_D);
}


void PlrStairImpl::read(Reader &input)
{
  setNr(input.read<int>());
  setIcon(input.read<int>());
  std::string dataType = input.readString(); // Should really check this
  setName(input.readString());
  setDesc(input.readLine());
  setLam(input.readNumber<std::string>());
  setTurb(input.readNumber<std::string>());
  setExpt(input.readNumber<std::string>());
  setHt(input.readNumber<std::string>());
  setArea(input.readNumber<std::string>());
  setPeople(input.readNumber<std::string>());
  setTread(input.read<int>());
  setU_A(input.read<int>());
  setU_D(input.read<int>());
}

std::string PlrStairImpl::write()
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_icon) + " plr_stair " + m_name + '\n';
  string += m_desc + '\n';
  string += ANY_TO_STR(m_lam) + ' ' + ANY_TO_STR(m_turb) + ' ' + ANY_TO_STR(m_expt) + ' '
    + ANY_TO_STR(m_Ht) + ' ' + ANY_TO_STR(m_Area) + ' ' + ANY_TO_STR(m_peo) + ' '
    + ANY_TO_STR(m_tread) + ' ' + ANY_TO_STR(m_u_A) + ' ' + ANY_TO_STR(m_u_D) + '\n';
  return string;
}

void PlrStairImpl::readDetails(Reader &input)
{
  setLam(input.readNumber<std::string>());
  setTurb(input.readNumber<std::string>());
  setExpt(input.readNumber<std::string>());
  setHt(input.readNumber<std::string>());
  setArea(input.readNumber<std::string>());
  setPeople(input.readNumber<std::string>());
  setTread(input.read<int>());
  setU_A(input.read<int>());
  setU_D(input.read<int>());
}

int PlrStairImpl::nr() const
{
  return m_nr;
}

void PlrStairImpl::setNr(const int nr)
{
  m_nr = nr;
}

int PlrStairImpl::icon() const
{
  return m_icon;
}

void PlrStairImpl::setIcon(const int icon)
{
  m_icon = icon;
}

std::string PlrStairImpl::name() const
{
  return m_name;
}

void PlrStairImpl::setName(const std::string &name)
{
  m_name = name;
}

std::string PlrStairImpl::desc() const
{
  return m_desc;
}

void PlrStairImpl::setDesc(const std::string &desc)
{
  m_desc = desc;
}

double PlrStairImpl::lam() const
{
  return openstudio::string_conversions::to<double>(m_lam);
}

bool PlrStairImpl::setLam(const double lam)
{
  m_lam = openstudio::string_conversions::number(lam);
  return true;
}

bool PlrStairImpl::setLam(const std::string &lam)
{
  return assign_if_valid<double>(lam, m_lam);
}

double PlrStairImpl::turb() const
{
  return openstudio::string_conversions::to<double>(m_turb);
}

bool PlrStairImpl::setTurb(const double turb)
{
  m_turb = openstudio::string_conversions::number(turb);
  return true;
}

bool PlrStairImpl::setTurb(const std::string &turb)
{
  return assign_if_valid<double>(turb, m_turb);
}

double PlrStairImpl::expt() const
{
  return openstudio::string_conversions::to<double>(m_expt);
}

bool PlrStairImpl::setExpt(const double expt)
{
  m_expt = openstudio::string_conversions::number(expt);
  return true;
}

bool PlrStairImpl::setExpt(const std::string &expt)
{
  return assign_if_valid<double>(expt, m_expt);
}

double PlrStairImpl::Ht() const
{
  return openstudio::string_conversions::to<double>(m_Ht);
}

bool PlrStairImpl::setHt(const double Ht)
{
  m_Ht = openstudio::string_conversions::number(Ht);
  return true;
}

bool PlrStairImpl::setHt(const std::string &Ht)
{
  return assign_if_valid<double>(Ht, m_Ht);
}

double PlrStairImpl::area() const
{
  return openstudio::string_conversions::to<double>(m_Area);
}

bool PlrStairImpl::setArea(const double Area)
{
  m_Area = openstudio::string_conversions::number(Area);
  return true;
}

bool PlrStairImpl::setArea(const std::string &Area)
{
  return assign_if_valid<double>(Area, m_Area);
}

double PlrStairImpl::people() const
{
  return openstudio::string_conversions::to<double>(m_peo);
}

bool PlrStairImpl::setPeople(const double peo)
{
  m_peo = openstudio::string_conversions::number(peo);
  return true;
}

bool PlrStairImpl::setPeople(const std::string &peo)
{
  return assign_if_valid<double>(peo, m_peo);
}

int PlrStairImpl::tread() const
{
  return m_tread;
}

void PlrStairImpl::setTread(const int tread)
{
  m_tread = tread;
}

int PlrStairImpl::u_A() const
{
  return m_u_A;
}

void PlrStairImpl::setU_A(const int u_A)
{
  m_u_A = u_A;
}

int PlrStairImpl::u_D() const
{
  return m_u_D;
}

void PlrStairImpl::setU_D(const int u_D)
{
  m_u_D = u_D;
}

void PlrShaftImpl::setDefaults()
{
  m_nr = 0;
  m_icon = 0;
  m_lam = PRJFLOAT("0.0");
  m_turb = PRJFLOAT("0.0");
  m_expt = PRJFLOAT("0.0");
  m_Ht = PRJFLOAT("0.0");
  m_area = PRJFLOAT("0.0");
  m_perim = PRJFLOAT("0.0");
  m_rough = PRJFLOAT("0.0");
  m_u_A = 0;
  m_u_D = 0;
  m_u_P = 0;
  m_u_R = 0;
}

PlrShaftImpl::PlrShaftImpl()
{
  setDefaults();
}

PlrShaftImpl::PlrShaftImpl(int nr,int icon,std::string name,std::string desc)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
}

PlrShaftImpl::PlrShaftImpl(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,
                           double Ht,double area,double perim,double rough,int u_A,int u_D,int u_P,int u_R)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setLam(lam);
  setTurb(turb);
  setExpt(expt);
  setHt(Ht);
  setArea(area);
  setPerim(perim);
  setRough(rough);
  setU_A(u_A);
  setU_D(u_D);
  setU_P(u_P);
  setU_R(u_R);
}

PlrShaftImpl::PlrShaftImpl(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,
                           std::string Ht,std::string area,std::string perim,std::string rough,int u_A,int u_D,int u_P,int u_R)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setLam(lam);
  setTurb(turb);
  setExpt(expt);
  setHt(Ht);
  setArea(area);
  setPerim(perim);
  setRough(rough);
  setU_A(u_A);
  setU_D(u_D);
  setU_P(u_P);
  setU_R(u_R);
}

void PlrShaftImpl::read(Reader &input)
{
  setNr(input.read<int>());
  setIcon(input.read<int>());
  std::string dataType = input.readString(); // Should really check this
  setName(input.readString());
  setDesc(input.readLine());
  setLam(input.readNumber<std::string>());
  setTurb(input.readNumber<std::string>());
  setExpt(input.readNumber<std::string>());
  setHt(input.readNumber<std::string>());
  setArea(input.readNumber<std::string>());
  setPerim(input.readNumber<std::string>());
  setRough(input.readNumber<std::string>());
  setU_A(input.read<int>());
  setU_D(input.read<int>());
  setU_P(input.read<int>());
  setU_R(input.read<int>());
}

std::string PlrShaftImpl::write()
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_icon) + " plr_shaft " + m_name + '\n';
  string += m_desc + '\n';
  string += ANY_TO_STR(m_lam) + ' ' + ANY_TO_STR(m_turb) + ' ' + ANY_TO_STR(m_expt) + ' ' + ANY_TO_STR(m_Ht)
    + ' ' + ANY_TO_STR(m_area) + ' ' + ANY_TO_STR(m_perim) + ' ' + ANY_TO_STR(m_rough) + ' '
    + ANY_TO_STR(m_u_A) + ' ' + ANY_TO_STR(m_u_D) + ' ' + ANY_TO_STR(m_u_P) + ' ' + ANY_TO_STR(m_u_R)
    + '\n';
  return string;
}

void PlrShaftImpl::readDetails(Reader &input)
{
  setLam(input.readNumber<std::string>());
  setTurb(input.readNumber<std::string>());
  setExpt(input.readNumber<std::string>());
  setHt(input.readNumber<std::string>());
  setArea(input.readNumber<std::string>());
  setPerim(input.readNumber<std::string>());
  setRough(input.readNumber<std::string>());
  setU_A(input.read<int>());
  setU_D(input.read<int>());
  setU_P(input.read<int>());
  setU_R(input.read<int>());
}

int PlrShaftImpl::nr() const
{
  return m_nr;
}

void PlrShaftImpl::setNr(const int nr)
{
  m_nr = nr;
}

int PlrShaftImpl::icon() const
{
  return m_icon;
}

void PlrShaftImpl::setIcon(const int icon)
{
  m_icon = icon;
}

std::string PlrShaftImpl::name() const
{
  return m_name;
}

void PlrShaftImpl::setName(const std::string &name)
{
  m_name = name;
}

std::string PlrShaftImpl::desc() const
{
  return m_desc;
}

void PlrShaftImpl::setDesc(const std::string &desc)
{
  m_desc = desc;
}

double PlrShaftImpl::lam() const
{
  return openstudio::string_conversions::to<double>(m_lam);
}

bool PlrShaftImpl::setLam(const double lam)
{
  m_lam = openstudio::string_conversions::number(lam);
  return true;
}

bool PlrShaftImpl::setLam(const std::string &lam)
{
  return assign_if_valid<double>(lam, m_lam);
}

double PlrShaftImpl::turb() const
{
  return openstudio::string_conversions::to<double>(m_turb);
}

bool PlrShaftImpl::setTurb(const double turb)
{
  m_turb = openstudio::string_conversions::number(turb);
  return true;
}

bool PlrShaftImpl::setTurb(const std::string &turb)
{
  return assign_if_valid<double>(turb, m_turb);
}

double PlrShaftImpl::expt() const
{
  return openstudio::string_conversions::to<double>(m_expt);
}

bool PlrShaftImpl::setExpt(const double expt)
{
  m_expt = openstudio::string_conversions::number(expt);
  return true;
}

bool PlrShaftImpl::setExpt(const std::string &expt)
{
  return assign_if_valid<double>(expt, m_expt);
}

double PlrShaftImpl::Ht() const
{
  return openstudio::string_conversions::to<double>(m_Ht);
}

bool PlrShaftImpl::setHt(const double Ht)
{
  m_Ht = openstudio::string_conversions::number(Ht);
  return true;
}

bool PlrShaftImpl::setHt(const std::string &Ht)
{
  return assign_if_valid<double>(Ht, m_Ht);
}

double PlrShaftImpl::area() const
{
  return openstudio::string_conversions::to<double>(m_area);
}

bool PlrShaftImpl::setArea(const double area)
{
  m_area = openstudio::string_conversions::number(area);
  return true;
}

bool PlrShaftImpl::setArea(const std::string &area)
{
  return assign_if_valid<double>(area, m_area);
}

double PlrShaftImpl::perim() const
{
  return openstudio::string_conversions::to<double>(m_perim);
}

bool PlrShaftImpl::setPerim(const double perim)
{
  m_perim = openstudio::string_conversions::number(perim);
  return true;
}

bool PlrShaftImpl::setPerim(const std::string &perim)
{
  return assign_if_valid<double>(perim, m_perim);
}

double PlrShaftImpl::rough() const
{
  return openstudio::string_conversions::to<double>(m_rough);
}

bool PlrShaftImpl::setRough(const double rough)
{
  m_rough = openstudio::string_conversions::number(rough);
  return true;
}

bool PlrShaftImpl::setRough(const std::string &rough)
{
  return assign_if_valid<double>(rough, m_rough);
}

int PlrShaftImpl::u_A() const
{
  return m_u_A;
}

void PlrShaftImpl::setU_A(const int u_A)
{
  m_u_A = u_A;
}

int PlrShaftImpl::u_D() const
{
  return m_u_D;
}

void PlrShaftImpl::setU_D(const int u_D)
{
  m_u_D = u_D;
}

int PlrShaftImpl::u_P() const
{
  return m_u_P;
}

void PlrShaftImpl::setU_P(const int u_P)
{
  m_u_P = u_P;
}

int PlrShaftImpl::u_R() const
{
  return m_u_R;
}

void PlrShaftImpl::setU_R(const int u_R)
{
  m_u_R = u_R;
}

void PlrBackDamperImpl::setDefaults()
{
  m_nr = 0;
  m_icon = 0;
  m_lam = PRJFLOAT("0.0");
  m_Cp = PRJFLOAT("0.0");
  m_xp = PRJFLOAT("0.0");
  m_Cn = PRJFLOAT("0.0");
  m_xn = PRJFLOAT("0.0");
}

PlrBackDamperImpl::PlrBackDamperImpl()
{
  setDefaults();
}

PlrBackDamperImpl::PlrBackDamperImpl(int nr,int icon,std::string name,std::string desc)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
}

PlrBackDamperImpl::PlrBackDamperImpl(int nr,int icon,std::string name,std::string desc,double lam,double Cp,double xp,double Cn,
                                     double xn)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setLam(lam);
  setCp(Cp);
  setXp(xp);
  setCn(Cn);
  setXn(xn);
}

PlrBackDamperImpl::PlrBackDamperImpl(int nr,int icon,std::string name,std::string desc,std::string lam,std::string Cp,std::string xp,std::string Cn,
                                     std::string xn)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setLam(lam);
  setCp(Cp);
  setXp(xp);
  setCn(Cn);
  setXn(xn);
}

void PlrBackDamperImpl::read(Reader &input)
{
  setNr(input.read<int>());
  setIcon(input.read<int>());
  std::string dataType = input.readString(); // Should really check this
  setName(input.readString());
  setDesc(input.readLine());
  setLam(input.readNumber<std::string>());
  setCp(input.readNumber<std::string>());
  setXp(input.readNumber<std::string>());
  setCn(input.readNumber<std::string>());
  setXn(input.readNumber<std::string>());
}

std::string PlrBackDamperImpl::write(std::string dataType)
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_icon) + ' ' + dataType + ' ' + m_name + '\n';
  string += m_desc + '\n';
  string += ANY_TO_STR(m_lam) + ' ' + ANY_TO_STR(m_Cp) + ' ' + ANY_TO_STR(m_xp) + ' ' + ANY_TO_STR(m_Cn)
    + ' ' + ANY_TO_STR(m_xn) + '\n';
  return string;
}

void PlrBackDamperImpl::readDetails(Reader &input)
{
  setLam(input.readNumber<std::string>());
  setCp(input.readNumber<std::string>());
  setXp(input.readNumber<std::string>());
  setCn(input.readNumber<std::string>());
  setXn(input.readNumber<std::string>());
}

int PlrBackDamperImpl::nr() const
{
  return m_nr;
}

void PlrBackDamperImpl::setNr(const int nr)
{
  m_nr = nr;
}

int PlrBackDamperImpl::icon() const
{
  return m_icon;
}

void PlrBackDamperImpl::setIcon(const int icon)
{
  m_icon = icon;
}

std::string PlrBackDamperImpl::name() const
{
  return m_name;
}

void PlrBackDamperImpl::setName(const std::string &name)
{
  m_name = name;
}

std::string PlrBackDamperImpl::desc() const
{
  return m_desc;
}

void PlrBackDamperImpl::setDesc(const std::string &desc)
{
  m_desc = desc;
}

double PlrBackDamperImpl::lam() const
{
  return openstudio::string_conversions::to<double>(m_lam);
}

bool PlrBackDamperImpl::setLam(const double lam)
{
  m_lam = openstudio::string_conversions::number(lam);
  return true;
}

bool PlrBackDamperImpl::setLam(const std::string &lam)
{
  return assign_if_valid<double>(lam, m_lam);
}

double PlrBackDamperImpl::Cp() const
{
  return openstudio::string_conversions::to<double>(m_Cp);
}

bool PlrBackDamperImpl::setCp(const double Cp)
{
  m_Cp = openstudio::string_conversions::number(Cp);
  return true;
}

bool PlrBackDamperImpl::setCp(const std::string &Cp)
{
  return assign_if_valid<double>(Cp, m_Cp);
}

double PlrBackDamperImpl::xp() const
{
  return openstudio::string_conversions::to<double>(m_xp);
}

bool PlrBackDamperImpl::setXp(const double xp)
{
  m_xp = openstudio::string_conversions::number(xp);
  return true;
}

bool PlrBackDamperImpl::setXp(const std::string &xp)
{
  return assign_if_valid<double>(xp, m_xp);
}

double PlrBackDamperImpl::Cn() const
{
  return openstudio::string_conversions::to<double>(m_Cn);
}

bool PlrBackDamperImpl::setCn(const double Cn)
{
  m_Cn = openstudio::string_conversions::number(Cn);
  return true;
}

bool PlrBackDamperImpl::setCn(const std::string &Cn)
{
  return assign_if_valid<double>(Cn, m_Cn);
}

double PlrBackDamperImpl::xn() const
{
  return openstudio::string_conversions::to<double>(m_xn);
}

bool PlrBackDamperImpl::setXn(const double xn)
{
  m_xn = openstudio::string_conversions::number(xn);
  return true;
}

bool PlrBackDamperImpl::setXn(const std::string &xn)
{
  return assign_if_valid<double>(xn, m_xn);
}

void QfrQuadraticImpl::setDefaults()
{
  m_nr = 0;
  m_icon = 0;
  m_a = PRJFLOAT("0.0");
  m_b = PRJFLOAT("0.0");
}

QfrQuadraticImpl::QfrQuadraticImpl()
{
  setDefaults();
}

QfrQuadraticImpl::QfrQuadraticImpl(int nr,int icon,std::string name,std::string desc)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
}

QfrQuadraticImpl::QfrQuadraticImpl(int nr,int icon,std::string name,std::string desc,double a,double b)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setA(a);
  setB(b);
}

QfrQuadraticImpl::QfrQuadraticImpl(int nr,int icon,std::string name,std::string desc,std::string a,std::string b)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setA(a);
  setB(b);
}

void QfrQuadraticImpl::read(Reader &input)
{
  setNr(input.read<int>());
  setIcon(input.read<int>());
  std::string dataType = input.readString(); // Should really check this
  setName(input.readString());
  setDesc(input.readLine());
  setA(input.readNumber<std::string>());
  setB(input.readNumber<std::string>());
}

std::string QfrQuadraticImpl::write(std::string dataType)
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_icon) + ' ' + dataType + ' ' + m_name + '\n';
  string += m_desc + '\n';
  string += ANY_TO_STR(m_a) + ' ' + ANY_TO_STR(m_b) + '\n';
  return string;
}

void QfrQuadraticImpl::readDetails(Reader &input)
{
  setA(input.readNumber<std::string>());
  setB(input.readNumber<std::string>());
}

int QfrQuadraticImpl::nr() const
{
  return m_nr;
}

void QfrQuadraticImpl::setNr(const int nr)
{
  m_nr = nr;
}

int QfrQuadraticImpl::icon() const
{
  return m_icon;
}

void QfrQuadraticImpl::setIcon(const int icon)
{
  m_icon = icon;
}

std::string QfrQuadraticImpl::name() const
{
  return m_name;
}

void QfrQuadraticImpl::setName(const std::string &name)
{
  m_name = name;
}

std::string QfrQuadraticImpl::desc() const
{
  return m_desc;
}

void QfrQuadraticImpl::setDesc(const std::string &desc)
{
  m_desc = desc;
}

double QfrQuadraticImpl::a() const
{
  return openstudio::string_conversions::to<double>(m_a);
}

bool QfrQuadraticImpl::setA(const double a)
{
  m_a = openstudio::string_conversions::number(a);
  return true;
}

bool QfrQuadraticImpl::setA(const std::string &a)
{
  return assign_if_valid<double>(a, m_a);
}

double QfrQuadraticImpl::b() const
{
  return openstudio::string_conversions::to<double>(m_b);
}

bool QfrQuadraticImpl::setB(const double b)
{
  m_b = openstudio::string_conversions::number(b);
  return true;
}

bool QfrQuadraticImpl::setB(const std::string &b)
{
  return assign_if_valid<double>(b, m_b);
}

void QfrCrackImpl::setDefaults()
{
  m_nr = 0;
  m_icon = 0;
  m_a = PRJFLOAT("0.0");
  m_b = PRJFLOAT("0.0");
  m_length = PRJFLOAT("0.0");
  m_width = PRJFLOAT("0.0");
  m_depth = PRJFLOAT("0.0");
  m_nB = 0;
  m_u_L = 0;
  m_u_W = 0;
  m_u_D = 0;
}

QfrCrackImpl::QfrCrackImpl()
{
  setDefaults();
}

QfrCrackImpl::QfrCrackImpl(int nr,int icon,std::string name,std::string desc)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
}

QfrCrackImpl::QfrCrackImpl(int nr,int icon,std::string name,std::string desc,double a,double b,double length,
                           double width,double depth,int nB,int u_L,int u_W,int u_D)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setA(a);
  setB(b);
  setLength(length);
  setWidth(width);
  setDepth(depth);
  setNB(nB);
  setU_L(u_L);
  setU_W(u_W);
  setU_D(u_D);
}

QfrCrackImpl::QfrCrackImpl(int nr,int icon,std::string name,std::string desc,std::string a,std::string b,std::string length,
                           std::string width,std::string depth,int nB,int u_L,int u_W,int u_D)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setA(a);
  setB(b);
  setLength(length);
  setWidth(width);
  setDepth(depth);
  setNB(nB);
  setU_L(u_L);
  setU_W(u_W);
  setU_D(u_D);
}

void QfrCrackImpl::read(Reader &input)
{
  setNr(input.read<int>());
  setIcon(input.read<int>());
  std::string dataType = input.readString(); // Should really check this
  setName(input.readString());
  setDesc(input.readLine());
  setA(input.readNumber<std::string>());
  setB(input.readNumber<std::string>());
  setLength(input.readNumber<std::string>());
  setWidth(input.readNumber<std::string>());
  setDepth(input.readNumber<std::string>());
  setNB(input.read<int>());
  setU_L(input.read<int>());
  setU_W(input.read<int>());
  setU_D(input.read<int>());
}

std::string QfrCrackImpl::write()
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_icon) + " qfr_crack " + m_name + '\n';
  string += m_desc + '\n';
  string += ANY_TO_STR(m_a) + ' ' + ANY_TO_STR(m_b) + ' ' + ANY_TO_STR(m_length) + ' ' + ANY_TO_STR(m_width)
    + ' ' + ANY_TO_STR(m_depth) + ' ' + ANY_TO_STR(m_nB) + ' ' + ANY_TO_STR(m_u_L) + ' '
    + ANY_TO_STR(m_u_W) + ' ' + ANY_TO_STR(m_u_D) + '\n';
  return string;
}

void QfrCrackImpl::readDetails(Reader &input)
{
  setA(input.readNumber<std::string>());
  setB(input.readNumber<std::string>());
  setLength(input.readNumber<std::string>());
  setWidth(input.readNumber<std::string>());
  setDepth(input.readNumber<std::string>());
  setNB(input.read<int>());
  setU_L(input.read<int>());
  setU_W(input.read<int>());
  setU_D(input.read<int>());
}

int QfrCrackImpl::nr() const
{
  return m_nr;
}

void QfrCrackImpl::setNr(const int nr)
{
  m_nr = nr;
}

int QfrCrackImpl::icon() const
{
  return m_icon;
}

void QfrCrackImpl::setIcon(const int icon)
{
  m_icon = icon;
}

std::string QfrCrackImpl::name() const
{
  return m_name;
}

void QfrCrackImpl::setName(const std::string &name)
{
  m_name = name;
}

std::string QfrCrackImpl::desc() const
{
  return m_desc;
}

void QfrCrackImpl::setDesc(const std::string &desc)
{
  m_desc = desc;
}

double QfrCrackImpl::a() const
{
  return openstudio::string_conversions::to<double>(m_a);
}

bool QfrCrackImpl::setA(const double a)
{
  m_a = openstudio::string_conversions::number(a);
  return true;
}

bool QfrCrackImpl::setA(const std::string &a)
{
  return assign_if_valid<double>(a, m_a);
}

double QfrCrackImpl::b() const
{
  return openstudio::string_conversions::to<double>(m_b);
}

bool QfrCrackImpl::setB(const double b)
{
  m_b = openstudio::string_conversions::number(b);
  return true;
}

bool QfrCrackImpl::setB(const std::string &b)
{
  return assign_if_valid<double>(b, m_b);
}

double QfrCrackImpl::length() const
{
  return openstudio::string_conversions::to<double>(m_length);
}

bool QfrCrackImpl::setLength(const double length)
{
  m_length = openstudio::string_conversions::number(length);
  return true;
}

bool QfrCrackImpl::setLength(const std::string &length)
{
  return assign_if_valid<double>(length, m_length);
}

double QfrCrackImpl::width() const
{
  return openstudio::string_conversions::to<double>(m_width);
}

bool QfrCrackImpl::setWidth(const double width)
{
  m_width = openstudio::string_conversions::number(width);
  return true;
}

bool QfrCrackImpl::setWidth(const std::string &width)
{
  return assign_if_valid<double>(width, m_width);
}

double QfrCrackImpl::depth() const
{
  return openstudio::string_conversions::to<double>(m_depth);
}

bool QfrCrackImpl::setDepth(const double depth)
{
  m_depth = openstudio::string_conversions::number(depth);
  return true;
}

bool QfrCrackImpl::setDepth(const std::string &depth)
{
  return assign_if_valid<double>(depth, m_depth);
}

int QfrCrackImpl::nB() const
{
  return m_nB;
}

void QfrCrackImpl::setNB(const int nB)
{
  m_nB = nB;
}

int QfrCrackImpl::u_L() const
{
  return m_u_L;
}

void QfrCrackImpl::setU_L(const int u_L)
{
  m_u_L = u_L;
}

int QfrCrackImpl::u_W() const
{
  return m_u_W;
}

void QfrCrackImpl::setU_W(const int u_W)
{
  m_u_W = u_W;
}

int QfrCrackImpl::u_D() const
{
  return m_u_D;
}

void QfrCrackImpl::setU_D(const int u_D)
{
  m_u_D = u_D;
}

void QfrTest2Impl::setDefaults()
{
  m_nr = 0;
  m_icon = 0;
  m_a = PRJFLOAT("0.0");
  m_b = PRJFLOAT("0.0");
  m_dP1 = PRJFLOAT("0.0");
  m_F1 = PRJFLOAT("0.0");
  m_dP2 = PRJFLOAT("0.0");
  m_F2 = PRJFLOAT("0.0");
  m_u_P1 = 0;
  m_u_F1 = 0;
  m_u_P2 = 0;
  m_u_F2 = 0;
}

QfrTest2Impl::QfrTest2Impl()
{
  setDefaults();
}

QfrTest2Impl::QfrTest2Impl(int nr,int icon,std::string name,std::string desc)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
}

QfrTest2Impl::QfrTest2Impl(int nr,int icon,std::string name,std::string desc,double a,double b,double dP1,double F1,
                           double dP2,double F2,int u_P1,int u_F1,int u_P2,int u_F2)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setA(a);
  setB(b);
  setDP1(dP1);
  setF1(F1);
  setDP2(dP2);
  setF2(F2);
  setU_P1(u_P1);
  setU_F1(u_F1);
  setU_P2(u_P2);
  setU_F2(u_F2);
}

QfrTest2Impl::QfrTest2Impl(int nr,int icon,std::string name,std::string desc,std::string a,std::string b,std::string dP1,std::string F1,
                           std::string dP2,std::string F2,int u_P1,int u_F1,int u_P2,int u_F2)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setA(a);
  setB(b);
  setDP1(dP1);
  setF1(F1);
  setDP2(dP2);
  setF2(F2);
  setU_P1(u_P1);
  setU_F1(u_F1);
  setU_P2(u_P2);
  setU_F2(u_F2);
}

void QfrTest2Impl::read(Reader &input)
{
  setNr(input.read<int>());
  setIcon(input.read<int>());
  std::string dataType = input.readString(); // Should really check this
  setName(input.readString());
  setDesc(input.readLine());
  setA(input.readNumber<std::string>());
  setB(input.readNumber<std::string>());
  setDP1(input.readNumber<std::string>());
  setF1(input.readNumber<std::string>());
  setDP2(input.readNumber<std::string>());
  setF2(input.readNumber<std::string>());
  setU_P1(input.read<int>());
  setU_F1(input.read<int>());
  setU_P2(input.read<int>());
  setU_F2(input.read<int>());
}

std::string QfrTest2Impl::write()
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_icon) + " qfr_test2 " + m_name + '\n';
  string += m_desc + '\n';
  string += ANY_TO_STR(m_a) + ' ' + ANY_TO_STR(m_b) + ' ' + ANY_TO_STR(m_dP1) + ' ' + ANY_TO_STR(m_F1)
    + ' ' + ANY_TO_STR(m_dP2) + ' ' + ANY_TO_STR(m_F2) + ' ' + ANY_TO_STR(m_u_P1) + ' '
    + ANY_TO_STR(m_u_F1) + ' ' + ANY_TO_STR(m_u_P2) + ' ' + ANY_TO_STR(m_u_F2) + '\n';
  return string;
}

void QfrTest2Impl::readDetails(Reader &input)
{
  setA(input.readNumber<std::string>());
  setB(input.readNumber<std::string>());
  setDP1(input.readNumber<std::string>());
  setF1(input.readNumber<std::string>());
  setDP2(input.readNumber<std::string>());
  setF2(input.readNumber<std::string>());
  setU_P1(input.read<int>());
  setU_F1(input.read<int>());
  setU_P2(input.read<int>());
  setU_F2(input.read<int>());
}

int QfrTest2Impl::nr() const
{
  return m_nr;
}

void QfrTest2Impl::setNr(const int nr)
{
  m_nr = nr;
}

int QfrTest2Impl::icon() const
{
  return m_icon;
}

void QfrTest2Impl::setIcon(const int icon)
{
  m_icon = icon;
}

std::string QfrTest2Impl::name() const
{
  return m_name;
}

void QfrTest2Impl::setName(const std::string &name)
{
  m_name = name;
}

std::string QfrTest2Impl::desc() const
{
  return m_desc;
}

void QfrTest2Impl::setDesc(const std::string &desc)
{
  m_desc = desc;
}

double QfrTest2Impl::a() const
{
  return openstudio::string_conversions::to<double>(m_a);
}

bool QfrTest2Impl::setA(const double a)
{
  m_a = openstudio::string_conversions::number(a);
  return true;
}

bool QfrTest2Impl::setA(const std::string &a)
{
  return assign_if_valid<double>(a, m_a);
}

double QfrTest2Impl::b() const
{
  return openstudio::string_conversions::to<double>(m_b);
}

bool QfrTest2Impl::setB(const double b)
{
  m_b = openstudio::string_conversions::number(b);
  return true;
}

bool QfrTest2Impl::setB(const std::string &b)
{
  return assign_if_valid<double>(b, m_b);
}

double QfrTest2Impl::dP1() const
{
  return openstudio::string_conversions::to<double>(m_dP1);
}

bool QfrTest2Impl::setDP1(const double dP1)
{
  m_dP1 = openstudio::string_conversions::number(dP1);
  return true;
}

bool QfrTest2Impl::setDP1(const std::string &dP1)
{
  return assign_if_valid<double>(dP1, m_dP1);
}

double QfrTest2Impl::F1() const
{
  return openstudio::string_conversions::to<double>(m_F1);
}

bool QfrTest2Impl::setF1(const double F1)
{
  m_F1 = openstudio::string_conversions::number(F1);
  return true;
}

bool QfrTest2Impl::setF1(const std::string &F1)
{
  return assign_if_valid<double>(F1, m_F1);
}

double QfrTest2Impl::dP2() const
{
  return openstudio::string_conversions::to<double>(m_dP2);
}

bool QfrTest2Impl::setDP2(const double dP2)
{
  m_dP2 = openstudio::string_conversions::number(dP2);
  return true;
}

bool QfrTest2Impl::setDP2(const std::string &dP2)
{
  return assign_if_valid<double>(dP2, m_dP2);
}

double QfrTest2Impl::F2() const
{
  return openstudio::string_conversions::to<double>(m_F2);
}

bool QfrTest2Impl::setF2(const double F2)
{
  m_F2 = openstudio::string_conversions::number(F2);
  return true;
}

bool QfrTest2Impl::setF2(const std::string &F2)
{
  return assign_if_valid<double>(F2, m_F2);
}

int QfrTest2Impl::u_P1() const
{
  return m_u_P1;
}

void QfrTest2Impl::setU_P1(const int u_P1)
{
  m_u_P1 = u_P1;
}

int QfrTest2Impl::u_F1() const
{
  return m_u_F1;
}

void QfrTest2Impl::setU_F1(const int u_F1)
{
  m_u_F1 = u_F1;
}

int QfrTest2Impl::u_P2() const
{
  return m_u_P2;
}

void QfrTest2Impl::setU_P2(const int u_P2)
{
  m_u_P2 = u_P2;
}

int QfrTest2Impl::u_F2() const
{
  return m_u_F2;
}

void QfrTest2Impl::setU_F2(const int u_F2)
{
  m_u_F2 = u_F2;
}

void AfeDorImpl::setDefaults()
{
  m_nr = 0;
  m_icon = 0;
  m_lam = PRJFLOAT("0.0");
  m_turb = PRJFLOAT("0.0");
  m_expt = PRJFLOAT("0.0");
  m_dTmin = PRJFLOAT("0.0");
  m_ht = PRJFLOAT("0.0");
  m_wd = PRJFLOAT("0.0");
  m_cd = PRJFLOAT("0.0");
  m_u_T = 0;
  m_u_H = 0;
  m_u_W = 0;
}

AfeDorImpl::AfeDorImpl()
{
  setDefaults();
}

AfeDorImpl::AfeDorImpl(int nr,int icon,std::string name,std::string desc)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
}

AfeDorImpl::AfeDorImpl(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,
                       double dTmin,double ht,double wd,double cd,int u_T,int u_H,int u_W)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setLam(lam);
  setTurb(turb);
  setExpt(expt);
  setDTmin(dTmin);
  setHeight(ht);
  setWidth(wd);
  setCd(cd);
  setU_T(u_T);
  setU_H(u_H);
  setU_W(u_W);
}

AfeDorImpl::AfeDorImpl(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,
                       std::string dTmin,std::string ht,std::string wd,std::string cd,int u_T,int u_H,int u_W)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setLam(lam);
  setTurb(turb);
  setExpt(expt);
  setDTmin(dTmin);
  setHeight(ht);
  setWidth(wd);
  setCd(cd);
  setU_T(u_T);
  setU_H(u_H);
  setU_W(u_W);
}

void AfeDorImpl::read(Reader &input)
{
  setNr(input.read<int>());
  setIcon(input.read<int>());
  std::string dataType = input.readString(); // Should really check this
  setName(input.readString());
  setDesc(input.readLine());
  setLam(input.readNumber<std::string>());
  setTurb(input.readNumber<std::string>());
  setExpt(input.readNumber<std::string>());
  setDTmin(input.readNumber<std::string>());
  setHeight(input.readNumber<std::string>());
  setWidth(input.readNumber<std::string>());
  setCd(input.readNumber<std::string>());
  setU_T(input.read<int>());
  setU_H(input.read<int>());
  setU_W(input.read<int>());
}

std::string AfeDorImpl::write()
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_icon) + " dor_door " + m_name + '\n';
  string += m_desc + '\n';
  string += ANY_TO_STR(m_lam) + ' ' + ANY_TO_STR(m_turb) + ' ' + ANY_TO_STR(m_expt) + ' '
    + ANY_TO_STR(m_dTmin) + ' ' + ANY_TO_STR(m_ht) + ' ' + ANY_TO_STR(m_wd) + ' '
    + ANY_TO_STR(m_cd) + ' ' + ANY_TO_STR(m_u_T) + ' ' + ANY_TO_STR(m_u_H) + ' '
    + ANY_TO_STR(m_u_W) + '\n';
  return string;
}

void AfeDorImpl::readDetails(Reader &input)
{
  setLam(input.readNumber<std::string>());
  setTurb(input.readNumber<std::string>());
  setExpt(input.readNumber<std::string>());
  setDTmin(input.readNumber<std::string>());
  setHeight(input.readNumber<std::string>());
  setWidth(input.readNumber<std::string>());
  setCd(input.readNumber<std::string>());
  setU_T(input.read<int>());
  setU_H(input.read<int>());
  setU_W(input.read<int>());
}

int AfeDorImpl::nr() const
{
  return m_nr;
}

void AfeDorImpl::setNr(const int nr)
{
  m_nr = nr;
}

int AfeDorImpl::icon() const
{
  return m_icon;
}

void AfeDorImpl::setIcon(const int icon)
{
  m_icon = icon;
}

std::string AfeDorImpl::name() const
{
  return m_name;
}

void AfeDorImpl::setName(const std::string &name)
{
  m_name = name;
}

std::string AfeDorImpl::desc() const
{
  return m_desc;
}

void AfeDorImpl::setDesc(const std::string &desc)
{
  m_desc = desc;
}

double AfeDorImpl::lam() const
{
  return openstudio::string_conversions::to<double>(m_lam);
}

bool AfeDorImpl::setLam(const double lam)
{
  m_lam = openstudio::string_conversions::number(lam);
  return true;
}

bool AfeDorImpl::setLam(const std::string &lam)
{
  return assign_if_valid<double>(lam, m_lam);
}

double AfeDorImpl::turb() const
{
  return openstudio::string_conversions::to<double>(m_turb);
}

bool AfeDorImpl::setTurb(const double turb)
{
  m_turb = openstudio::string_conversions::number(turb);
  return true;
}

bool AfeDorImpl::setTurb(const std::string &turb)
{
  return assign_if_valid<double>(turb, m_turb);
}

double AfeDorImpl::expt() const
{
  return openstudio::string_conversions::to<double>(m_expt);
}

bool AfeDorImpl::setExpt(const double expt)
{
  m_expt = openstudio::string_conversions::number(expt);
  return true;
}

bool AfeDorImpl::setExpt(const std::string &expt)
{
  return assign_if_valid<double>(expt, m_expt);
}

double AfeDorImpl::dTmin() const
{
  return openstudio::string_conversions::to<double>(m_dTmin);
}

bool AfeDorImpl::setDTmin(const double dTmin)
{
  m_dTmin = openstudio::string_conversions::number(dTmin);
  return true;
}

bool AfeDorImpl::setDTmin(const std::string &dTmin)
{
  return assign_if_valid<double>(dTmin, m_dTmin);
}

double AfeDorImpl::height() const
{
  return openstudio::string_conversions::to<double>(m_ht);
}

bool AfeDorImpl::setHeight(const double ht)
{
  m_ht = openstudio::string_conversions::number(ht);
  return true;
}

bool AfeDorImpl::setHeight(const std::string &ht)
{
  return assign_if_valid<double>(ht, m_ht);
}

double AfeDorImpl::width() const
{
  return openstudio::string_conversions::to<double>(m_wd);
}

bool AfeDorImpl::setWidth(const double wd)
{
  m_wd = openstudio::string_conversions::number(wd);
  return true;
}

bool AfeDorImpl::setWidth(const std::string &wd)
{
  return assign_if_valid<double>(wd, m_wd);
}

double AfeDorImpl::cd() const
{
  return openstudio::string_conversions::to<double>(m_cd);
}

bool AfeDorImpl::setCd(const double cd)
{
  m_cd = openstudio::string_conversions::number(cd);
  return true;
}

bool AfeDorImpl::setCd(const std::string &cd)
{
  return assign_if_valid<double>(cd, m_cd);
}

int AfeDorImpl::u_T() const
{
  return m_u_T;
}

void AfeDorImpl::setU_T(const int u_T)
{
  m_u_T = u_T;
}

int AfeDorImpl::u_H() const
{
  return m_u_H;
}

void AfeDorImpl::setU_H(const int u_H)
{
  m_u_H = u_H;
}

int AfeDorImpl::u_W() const
{
  return m_u_W;
}

void AfeDorImpl::setU_W(const int u_W)
{
  m_u_W = u_W;
}

void DrPl2Impl::setDefaults()
{
  m_nr = 0;
  m_icon = 0;
  m_lam = PRJFLOAT("0.0");
  m_turb = PRJFLOAT("0.0");
  m_expt = PRJFLOAT("0.0");
  m_dH = PRJFLOAT("0.0");
  m_ht = PRJFLOAT("0.0");
  m_wd = PRJFLOAT("0.0");
  m_cd = PRJFLOAT("0.0");
  m_u_H = 0;
  m_u_W = 0;
}

DrPl2Impl::DrPl2Impl()
{
  setDefaults();
}

DrPl2Impl::DrPl2Impl(int nr,int icon,std::string name,std::string desc)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
}

DrPl2Impl::DrPl2Impl(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double dH,
                     double ht,double wd,double cd,int u_H,int u_W)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setLam(lam);
  setTurb(turb);
  setExpt(expt);
  setDH(dH);
  setHeight(ht);
  setWidth(wd);
  setCd(cd);
  setU_H(u_H);
  setU_W(u_W);
}

DrPl2Impl::DrPl2Impl(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string dH,
                     std::string ht,std::string wd,std::string cd,int u_H,int u_W)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setLam(lam);
  setTurb(turb);
  setExpt(expt);
  setDH(dH);
  setHeight(ht);
  setWidth(wd);
  setCd(cd);
  setU_H(u_H);
  setU_W(u_W);
}

void DrPl2Impl::read(Reader &input)
{
  setNr(input.read<int>());
  setIcon(input.read<int>());
  std::string dataType = input.readString(); // Should really check this
  setName(input.readString());
  setDesc(input.readLine());
  setLam(input.readNumber<std::string>());
  setTurb(input.readNumber<std::string>());
  setExpt(input.readNumber<std::string>());
  setDH(input.readNumber<std::string>());
  setHeight(input.readNumber<std::string>());
  setWidth(input.readNumber<std::string>());
  setCd(input.readNumber<std::string>());
  setU_H(input.read<int>());
  setU_W(input.read<int>());
}

std::string DrPl2Impl::write()
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_icon) + " dor_pl2 " + m_name + '\n';
  string += m_desc + '\n';
  string += ANY_TO_STR(m_lam) + ' ' + ANY_TO_STR(m_turb) + ' ' + ANY_TO_STR(m_expt) + ' '
    + ANY_TO_STR(m_dH) + ' ' + ANY_TO_STR(m_ht) + ' ' + ANY_TO_STR(m_wd) + ' ' + ANY_TO_STR(m_cd)
    + ' ' + ANY_TO_STR(m_u_H) + ' ' + ANY_TO_STR(m_u_W) + '\n';
  return string;
}

void DrPl2Impl::readDetails(Reader &input)
{
  setLam(input.readNumber<std::string>());
  setTurb(input.readNumber<std::string>());
  setExpt(input.readNumber<std::string>());
  setDH(input.readNumber<std::string>());
  setHeight(input.readNumber<std::string>());
  setWidth(input.readNumber<std::string>());
  setCd(input.readNumber<std::string>());
  setU_H(input.read<int>());
  setU_W(input.read<int>());
}

int DrPl2Impl::nr() const
{
  return m_nr;
}

void DrPl2Impl::setNr(const int nr)
{
  m_nr = nr;
}

int DrPl2Impl::icon() const
{
  return m_icon;
}

void DrPl2Impl::setIcon(const int icon)
{
  m_icon = icon;
}

std::string DrPl2Impl::name() const
{
  return m_name;
}

void DrPl2Impl::setName(const std::string &name)
{
  m_name = name;
}

std::string DrPl2Impl::desc() const
{
  return m_desc;
}

void DrPl2Impl::setDesc(const std::string &desc)
{
  m_desc = desc;
}

double DrPl2Impl::lam() const
{
  return openstudio::string_conversions::to<double>(m_lam);
}

bool DrPl2Impl::setLam(const double lam)
{
  m_lam = openstudio::string_conversions::number(lam);
  return true;
}

bool DrPl2Impl::setLam(const std::string &lam)
{
  return assign_if_valid<double>(lam, m_lam);
}

double DrPl2Impl::turb() const
{
  return openstudio::string_conversions::to<double>(m_turb);
}

bool DrPl2Impl::setTurb(const double turb)
{
  m_turb = openstudio::string_conversions::number(turb);
  return true;
}

bool DrPl2Impl::setTurb(const std::string &turb)
{
  return assign_if_valid<double>(turb, m_turb);
}

double DrPl2Impl::expt() const
{
  return openstudio::string_conversions::to<double>(m_expt);
}

bool DrPl2Impl::setExpt(const double expt)
{
  m_expt = openstudio::string_conversions::number(expt);
  return true;
}

bool DrPl2Impl::setExpt(const std::string &expt)
{
  return assign_if_valid<double>(expt, m_expt);
}

double DrPl2Impl::dH() const
{
  return openstudio::string_conversions::to<double>(m_dH);
}

bool DrPl2Impl::setDH(const double dH)
{
  m_dH = openstudio::string_conversions::number(dH);
  return true;
}

bool DrPl2Impl::setDH(const std::string &dH)
{
  return assign_if_valid<double>(dH, m_dH);
}

double DrPl2Impl::height() const
{
  return openstudio::string_conversions::to<double>(m_ht);
}

bool DrPl2Impl::setHeight(const double ht)
{
  m_ht = openstudio::string_conversions::number(ht);
  return true;
}

bool DrPl2Impl::setHeight(const std::string &ht)
{
  return assign_if_valid<double>(ht, m_ht);
}

double DrPl2Impl::width() const
{
  return openstudio::string_conversions::to<double>(m_wd);
}

bool DrPl2Impl::setWidth(const double wd)
{
  m_wd = openstudio::string_conversions::number(wd);
  return true;
}

bool DrPl2Impl::setWidth(const std::string &wd)
{
  return assign_if_valid<double>(wd, m_wd);
}

double DrPl2Impl::cd() const
{
  return openstudio::string_conversions::to<double>(m_cd);
}

bool DrPl2Impl::setCd(const double cd)
{
  m_cd = openstudio::string_conversions::number(cd);
  return true;
}

bool DrPl2Impl::setCd(const std::string &cd)
{
  return assign_if_valid<double>(cd, m_cd);
}

int DrPl2Impl::u_H() const
{
  return m_u_H;
}

void DrPl2Impl::setU_H(const int u_H)
{
  m_u_H = u_H;
}

int DrPl2Impl::u_W() const
{
  return m_u_W;
}

void DrPl2Impl::setU_W(const int u_W)
{
  m_u_W = u_W;
}

void AfeFlowImpl::setDefaults()
{
  m_nr = 0;
  m_icon = 0;
  m_Flow = PRJFLOAT("0.0");
  m_u_F = 0;
}

AfeFlowImpl::AfeFlowImpl()
{
  setDefaults();
}

AfeFlowImpl::AfeFlowImpl(int nr,int icon,std::string name,std::string desc)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
}

AfeFlowImpl::AfeFlowImpl(int nr,int icon,std::string name,std::string desc,double Flow,int u_F)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setFlow(Flow);
  setU_F(u_F);
}

AfeFlowImpl::AfeFlowImpl(int nr,int icon,std::string name,std::string desc,std::string Flow,int u_F)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setFlow(Flow);
  setU_F(u_F);
}

void AfeFlowImpl::read(Reader &input)
{
  setNr(input.read<int>());
  setIcon(input.read<int>());
  std::string dataType = input.readString(); // Should really check this
  setName(input.readString());
  setDesc(input.readLine());
  setFlow(input.readNumber<std::string>());
  setU_F(input.read<int>());
}

std::string AfeFlowImpl::write(std::string dataType)
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_icon) + ' ' + dataType + ' ' + m_name + '\n';
  string += m_desc + '\n';
  string += ANY_TO_STR(m_Flow) + ' ' + ANY_TO_STR(m_u_F) + '\n';
  return string;
}

void AfeFlowImpl::readDetails(Reader &input)
{
  setFlow(input.readNumber<std::string>());
  setU_F(input.read<int>());
}

int AfeFlowImpl::nr() const
{
  return m_nr;
}

void AfeFlowImpl::setNr(const int nr)
{
  m_nr = nr;
}

int AfeFlowImpl::icon() const
{
  return m_icon;
}

void AfeFlowImpl::setIcon(const int icon)
{
  m_icon = icon;
}

std::string AfeFlowImpl::name() const
{
  return m_name;
}

void AfeFlowImpl::setName(const std::string &name)
{
  m_name = name;
}

std::string AfeFlowImpl::desc() const
{
  return m_desc;
}

void AfeFlowImpl::setDesc(const std::string &desc)
{
  m_desc = desc;
}

double AfeFlowImpl::Flow() const
{
  return openstudio::string_conversions::to<double>(m_Flow);
}

bool AfeFlowImpl::setFlow(const double Flow)
{
  m_Flow = openstudio::string_conversions::number(Flow);
  return true;
}

bool AfeFlowImpl::setFlow(const std::string &Flow)
{
  return assign_if_valid<double>(Flow, m_Flow);
}

int AfeFlowImpl::u_F() const
{
  return m_u_F;
}

void AfeFlowImpl::setU_F(const int u_F)
{
  m_u_F = u_F;
}

void AfeFanImpl::setDefaults()
{
  m_nr = 0;
  m_icon = 0;
  m_lam = PRJFLOAT("0.0");
  m_turb = PRJFLOAT("0.0");
  m_expt = PRJFLOAT("0.0");
  m_rdens = PRJFLOAT("0.0");
  m_fdf = PRJFLOAT("0.0");
  m_sop = PRJFLOAT("0.0");
  m_off = PRJFLOAT("0.0");
  m_Sarea = PRJFLOAT("0.0");
  m_u_Sa = 0;
}

AfeFanImpl::AfeFanImpl()
{
  setDefaults();
}

AfeFanImpl::AfeFanImpl(int nr,int icon,std::string name,std::string desc)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
}

AfeFanImpl::AfeFanImpl(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,
                       double rdens,double fdf,double sop,double off,std::vector<double> fpc,double Sarea,
                       int u_Sa,std::vector<FanDataPoint> data)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setLam(lam);
  setTurb(turb);
  setExpt(expt);
  setRdens(rdens);
  setFdf(fdf);
  setSop(sop);
  setOff(off);
  setFpc(fpc);
  setSarea(Sarea);
  setU_Sa(u_Sa);
  setData(data);
}

AfeFanImpl::AfeFanImpl(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,
                       std::string rdens,std::string fdf,std::string sop,std::string off,std::vector<std::string> fpc,std::string Sarea,
                       int u_Sa,std::vector<FanDataPoint> data)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setLam(lam);
  setTurb(turb);
  setExpt(expt);
  setRdens(rdens);
  setFdf(fdf);
  setSop(sop);
  setOff(off);
  setFpc(fpc);
  setSarea(Sarea);
  setU_Sa(u_Sa);
  setData(data);
}

void AfeFanImpl::read(Reader &input)
{
  setNr(input.read<int>());
  setIcon(input.read<int>());
  std::string dataType = input.readString(); // Should really check this
  setName(input.readString());
  setDesc(input.readLine());
  setLam(input.readNumber<std::string>());
  setTurb(input.readNumber<std::string>());
  setExpt(input.readNumber<std::string>());
  setRdens(input.readNumber<std::string>());
  setFdf(input.readNumber<std::string>());
  setSop(input.readNumber<std::string>());
  setOff(input.readNumber<std::string>());
  std::vector<std::string> fpc;
  for(int i=0;i<4;i++)
  {
    fpc.push_back(input.read<std::string>());
  }
  setFpc(fpc);
  int npts = input.read<int>();
  setSarea(input.readNumber<std::string>());
  setU_Sa(input.read<int>());
  std::vector<FanDataPoint> data;
  for(int i=0;i<npts;i++)
  {
    FanDataPoint object;
    object.read(input);
    data.push_back(object);
  }
  setData(data);
}

std::string AfeFanImpl::write()
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_icon) + " fan_fan " + m_name + '\n';
  string += m_desc + '\n';
  string += ANY_TO_STR(m_lam) + ' ' + ANY_TO_STR(m_turb) + ' ' + ANY_TO_STR(m_expt) + ' ' + ANY_TO_STR(m_rdens)
    + ' ' + ANY_TO_STR(m_fdf) + ' ' + ANY_TO_STR(m_sop) + ' ' + ANY_TO_STR(m_off) + '\n';
  for(int i=0;i<4;i++)
    string += ANY_TO_STR(m_fpc[i]) + ' ';
  string += '\n';
  string += ANY_TO_STR((int)m_data.size()) + ' ' + ANY_TO_STR(m_Sarea) + ' ' + ANY_TO_STR(m_u_Sa) + '\n';
  for(unsigned int i=0;i<m_data.size();i++)
  {
    string += m_data[i].write();
  }
  return string;
}

void AfeFanImpl::readDetails(Reader &input)
{
  setLam(input.readNumber<std::string>());
  setTurb(input.readNumber<std::string>());
  setExpt(input.readNumber<std::string>());
  setRdens(input.readNumber<std::string>());
  setFdf(input.readNumber<std::string>());
  setSop(input.readNumber<std::string>());
  setOff(input.readNumber<std::string>());
  std::vector<std::string> fpc;
  for(int i=0;i<4;i++)
  {
    fpc.push_back(input.read<std::string>());
  }
  setFpc(fpc);
  int npts = input.read<int>();
  setSarea(input.readNumber<std::string>());
  setU_Sa(input.read<int>());
  std::vector<FanDataPoint> data;
  for(int i=0;i<npts;i++)
  {
    FanDataPoint object;
    object.read(input);
    data.push_back(object);
  }
  setData(data);
}

int AfeFanImpl::nr() const
{
  return m_nr;
}

void AfeFanImpl::setNr(const int nr)
{
  m_nr = nr;
}

int AfeFanImpl::icon() const
{
  return m_icon;
}

void AfeFanImpl::setIcon(const int icon)
{
  m_icon = icon;
}

std::string AfeFanImpl::name() const
{
  return m_name;
}

void AfeFanImpl::setName(const std::string &name)
{
  m_name = name;
}

std::string AfeFanImpl::desc() const
{
  return m_desc;
}

void AfeFanImpl::setDesc(const std::string &desc)
{
  m_desc = desc;
}

double AfeFanImpl::lam() const
{
  return openstudio::string_conversions::to<double>(m_lam);
}

bool AfeFanImpl::setLam(const double lam)
{
  m_lam = openstudio::string_conversions::number(lam);
  return true;
}

bool AfeFanImpl::setLam(const std::string &lam)
{
  return assign_if_valid<double>(lam, m_lam);
}

double AfeFanImpl::turb() const
{
  return openstudio::string_conversions::to<double>(m_turb);
}

bool AfeFanImpl::setTurb(const double turb)
{
  m_turb = openstudio::string_conversions::number(turb);
  return true;
}

bool AfeFanImpl::setTurb(const std::string &turb)
{
  return assign_if_valid<double>(turb, m_turb);
}

double AfeFanImpl::expt() const
{
  return openstudio::string_conversions::to<double>(m_expt);
}

bool AfeFanImpl::setExpt(const double expt)
{
  m_expt = openstudio::string_conversions::number(expt);
  return true;
}

bool AfeFanImpl::setExpt(const std::string &expt)
{
  return assign_if_valid<double>(expt, m_expt);
}

double AfeFanImpl::rdens() const
{
  return openstudio::string_conversions::to<double>(m_rdens);
}

bool AfeFanImpl::setRdens(const double rdens)
{
  m_rdens = openstudio::string_conversions::number(rdens);
  return true;
}

bool AfeFanImpl::setRdens(const std::string &rdens)
{
  return assign_if_valid<double>(rdens, m_rdens);
}

double AfeFanImpl::fdf() const
{
  return openstudio::string_conversions::to<double>(m_fdf);
}

bool AfeFanImpl::setFdf(const double fdf)
{
  m_fdf = openstudio::string_conversions::number(fdf);
  return true;
}

bool AfeFanImpl::setFdf(const std::string &fdf)
{
  return assign_if_valid<double>(fdf, m_fdf);
}

double AfeFanImpl::sop() const
{
  return openstudio::string_conversions::to<double>(m_sop);
}

bool AfeFanImpl::setSop(const double sop)
{
  m_sop = openstudio::string_conversions::number(sop);
  return true;
}

bool AfeFanImpl::setSop(const std::string &sop)
{
  return assign_if_valid<double>(sop, m_sop);
}

double AfeFanImpl::off() const
{
  return openstudio::string_conversions::to<double>(m_off);
}

bool AfeFanImpl::setOff(const double off)
{
  m_off = openstudio::string_conversions::number(off);
  return true;
}

bool AfeFanImpl::setOff(const std::string &off)
{
  return assign_if_valid<double>(off, m_off);
}

std::vector<double> AfeFanImpl::fpc() const
{
  std::vector<double> out;
  for(int i=0;i<4;i++)
  {
    out.push_back(openstudio::string_conversions::to<double>(m_fpc[i]));
  }
  return out;
}

bool AfeFanImpl::setFpc(const std::vector<double> &fpc)
{
  for(int i=0;i<4;i++)
  {
    m_fpc.push_back(openstudio::string_conversions::number(fpc[i]));
  }
  return true;
}

bool AfeFanImpl::setFpc(const std::vector<std::string> &fpc)
{
  if (std::all_of(fpc.begin(), fpc.end(), is_valid<double, std::string>))
  {
    m_fpc = fpc;
    return true;
  } else {
    return false;
  }
}

double AfeFanImpl::Sarea() const
{
  return openstudio::string_conversions::to<double>(m_Sarea);
}

bool AfeFanImpl::setSarea(const double Sarea)
{
  m_Sarea = openstudio::string_conversions::number(Sarea);
  return true;
}

bool AfeFanImpl::setSarea(const std::string &Sarea)
{
  return assign_if_valid<double>(Sarea, m_Sarea);
}

int AfeFanImpl::u_Sa() const
{
  return m_u_Sa;
}

void AfeFanImpl::setU_Sa(const int u_Sa)
{
  m_u_Sa = u_Sa;
}

std::vector<FanDataPoint> AfeFanImpl::data() const
{
  return m_data;
}

void AfeFanImpl::setData(const std::vector<FanDataPoint> &data)
{
  m_data = data;
}

void AfeCsfImpl::setDefaults()
{
  m_nr = 0;
  m_icon = 0;
  m_u_x = 0;
  m_u_y = 0;
}

AfeCsfImpl::AfeCsfImpl()
{
  setDefaults();
}

AfeCsfImpl::AfeCsfImpl(int nr,int icon,std::string name,std::string desc)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
}

AfeCsfImpl::AfeCsfImpl(int nr,int icon,std::string name,std::string desc,int u_x,int u_y,
                       std::vector<XyDataPoint> data)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setU_x(u_x);
  setU_y(u_y);
  setData(data);
}

void AfeCsfImpl::read(Reader &input)
{
  setNr(input.read<int>());
  setIcon(input.read<int>());
  std::string dataType = input.readString(); // Should really check this
  setName(input.readString());
  setDesc(input.readLine());
  int npts = input.read<int>();
  setU_x(input.read<int>());
  setU_y(input.read<int>());
  for(int i=0;i<npts;i++)
  {
    XyDataPoint object;
    object.read(input);
    m_data.push_back(object);
  }
}

void AfeCsfImpl::readDetails(Reader &input)
{
  int npts = input.read<int>();
  setU_x(input.read<int>());
  setU_y(input.read<int>());
  for(int i=0;i<npts;i++)
  {
    XyDataPoint object;
    object.read(input);
    m_data.push_back(object);
  }

}

std::string AfeCsfImpl::write(std::string datatype)
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_icon) + ' ' + datatype + ' ' + m_name + '\n';
  string += m_desc + '\n';
  string += ANY_TO_STR((int)m_data.size()) + ' ' + ANY_TO_STR(m_u_x) + ' ' + ANY_TO_STR(m_u_y) + '\n';
  for(unsigned int i=0;i<m_data.size();i++)
  {
    string += m_data[i].write();
  }
  return string;
}

int AfeCsfImpl::nr() const
{
  return m_nr;
}

void AfeCsfImpl::setNr(const int nr)
{
  m_nr = nr;
}

int AfeCsfImpl::icon() const
{
  return m_icon;
}

void AfeCsfImpl::setIcon(const int icon)
{
  m_icon = icon;
}

std::string AfeCsfImpl::name() const
{
  return m_name;
}

void AfeCsfImpl::setName(const std::string &name)
{
  m_name = name;
}

std::string AfeCsfImpl::desc() const
{
  return m_desc;
}

void AfeCsfImpl::setDesc(const std::string &desc)
{
  m_desc = desc;
}

int AfeCsfImpl::u_x() const
{
  return m_u_x;
}

void AfeCsfImpl::setU_x(const int u_x)
{
  m_u_x = u_x;
}

int AfeCsfImpl::u_y() const
{
  return m_u_y;
}

void AfeCsfImpl::setU_y(const int u_y)
{
  m_u_y = u_y;
}

std::vector<XyDataPoint> AfeCsfImpl::data() const
{
  return m_data;
}

void AfeCsfImpl::setData(const std::vector<XyDataPoint> &data)
{
  m_data = data;
}

void AfeSupImpl::setDefaults()
{
  m_nr = 0;
  m_icon = 0;
  m_sched = 0;
  m_u_H = 0;
}

AfeSupImpl::AfeSupImpl()
{
  setDefaults();
}

AfeSupImpl::AfeSupImpl(int nr,int icon,std::string name,std::string desc)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
}

AfeSupImpl::AfeSupImpl(int nr,int icon,std::string name,std::string desc,int sched,int u_H,
                       std::vector<AirflowSubelementData> subelements)
{
  setDefaults();
  setNr(nr);
  setIcon(icon);
  setName(name);
  setDesc(desc);
  setSched(sched);
  setU_H(u_H);
  setSubelements(subelements);
}

void AfeSupImpl::read(Reader &input)
{
  setNr(input.read<int>());
  setIcon(input.read<int>());
  std::string dataType = input.readString(); // Should really check this
  setName(input.readString());
  setDesc(input.readLine());
  int nse = input.read<int>();
  setSched(input.read<int>());
  setU_H(input.read<int>());
  std::vector<AirflowSubelementData> subelements;
  for(int i=0;i<nse;i++)
  {
    AirflowSubelementData object;
    object.read(input);
    subelements.push_back(object);
  }
  setSubelements(subelements);
}

std::string AfeSupImpl::write()
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_icon) + " sup_afe " + m_name + '\n';
  string += m_desc + '\n';
  string += ANY_TO_STR((int)m_subelements.size()) + ' ' + ANY_TO_STR(m_sched) + ' ' + ANY_TO_STR(m_u_H) + '\n';
  for(unsigned int i=0;i<m_subelements.size();i++)
  {
    string += m_subelements[i].write();
  }
  return string;
}

void AfeSupImpl::readDetails(Reader &input)
{
  int nse = input.read<int>();
  setSched(input.read<int>());
  setU_H(input.read<int>());
  std::vector<AirflowSubelementData> subelements;
  for(int i=0;i<nse;i++)
  {
    AirflowSubelementData object;
    object.read(input);
    subelements.push_back(object);
  }
  setSubelements(subelements);
}

int AfeSupImpl::nr() const
{
  return m_nr;
}

void AfeSupImpl::setNr(const int nr)
{
  m_nr = nr;
}

int AfeSupImpl::icon() const
{
  return m_icon;
}

void AfeSupImpl::setIcon(const int icon)
{
  m_icon = icon;
}

std::string AfeSupImpl::name() const
{
  return m_name;
}

void AfeSupImpl::setName(const std::string &name)
{
  m_name = name;
}

std::string AfeSupImpl::desc() const
{
  return m_desc;
}

void AfeSupImpl::setDesc(const std::string &desc)
{
  m_desc = desc;
}

int AfeSupImpl::sched() const
{
  return m_sched;
}

void AfeSupImpl::setSched(const int sched)
{
  m_sched = sched;
}

int AfeSupImpl::u_H() const
{
  return m_u_H;
}

void AfeSupImpl::setU_H(const int u_H)
{
  m_u_H = u_H;
}

std::vector<AirflowSubelementData> AfeSupImpl::subelements() const
{
  return m_subelements;
}

void AfeSupImpl::setSubelements(const std::vector<AirflowSubelementData> &subelements)
{
  m_subelements = subelements;
}

} // detail
} // contam
} // openstudio
