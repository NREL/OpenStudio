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
#ifndef ISOMODEL_HEATING_HPP
#define ISOMODEL_HEATING_HPP

namespace openstudio {
namespace isomodel {
class Heating
  {
  public:
    double temperatureSetPointOccupied() const {return _temperatureSetPointOccupied;}
    double temperatureSetPointUnoccupied() const {return _temperatureSetPointUnoccupied;}
    double hvacLossFactor() const {return _hvacLossFactor;}
    double hotcoldWasteFactor() const {return _hotcoldWasteFactor;}
    double efficiency() const {return _efficiency;}
    double energyType() const {return _energyType;}
    double pumpControlReduction() const {return _pumpControlReduction;}
    double hotWaterDemand() const {return _hotWaterDemand;}
    double hotWaterDistributionEfficiency() const {return _hotWaterDistributionEfficiency;}
    double hotWaterSystemEfficiency() const {return _hotWaterSystemEfficiency;}
    double hotWaterEnergyType() const {return _hotWaterEnergyType;}

    void setTemperatureSetPointOccupied(double value) {_temperatureSetPointOccupied = value;}
    void setTemperatureSetPointUnoccupied(double value) {_temperatureSetPointUnoccupied = value;}
    void setHvacLossFactor(double value) {_hvacLossFactor = value;}
    void setHotcoldWasteFactor(double value) {_hotcoldWasteFactor = value;}
    void setEfficiency(double value) {_efficiency = value;}
    void setEnergyType(double value) {_energyType = value;}
    void setPumpControlReduction(double value) {_pumpControlReduction = value;}
    void setHotWaterDemand(double value) {_hotWaterDemand = value;}
    void setHotWaterDistributionEfficiency(double value) {_hotWaterDistributionEfficiency = value;}
    void setHotWaterSystemEfficiency(double value) {_hotWaterSystemEfficiency = value;}
    void setHotWaterEnergyType(double value) {_hotWaterEnergyType = value;}

  private:
    double _temperatureSetPointOccupied;
    double _temperatureSetPointUnoccupied;
    double _hvacLossFactor;
    double _efficiency;
    double _energyType;
    double _pumpControlReduction;
    double _hotWaterDemand;
    double _hotWaterDistributionEfficiency;
    double _hotWaterSystemEfficiency;
    double _hotWaterEnergyType;
    double _hotcoldWasteFactor;

  };

} // isomodel
} // openstudio
#endif // ISOMODEL_HEATING_HPP

