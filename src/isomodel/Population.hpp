/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef ISOMODEL_POPULATION_HPP
#define ISOMODEL_POPULATION_HPP

namespace openstudio {
namespace isomodel {
  class Population
  {
   public:
    double hoursEnd() const {
      return _hoursEnd;
    }
    double hoursStart() const {
      return _hoursStart;
    }
    double daysEnd() const {
      return _daysEnd;
    }
    double daysStart() const {
      return _daysStart;
    }
    double densityOccupied() const {
      return _densityOccupied;
    }
    double densityUnoccupied() const {
      return _densityUnoccupied;
    }
    double heatGainPerPerson() const {
      return _heatGainPerPerson;
    }

    void setHoursEnd(double value) {
      _hoursEnd = value;
    }
    void setHoursStart(double value) {
      _hoursStart = value;
    }
    void setDaysEnd(double value) {
      _daysEnd = value;
    }
    void setDaysStart(double value) {
      _daysStart = value;
    }
    void setDensityOccupied(double value) {
      _densityOccupied = value;
    }
    void setDensityUnoccupied(double value) {
      _densityUnoccupied = value;
    }
    void setHeatGainPerPerson(double value) {
      _heatGainPerPerson = value;
    }

   private:
    double _hoursEnd = 0;
    double _hoursStart = 0;
    double _daysEnd = 0;
    double _daysStart = 0;
    double _densityOccupied = 0;
    double _densityUnoccupied = 0;
    double _heatGainPerPerson = 0;
  };

}  // namespace isomodel
}  // namespace openstudio
#endif  // ISOMODEL_POPULATION_HPP
