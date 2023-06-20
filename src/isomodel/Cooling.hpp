/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef ISOMODEL_COOLING_HPP
#define ISOMODEL_COOLING_HPP

namespace openstudio {
namespace isomodel {
  class Cooling
  {
   public:
    double temperatureSetPointOccupied() const {
      return _temperatureSetPointOccupied;
    }
    double temperatureSetPointUnoccupied() const {
      return _temperatureSetPointUnoccupied;
    }
    double cop() const {
      return _cOP;
    }
    double partialLoadValue() const {
      return _partialLoadValue;
    }
    double hvacLossFactor() const {
      return _hvacLossFactor;
    }
    double pumpControlReduction() const {
      return _pumpControlReduction;
    }

    void setTemperatureSetPointOccupied(double value) {
      _temperatureSetPointOccupied = value;
    }
    void setTemperatureSetPointUnoccupied(double value) {
      _temperatureSetPointUnoccupied = value;
    }
    void setCOP(double value) {
      _cOP = value;
    }
    void setPartialLoadValue(double value) {
      _partialLoadValue = value;
    }
    void setHvacLossFactor(double value) {
      _hvacLossFactor = value;
    }
    void setPumpControlReduction(double value) {
      _pumpControlReduction = value;
    }

   private:
    double _temperatureSetPointOccupied = 0;
    double _temperatureSetPointUnoccupied = 0;
    double _cOP = 0;
    double _partialLoadValue = 0;
    double _hvacLossFactor = 0;
    double _pumpControlReduction = 0;
  };

}  // namespace isomodel
}  // namespace openstudio
#endif  // ISOMODEL_COOLING_HPP
