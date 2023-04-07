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

#ifndef ISOMODEL_BUILDING_HPP
#define ISOMODEL_BUILDING_HPP

namespace openstudio {
namespace isomodel {
  class Building
  {
   public:
    double lightingOccupancySensor() const {
      return _lightingOccupancySensor;
    }
    double constantIllumination() const {
      return _constantIllumination;
    }
    double electricApplianceHeatGainOccupied() const {
      return _electricApplianceHeatGainOccupied;
    }
    double electricApplianceHeatGainUnoccupied() const {
      return _electricApplianceHeatGainUnoccupied;
    }
    double gasApplianceHeatGainOccupied() const {
      return _gasApplianceHeatGainOccupied;
    }
    double gasApplianceHeatGainUnoccupied() const {
      return _gasApplianceHeatGainUnoccupied;
    }
    double buildingEnergyManagement() const {
      return _buildingEnergyManagement;
    }

    void setLightingOccupancySensor(double value) {
      _lightingOccupancySensor = value;
    }
    void setConstantIllumination(double value) {
      _constantIllumination = value;
    }
    void setElectricApplianceHeatGainOccupied(double value) {
      _electricApplianceHeatGainOccupied = value;
    }
    void setElectricApplianceHeatGainUnoccupied(double value) {
      _electricApplianceHeatGainUnoccupied = value;
    }
    void setGasApplianceHeatGainOccupied(double value) {
      _gasApplianceHeatGainOccupied = value;
    }
    void setGasApplianceHeatGainUnoccupied(double value) {
      _gasApplianceHeatGainUnoccupied = value;
    }
    void setBuildingEnergyManagement(double value) {
      _buildingEnergyManagement = value;
    }

   private:
    double _lightingOccupancySensor = 0;
    double _constantIllumination = 0;
    double _electricApplianceHeatGainOccupied = 0;
    double _electricApplianceHeatGainUnoccupied = 0;
    double _gasApplianceHeatGainOccupied = 0;
    double _gasApplianceHeatGainUnoccupied = 0;
    double _buildingEnergyManagement = 0;
  };

}  // namespace isomodel
}  // namespace openstudio
#endif  // ISOMODEL_BUILDING_HPP
