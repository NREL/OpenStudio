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

#ifndef ISOMODEL_HEATING_HPP
#define ISOMODEL_HEATING_HPP

namespace openstudio {
namespace isomodel {
  class Heating
  {
   public:
    double temperatureSetPointOccupied() const {
      return _temperatureSetPointOccupied;
    }
    double temperatureSetPointUnoccupied() const {
      return _temperatureSetPointUnoccupied;
    }
    double hvacLossFactor() const {
      return _hvacLossFactor;
    }
    double hotcoldWasteFactor() const {
      return _hotcoldWasteFactor;
    }
    double efficiency() const {
      return _efficiency;
    }
    double energyType() const {
      return _energyType;
    }
    double pumpControlReduction() const {
      return _pumpControlReduction;
    }
    double hotWaterDemand() const {
      return _hotWaterDemand;
    }
    double hotWaterDistributionEfficiency() const {
      return _hotWaterDistributionEfficiency;
    }
    double hotWaterSystemEfficiency() const {
      return _hotWaterSystemEfficiency;
    }
    double hotWaterEnergyType() const {
      return _hotWaterEnergyType;
    }

    void setTemperatureSetPointOccupied(double value) {
      _temperatureSetPointOccupied = value;
    }
    void setTemperatureSetPointUnoccupied(double value) {
      _temperatureSetPointUnoccupied = value;
    }
    void setHvacLossFactor(double value) {
      _hvacLossFactor = value;
    }
    void setHotcoldWasteFactor(double value) {
      _hotcoldWasteFactor = value;
    }
    void setEfficiency(double value) {
      _efficiency = value;
    }
    void setEnergyType(double value) {
      _energyType = value;
    }
    void setPumpControlReduction(double value) {
      _pumpControlReduction = value;
    }
    void setHotWaterDemand(double value) {
      _hotWaterDemand = value;
    }
    void setHotWaterDistributionEfficiency(double value) {
      _hotWaterDistributionEfficiency = value;
    }
    void setHotWaterSystemEfficiency(double value) {
      _hotWaterSystemEfficiency = value;
    }
    void setHotWaterEnergyType(double value) {
      _hotWaterEnergyType = value;
    }

   private:
    double _temperatureSetPointOccupied = 0;
    double _temperatureSetPointUnoccupied = 0;
    double _hvacLossFactor = 0;
    double _efficiency = 0;
    double _energyType = 0;
    double _pumpControlReduction = 0;
    double _hotWaterDemand = 0;
    double _hotWaterDistributionEfficiency = 0;
    double _hotWaterSystemEfficiency = 0;
    double _hotWaterEnergyType = 0;
    double _hotcoldWasteFactor = 0;
  };

}  // namespace isomodel
}  // namespace openstudio
#endif  // ISOMODEL_HEATING_HPP
