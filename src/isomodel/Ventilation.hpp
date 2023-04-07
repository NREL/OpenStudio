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

#ifndef ISOMODEL_VENTILATION_HPP
#define ISOMODEL_VENTILATION_HPP

namespace openstudio {
namespace isomodel {
  class Ventilation
  {
   public:
    double supplyRate() const {
      return _supplyRate;
    }
    double supplyDifference() const {
      return _supplyDifference;
    }
    double heatRecoveryEfficiency() const {
      return _heatRecoveryEfficiency;
    }
    double exhaustAirRecirculated() const {
      return _exhaustAirRecirculated;
    }
    double type() const {
      return _type;
    }
    double fanPower() const {
      return _fanPower;
    }
    double fanControlFactor() const {
      return _fanControlFactor;
    }
    double wasteFactor() const {
      return _wasteFactor;
    }

    void setSupplyRate(double value) {
      _supplyRate = value;
    }
    void setSupplyDifference(double value) {
      _supplyDifference = value;
    }
    void setHeatRecoveryEfficiency(double value) {
      _heatRecoveryEfficiency = value;
    }
    void setExhaustAirRecirculated(double value) {
      _exhaustAirRecirculated = value;
    }
    void setType(double value) {
      _type = value;
    }
    void setFanPower(double value) {
      _fanPower = value;
    }
    void setFanControlFactor(double value) {
      _fanControlFactor = value;
    }
    void setWasteFactor(double value) {
      _wasteFactor = value;
    }

   private:
    double _supplyRate = 0;
    double _supplyDifference = 0;
    double _heatRecoveryEfficiency = 0;
    double _exhaustAirRecirculated = 0;
    double _type = 0;
    double _fanPower = 0;
    double _fanControlFactor = 0;
    double _wasteFactor = 0;
  };
}  // namespace isomodel
}  // namespace openstudio
#endif  // ISOMODEL_VENTILATION_HPP
