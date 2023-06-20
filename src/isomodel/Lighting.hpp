/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef ISOMODEL_LIGHTING_HPP
#define ISOMODEL_LIGHTING_HPP

namespace openstudio {
namespace isomodel {
  class Lighting
  {
   public:
    double powerDensityOccupied() const {
      return _powerDensityOccupied;
    }
    double powerDensityUnoccupied() const {
      return _powerDensityUnoccupied;
    }
    double dimmingFraction() const {
      return _dimmingFraction;
    }
    double exteriorEnergy() const {
      return _exteriorEnergy;
    }

    void setPowerDensityOccupied(double value) {
      _powerDensityOccupied = value;
    }
    void setPowerDensityUnoccupied(double value) {
      _powerDensityUnoccupied = value;
    }
    void setDimmingFraction(double value) {
      _dimmingFraction = value;
    }
    void setExteriorEnergy(double value) {
      _exteriorEnergy = value;
    }

   private:
    double _powerDensityOccupied = 0;
    double _powerDensityUnoccupied = 0;
    double _dimmingFraction = 0;
    double _exteriorEnergy = 0;
  };

}  // namespace isomodel
}  // namespace openstudio
#endif  // ISOMODEL_LIGHTING_HPP
