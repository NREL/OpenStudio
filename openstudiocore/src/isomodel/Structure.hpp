/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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
#ifndef ISOMODEL_STRUCTURE_HPP
#define ISOMODEL_STRUCTURE_HPP

#include "../utilities/data/Vector.hpp"

namespace openstudio {
namespace isomodel {
class Structure
  {
  public:
    double floorArea() const {return _floorArea;}
    const Vector &wallArea() const {return _wallArea;}
    const Vector &windowArea() const {return _windowArea;}
    const Vector &wallUniform() const {return _wallUniform;}
    const Vector &windowUniform() const {return _windowUniform;}
    const Vector &wallThermalEmissivity() const {return _wallThermalEmissivity;}
    const Vector &wallSolarAbsorbtion() const {return _wallSolarAbsorbtion;}
    double windowShadingDevice() const {return _windowShadingDevice;}
    const Vector &windowNormalIncidenceSolarEnergyTransmittance() const {return _windowNormalIncidenceSolarEnergyTransmittance;}
    const Vector &windowShadingCorrectionFactor() const {return _windowShadingCorrectionFactor;}
    double interiorHeatCapacity() const {return _interiorHeatCapacity;}
    double wallHeatCapacity() const {return _wallHeatCapacity;}
    double buildingHeight() const {return _buildingHeight;}
    double infiltrationRate() const {return _infiltrationRate;}
    void setFloorArea(double value) {_floorArea = value;}
    void setWallArea(const Vector &value) {_wallArea = value;}
    void setWindowArea(const Vector &value) {_windowArea = value;}
    void setWallUniform(const Vector &value) {_wallUniform = value;}
    void setWindowUniform(const Vector &value) {_windowUniform = value;}
    void setWallThermalEmissivity(const Vector &value) {_wallThermalEmissivity = value;}
    void setWallSolarAbsorbtion(const Vector &value) {_wallSolarAbsorbtion = value;}
    void setWindowShadingDevice(double value) {_windowShadingDevice = value;}
    void setWindowNormalIncidenceSolarEnergyTransmittance(const Vector &value) {_windowNormalIncidenceSolarEnergyTransmittance = value;}
    void setWindowShadingCorrectionFactor(const Vector &value) {_windowShadingCorrectionFactor = value;}
    void setInteriorHeatCapacity(double value) {_interiorHeatCapacity = value;}
    void setWallHeatCapacity(double value) {_wallHeatCapacity = value;}
    void setBuildingHeight(double value) {_buildingHeight = value;}
    void setInfiltrationRate(double value) {_infiltrationRate = value;}

  private:
    double _floorArea;
    Vector _wallArea;
    Vector _windowArea;
    Vector _wallUniform;
    Vector _windowUniform;
    Vector _wallThermalEmissivity;
    Vector _wallSolarAbsorbtion;
    double _windowShadingDevice;
    Vector _windowNormalIncidenceSolarEnergyTransmittance;
    Vector _windowShadingCorrectionFactor;
    double _interiorHeatCapacity;
    double _wallHeatCapacity;
    double _buildingHeight;
    double _infiltrationRate;

  };

} // isomodel
} // openstudio
#endif // ISOMODEL_STRUCTURE_HPP
