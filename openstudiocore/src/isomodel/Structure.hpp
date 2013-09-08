/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <utilities/data/Vector.hpp>

namespace openstudio {
namespace isomodel {
class Structure
  {
  public:
    Structure(void);
    ~Structure(void);
    double floorArea() const {return _floorArea;}
    Vector wallArea() const {return _wallArea;}
    Vector windowArea() const {return _windowArea;}
    Vector wallUniform() const {return _wallUniform;}
    Vector windowUniform() const {return _windowUniform;}
    Vector wallThermalEmissivity() const {return _wallThermalEmissivity;}
    Vector wallSolarAbsorbtion() const {return _wallSolarAbsorbtion;}
    double windowShadingDevice() const {return _windowShadingDevice;}
    Vector windowNormalIncidenceSolarEnergyTransmittance() const {return _windowNormalIncidenceSolarEnergyTransmittance;}
    Vector windowShadingCorrectionFactor() const {return _windowShadingCorrectionFactor;}
    double interiorHeatCapacity() const {return _interiorHeatCapacity;}
    double wallHeatCapacity() const {return _wallHeatCapacity;}
    double stories() const {return _stories;}
    double floorToFloorHeight() const {return _floorToFloorHeight;}
    double infiltrationRate() const {return _infiltrationRate;}
    void setFloorArea(double value) {_floorArea = value;}
    void setWallArea(Vector value) {_wallArea = value;}
    void setWindowArea(Vector value) {_windowArea = value;}
    void setWallUniform(Vector value) {_wallUniform = value;}
    void setWindowUniform(Vector value) {_windowUniform = value;}
    void setWallThermalEmissivity(Vector value) {_wallThermalEmissivity = value;}
    void setWallSolarAbsorbtion(Vector value) {_wallSolarAbsorbtion = value;}
    void setWindowShadingDevice(double value) {_windowShadingDevice = value;}
    void setWindowNormalIncidenceSolarEnergyTransmittance(Vector value) {_windowNormalIncidenceSolarEnergyTransmittance = value;}
    void setWindowShadingCorrectionFactor(Vector value) {_windowShadingCorrectionFactor = value;}
    void setInteriorHeatCapacity(double value) {_interiorHeatCapacity = value;}
    void setWallHeatCapacity(double value) {_wallHeatCapacity = value;}
    void setStories(double value) {_stories = value;}
    void setFloorToFloorHeight(double value) {_floorToFloorHeight = value;}
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
    double _stories;
    double _floorToFloorHeight;
    double _infiltrationRate;

  };

} // isomodel
} // openstudio
#endif // ISOMODEL_STRUCTURE_HPP