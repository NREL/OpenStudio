
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
#ifndef ISOMODEL_VENTILATION_HPP
#define ISOMODEL_VENTILATION_HPP

namespace openstudio {
namespace isomodel {
class Ventilation
  {
  public:
    double supplyRate() const {return _supplyRate;}
    double supplyDifference() const {return _supplyDifference;}
    double heatRecoveryEfficiency() const {return _heatRecoveryEfficiency;}
    double exhaustAirRecirculated() const {return _exhaustAirRecirculated;}
    double type() const {return _type;}
    double fanPower() const {return _fanPower;}
    double fanControlFactor() const {return _fanControlFactor;}
    double wasteFactor() const {return _wasteFactor;}

    void setSupplyRate(double value) {_supplyRate = value;}
    void setSupplyDifference(double value) {_supplyDifference = value;}
    void setHeatRecoveryEfficiency(double value) {_heatRecoveryEfficiency = value;}
    void setExhaustAirRecirculated(double value) {_exhaustAirRecirculated = value;}
    void setType(double value) {_type = value;}
    void setFanPower(double value) {_fanPower = value;}
    void setFanControlFactor(double value) {_fanControlFactor = value;}
    void setWasteFactor(double value) {_wasteFactor = value;}

  private:
    double _supplyRate;
    double _supplyDifference;
    double _heatRecoveryEfficiency;
    double _exhaustAirRecirculated;
    double _type;
    double _fanPower;
    double _fanControlFactor;
    double _wasteFactor;
  };
} // isomodel
} // openstudio
#endif // ISOMODEL_VENTILATION_HPP
