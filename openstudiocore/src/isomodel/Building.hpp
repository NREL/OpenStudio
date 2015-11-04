/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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
#ifndef ISOMODEL_BUILDING_HPP
#define ISOMODEL_BUILDING_HPP

namespace openstudio {
namespace isomodel {
class Building
  {
  public:
    double lightingOccupancySensor() const {return _lightingOccupancySensor;}
    double constantIllumination() const {return _constantIllumination;}
    double electricApplianceHeatGainOccupied() const {return _electricApplianceHeatGainOccupied;}
    double electricApplianceHeatGainUnoccupied() const {return _electricApplianceHeatGainUnoccupied;}
    double gasApplianceHeatGainOccupied() const {return _gasApplianceHeatGainOccupied;}
    double gasApplianceHeatGainUnoccupied() const {return _gasApplianceHeatGainUnoccupied;}
    double buildingEnergyManagement() const {return _buildingEnergyManagement;}

    void setLightingOccupancySensor(double value) {_lightingOccupancySensor = value;}
    void setConstantIllumination(double value) {_constantIllumination = value;}
    void setElectricApplianceHeatGainOccupied(double value) {_electricApplianceHeatGainOccupied = value;}
    void setElectricApplianceHeatGainUnoccupied(double value) {_electricApplianceHeatGainUnoccupied = value;}
    void setGasApplianceHeatGainOccupied(double value) {_gasApplianceHeatGainOccupied = value;}
    void setGasApplianceHeatGainUnoccupied(double value) {_gasApplianceHeatGainUnoccupied = value;}
    void setBuildingEnergyManagement(double value) {_buildingEnergyManagement = value;}

  private:
    double _lightingOccupancySensor;
    double _constantIllumination;
    double _electricApplianceHeatGainOccupied;
    double _electricApplianceHeatGainUnoccupied;
    double _gasApplianceHeatGainOccupied;
    double _gasApplianceHeatGainUnoccupied;
    double _buildingEnergyManagement;
  };
  

} // isomodel
} // openstudio
#endif // ISOMODEL_BUILDING_HPP

