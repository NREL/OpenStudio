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
