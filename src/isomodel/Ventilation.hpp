/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef ISOMODEL_VENTILATION_HPP
#define ISOMODEL_VENTILATION_HPP

namespace openstudio {
namespace isomodel {
  class Ventilation
  {
   public:
    double supplyRate() const {
      return _supplyRate;
    }
    double supplyDifference() const {
      return _supplyDifference;
    }
    double heatRecoveryEfficiency() const {
      return _heatRecoveryEfficiency;
    }
    double exhaustAirRecirculated() const {
      return _exhaustAirRecirculated;
    }
    double type() const {
      return _type;
    }
    double fanPower() const {
      return _fanPower;
    }
    double fanControlFactor() const {
      return _fanControlFactor;
    }
    double wasteFactor() const {
      return _wasteFactor;
    }

    void setSupplyRate(double value) {
      _supplyRate = value;
    }
    void setSupplyDifference(double value) {
      _supplyDifference = value;
    }
    void setHeatRecoveryEfficiency(double value) {
      _heatRecoveryEfficiency = value;
    }
    void setExhaustAirRecirculated(double value) {
      _exhaustAirRecirculated = value;
    }
    void setType(double value) {
      _type = value;
    }
    void setFanPower(double value) {
      _fanPower = value;
    }
    void setFanControlFactor(double value) {
      _fanControlFactor = value;
    }
    void setWasteFactor(double value) {
      _wasteFactor = value;
    }

   private:
    double _supplyRate = 0;
    double _supplyDifference = 0;
    double _heatRecoveryEfficiency = 0;
    double _exhaustAirRecirculated = 0;
    double _type = 0;
    double _fanPower = 0;
    double _fanControlFactor = 0;
    double _wasteFactor = 0;
  };
}  // namespace isomodel
}  // namespace openstudio
#endif  // ISOMODEL_VENTILATION_HPP
