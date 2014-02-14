/**********************************************************************
*  Copyright (c) 2013-2014, Alliance for Sustainable Energy.
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
#include "PrjSubobjectsImpl.hpp"

namespace openstudio {
namespace contam {

void WeatherDataImpl::setDefaults()
{
  m_Tambt=RX7("0.0");
  m_barpres=RX7("0.0");
  m_windspd=RX7("0.0");
  m_winddir=RX7("0.0");
  m_relhum=RX7("0.0");
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
  setTambt(input.readNumber<std::string>(FILELINE));
  setBarpres(input.readNumber<std::string>(FILELINE));
  setWindspd(input.readNumber<std::string>(FILELINE));
  setWinddir(input.readNumber<std::string>(FILELINE));
  setRelhum(input.readNumber<std::string>(FILELINE));
  setDaytyp(input.read<int>(FILELINE));
  setUTa(input.read<int>(FILELINE));
  setUbP(input.read<int>(FILELINE));
  setUws(input.read<int>(FILELINE));
  setUwd(input.read<int>(FILELINE));
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
  STR_TO_RX7(Tambt).toDouble(&ok);
  if(ok)
  {
    m_Tambt = STR_TO_RX7(Tambt);
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
  STR_TO_RX7(barpres).toDouble(&ok);
  if(ok)
  {
    m_barpres = STR_TO_RX7(barpres);
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
  STR_TO_RX7(windspd).toDouble(&ok);
  if(ok)
  {
    m_windspd = STR_TO_RX7(windspd);
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
  STR_TO_RX7(winddir).toDouble(&ok);
  if(ok)
  {
    m_winddir = STR_TO_RX7(winddir);
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
  STR_TO_RX7(relhum).toDouble(&ok);
  if(ok)
  {
    m_relhum = STR_TO_RX7(relhum);
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
  setIcon(input.read<int>(FILELINE));
  setCol(input.read<int>(FILELINE));
  setRow(input.read<int>(FILELINE));
  setNr(input.read<int>(FILELINE));
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

uint IconImpl::bits()
{
  if(isWall())
    switch(m_icon)
  {
    case WL_EW:
      return BIT_EW;
    case WL_NS:
      return BIT_NS;
    case WL_ES:
      return BIT_ES;
    case WL_SW:
      return BIT_SW;
    case WL_NW:
      return BIT_NW;
    case WL_NE:
      return BIT_NE;
    case WL_NES:
      return BIT_NES;
    case WL_ESW:
      return BIT_ESW;
    case WL_NSW:
      return BIT_NSW;
    case WL_NEW:
      return BIT_NEW;
    case WL_NESW:
      return BIT_NESW;
  }
  return 0;
}

} // contam
} // openstudio
