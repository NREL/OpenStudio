/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef AIRFLOW_CONTAM_PRJSUBOBJECTSIMPL_HPP
#define AIRFLOW_CONTAM_PRJSUBOBJECTSIMPL_HPP

#include "PrjDefines.hpp"
#include "PrjReader.hpp"

#include "../../utilities/core/String.hpp"

namespace openstudio {
namespace contam {
  namespace detail {

    class WeatherDataImpl
    {
     public:
      WeatherDataImpl();
      WeatherDataImpl(std::string Tambt, std::string barpres, std::string windspd, std::string winddir, std::string relhum, int daytyp, int uTa,
                      int ubP, int uws, int uwd);
      WeatherDataImpl(double Tambt, double barpres, double windspd, double winddir, double relhum, int daytyp, int uTa, int ubP, int uws, int uwd);
      void read(Reader& reader);
      std::string write();
      /** Returns the ambient temperature [K] */
      double Tambt() const;
      /** Sets the ambient temperature [K] */
      bool setTambt(const double Tambt);
      /** Sets the ambient temperature [K] */
      bool setTambt(const std::string& Tambt);
      /** Returns the barometric pressure [Pa] NOT corrected to sea level */
      double barpres() const;
      /** Sets the barometric pressure [Pa] NOT corrected to sea level */
      bool setBarpres(const double barpres);
      /** Sets the barometric pressure [Pa] NOT corrected to sea level */
      bool setBarpres(const std::string& barpres);
      /** Returns the wind speed [m/s] */
      double windspd() const;
      /** Sets the wind speed [m/s] */
      bool setWindspd(const double windspd);
      /** Sets the wind speed [m/s] */
      bool setWindspd(const std::string& windspd);
      /** Returns the wind direction: 0 = N, 90 = E, 180 = S, ... */
      double winddir() const;
      /** Sets the wind direction: 0 = N, 90 = E, 180 = S, ... */
      bool setWinddir(const double winddir);
      /** Sets the wind direction: 0 = N, 90 = E, 180 = S, ... */
      bool setWinddir(const std::string& winddir);
      /** Returns the relative humidity: 0.0 to 1.0 */
      double relhum() const;
      /** Sets the relative humidity: 0.0 to 1.0 */
      bool setRelhum(const double relhum);
      /** Sets the relative humidity: 0.0 to 1.0 */
      bool setRelhum(const std::string& relhum);
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

      PRJFLOAT m_Tambt;    // ambient temperature [K] (R4)
      PRJFLOAT m_barpres;  // barometric pressure [Pa] NOT corrected to sea level (R4)
      PRJFLOAT m_windspd;  // wind speed [m/s] (R4)
      PRJFLOAT m_winddir;  // wind direction: 0 = N, 90 = E, 180 = S, ...; (R4)
      PRJFLOAT m_relhum;   // relative humidity: 0.0 to 1.0 (R4)
      int m_daytyp;        // type of day (1-12) (I2)
      int m_uTa;           // units for Tambt (I2) {W}
      int m_ubP;           // units for barpres (I2) {W}
      int m_uws;           // units for windspd (I2) {W}
      int m_uwd;           // units for winddir (I2) {W}
    };

    class IconImpl
    {
     public:
      /** @name Constructors */
      //@{

      /** Create a new object with default values */
      IconImpl();
      /** Create a new object */
      IconImpl(int icon, int col, int row, int nr);

      //@}
      /** @name Input and Output */
      //@{

      /** Read an object from a Reader */
      void read(Reader& input);
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
      int m_col;   // column position on the SketchPad (I2) {W}
      int m_row;   // row position on the SketchPad (I2) {W}
      int m_nr;    // zone, path, duct, etc., number (I2) {W}
    };

  }  // namespace detail
}  // namespace contam
}  // namespace openstudio

#endif  // AIRFLOW_CONTAM_PRJSUBOBJECTSIMPL_HPP
