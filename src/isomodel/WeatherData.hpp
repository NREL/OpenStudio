/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef ISOMODEL_WEATHERDATA_HPP
#define ISOMODEL_WEATHERDATA_HPP

#include "ISOModelAPI.hpp"
#include "../utilities/data/Vector.hpp"
#include "../utilities/data/Matrix.hpp"

namespace openstudio {
namespace isomodel {

  class ISOMODEL_API WeatherData
  {
   public:
    /**
   * mean monthly Global Horizontal Radiation (W/m2)
   */
    const Vector& mEgh() const {
      return _mEgh;
    }
    void setMEgh(Vector val) {
      _mEgh = val;
    }
    /**
   * mean monthly dry bulb temp (C)
   */
    const Vector& mdbt() const {
      return _mdbt;
    }
    void setMdbt(Vector val) {
      _mdbt = val;
    }
    /**
   * mean monthly wind speed; (m/s)
   */
    const Vector& mwind() const {
      return _mwind;
    }
    void setMwind(Vector val) {
      _mwind = val;
    }

    /**
   * mean monthly total solar radiation (W/m2) on a vertical surface for each of the 8 cardinal directions
   */
    const Matrix& msolar() const {
      return _msolar;
    }
    /**
   * mean monthly dry bulb temp for each of the 24 hours of the day (C)
   */
    const Matrix& mhdbt() const {
      return _mhdbt;
    }
    /**
   * mean monthly Global Horizontal Radiation for each of the 24 hours of the day (W/m2)
   */
    const Matrix& mhEgh() const {
      return _mhEgh;
    }

    void setMhEgh(const Matrix& val) {
      _mhEgh = val;
    }
    void setMsolar(const Matrix& val) {
      _msolar = val;
    }
    void setMhdbt(const Matrix& val) {
      _mhdbt = val;
    }

   private:
    Matrix _msolar;
    Matrix _mhdbt;
    Matrix _mhEgh;
    Vector _mEgh;
    Vector _mdbt;
    Vector _mwind;
  };
}  // namespace isomodel
}  // namespace openstudio

#endif  // ISOMODEL_WEATHERDATA_HPP
