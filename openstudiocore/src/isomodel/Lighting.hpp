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
#ifndef ISOMODEL_LIGHTING_HPP
#define ISOMODEL_LIGHTING_HPP

namespace openstudio {
namespace isomodel {
class Lighting
  {
  public:
    double powerDensityOccupied() const {return _powerDensityOccupied;}
    double powerDensityUnoccupied() const {return _powerDensityUnoccupied;}
    double dimmingFraction() const {return _dimmingFraction;}
    double exteriorEnergy() const {return _exteriorEnergy;}

    void setPowerDensityOccupied(double value) {_powerDensityOccupied = value;}
    void setPowerDensityUnoccupied(double value) {_powerDensityUnoccupied = value;}
    void setDimmingFraction(double value) {_dimmingFraction = value;}
    void setExteriorEnergy(double value) {_exteriorEnergy = value;}

  private:
    double _powerDensityOccupied;
    double _powerDensityUnoccupied;
    double _dimmingFraction;
    double _exteriorEnergy;

  };

} // isomodel
} // openstudio
#endif // ISOMODEL_LIGHTING_HPP
