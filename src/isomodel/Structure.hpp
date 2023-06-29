/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
