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

#ifndef AIRFLOW_CONTAM_PRJSUBOBJECTS_HPP
#define AIRFLOW_CONTAM_PRJSUBOBJECTS_HPP

#include "PrjDefines.hpp"
#include "PrjReader.hpp"
#include "../AirflowAPI.hpp"

namespace openstudio {
namespace contam {

  class Reader;
  namespace detail {
    class WeatherDataImpl;
    class IconImpl;
  }  // namespace detail

  /** WeatherData objects contain the simplified weather data associated with a CONTAM model. */
  class AIRFLOW_API WeatherData
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Create a new object with default values */
    WeatherData();
    /** Create a new object */
    WeatherData(std::string Tambt, std::string barpres, std::string windspd, std::string winddir, std::string relhum, int daytyp, int uTa, int ubP,
                int uws, int uwd);
    /** Create a new object */
    WeatherData(double Tambt, double barpres, double windspd, double winddir, double relhum, int daytyp, int uTa, int ubP, int uws, int uwd);
    /** Create a new object from another object */
    WeatherData(const WeatherData& other);
    /** Destroy the object */
    ~WeatherData() = default;

    //@}
    /** @name Operators */
    //@{

    /** Copy operator */
    WeatherData& operator=(const WeatherData& other);
    /** Equality operator */
    bool operator==(const WeatherData& other) const;
    /** Inequality operator */
    bool operator!=(const WeatherData& other) const;

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
    //@}

   private:
    std::shared_ptr<detail::WeatherDataImpl> m_impl;
  };

  /** The Icon object contains basic information about sketchpad icons. This object
*  is not strictly necessary for creating CONTAM models, but is needed to read
*  models created in ContamW. */
  class AIRFLOW_API Icon
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Create a new object with default values. */
    Icon();
    /** Create a new object. */
    Icon(int icon, int col, int row, int nr);
    /** Create a new object from another object. */
    Icon(const Icon& other);
    /** Destroy the object. */
    ~Icon() = default;

    //@}
    /** @name Operators */
    //@{

    /** Copy operator */
    Icon& operator=(const Icon& other);
    /** Equality operator */
    bool operator==(const Icon& other) const;
    /** Inequality operator */
    bool operator!=(const Icon& other) const;

    //@}
    /** @name Input and Output */
    //@{

    /** Read an object from a Reader. */
    void read(Reader& input);
    /** Write the object to a string. */
    std::string write();

    //@}
    /** @name Getters and Setters */
    //@{

    /** Returns the icon type - see 'special symbols' in contam.h. */
    int icon() const;
    /** Sets the icon type - see 'special symbols' in contam.h. */
    void setIcon(const int icon);
    /** Returns the icon's column position on the SketchPad. */
    int col() const;
    /** Sets the icon's column position on the SketchPad. */
    void setCol(const int col);
    /** Returns the icon's row position on the SketchPad. */
    int row() const;
    /** Sets the icon's row position on the SketchPad. */
    void setRow(const int row);
    /** Returns the icon's zone, path, duct, etc., number. */
    int nr() const;
    /** Sets the icon's zone, path, duct, etc., number. */
    void setNr(const int nr);

    //@}
    /** @name Miscellaneous Functions */
    //@{

    /** Returns true if the icon is a wall icon. */
    bool isWall();

    //@}

   private:
    std::shared_ptr<detail::IconImpl> m_impl;
  };

  /** FanDataPoint is an object for defining fan characteristics. It contains a single
*  pressure and flow rate data point. */
  class AIRFLOW_API FanDataPoint
  {
   public:
    /** @name Constructors */
    //@{
    /** Create a new object with default values. */
    FanDataPoint();
    /** Create a new object. */
    FanDataPoint(double mF, int u_mF, double dP, int u_dP, double rP, int u_rP);
    /** Create a new object. */
    FanDataPoint(std::string mF, int u_mF, std::string dP, int u_dP, std::string rP, int u_rP);

    //@}
    /** @name Input and Output */
    //@{

    /** Read an object from a Reader. */
    void read(Reader& reader);
    /** Write the object to a string. */
    std::string write();

    //@}
    /** @name Getters and Setters */
    //@{

    /** Returns the measured flow rate [kg/s]. */
    double mF() const;
    /** Sets the measured flow rate [kg/s]. */
    bool setMF(const double mF);
    /** Sets the measured flow rate [kg/s]. */
    bool setMF(const std::string& mF);
    /** Returns the measured flow rate display units. */
    int u_mF() const;
    /** Sets the measured flow rate display units. */
    void setU_mF(const int u_mF);
    /** Returns the measured pressure rise [Pa]. */
    double dP() const;
    /** Sets the measured pressure rise [Pa]. */
    bool setDP(const double dP);
    /** Sets the measured pressure rise [Pa]. */
    bool setDP(const std::string& dP);
    /** Returns the measured pressure rise display units. */
    int u_dP() const;
    /** Sets the measured pressure rise display units. */
    void setU_dP(const int u_dP);
    /** Returns the revised pressure rise [Pa]. */
    double rP() const;
    /** Sets the revised pressure rise [Pa]. */
    bool setRP(const double rP);
    /** Sets the revised pressure rise [Pa]. */
    bool setRP(const std::string& rP);
    /** Returns the revised pressure rise display units. */
    int u_rP() const;
    /** Sets the revised pressure rise display units. */
    void setU_rP(const int u_rP);
    //@}
   private:
    void setDefaults();

    PRJFLOAT m_mF;  // measured flow rates [kg/s] (R4) {W}
    int m_u_mF;     // units of measured flows (I2) {W}
    PRJFLOAT m_dP;  // measured pressure rises [Pa] (R4) {W}
    int m_u_dP;     // units of pressure rises (I2) {W}
    PRJFLOAT m_rP;  // revised pressure rises [Pa] (R4) {W}
    int m_u_rP;     // units of revised pressures (I2) {W}
  };

  /** The XyDataPoint object is used by a number of the airflow element objects to
*  store data specific to the element. */
  class AIRFLOW_API XyDataPoint
  {
   public:
    /** @name Constructors */
    //@{

    /** Create a new object with default values. */
    XyDataPoint();
    /** Create a new object. */
    XyDataPoint(double x, double y);
    /** Create a new object. */
    XyDataPoint(std::string x, std::string y);

    //@}
    /** @name Input and Output */
    //@{

    /** Read an object from a Reader. */
    void read(Reader& reader);
    /** Write the object to a string. */
    std::string write();

    //@}
    /** @name Getters and Setters */
    //@{

    /** Returns the value of independent variable. */
    double x() const;
    /** Sets the value of independent variable. */
    bool setX(const double x);
    /** Sets the value of independent variable. */
    bool setX(const std::string& x);
    /** Returns the value of dependent variable. */
    double y() const;
    /** Sets the value of dependent variable. */
    bool setY(const double y);
    /** Sets the value of dependent variable. */
    bool setY(const std::string& y);
    //@}
   private:
    void setDefaults();

    PRJFLOAT m_x;  // value of independent variable (R4)
    PRJFLOAT m_y;  // value of dependent variable (R4)
  };

  /** The AirflowSubelementData object stores specific details for an
*  airflow element that is part of a super element. */
  class AIRFLOW_API AirflowSubelementData
  {
   public:
    /** @name Constructors */
    //@{

    /** Create a new object with default values. */
    AirflowSubelementData();
    /** Create a new object. */
    AirflowSubelementData(int nr, double relHt, int filt);
    /** Create a new object. */
    AirflowSubelementData(int nr, std::string relHt, int filt);

    //@}
    /** @name Input and Output */
    //@{

    /** Read an object from a Reader. */
    void read(Reader& reader);
    /** Write the object to a string. */
    std::string write();

    //@}
    /** @name Getters and Setters */
    //@{

    /** Returns the sub-element number. */
    int nr() const;
    /** Sets the sub-element number. */
    void setNr(const int nr);
    /** Returns the relative height of the sub-element. */
    double relHt() const;
    /** Sets the relative height of the sub-element. */
    bool setRelHt(const double relHt);
    /** Sets the relative height of the sub-element. */
    bool setRelHt(const std::string& relHt);
    /** Returns the filtering flag: (1=filtered, 0=not). */
    int filt() const;
    /** Sets the filtering flag (1=filtered, 0=not). */
    void setFilt(const int filt);
    //@}
   private:
    void setDefaults();

    int m_nr;          // sub-element number (IX)
    PRJFLOAT m_relHt;  // relative height of sub-element (R4)
    int m_filt;        // 1= filtered, 0= not (I2)
  };

  /** The PressureCoefficientPoint object contains a single wind pressure coefficient
*  data point. The azimuthal angle is stored in degrees from north with the
*  corresponding pressure coefficient. */
  class AIRFLOW_API PressureCoefficientPoint
  {
   public:
    /** @name Constructors */
    //@{

    /** Create a new object with default values. */
    PressureCoefficientPoint();
    /** Create a new object. */
    PressureCoefficientPoint(double azm, double coef);
    /** Create a new object. */
    PressureCoefficientPoint(std::string azm, std::string coef);

    //@}
    /** @name Input and Output */
    //@{

    /** Read an object from a Reader. */
    void read(Reader& reader);
    /** Write the object to a string. */
    std::string write();

    //@}
    /** @name Getters and Setters */
    //@{

    /** Returns the wind azimuth value [degrees]. */
    double azm() const;
    /** Sets the wind azimuth value [degrees]. */
    bool setAzm(const double azm);
    /** Sets the wind azimuth value [degrees]. */
    bool setAzm(const std::string& azm);
    /** Returns the normalized wind pressure coefficient. */
    double coef() const;
    /** Sets the normalized wind pressure coefficient. */
    bool setCoef(const double coef);
    /** Sets the normalized wind pressure coefficient. */
    bool setCoef(const std::string& coef);
    //@}
   private:
    void setDefaults();

    PRJFLOAT m_azm;   // wind azimuth value {R4} [degrees]
    PRJFLOAT m_coef;  // normalized wind pressure coefficients {R4} [-]
  };

  /** SchedulePoint stores the time (in hh:mm:ss format) and control value for
*  a day schedule data point. */
  class AIRFLOW_API SchedulePoint
  {
   public:
    /** @name Constructors */
    //@{

    /** Create a new object with default values. */
    SchedulePoint();
    /** Create a new object. */
    SchedulePoint(std::string time, double ctrl);
    /** Create a new object. */
    SchedulePoint(std::string time, std::string ctrl);

    //@}
    /** @name Input and Output */
    //@{

    /** Read an object from a Reader. */
    void read(Reader& reader);
    /** Write the object to a string. */
    std::string write();

    //@}
    /** @name Getters and Setters */
    //@{

    /** Returns the time-of-day (hh:mm:ss). */
    std::string time() const;
    /** Sets the time-of-day (hh:mm:ss). */
    void setTime(const std::string& time);
    /** Returns the control value. */
    double ctrl() const;
    /** Sets the control value. */
    bool setCtrl(const double ctrl);
    /** Sets the control value. */
    bool setCtrl(const std::string& ctrl);
    //@}
   private:
    void setDefaults();

    std::string m_time;  // time-of-day [s] (hh:mm:ss)
    PRJFLOAT m_ctrl;     // corresponding control value (R4) [-]
  };

}  // namespace contam
}  // namespace openstudio

#endif  // AIRFLOW_CONTAM_PRJSUBOBJECTS_HPP
