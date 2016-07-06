/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#ifndef MODEL_ELECTRICALSTORAGE_HPP
#define MODEL_ELECTRICALSTORAGE_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"
#include "ThermalZone.hpp"

namespace openstudio {
namespace model {

class ElectricLoadCenterDistribution;

namespace detail{
  class ElectricalStorage_Impl;
}

/** ElectricStorage is a ParentObject.
It is the Base Class of both ElectricLoadCenterStorageSimple and ElectricLoadCenterStorageBattery*/
class MODEL_API ElectricalStorage : public ParentObject {

  public:

    ElectricalStorage(IddObjectType type,const Model& model);

    virtual ~ElectricalStorage() {}

    boost::optional<ElectricLoadCenterDistribution> electricLoadCenterDistribution() const;

    virtual boost::optional<ThermalZone> thermalZone() const;

    virtual bool setThermalZone(const ThermalZone& thermalZone);

    virtual void resetThermalZone();

  protected:

    friend class Model;

    friend class openstudio::IdfObject;

    /// @cond 

    typedef detail::ElectricalStorage_Impl ImplType;

    explicit ElectricalStorage(std::shared_ptr<detail::ElectricalStorage_Impl> impl);

  private:

    REGISTER_LOGGER("openstudio.model.ElectricalStorage");

  /// @endcond 

};

typedef boost::optional<ElectricalStorage> OptionalElectricalStorage;

typedef std::vector<ElectricalStorage> ElectricalStorageVector;

} // model
} // openstudio

#endif // MODEL_ELECTRICALSTORAGE_HPP

