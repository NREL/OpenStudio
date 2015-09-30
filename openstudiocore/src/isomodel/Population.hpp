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
#ifndef ISOMODEL_POPULATION_HPP
#define ISOMODEL_POPULATION_HPP

namespace openstudio {
namespace isomodel {
class Population
  {
  public:
    double hoursEnd() const {return _hoursEnd;}
    double hoursStart() const {return _hoursStart;}
    double daysEnd() const {return _daysEnd;}
    double daysStart() const {return _daysStart;}
    double densityOccupied() const {return _densityOccupied;}
    double densityUnoccupied() const {return _densityUnoccupied;}
    double heatGainPerPerson() const {return _heatGainPerPerson;}

    void setHoursEnd(double value) {_hoursEnd = value;}
    void setHoursStart(double value) {_hoursStart = value;}
    void setDaysEnd(double value) {_daysEnd = value;}
    void setDaysStart(double value) {_daysStart = value;}
    void setDensityOccupied(double value) {_densityOccupied = value;}
    void setDensityUnoccupied(double value) {_densityUnoccupied = value;}
    void setHeatGainPerPerson(double value) {_heatGainPerPerson = value;}

  private:
    double _hoursEnd;
    double _hoursStart;
    double _daysEnd;
    double _daysStart;
    double _densityOccupied;
    double _densityUnoccupied;
    double _heatGainPerPerson;

  };

} // isomodel
} // openstudio
#endif // ISOMODEL_POPULATION_HPP
