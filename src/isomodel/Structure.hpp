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

#ifndef ISOMODEL_STRUCTURE_HPP
#define ISOMODEL_STRUCTURE_HPP

#include "../utilities/data/Vector.hpp"

namespace openstudio {
namespace isomodel {
  class Structure
  {
   public:
    double floorArea() const {
      return _floorArea;
    }
    const Vector& wallArea() const {
      return _wallArea;
    }
    const Vector& windowArea() const {
      return _windowArea;
    }
    const Vector& wallUniform() const {
      return _wallUniform;
    }
    const Vector& windowUniform() const {
      return _windowUniform;
    }
    const Vector& wallThermalEmissivity() const {
      return _wallThermalEmissivity;
    }
    const Vector& wallSolarAbsorbtion() const {
      return _wallSolarAbsorbtion;
    }
    double windowShadingDevice() const {
      return _windowShadingDevice;
    }
    const Vector& windowNormalIncidenceSolarEnergyTransmittance() const {
      return _windowNormalIncidenceSolarEnergyTransmittance;
    }
    const Vector& windowShadingCorrectionFactor() const {
      return _windowShadingCorrectionFactor;
    }
    double interiorHeatCapacity() const {
      return _interiorHeatCapacity;
    }
    double wallHeatCapacity() const {
      return _wallHeatCapacity;
    }
    double buildingHeight() const {
      return _buildingHeight;
    }
    double infiltrationRate() const {
      return _infiltrationRate;
    }
    void setFloorArea(double value) {
      _floorArea = value;
    }
    void setWallArea(const Vector& value) {
      _wallArea = value;
    }
    void setWindowArea(const Vector& value) {
      _windowArea = value;
    }
    void setWallUniform(const Vector& value) {
      _wallUniform = value;
    }
    void setWindowUniform(const Vector& value) {
      _windowUniform = value;
    }
    void setWallThermalEmissivity(const Vector& value) {
      _wallThermalEmissivity = value;
    }
    void setWallSolarAbsorbtion(const Vector& value) {
      _wallSolarAbsorbtion = value;
    }
    void setWindowShadingDevice(double value) {
      _windowShadingDevice = value;
    }
    void setWindowNormalIncidenceSolarEnergyTransmittance(const Vector& value) {
      _windowNormalIncidenceSolarEnergyTransmittance = value;
    }
    void setWindowShadingCorrectionFactor(const Vector& value) {
      _windowShadingCorrectionFactor = value;
    }
    void setInteriorHeatCapacity(double value) {
      _interiorHeatCapacity = value;
    }
    void setWallHeatCapacity(double value) {
      _wallHeatCapacity = value;
    }
    void setBuildingHeight(double value) {
      _buildingHeight = value;
    }
    void setInfiltrationRate(double value) {
      _infiltrationRate = value;
    }

   private:
    double _floorArea = 0;
    Vector _wallArea;
    Vector _windowArea;
    Vector _wallUniform;
    Vector _windowUniform;
    Vector _wallThermalEmissivity;
    Vector _wallSolarAbsorbtion;
    double _windowShadingDevice = 0;
    Vector _windowNormalIncidenceSolarEnergyTransmittance;
    Vector _windowShadingCorrectionFactor;
    double _interiorHeatCapacity = 0;
    double _wallHeatCapacity = 0;
    double _buildingHeight = 0;
    double _infiltrationRate = 0;
  };

}  // namespace isomodel
}  // namespace openstudio
#endif  // ISOMODEL_STRUCTURE_HPP
