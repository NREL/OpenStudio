/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
