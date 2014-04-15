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
#ifndef PRJSUBOBJECTSIMPL_H
#define PRJSUBOBJECTSIMPL_H

#include "PrjDefines.hpp"
#include "PrjReader.hpp"

#include <utilities/core/String.hpp>

#include <QVector>

namespace openstudio {
namespace contam {
namespace detail {

class WeatherDataImpl
{
public:
  WeatherDataImpl();
  WeatherDataImpl(std::string Tambt,std::string barpres,std::string windspd,std::string winddir,std::string relhum,int daytyp,
    int uTa,int ubP,int uws,int uwd);
  WeatherDataImpl(double Tambt,double barpres,double windspd,double winddir,double relhum,int daytyp,
    int uTa,int ubP,int uws,int uwd);
  void read(Reader &reader);
  std::string write();
  /** Returns the ambient temperature [K] */
  double Tambt() const;
  /** Sets the ambient temperature [K] */
  bool setTambt(const double Tambt);
  /** Sets the ambient temperature [K] */
  bool setTambt(const std::string &Tambt);
  /** Returns the barometric pressure [Pa] NOT corrected to sea level */
  double barpres() const;
  /** Sets the barometric pressure [Pa] NOT corrected to sea level */
  bool setBarpres(const double barpres);
  /** Sets the barometric pressure [Pa] NOT corrected to sea level */
  bool setBarpres(const std::string &barpres);
  /** Returns the wind speed [m/s] */
  double windspd() const;
  /** Sets the wind speed [m/s] */
  bool setWindspd(const double windspd);
  /** Sets the wind speed [m/s] */
  bool setWindspd(const std::string &windspd);
  /** Returns the wind direction: 0 = N, 90 = E, 180 = S, ... */
  double winddir() const;
  /** Sets the wind direction: 0 = N, 90 = E, 180 = S, ... */
  bool setWinddir(const double winddir);
  /** Sets the wind direction: 0 = N, 90 = E, 180 = S, ... */
  bool setWinddir(const std::string &winddir);
  /** Returns the relative humidity: 0.0 to 1.0 */
  double relhum() const;
  /** Sets the relative humidity: 0.0 to 1.0 */
  bool setRelhum(const double relhum);
  /** Sets the relative humidity: 0.0 to 1.0 */
  bool setRelhum(const std::string &relhum);
  /** Returns the type of day (1-12) */
  int daytyp() const;
  /** Sets the type of day (1-12) */
  void setDaytyp(const int daytyp);
  /** Returns the units for Tambt */
  int uTa() const;
  /** Sets the units for Tambt */
  void setUTa(const int uTa);
  /** Returns the units for barometric pressure */
  int ubP() const;
  /** Sets the units for barometric pressure */
  void setUbP(const int ubP);
  /** Returns the units for wind speed */
  int uws() const;
  /** Sets the units for wind speed */
  void setUws(const int uws);
  /** Returns the units for wind direction */
  int uwd() const;
  /** Sets the units for wind speed */
  void setUwd(const int uwd);

private:
  void setDefaults();

  PRJFLOAT m_Tambt;  // ambient temperature [K] (R4)
  PRJFLOAT m_barpres;  // barometric pressure [Pa] NOT corrected to sea level (R4)
  PRJFLOAT m_windspd;  // wind speed [m/s] (R4)
  PRJFLOAT m_winddir;  // wind direction: 0 = N, 90 = E, 180 = S, ...; (R4)
  PRJFLOAT m_relhum;  // relative humidity: 0.0 to 1.0 (R4)
  int m_daytyp;  // type of day (1-12) (I2)
  int m_uTa;  // units for Tambt (I2) {W}
  int m_ubP;  // units for barpres (I2) {W}
  int m_uws;  // units for windspd (I2) {W}
  int m_uwd;  // units for winddir (I2) {W}
};


class IconImpl
{
public:
  /** @name Constructors */
  //@{

  /** Create a new object with default values */
  IconImpl();
  /** Create a new object */
  IconImpl(int icon,int col,int row,int nr);

  //@}
  /** @name Input and Output */
  //@{

  /** Read an object from a Reader */
  void read(Reader &input);
  /** Write the object to a string */
  std::string write();

  //@}
  /** @name Getters and Setters */
  //@{

  /** Returns the icon type - see 'special symbols' in contam.h */
  int icon() const;
  /** Sets the icon type - see 'special symbols' in contam.h */
  void setIcon(const int icon);
  /** Returns the icon's column position on the SketchPad */
  int col() const;
  /** Sets the icon's column position on the SketchPad */
  void setCol(const int col);
  /** Returns the icon's row position on the SketchPad */
  int row() const;
  /** Sets the icon's row position on the SketchPad */
  void setRow(const int row);
  /** Returns the icon's zone, path, duct, etc., number */
  int nr() const;
  /** Sets the icon's zone, path, duct, etc., number */
  void setNr(const int nr);

  //@}
  /** @name Miscellaneous Functions */
  //@{

  /** Returns true if the icon is a wall icon. */
  bool isWall();

  //@}

private:
  void setDefaults();

  int m_icon;  // icon type - see 'special symbols' in contam.h (I2) {W}
  int m_col;  // column position on the SketchPad (I2) {W}
  int m_row;  // row position on the SketchPad (I2) {W}
  int m_nr;  // zone, path, duct, etc., number (I2) {W}
};

} // detail
} // contam
} // openstudio

#endif
