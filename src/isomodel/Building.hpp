/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
