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

#include "PrjSubobjectsImpl.hpp"

namespace openstudio {
namespace contam {
namespace detail {

void WeatherDataImpl::setDefaults()
{
  m_Tambt=PRJFLOAT("0.0");
  m_barpres=PRJFLOAT("0.0");
  m_windspd=PRJFLOAT("0.0");
  m_winddir=PRJFLOAT("0.0");
  m_relhum=PRJFLOAT("0.0");
  m_daytyp=0;
  m_uTa=0;
  m_ubP=0;
  m_uws=0;
  m_uwd=0;
}

WeatherDataImpl::WeatherDataImpl()
{
  setDefaults();
}

WeatherDataImpl::WeatherDataImpl(std::string Tambt,std::string barpres,std::string windspd,std::string winddir,std::string relhum,
                                 int daytyp,int uTa,int ubP,int uws,int uwd)
{
  setDefaults();
  setTambt(Tambt);
  setBarpres(barpres);
  setWindspd(windspd);
  setWinddir(winddir);
  setRelhum(relhum);
  setDaytyp(daytyp);
  setUTa(uTa);
  setUbP(ubP);
  setUws(uws);
  setUwd(uwd);
}

WeatherDataImpl::WeatherDataImpl(double Tambt,double barpres,double windspd,double winddir,double relhum,
                                 int daytyp,int uTa,int ubP,int uws,int uwd)
{
  setDefaults();
  setTambt(Tambt);
  setBarpres(barpres);
  setWindspd(windspd);
  setWinddir(winddir);
  setRelhum(relhum);
  setDaytyp(daytyp);
  setUTa(uTa);
  setUbP(ubP);
  setUws(uws);
  setUwd(uwd);
}

void WeatherDataImpl::read(Reader &input)
{
  setTambt(input.readNumber<std::string>());
  setBarpres(input.readNumber<std::string>());
  setWindspd(input.readNumber<std::string>());
  setWinddir(input.readNumber<std::string>());
  setRelhum(input.readNumber<std::string>());
  setDaytyp(input.read<int>());
  setUTa(input.read<int>());
  setUbP(input.read<int>());
  setUws(input.read<int>());
  setUwd(input.read<int>());
}

std::string WeatherDataImpl::write()
{
  std::string string;
  string += ANY_TO_STR(m_Tambt) + ' ' + ANY_TO_STR(m_barpres) + ' ' + ANY_TO_STR(m_windspd) + ' ' + ANY_TO_STR(m_winddir) + ' ' + ANY_TO_STR(m_relhum) + ' ' + ANY_TO_STR(m_daytyp) + ' ' + ANY_TO_STR(m_uTa) + ' ' + ANY_TO_STR(m_ubP) + ' ' + ANY_TO_STR(m_uws) + ' ' + ANY_TO_STR(m_uwd) + '\n';
  return string;
}

double WeatherDataImpl::Tambt() const
{
  return m_Tambt.toDouble();
}

bool WeatherDataImpl::setTambt(const double Tambt)
{
  m_Tambt = QString::number(Tambt);
  return true;
}

bool WeatherDataImpl::setTambt(const std::string &Tambt)
{
  bool ok;
  FLOAT_CHECK(Tambt,&ok);
  if(ok)
  {
    m_Tambt = STR_TO_FLOAT(Tambt);
    return true;
  }
  return false;
}

double WeatherDataImpl::barpres() const
{
  return m_barpres.toDouble();
}

bool WeatherDataImpl::setBarpres(const double barpres)
{
  m_barpres = QString::number(barpres);
  return true;
}

bool WeatherDataImpl::setBarpres(const std::string &barpres)
{
  bool ok;
  FLOAT_CHECK(barpres,&ok);
  if(ok)
  {
    m_barpres = STR_TO_FLOAT(barpres);
    return true;
  }
  return false;
}

double WeatherDataImpl::windspd() const
{
  return m_windspd.toDouble();
}

bool WeatherDataImpl::setWindspd(const double windspd)
{
  m_windspd = QString::number(windspd);
  return true;
}

bool WeatherDataImpl::setWindspd(const std::string &windspd)
{
  bool ok;
  FLOAT_CHECK(windspd,&ok);
  if(ok)
  {
    m_windspd = STR_TO_FLOAT(windspd);
    return true;
  }
  return false;
}

double WeatherDataImpl::winddir() const
{
  return m_winddir.toDouble();
}

bool WeatherDataImpl::setWinddir(const double winddir)
{
  m_winddir = QString::number(winddir);
  return true;
}

bool WeatherDataImpl::setWinddir(const std::string &winddir)
{
  bool ok;
  FLOAT_CHECK(winddir,&ok);
  if(ok)
  {
    m_winddir = STR_TO_FLOAT(winddir);
    return true;
  }
  return false;
}

double WeatherDataImpl::relhum() const
{
  return m_relhum.toDouble();
}

bool WeatherDataImpl::setRelhum(const double relhum)
{
  m_relhum = QString::number(relhum);
  return true;
}

bool WeatherDataImpl::setRelhum(const std::string &relhum)
{
  bool ok;
  FLOAT_CHECK(relhum,&ok);
  if(ok)
  {
    m_relhum = STR_TO_FLOAT(relhum);
    return true;
  }
  return false;
}

int WeatherDataImpl::daytyp() const
{
  return m_daytyp;
}

void WeatherDataImpl::setDaytyp(const int daytyp)
{
  m_daytyp = daytyp;
}

int WeatherDataImpl::uTa() const
{
  return m_uTa;
}

void WeatherDataImpl::setUTa(const int uTa)
{
  m_uTa = uTa;
}

int WeatherDataImpl::ubP() const
{
  return m_ubP;
}

void WeatherDataImpl::setUbP(const int ubP)
{
  m_ubP = ubP;
}

int WeatherDataImpl::uws() const
{
  return m_uws;
}

void WeatherDataImpl::setUws(const int uws)
{
  m_uws = uws;
}

int WeatherDataImpl::uwd() const
{
  return m_uwd;
}

void WeatherDataImpl::setUwd(const int uwd)
{
  m_uwd = uwd;
}

void IconImpl::setDefaults()
{
  m_icon = 0;
  m_col = 0;
  m_row = 0;
  m_nr = 0;
}

IconImpl::IconImpl()
{
  setDefaults();
}

IconImpl::IconImpl(int icon,int col,int row,int nr)
{
  setDefaults();
  setIcon(icon);
  setCol(col);
  setRow(row);
  setNr(nr);
}

void IconImpl::read(Reader &input)
{
  setIcon(input.read<int>());
  setCol(input.read<int>());
  setRow(input.read<int>());
  setNr(input.read<int>());
}

std::string IconImpl::write()
{
  std::string string;
  string += ANY_TO_STR(m_icon) + ' ' + ANY_TO_STR(m_col) + ' ' + ANY_TO_STR(m_row) + ' ' + ANY_TO_STR(m_nr) + '\n';
  return string;
}

int IconImpl::icon() const
{
  return m_icon;
}

void IconImpl::setIcon(const int icon)
{
  m_icon = icon;
}

int IconImpl::col() const
{
  return m_col;
}

void IconImpl::setCol(const int col)
{
  m_col = col;
}

int IconImpl::row() const
{
  return m_row;
}

void IconImpl::setRow(const int row)
{
  m_row = row;
}

int IconImpl::nr() const
{
  return m_nr;
}

void IconImpl::setNr(const int nr)
{
  m_nr = nr;
}

bool IconImpl::isWall()
{
  return (m_icon >= WL_EW) && (m_icon <= WL_NESW);
}

} // detail
} // contam
} // openstudio
