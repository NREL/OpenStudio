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
#ifndef ISOMODEL_COOLING_HPP
#define ISOMODEL_COOLING_HPP

namespace openstudio {
namespace isomodel {
  class Cooling
  {
  public:
    double temperatureSetPointOccupied() const  {return _temperatureSetPointOccupied;}
    double temperatureSetPointUnoccupied() const {return _temperatureSetPointUnoccupied;}
    double cop() const {return _cOP;}
    double partialLoadValue() const {return _partialLoadValue;}
    double hvacLossFactor() const {return _hvacLossFactor;}
    double pumpControlReduction() const {return _pumpControlReduction;}

    void setTemperatureSetPointOccupied(double value) {_temperatureSetPointOccupied = value;}
    void setTemperatureSetPointUnoccupied(double value) {_temperatureSetPointUnoccupied = value;}
    void setCOP(double value) {_cOP = value;}
    void setPartialLoadValue(double value) {_partialLoadValue = value;}
    void setHvacLossFactor(double value) {_hvacLossFactor = value;}
    void setPumpControlReduction(double value) {_pumpControlReduction = value;}

  private:
    double _temperatureSetPointOccupied;
    double _temperatureSetPointUnoccupied;
    double _cOP;
    double _partialLoadValue;
    double _hvacLossFactor;
    double _pumpControlReduction;
  };

} // isomodel
} // openstudio
#endif // ISOMODEL_COOLING_HPP

